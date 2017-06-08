//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/vstsinglecomponenteffect.cpp
// Created by  : Steinberg, 03/2008
// Description : Basic Audio Effect Implementation in one component
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

#include "vstsinglecomponenteffect.h"

//-----------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
// SingleComponentEffect Implementation
//-----------------------------------------------------------------------------
SingleComponentEffect::SingleComponentEffect ()
: audioInputs (kAudio, kInput)
, audioOutputs (kAudio, kOutput)
, eventInputs (kEvent, kInput)
, eventOutputs (kEvent, kOutput)
{
	processSetup.maxSamplesPerBlock = 1024;
	processSetup.processMode = Vst::kRealtime;
	processSetup.sampleRate = 44100.0;
	processSetup.symbolicSampleSize = Vst::kSample32;
}

//-----------------------------------------------------------------------------
SingleComponentEffect::~SingleComponentEffect ()
{
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API SingleComponentEffect::initialize (FUnknown* context)
{
	return EditController::initialize (context);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API SingleComponentEffect::terminate ()
{
	parameters.removeAll ();
	removeAllBusses ();

	return EditController::terminate ();
}

//-----------------------------------------------------------------------------
int32 PLUGIN_API SingleComponentEffect::getBusCount (MediaType type, BusDirection dir)
{
	BusList* busList = getBusList (type, dir);
	return busList ? static_cast<int32>(busList->size ()) : 0;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API SingleComponentEffect::getBusInfo (MediaType type, BusDirection dir, int32 index,
                                                      BusInfo& info)
{
	BusList* busList = getBusList (type, dir);
	if (busList == 0)
		return kInvalidArgument;
	if (index >= static_cast<int32> (busList->size ()))
		return kInvalidArgument;

	Bus* bus = busList->at (index);
	info.mediaType = type;
	info.direction = dir;
	if (bus->getInfo (info))
		return kResultTrue;
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API SingleComponentEffect::activateBus (MediaType type, BusDirection dir,
                                                       int32 index, TBool state)
{
	BusList* busList = getBusList (type, dir);
	Bus* bus = busList ? (Bus*)busList->at (index) : 0;
	if (bus)
	{
		bus->setActive (state);
		return kResultTrue;
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
AudioBus* SingleComponentEffect::addAudioInput (const TChar* name, SpeakerArrangement arr,
                                                BusType busType, int32 flags)
{
	AudioBus* newBus = new AudioBus (name, busType, flags, arr);
	audioInputs.push_back (IPtr<Vst::Bus> (newBus, false));
	return newBus;
}

//-----------------------------------------------------------------------------
AudioBus* SingleComponentEffect::addAudioOutput (const TChar* name, SpeakerArrangement arr,
                                                 BusType busType, int32 flags)
{
	AudioBus* newBus = new AudioBus (name, busType, flags, arr);
	audioOutputs.push_back (IPtr<Vst::Bus> (newBus, false));
	return newBus;
}

//-----------------------------------------------------------------------------
EventBus* SingleComponentEffect::addEventInput (const TChar* name, int32 channels, BusType busType,
                                                int32 flags)
{
	EventBus* newBus = new EventBus (name, busType, flags, channels);
	eventInputs.push_back (IPtr<Vst::Bus> (newBus, false));
	return newBus;
}

//-----------------------------------------------------------------------------
EventBus* SingleComponentEffect::addEventOutput (const TChar* name, int32 channels, BusType busType,
                                                 int32 flags)
{
	EventBus* newBus = new EventBus (name, busType, flags, channels);
	eventOutputs.push_back (IPtr<Vst::Bus> (newBus, false));
	return newBus;
}

//-----------------------------------------------------------------------------
tresult SingleComponentEffect::removeAudioBusses ()
{
	audioInputs.clear ();
	audioOutputs.clear ();

	return kResultOk;
}

//-----------------------------------------------------------------------------
tresult SingleComponentEffect::removeEventBusses ()
{
	eventInputs.clear ();
	eventOutputs.clear ();

	return kResultOk;
}

//-----------------------------------------------------------------------------
tresult SingleComponentEffect::removeAllBusses ()
{
	removeAudioBusses ();
	removeEventBusses ();

	return kResultOk;
}

//-----------------------------------------------------------------------------
// IAudioProcessor
//-----------------------------------------------------------------------------
tresult PLUGIN_API SingleComponentEffect::setBusArrangements (SpeakerArrangement* inputs,
                                                              int32 numIns,
                                                              SpeakerArrangement* outputs,
                                                              int32 numOuts)
{
	if (numIns < 0 || numOuts < 0)
		return kInvalidArgument;

	if (numIns > static_cast<int32> (audioInputs.size ()) ||
	    numOuts > static_cast<int32> (audioOutputs.size ()))
		return kResultFalse;

	for (int32 index = 0; index < static_cast<int32>(audioInputs.size ()); ++index)
	{
		if (index >= numIns)
			break;
		FCast<Vst::AudioBus> (audioInputs[index].get ())->setArrangement (inputs[index]);
	}

	for (int32 index = 0; index < static_cast<int32>(audioOutputs.size ()); ++index)
	{
		if (index >= numOuts)
			break;
		FCast<Vst::AudioBus> (audioOutputs[index].get ())->setArrangement (outputs[index]);
	}

	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API SingleComponentEffect::getBusArrangement (BusDirection dir, int32 busIndex,
                                                             SpeakerArrangement& arr)
{
	BusList* busList = getBusList (kAudio, dir);
	AudioBus* audioBus = busList ? FCast<Vst::AudioBus> (busList->at (busIndex)) : 0;
	if (audioBus)
	{
		arr = audioBus->getArrangement ();
		return kResultTrue;
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API SingleComponentEffect::setupProcessing (ProcessSetup& newSetup)
{
	if (canProcessSampleSize (newSetup.symbolicSampleSize) != kResultTrue)
		return kResultFalse;

	processSetup = newSetup;
	return kResultOk;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API SingleComponentEffect::canProcessSampleSize (int32 symbolicSampleSize)
{
	return symbolicSampleSize == kSample32 ? kResultTrue : kResultFalse;
}

//-----------------------------------------------------------------------------
BusList* SingleComponentEffect::getBusList (MediaType type, BusDirection dir)
{
	if (type == kAudio)
		return dir == kInput ? &audioInputs : &audioOutputs;
	else if (type == kEvent)
		return dir == kInput ? &eventInputs : &eventOutputs;
	return 0;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API SingleComponentEffect::queryInterface (const TUID iid, void** obj)
{
	if (memcmp (iid, IConnectionPoint::iid, sizeof (::Steinberg::TUID)) == 0)
	{
		// no need to expose IConnectionPoint to the host
		return kNoInterface;
	}
	DEF_INTERFACE (IComponent)
	DEF_INTERFACE (IAudioProcessor)
	return EditController::queryInterface (iid, obj);
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg

// work around for the name clash of IComponent::setState and IEditController::setState
// make sure that vsteditcontroller.cpp is otherwise excluded from your project
#define setState setEditorState
#define getState getEditorState
#include "public.sdk/source/vst/vsteditcontroller.cpp"
#undef setState
#undef getState
