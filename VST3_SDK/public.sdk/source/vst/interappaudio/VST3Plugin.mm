//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/interappaudio/VST3Plugin.mm
// Created by  : Steinberg, 08/2013.
// Description : VST 3 InterAppAudio
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2017, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#import "VST3Plugin.h"
#import "HostApp.h"
#import "pluginterfaces/base/ipluginbase.h"
#import "pluginterfaces/vst/ivstmessage.h"
#import "pluginterfaces/vst/ivstmidicontrollers.h"
#import "pluginterfaces/vst/ivstinterappaudio.h"
#import "public.sdk/source/vst/auwrapper/NSDataIBStream.h"
#import "public.sdk/source/vst/hosting/hostclasses.h"
#import "base/source/updatehandler.h"
#import <libkern/OSAtomic.h>

//------------------------------------------------------------------------
extern "C"
{
	bool bundleEntry (CFBundleRef);
	bool bundleExit (void);
}

namespace Steinberg {
namespace Vst {
namespace InterAppAudio {

//------------------------------------------------------------------------
__attribute__((constructor))
static void InitUpdateHandler ()
{
	UpdateHandler::instance ();
}

//------------------------------------------------------------------------
VST3Plugin::VST3Plugin ()
: processor (0)
, editController (0)
, timer (0)
, processing (false)
{
	processData.processContext = &processContext;
	processData.inputParameterChanges = &inputParamChanges;
	processData.outputParameterChanges = &outputParamChanges;
	processData.inputEvents = &inputEvents;

}

//------------------------------------------------------------------------
VST3Plugin::~VST3Plugin ()
{
}

//------------------------------------------------------------------------
bool VST3Plugin::init ()
{
	if (processor == 0 && editController == 0)
	{
		::bundleEntry (CFBundleGetMainBundle ());
		createProcessorAndController ();
	}
	return processor && editController;
}

//------------------------------------------------------------------------
void VST3Plugin::createProcessorAndController ()
{
	Steinberg::IPluginFactory* factory = GetPluginFactory ();
	if (factory == 0)
		return;
	IComponent* component = 0;
	PClassInfo classInfo;
	int32 classCount = factory->countClasses ();
	for (int32 i = 0; i < classCount; i++)
	{
		if (factory->getClassInfo (i, &classInfo) != kResultTrue)
			return;
		if (strcmp (classInfo.category, kVstAudioEffectClass) == 0)
		{
			if (factory->createInstance (classInfo.cid, IComponent::iid, (void **)&component) != kResultTrue)
			{
				return;
			}
			break;
		}
	}
	if (component)
	{
		if (component->initialize (InterAppAudioHostApp::instance ()->unknownCast ()) != kResultTrue)
		{
			component->release ();
			return;
		}
		if (component->queryInterface (IEditController::iid, (void**)&editController) != kResultTrue)
		{
			FUID controllerCID;
			if (component->getControllerClassId (controllerCID) == kResultTrue && controllerCID.isValid ())
			{
				if (factory->createInstance (controllerCID, IEditController::iid, (void**)&editController) != kResultTrue)
					return;
				editController->setComponentHandler (this);
				if (editController->initialize (InterAppAudioHostApp::instance ()->unknownCast ()) != kResultTrue)
				{
					component->release ();
					editController->release ();
					editController = 0;
					return;
				}
				FUnknownPtr<IConnectionPoint> compConnection (component);
				FUnknownPtr<IConnectionPoint> ctrlerConnection (editController);
				if (compConnection && ctrlerConnection)
				{
					compConnection->connect (ctrlerConnection);
					ctrlerConnection->connect (compConnection);
				}
			}
			else
			{
				component->release ();
				return;
			}
		}
		component->queryInterface (IAudioProcessor::iid, (void**)&processor);
		if (processor == 0)
		{
			if (editController)
			{
				editController->release ();
				editController = 0;
			}
		}
		else
		{
			NSMutableData* data = [NSMutableData new];
			NSMutableDataIBStream state (data);
			if (component->getState (&state) == kResultTrue)
			{
				state.seek (0, IBStream::kIBSeekSet);
				editController->setComponentState (&state);
			}
			int32 paramCount = editController->getParameterCount ();
			inputParamChanges.setMaxParameters (paramCount);
			inputParamChangeTransfer.setMaxParameters (paramCount);
			outputParamChanges.setMaxParameters (paramCount);
			outputParamChangeTransfer.setMaxParameters (paramCount);
			midiControllerToParamIDMap = createMIDIControllerToParamIDMap ();
			uiScheduledEvents.resize (kMaxUIEvents);
		}
		component->release ();
	}
}

//------------------------------------------------------------------------
VST3Plugin::MIDIControllerToParamIDMap VST3Plugin::createMIDIControllerToParamIDMap ()
{
	MIDIControllerToParamIDMap newMap;

	FUnknownPtr<IMidiMapping> midiMapping (editController);
	if (midiMapping)
	{
		uint16 channelCount = 0;
		FUnknownPtr<IComponent> component (processor);
		if (component)
		{
			int32 busCount = component->getBusCount (kEvent, kInput);
			if (busCount > 0)
			{
				BusInfo busInfo;
				if (component->getBusInfo (kEvent, kInput, 0, busInfo) == kResultTrue)
				{
					channelCount = busInfo.channelCount;
				}
			}
		}
		
		ParamID paramID;
		for (int32 channel = 0; channel < channelCount; channel++)
		{
			for (CtrlNumber ctrler = 0; ctrler < kCountCtrlNumber; ctrler++)
			{
				if (midiMapping->getMidiControllerAssignment (0, channel, ctrler, paramID) == kResultTrue)
				{
					newMap.insert (std::make_pair (channelAndCtrlNumber (channel, ctrler), paramID));
				}
			}
		}
	}
	return newMap;
}

//------------------------------------------------------------------------
tresult VST3Plugin::scheduleEventFromUI (Event& event)
{
	if (event.type == Event::kNoteOnEvent)
		event.noteOn.noteId = lastNodeID++;
	return uiScheduledEvents.write (event) ? kResultTrue : kResultFalse;
}

//------------------------------------------------------------------------
NSData* VST3Plugin::getProcessorState ()
{
	if (processor)
	{
		NSMutableData* data = [NSMutableData new];
		NSMutableDataIBStream state (data);
		FUnknownPtr<IComponent> comp (processor);
		if (comp->getState (&state) == kResultTrue)
		{
			return data;
		}
	}
	return nil;
}

//------------------------------------------------------------------------
bool VST3Plugin::setProcessorState (NSData* data)
{
	if (editController && processor)
	{
		NSDataIBStream stream (data);
		FUnknownPtr<IComponent> comp (processor);
		if (comp->setState (&stream) == kResultTrue)
		{
			stream.seek (0, IBStream::kIBSeekSet);
			editController->setComponentState (&stream);
			return true;
		}
	}
	
	return false;
}

//------------------------------------------------------------------------
NSData* VST3Plugin::getControllerState ()
{
	if (editController)
	{
		NSMutableData* data = [NSMutableData new];
		NSMutableDataIBStream state (data);
		if (editController->getState (&state) == kResultTrue)
		{
			return data;
		}
	}
	return nil;
}

//------------------------------------------------------------------------
bool VST3Plugin::setControllerState (NSData* data)
{
	if (editController)
	{
		NSDataIBStream stream (data);
		if (editController->setState (&stream) == kResultTrue)
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------
void VST3Plugin::willStartAudio (AudioIO* audioIO)
{
	noteIDPitchMap.clear ();
	lastNodeID.store (0);

	ProcessSetup setup;
	setup.processMode = kRealtime;
	setup.symbolicSampleSize = kSample32;
	setup.maxSamplesPerBlock = 4096;// TODO:
	setup.sampleRate = audioIO->getSampleRate ();
	processor->setupProcessing (setup);
	SpeakerArrangement inputs[1];
	SpeakerArrangement outputs[1];
	inputs[0] = SpeakerArr::kStereo;
	outputs[0] = SpeakerArr::kStereo;
	processor->setBusArrangements (inputs, 1, outputs, 1);
	
	FUnknownPtr<IComponent> comp (processor);
	comp->setActive (true);

	processData.prepare (*comp);

	FUnknownPtr<IInterAppAudioConnectionNotification> iaaConnectionNotification (editController);
	if (iaaConnectionNotification)
	{
		iaaConnectionNotification->onInterAppAudioConnectionStateChange (audioIO->getInterAppAudioConnected () ? true : false);
	}

	timer = Timer::create (this, 16);
}

//------------------------------------------------------------------------
void VST3Plugin::didStopAudio (AudioIO* audioIO)
{
	processor->setProcessing (false); // TODO: currently not called in Audio Thread as it should according to the VST3 spec
	processing = false;
	
	FUnknownPtr<IComponent> comp (processor);
	comp->setActive (false);
	timer->release ();
	timer = 0;
}

//------------------------------------------------------------------------
void VST3Plugin::onMIDIEvent (UInt32 inStatus, UInt32 data1, UInt32 data2, UInt32 sampleOffset, bool withinRealtimeThread)
{
	Event e = {};
	e.flags = Event::kIsLive;
	
	uint16 status = inStatus & 0xF0;
	uint16 channel = inStatus & 0x0F;
	if (status == 0x90 && data2 != 0) // note on
	{
		auto noteID = noteIDPitchMap.find ((channel << 8) + data1);
		if (noteID != noteIDPitchMap.end ())
		{
			// for now, we just turn off the old note on
			Event e2 = {};
			e2.type = Event::kNoteOffEvent;
			e2.noteOff.noteId = noteID->second;
			e2.noteOff.channel = channel;
			e2.noteOff.pitch = data1;
			e2.noteOff.velocity = (float)data2 / 128.f;
			e2.sampleOffset = 0;
			inputEvents.addEvent (e2);
			noteIDPitchMap.erase (noteID);
		}
		e.type = Event::kNoteOnEvent;
		e.noteOn.channel = channel;
		e.noteOn.pitch = data1;
		e.noteOn.velocity = (float)data2 / 128.f;
		e.noteOn.length = -1;
		e.sampleOffset = sampleOffset;
		if (withinRealtimeThread)
		{
			e.noteOn.noteId = lastNodeID++;
			inputEvents.addEvent (e);
			noteIDPitchMap.insert (std::make_pair ((channel << 8) + data1, e.noteOn.noteId));
		}
		else
		{
			scheduleEventFromUI (e);
		}
	}
	else if (status == 0x80 || (status == 0x90 && data2 == 0)) // note off
	{
		auto noteID = noteIDPitchMap.find ((channel << 8) + data1);
		if (noteID != noteIDPitchMap.end())
		{
			e.type = Event::kNoteOffEvent;
			e.noteOff.noteId = noteID->second;
			e.noteOff.channel = channel;
			e.noteOff.pitch = data1;
			e.noteOff.velocity = (float)data2 / 128.f;
			e.sampleOffset = sampleOffset;
			if (withinRealtimeThread)
			{
				inputEvents.addEvent (e);
				noteIDPitchMap.erase (noteID);
			}
			else
			{
				scheduleEventFromUI (e);
			}
		}
		else
		{
			NSLog (@"NoteID not found:%d", (unsigned int)data1);
		}
	}
	else if (status == 0xb0 && data1 < kAfterTouch) // controller
	{
		auto it = midiControllerToParamIDMap.find (channelAndCtrlNumber (channel, data1));
		if (it != midiControllerToParamIDMap.end ())
		{
			ParamValue value = (ParamValue)data2 / 128.;
			if (withinRealtimeThread)
			{
				int32 index;
				IParamValueQueue* queue = inputParamChanges.addParameterData (it->second, index);
				if (queue)
				{
					queue->addPoint (sampleOffset, value, index);
				}
			}
			else
			{
				inputParamChangeTransfer.addChange (it->second, value, sampleOffset);
			}
		}
	}
	else if (status == 0xe0) // pitch bend
	{
		auto it = midiControllerToParamIDMap.find (channelAndCtrlNumber (channel, kPitchBend));
		if (it != midiControllerToParamIDMap.end ())
		{
			uint16 _14bit;
			_14bit = (uint16)data2;
			_14bit <<= 7;
			_14bit |= (uint16)data1;
			ParamValue value = (double)_14bit / (double)0x3fff;

			if (withinRealtimeThread)
			{
				int32 index;
				IParamValueQueue* queue = inputParamChanges.addParameterData (it->second, index);
				if (queue)
				{
					queue->addPoint (sampleOffset, value, index);
				}
			}
			else
			{
				inputParamChangeTransfer.addChange (it->second, value, sampleOffset);
			}
		}
	}
	else if (status == 0xd0) // aftertouch
	{
		auto it = midiControllerToParamIDMap.find (channelAndCtrlNumber (channel, kAfterTouch));
		if (it != midiControllerToParamIDMap.end ())
		{
			ParamValue value = (ParamValue)data1 / 128.;
			if (withinRealtimeThread)
			{
				int32 index;
				IParamValueQueue* queue = inputParamChanges.addParameterData (it->second, index);
				if (queue)
				{
					queue->addPoint (sampleOffset, value, index);
				}
			}
			else
			{
				inputParamChangeTransfer.addChange (it->second, value, sampleOffset);
			}
		}
	}
}

//------------------------------------------------------------------------
void VST3Plugin::process (const AudioTimeStamp* timeStamp, UInt32 busNumber, UInt32 numFrames, AudioBufferList* ioData, bool& outputIsSilence, AudioIO* audioIO)
{
	if (processing == false)
	{
		processor->setProcessing (true);
		processing = true;
	}
	updateProcessContext (audioIO);
	if (timeStamp)
		processContext.systemTime = timeStamp->mHostTime;

	// TODO: silence state update
	for (UInt32 i = 0; i < ioData->mNumberBuffers; i++)
	{
		processData.setChannelBuffer (kInput, 0, i, (float*)ioData->mBuffers[i].mData);
		processData.setChannelBuffer (kOutput, 0, i, (float*)ioData->mBuffers[i].mData);
	}
	
	Event e;
	while (uiScheduledEvents.read (e))
	{
		inputEvents.addEvent (e);
		if (e.type == Event::kNoteOnEvent)
		{
			auto noteID = noteIDPitchMap.find ((e.noteOn.channel << 8) + e.noteOn.pitch);
			if (noteID == noteIDPitchMap.end ())
				noteIDPitchMap.insert (std::make_pair ((e.noteOn.channel << 8) + e.noteOn.pitch, e.noteOn.noteId));
		}
	}
	
	processData.numSamples = numFrames;
	inputParamChangeTransfer.transferChangesTo (inputParamChanges);
	if (processor->process (processData) == kResultTrue)
	{
		inputParamChanges.clearQueue ();
		outputParamChangeTransfer.transferChangesFrom (outputParamChanges);
		outputParamChanges.clearQueue ();
		inputEvents.clear ();
	}
}

//------------------------------------------------------------------------
void VST3Plugin::updateProcessContext (AudioIO* audioIO)
{
	memset (&processContext, 0, sizeof (ProcessContext));
	processContext.sampleRate = audioIO->getSampleRate ();
	Float64 beat = 0., tempo = 0.;
	if (audioIO->getBeatAndTempo (beat, tempo))
	{
		processContext.state |= ProcessContext::kTempoValid | ProcessContext::kProjectTimeMusicValid;
		processContext.tempo = tempo;
		processContext.projectTimeMusic = beat;
	}
	else
	{
		processContext.state |= ProcessContext::kTempoValid;
		processContext.tempo = 120.;
	}
	UInt32 deltaSampleOffsetToNextBeat = 0;
	Float32 timeSigNumerator = 0;
	UInt32 timeSigDenominator = 0;
	Float64 currentMeasureDownBeat = 0;
	if (audioIO->getMusicalTimeLocation (deltaSampleOffsetToNextBeat, timeSigNumerator, timeSigDenominator, currentMeasureDownBeat))
	{
		processContext.state |= ProcessContext::kTimeSigValid | ProcessContext::kBarPositionValid | ProcessContext::kClockValid;
		processContext.timeSigNumerator = timeSigNumerator;
		processContext.timeSigDenominator = timeSigDenominator;
		processContext.samplesToNextClock = deltaSampleOffsetToNextBeat;
		processContext.barPositionMusic = currentMeasureDownBeat;
	}
	Boolean isPlaying;
	Boolean isRecording;
	Boolean transportStateChanged;
	Float64 currentSampleInTimeLine;
	Boolean isCycling;
	Float64 cycleStartBeat;
	Float64 cycleEndBeat;
	if (audioIO->getTransportState (isPlaying, isRecording, transportStateChanged, currentSampleInTimeLine, isCycling, cycleStartBeat, cycleEndBeat))
	{
		processContext.state |= ProcessContext::kCycleValid;
		processContext.cycleStartMusic = cycleStartBeat;
		processContext.cycleEndMusic = cycleEndBeat;
		processContext.projectTimeSamples = currentSampleInTimeLine;
		if (isPlaying)
			processContext.state |= ProcessContext::kPlaying;
		if (isCycling)
			processContext.state |= ProcessContext::kCycleActive;
		if (isRecording)
			processContext.state |= ProcessContext::kRecording;
	}
}

//------------------------------------------------------------------------
tresult PLUGIN_API VST3Plugin::beginEdit (ParamID id)
{
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VST3Plugin::performEdit (ParamID id, ParamValue valueNormalized)
{
	inputParamChangeTransfer.addChange (id, valueNormalized, 0);
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VST3Plugin::endEdit (ParamID id)
{
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VST3Plugin::restartComponent (int32 flags)
{
	tresult result = kNotImplemented;

	return result;
}

//------------------------------------------------------------------------
void VST3Plugin::onTimer (Timer* timer)
{
	ParamID paramID;
	ParamValue paramValue;
	int32 sampleOffset;
	while (outputParamChangeTransfer.getNextChange (paramID, paramValue, sampleOffset))
	{
		editController->setParamNormalized (paramID, paramValue);
	}
	UpdateHandler::instance ()->triggerDeferedUpdates ();
}

}}}
