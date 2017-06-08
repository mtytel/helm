//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/interappaudio/AudioIO.h
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

#pragma once

/// \cond ignore

#include <AudioUnit/AUComponent.h>
#include <AudioToolbox/AudioToolbox.h>
#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"
#include <vector>

#ifndef __OBJC__
struct UIImage;
struct NSString;
#endif

namespace Steinberg {
namespace Vst {
namespace InterAppAudio {

class AudioIO;

//------------------------------------------------------------------------
class IMidiProcessor
{
public:
	virtual void onMIDIEvent (UInt32 status, UInt32 data1, UInt32 data2, UInt32 sampleOffset, bool withinRealtimeThread) = 0;
};

//------------------------------------------------------------------------
class IAudioIOProcessor : public IMidiProcessor
{
public:
	virtual void willStartAudio (AudioIO* audioIO) = 0;
	virtual void didStopAudio (AudioIO* audioIO) = 0;

	virtual void process (const AudioTimeStamp* timeStamp, UInt32 busNumber, UInt32 numFrames, AudioBufferList* ioData, bool& outputIsSilence, AudioIO* audioIO) = 0;
};

//------------------------------------------------------------------------
class AudioIO
{
public:
	static AudioIO* instance ();

	tresult init (OSType type, OSType subType, OSType manufacturer, CFStringRef name);

	bool switchToHost ();
	bool sendRemoteControlEvent (AudioUnitRemoteControlEvent event);
	UIImage* getHostIcon ();
	
	tresult start ();
	tresult stop ();

	tresult addProcessor (IAudioIOProcessor* processor);
	tresult removeProcessor (IAudioIOProcessor* processor);
	
	// accessors
	AudioUnit getRemoteIO () const { return remoteIO; }
	SampleRate getSampleRate () const { return sampleRate; }
	bool getInterAppAudioConnected () const { return interAppAudioConnected; }

	// host context information
	bool getBeatAndTempo (Float64& beat, Float64& tempo);
	bool getMusicalTimeLocation (UInt32& deltaSampleOffset, Float32& timeSigNumerator, UInt32& timeSigDenominator, Float64& downBeat);
	bool getTransportState (Boolean& isPlaying, Boolean& isRecording, Boolean& transportStateChanged, Float64& sampleInTimeLine, Boolean& isCycling, Float64& cycleStartBeat, Float64& cycleEndBeat);

	void setStaticFallbackTempo (Float64 tempo) {staticTempo = tempo; }
	Float64 getStaticFallbackTempo () const { return staticTempo; }

	static NSString* kConnectionStateChange;
//------------------------------------------------------------------------
protected:
	AudioIO ();
	~AudioIO ();

	static OSStatus inputCallbackStatic (void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData);
	static OSStatus renderCallbackStatic (void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData);
	static void propertyChangeStatic (void *inRefCon, AudioUnit inUnit, AudioUnitPropertyID inID, AudioUnitScope inScope, AudioUnitElement inElement);
	static void midiEventCallbackStatic (void *inRefCon, UInt32 inStatus, UInt32 inData1, UInt32 inData2, UInt32 inOffsetSampleFrame);

	OSStatus inputCallback (AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData);
	OSStatus renderCallback (AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData);
	void midiEventCallback (UInt32 inStatus, UInt32 inData1, UInt32 inData2, UInt32 inOffsetSampleFrame);

	void remoteIOPropertyChanged (AudioUnitPropertyID inID, AudioUnitScope inScope, AudioUnitElement inElement);

	void setAudioSessionActive (bool state);
	tresult setupRemoteIO (OSType type);
	tresult setupAUGraph (OSType type);
	void updateInterAppAudioConnectionState ();
	
	AudioUnit remoteIO;
	AUGraph graph;
	AudioBufferList* ioBufferList;
	HostCallbackInfo hostCallback;
	UInt32 maxFrames;
	Float64 staticTempo;
	SampleRate sampleRate;
	bool interAppAudioConnected;
	std::vector<IAudioIOProcessor*> audioProcessors;

	enum InternalState {
		kUninitialized,
		kInitialized,
		kStarted,
	};
	InternalState internalState;
};

//------------------------------------------------------------------------
} // namespace InterAppAudio
} // namespace Vst
} // namespace Steinberg

/// \endcond
