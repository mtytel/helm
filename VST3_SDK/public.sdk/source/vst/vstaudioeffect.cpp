//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/vstaudioeffect.cpp
// Created by  : Steinberg, 04/2005
// Description : Basic Audio Effect Implementation
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

#include "vstaudioeffect.h"

namespace Steinberg {
namespace Vst {


//------------------------------------------------------------------------
// AudioEffect
//------------------------------------------------------------------------
AudioEffect::AudioEffect ()
{
	processSetup.maxSamplesPerBlock = 1024;
	processSetup.processMode = Vst::kRealtime;
	processSetup.sampleRate = 44100.0;
	processSetup.symbolicSampleSize = Vst::kSample32;
}

//------------------------------------------------------------------------
AudioBus* AudioEffect::addAudioInput (const TChar* name, SpeakerArrangement arr,
									  BusType busType, int32 flags)
{
	AudioBus* newBus = new AudioBus (name, busType, flags, arr);
	audioInputs.push_back (IPtr<Vst::Bus> (newBus, false));
	return newBus;
}

//------------------------------------------------------------------------
AudioBus* AudioEffect::addAudioOutput (const TChar* name, SpeakerArrangement arr,
									   BusType busType, int32 flags)
{
	AudioBus* newBus = new AudioBus (name, busType, flags, arr);
	audioOutputs.push_back (IPtr<Vst::Bus> (newBus, false));
	return newBus;
}

//------------------------------------------------------------------------
AudioBus* AudioEffect::getAudioInput (int32 index)
{
	AudioBus* bus = FCast<Vst::AudioBus> (audioInputs.at (index));
	return bus;
}

//------------------------------------------------------------------------
AudioBus* AudioEffect::getAudioOutput (int32 index)
{
	AudioBus* bus = FCast<Vst::AudioBus> (audioOutputs.at (index));
	return bus;
}

//------------------------------------------------------------------------
EventBus* AudioEffect::addEventInput (const TChar* name, int32 channels,
									  BusType busType, int32 flags)
{
	EventBus* newBus = new EventBus (name, busType, flags, channels);
	eventInputs.push_back (IPtr<Vst::Bus> (newBus, false));
	return newBus;
}

//------------------------------------------------------------------------
EventBus* AudioEffect::addEventOutput (const TChar* name, int32 channels,
									   BusType busType, int32 flags)
{
	EventBus* newBus = new EventBus (name, busType, flags, channels);
	eventOutputs.push_back (IPtr<Vst::Bus> (newBus, false));
	return newBus;
}

//------------------------------------------------------------------------
EventBus* AudioEffect::getEventInput (int32 index)
{
	EventBus* bus = FCast<Vst::EventBus> (eventInputs.at (index));
	return bus;
}

//------------------------------------------------------------------------
EventBus* AudioEffect::getEventOutput (int32 index)
{
	EventBus* bus = FCast<Vst::EventBus> (eventOutputs.at (index));
	return bus;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AudioEffect::setBusArrangements (SpeakerArrangement* inputs, int32 numIns,
													SpeakerArrangement* outputs, int32 numOuts)
{
	if (numIns < 0 || numOuts < 0)
		return kInvalidArgument;

	if (numIns > static_cast<int32> (audioInputs.size ()) ||
	    numOuts > static_cast<int32> (audioOutputs.size ()))
		return kResultFalse;

	for (int32 index = 0; index < audioInputs.size (); ++index)
	{
		if (index >= numIns)
			break;
		FCast<Vst::AudioBus> (audioInputs[index].get ())->setArrangement (inputs[index]);
	}

	for (int32 index = 0; index < audioOutputs.size (); ++index)
	{
		if (index >= numOuts)
			break;
		FCast<Vst::AudioBus> (audioOutputs[index].get ())->setArrangement (outputs[index]);
	}

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AudioEffect::getBusArrangement (BusDirection dir, int32 busIndex, SpeakerArrangement& arr)
{
	BusList* busList = getBusList (kAudio, dir);
	if (!busList || busIndex < 0 || busList->size () <= busIndex)
		return kInvalidArgument;
	AudioBus* audioBus = FCast<Vst::AudioBus> (busList->at (busIndex));
	if (audioBus)
	{
		arr = audioBus->getArrangement ();
		return kResultTrue;
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AudioEffect::setupProcessing (ProcessSetup& newSetup)
{
	processSetup.maxSamplesPerBlock = newSetup.maxSamplesPerBlock;
	processSetup.processMode = newSetup.processMode;
	processSetup.sampleRate = newSetup.sampleRate;

	if (canProcessSampleSize (newSetup.symbolicSampleSize) != kResultTrue)
		return kResultFalse;

	processSetup.symbolicSampleSize = newSetup.symbolicSampleSize;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AudioEffect::setProcessing (TBool /*state*/)
{
	return kNotImplemented;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AudioEffect::canProcessSampleSize (int32 symbolicSampleSize)
{
	return symbolicSampleSize == kSample32 ? kResultTrue : kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AudioEffect::process (ProcessData& /*data*/)
{
	return kNotImplemented;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
