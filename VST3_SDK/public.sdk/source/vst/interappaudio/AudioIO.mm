//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/interappaudio/AudioIO.mm
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

#import "AudioIO.h"
#import "MidiIO.h"
#import "pluginterfaces/base/fstrdefs.h"
#import <AudioUnit/AudioUnit.h>
#import <AVFoundation/AVAudioSession.h>
#import <UIKit/UIKit.h>

#define FORCE_INLINE	__attribute__((always_inline))

namespace Steinberg {
namespace Vst {
namespace InterAppAudio {

//------------------------------------------------------------------------
static AudioBufferList* createBuffers (uint32 numChannels, uint32 maxFrames, uint32 frameSize)
{
	AudioBufferList* result = (AudioBufferList*) malloc (sizeof (AudioBufferList) + sizeof (AudioBuffer) * numChannels);
	result->mNumberBuffers = numChannels;
	for (int32 i = 0; i < numChannels; i++)
	{
		result->mBuffers[i].mDataByteSize = maxFrames * sizeof (float);
		result->mBuffers[i].mData = calloc (1, result->mBuffers[i].mDataByteSize);
		result->mBuffers[i].mNumberChannels = 1;
	}
	return result;
}

//------------------------------------------------------------------------
static void freeAudioBufferList (AudioBufferList* audioBufferList)
{
	for (uint32 i = 0; i < audioBufferList->mNumberBuffers; i++)
	{
		free (audioBufferList->mBuffers[i].mData);
	}
	free (audioBufferList);
}

//------------------------------------------------------------------------
NSString* AudioIO::kConnectionStateChange = @"AudioIO::kConnectionStateChange";

//------------------------------------------------------------------------
AudioIO::AudioIO ()
: remoteIO (0)
, graph (0)
, ioBufferList (0)
, maxFrames (4096)
, staticTempo (120.)
, interAppAudioConnected (false)
, internalState (kUninitialized)
{
	sampleRate = [[AVAudioSession sharedInstance] sampleRate];
	memset (&hostCallback, 0, sizeof (HostCallbackInfo));
	
	MidiIO::instance ();
}

//------------------------------------------------------------------------
AudioIO::~AudioIO ()
{
	if (ioBufferList)
		freeAudioBufferList (ioBufferList);
}

//------------------------------------------------------------------------
AudioIO* AudioIO::instance ()
{
	static AudioIO gInstance;
	return &gInstance;
}

//------------------------------------------------------------------------
tresult AudioIO::setupRemoteIO (OSType type)
{
	if (remoteIO != 0)
	{
		AudioStreamBasicDescription streamFormat = {};
		streamFormat.mChannelsPerFrame	= 2;
		streamFormat.mSampleRate		= sampleRate;
		streamFormat.mFormatID			= kAudioFormatLinearPCM;
		streamFormat.mFormatFlags		= kAudioFormatFlagsNativeFloatPacked | kAudioFormatFlagIsNonInterleaved;
		streamFormat.mBytesPerFrame		= streamFormat.mBytesPerPacket = sizeof(Float32);
		streamFormat.mBitsPerChannel	= 32;
		streamFormat.mFramesPerPacket	= 1;
		
		OSStatus status = AudioUnitSetProperty (remoteIO,
												kAudioUnitProperty_StreamFormat,
												kAudioUnitScope_Output,
												1,
												&streamFormat,
												sizeof(streamFormat));
		if (status != noErr)
			return kInternalError;

		status = AudioUnitSetProperty ( remoteIO,
										kAudioUnitProperty_StreamFormat,
										kAudioUnitScope_Input,
										0,
										&streamFormat,
										sizeof(streamFormat));
		if (status != noErr)
			return kInternalError;
		
		status = AudioUnitSetProperty ( remoteIO,
										kAudioUnitProperty_MaximumFramesPerSlice,
										kAudioUnitScope_Global,
										1,
										&maxFrames,
										sizeof(maxFrames));

		if (status != noErr)
			return kInternalError;

		bool needInput = (type == kAudioUnitType_RemoteGenerator || type == kAudioUnitType_RemoteInstrument) == false;
		UInt32 flag = 1;
		if (needInput)
		{
			// enable IO Input
			status = AudioUnitSetProperty  (remoteIO,
											kAudioOutputUnitProperty_EnableIO,
											kAudioUnitScope_Input,
											1,
											&flag,
											sizeof(flag));
			if (status != noErr)
				return kInternalError;
		}
		
		// enable IO Output
		status = AudioUnitSetProperty  (remoteIO,
										kAudioOutputUnitProperty_EnableIO,
										kAudioUnitScope_Output,
										0,
										&flag,
										sizeof(flag));
		if (status != noErr)
			return kInternalError;

		AURenderCallbackStruct renderCallback = {};
		if (needInput)
		{
			renderCallback.inputProc = inputCallbackStatic;
			renderCallback.inputProcRefCon = this;
			status = AudioUnitSetProperty (remoteIO, kAudioOutputUnitProperty_SetInputCallback, kAudioUnitScope_Global, 1, &renderCallback, sizeof (renderCallback));
			if (status != noErr)
				return kInternalError;
		}

		renderCallback.inputProc = renderCallbackStatic;
		renderCallback.inputProcRefCon = this;
		status = AudioUnitSetProperty (remoteIO, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Global, 0, &renderCallback, sizeof (renderCallback));
		if (status != noErr)
			return kInternalError;

		if (type == kAudioUnitType_RemoteInstrument || type == kAudioUnitType_RemoteMusicEffect)
		{
			AudioOutputUnitMIDICallbacks callBackStruct = {};
			callBackStruct.userData = this;
			callBackStruct.MIDIEventProc = midiEventCallbackStatic;
			status = AudioUnitSetProperty (remoteIO, kAudioOutputUnitProperty_MIDICallbacks, kAudioUnitScope_Global, 0, &callBackStruct, sizeof (callBackStruct));
			if (status != noErr)
			{
				NSLog (@"Setting MIDICallback on OutputUnit failed");
			}
		}

		if (ioBufferList)
			freeAudioBufferList (ioBufferList);
		ioBufferList = createBuffers (streamFormat.mChannelsPerFrame, maxFrames, streamFormat.mBytesPerFrame);
		if (ioBufferList == 0)
			return kOutOfMemory;

		status = AudioUnitAddPropertyListener (remoteIO, kAudioUnitProperty_IsInterAppConnected, propertyChangeStatic, this);
		status = AudioUnitAddPropertyListener (remoteIO, kAudioOutputUnitProperty_HostTransportState, propertyChangeStatic, this);

		return kResultTrue;
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
tresult AudioIO::setupAUGraph (OSType type)
{
	if (graph == 0)
	{
		OSStatus status = NewAUGraph (&graph);
		if (status != noErr)
			return kInternalError;
		
		AudioComponentDescription iOUnitDescription;
		iOUnitDescription.componentManufacturer = kAudioUnitManufacturer_Apple;
		iOUnitDescription.componentFlags = 0;
		iOUnitDescription.componentFlagsMask = 0;
		iOUnitDescription.componentType = kAudioUnitType_Output;
		iOUnitDescription.componentSubType = kAudioUnitSubType_RemoteIO;

		AUNode remoteIONode;
		status = AUGraphAddNode (graph, &iOUnitDescription, &remoteIONode);
		if (status != noErr)
			return kInternalError;
		status = AUGraphOpen(graph);
		if (status != noErr)
			return kInternalError;
		status = AUGraphNodeInfo (graph, remoteIONode, 0, &remoteIO);
		if (status != noErr)
			return kInternalError;
		return setupRemoteIO (type);
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
void AudioIO::updateInterAppAudioConnectionState ()
{
	if (remoteIO)
	{
		UInt32 connected;
		UInt32 dataSize = sizeof (connected);
		OSStatus status = AudioUnitGetProperty(remoteIO, kAudioUnitProperty_IsInterAppConnected, kAudioUnitScope_Global, 0, &connected, &dataSize);
		if (status == noErr)
		{
			if (interAppAudioConnected != connected)
			{
				if (connected)
				{
					UInt32 size = sizeof (HostCallbackInfo);
					status = AudioUnitGetProperty (remoteIO, kAudioUnitProperty_HostCallbacks, kAudioUnitScope_Global, 0, &hostCallback, &size);
				}
				else
				{
					memset (&hostCallback, 0, sizeof (HostCallbackInfo));
				}
				interAppAudioConnected = connected > 0 ? true : false;
				[[NSNotificationCenter defaultCenter] postNotificationName:kConnectionStateChange object:nil];
			}
		}
	}
}

//------------------------------------------------------------------------
tresult AudioIO::init (OSType type, OSType subType, OSType manufacturer, CFStringRef name)
{
	tresult result = setupAUGraph (type);
	if (result != kResultTrue)
		return result;

	AudioComponentDescription desc = { type, subType, manufacturer, 0, 0 };
	OSStatus status = AudioOutputUnitPublish (&desc, name, 0, remoteIO);
	if (status != noErr)
	{
		NSLog (@"AudioOutputUnitPublish failed with status:%d", (int)status);
	}
	internalState = kInitialized;

	return result;
}

//------------------------------------------------------------------------
void AudioIO::setAudioSessionActive (bool state)
{
	NSError* error;
	
	AVAudioSession *session = [AVAudioSession sharedInstance];
	[session setPreferredSampleRate: sampleRate error:&error];
	[session setCategory: AVAudioSessionCategoryPlayback withOptions: AVAudioSessionCategoryOptionMixWithOthers error:&error];
	[session setActive:(state ? YES : NO) error:&error];
}

//------------------------------------------------------------------------
tresult AudioIO::start ()
{
	if (internalState == kInitialized)
	{
		bool appIsActive = [UIApplication sharedApplication].applicationState == UIApplicationStateActive;
		if (!(appIsActive || interAppAudioConnected))
		{
			return kResultFalse;
		}
		setAudioSessionActive (true);
		
		Boolean graphInitialized = true;
		OSStatus status = AUGraphIsInitialized (graph, &graphInitialized);
		if (status != noErr)
			return kInternalError;
		
		if (graphInitialized == false)
		{
			status = AUGraphInitialize (graph);
			if (status != noErr)
				return kInternalError;
			updateInterAppAudioConnectionState ();
		}
		
		for (auto processor : audioProcessors)
		{
			processor->willStartAudio (this);
		}
		status = AUGraphStart (graph);
		if (status == noErr)
		{
			internalState = kStarted;
			updateInterAppAudioConnectionState ();
			return kResultTrue;
		}
		return kInternalError;
	}
	
	return kResultFalse;
}

//------------------------------------------------------------------------
tresult AudioIO::stop ()
{
	if (internalState == kStarted)
	{
		if (AUGraphStop (graph) == noErr)
		{
			for (auto processor : audioProcessors)
			{
				processor->didStopAudio (this);
			}
			internalState = kInitialized;
			if (interAppAudioConnected == false)
				setAudioSessionActive (false);
			return kResultTrue;
		}
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
tresult AudioIO::addProcessor (IAudioIOProcessor* processor)
{
	if (internalState == kInitialized)
	{
		audioProcessors.push_back (processor);
		MidiIO::instance ().addProcessor (processor);
		return kResultTrue;
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
tresult AudioIO::removeProcessor (IAudioIOProcessor* processor)
{
	if (internalState == kInitialized)
	{
		auto it = std::find (audioProcessors.begin (), audioProcessors.end (), processor);
		if (it != audioProcessors.end ())
		{
			audioProcessors.erase (it);
			MidiIO::instance ().removeProcessor (processor);
			return kResultTrue;
		}
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
bool AudioIO::switchToHost ()
{
	if (remoteIO && interAppAudioConnected)
	{
        CFURLRef instrumentUrl;
        UInt32 dataSize = sizeof(instrumentUrl);
        OSStatus result = AudioUnitGetProperty (remoteIO, kAudioUnitProperty_PeerURL, kAudioUnitScope_Global, 0, &instrumentUrl, &dataSize);
        if (result == noErr)
		{
            [[UIApplication sharedApplication] openURL:(__bridge NSURL*)instrumentUrl];
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------
bool AudioIO::sendRemoteControlEvent (AudioUnitRemoteControlEvent event)
{
	if (remoteIO && interAppAudioConnected)
	{
		UInt32 controlEvent = event;
		UInt32 dataSize = sizeof (controlEvent);
		OSStatus status = AudioUnitSetProperty (remoteIO, kAudioOutputUnitProperty_RemoteControlToHost, kAudioUnitScope_Global, 0, &controlEvent, dataSize);
		return status == noErr;
	}
	return false;
}

//------------------------------------------------------------------------
UIImage* AudioIO::getHostIcon ()
{
	if (remoteIO && interAppAudioConnected)
	{
		return AudioOutputUnitGetHostIcon (remoteIO, 128);
	}
	return nil;
}

//------------------------------------------------------------------------
bool AudioIO::getBeatAndTempo (Float64& beat, Float64& tempo)
{
	if (hostCallback.beatAndTempoProc)
	{
		if (hostCallback.beatAndTempoProc (hostCallback.hostUserData, &beat, &tempo) == noErr)
			return true;
	}
	tempo = staticTempo;
	beat = 0;
	return true;
}

//------------------------------------------------------------------------
bool AudioIO::getMusicalTimeLocation (UInt32& deltaSampleOffset, Float32& timeSigNumerator, UInt32& timeSigDenominator, Float64& downBeat)
{
	if (hostCallback.musicalTimeLocationProc)
	{
		if (hostCallback.musicalTimeLocationProc (hostCallback.hostUserData, &deltaSampleOffset, &timeSigNumerator, &timeSigDenominator, &downBeat) == noErr)
			return true;
	}
	return false;
}

//------------------------------------------------------------------------
bool AudioIO::getTransportState (Boolean& isPlaying, Boolean& isRecording, Boolean& transportStateChanged, Float64& sampleInTimeLine, Boolean& isCycling, Float64& cycleStartBeat, Float64& cycleEndBeat)
{
	if (hostCallback.transportStateProc2)
	{
		if (hostCallback.transportStateProc2 (hostCallback.hostUserData, &isPlaying, &isRecording, &transportStateChanged, &sampleInTimeLine, &isCycling, &cycleStartBeat, &cycleEndBeat) == noErr)
			return true;
	}
	return false;
}

//------------------------------------------------------------------------
FORCE_INLINE void AudioIO::remoteIOPropertyChanged (AudioUnitPropertyID inID, AudioUnitScope inScope, AudioUnitElement inElement)
{
	if (inID == kAudioUnitProperty_IsInterAppConnected)
	{
		bool wasConnected = interAppAudioConnected;
		updateInterAppAudioConnectionState ();
		if (wasConnected != interAppAudioConnected)
		{
			stop ();
			start ();
		}
	}
}

//------------------------------------------------------------------------
FORCE_INLINE OSStatus AudioIO::renderCallback (AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData)
{
	if (ioData->mNumberBuffers == ioBufferList->mNumberBuffers)
	{
		for (uint32 i = 0; i < ioData->mNumberBuffers; i++)
		{
			memcpy (ioData->mBuffers[i].mData, ioBufferList->mBuffers[i].mData, ioData->mBuffers[i].mDataByteSize);
		}
		bool outputIsSilence = ioActionFlags ? *ioActionFlags & kAudioUnitRenderAction_OutputIsSilence : false;
		for (auto processor : audioProcessors)
		{
			outputIsSilence = false;
			processor->process (inTimeStamp, inBusNumber, inNumberFrames, ioData, outputIsSilence, this);
		}
		if (ioActionFlags)
		{
			*ioActionFlags = outputIsSilence ? kAudioUnitRenderAction_OutputIsSilence : 0;
		}
	}
	return noErr;
}

//------------------------------------------------------------------------
FORCE_INLINE OSStatus AudioIO::inputCallback (AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData)
{
	OSStatus status = AudioUnitRender (remoteIO, ioActionFlags, inTimeStamp, inBusNumber, inNumberFrames, ioBufferList);
	return status;
}

//------------------------------------------------------------------------
FORCE_INLINE void AudioIO::midiEventCallback (UInt32 inStatus, UInt32 inData1, UInt32 inData2, UInt32 inOffsetSampleFrame)
{
	for (auto processor : audioProcessors)
	{
		processor->onMIDIEvent (inStatus, inData1, inData2, inOffsetSampleFrame, true);
	}
}

//------------------------------------------------------------------------
OSStatus AudioIO::inputCallbackStatic (void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData)
{
	AudioIO* io = (AudioIO*)inRefCon;
	return io->inputCallback (ioActionFlags, inTimeStamp, inBusNumber, inNumberFrames, ioData);
}

//------------------------------------------------------------------------
OSStatus AudioIO::renderCallbackStatic (void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData)
{
	AudioIO* io = (AudioIO*)inRefCon;
	return io->renderCallback (ioActionFlags, inTimeStamp, inBusNumber, inNumberFrames, ioData);
}

//------------------------------------------------------------------------
void AudioIO::propertyChangeStatic (void *inRefCon, AudioUnit inUnit, AudioUnitPropertyID inID, AudioUnitScope inScope, AudioUnitElement inElement)
{
	AudioIO* audioIO = (AudioIO*)inRefCon;
	audioIO->remoteIOPropertyChanged(inID, inScope, inElement);
}

//------------------------------------------------------------------------
void AudioIO::midiEventCallbackStatic (void *inRefCon, UInt32 inStatus, UInt32 inData1, UInt32 inData2, UInt32 inOffsetSampleFrame)
{
	AudioIO* audioIO = (AudioIO*)inRefCon;
	audioIO->midiEventCallback(inStatus, inData1, inData2, inOffsetSampleFrame);
}

}}}
