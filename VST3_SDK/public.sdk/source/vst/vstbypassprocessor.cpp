//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/vstbypassprocessor.cpp
// Created by  : Steinberg, 04/2015
// Description : Example of bypass support Implementation
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

#include "vstbypassprocessor.h"
#include "vstspeakerarray.h"

#include <cstdlib>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
static bool delay (int32 sampleFrames, float* inStream, float* outStream, float* delayBuffer,
                   int32 bufferSize, int32 bufferInPos, int32 bufferOutPos)
{
	// delay inStream
	int32 remain, inFrames, outFrames;
	float* bufIn;
	float* bufOut;

	remain = sampleFrames;
	while (remain > 0)
	{
		bufIn = delayBuffer + bufferInPos;
		bufOut = delayBuffer + bufferOutPos;

		if (bufferInPos > bufferOutPos)
			inFrames = bufferSize - bufferInPos;
		else
			inFrames = bufferOutPos - bufferInPos;

		outFrames = bufferSize - bufferOutPos;

		if (inFrames > remain)
			inFrames = remain;

		if (outFrames > inFrames)
			outFrames = inFrames;

		// order important for in-place processing!
		memcpy (bufIn, inStream, inFrames * sizeof (float)); // copy to buffer
		memcpy (outStream, bufOut, outFrames * sizeof (float)); // copy from buffer

		inStream += inFrames;
		outStream += outFrames;

		bufferInPos += inFrames;
		if (bufferInPos >= bufferSize)
			bufferInPos -= bufferSize;
		bufferOutPos += outFrames;
		if (bufferOutPos >= bufferSize)
			bufferOutPos -= bufferSize;

		if (inFrames > outFrames)
		{
			// still some output to copy
			bufOut = delayBuffer + bufferOutPos;
			outFrames = inFrames - outFrames;

			memcpy (outStream, bufOut, outFrames * sizeof (float)); // copy from buffer

			outStream += outFrames;

			bufferOutPos += outFrames;
			if (bufferOutPos >= bufferSize)
				bufferOutPos -= bufferSize;
		}

		remain -= inFrames;
	}

	return true;
}


//------------------------------------------------------------------------
// AudioBuffer Implementation
//------------------------------------------------------------------------
AudioBuffer::AudioBuffer ()
: mBuffer (nullptr)
, mMaxSamples (0)
{}

//------------------------------------------------------------------------
AudioBuffer::~AudioBuffer ()
{
	resize (0);
}

//------------------------------------------------------------------------
void AudioBuffer::resize (int32 _maxSamples)
{
	if (mMaxSamples != _maxSamples)
	{
		mMaxSamples = _maxSamples;
		if (mMaxSamples <= 0)
		{
			if (mBuffer)
				free (mBuffer),
				mBuffer = nullptr;
		}
		else
		{
			if (mBuffer)
				mBuffer = (float*)realloc (mBuffer, mMaxSamples * sizeof (float));
			else
				mBuffer = (float*)malloc (mMaxSamples * sizeof (float));
		}
	}
}

//------------------------------------------------------------------------
void AudioBuffer::clear (int32 numSamples)
{
	if (mBuffer)
	{
		int32 count = numSamples < mMaxSamples ? numSamples : mMaxSamples;
		memset (mBuffer, 0, count * sizeof (float));
	}
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
// BypassProcessor Implementation
//------------------------------------------------------------------------
BypassProcessor::BypassProcessor () : mActive (false), mMainIOBypass (false)
{
	for (int32 i = 0; i < kMaxChannelsSupported; i++)
	{
		mInputPinLookup[i] = -1;
		mDelays[i] = nullptr;
	}
}

//------------------------------------------------------------------------
BypassProcessor::~BypassProcessor () { reset (); }

//------------------------------------------------------------------------
void BypassProcessor::setup (IAudioProcessor& audioProcessor, ProcessSetup& processSetup,
                             int32 delaySamples)
{
	reset ();
	
	SpeakerArrangement inputArr = 0;
	bool hasInput = audioProcessor.getBusArrangement (kInput, 0, inputArr) == kResultOk;

	SpeakerArrangement outputArr = 0;
	bool hasOutput = audioProcessor.getBusArrangement (kOutput, 0, outputArr) == kResultOk;

	mMainIOBypass = hasInput && hasOutput;
	if (!mMainIOBypass)
		return;

	// create lookup table (in <- out) and delays...
	SpeakerArray inArray (inputArr);
	SpeakerArray outArray (outputArr);

	// security check (todo)
	if (outArray.total () >= kMaxChannelsSupported)
		return;

	for (int32 i = 0; i < outArray.total (); i++)
	{
		if (outArray.at (i) == Vst::kSpeakerL)
		{
			if (inArray.total () == 1 && inArray.at (0) == Vst::kSpeakerM)
			{
				mInputPinLookup[i] = 0;
			}
			else
				mInputPinLookup[i] = inArray.getSpeakerIndex (outArray.at (i));
		}
		else
			mInputPinLookup[i] = inArray.getSpeakerIndex (outArray.at (i));

		mDelays[i] = new Delay (processSetup.maxSamplesPerBlock, delaySamples);
		mDelays[i]->flush ();
	}
}

//------------------------------------------------------------------------
void BypassProcessor::reset ()
{
	mMainIOBypass = false;

	for (int32 i = 0; i < kMaxChannelsSupported; i++)
	{
		mInputPinLookup[i] = -1;
		if (mDelays[i])
		{
			delete mDelays[i];
			mDelays[i] = nullptr;
		}
	}
}

//------------------------------------------------------------------------
void BypassProcessor::setActive (bool state)
{
	if (mActive == state)
		return;

	mActive = state;

	// flush delays when turning on
	if (state && mMainIOBypass)
		for (int32 i = 0; i < kMaxChannelsSupported; i++)
		{
			if (!mDelays[i])
				break;
			mDelays[i]->flush ();
		}
}

//------------------------------------------------------------------------
void BypassProcessor::process (ProcessData& data)
{
	if (mMainIOBypass)
	{
		AudioBusBuffers& inBus = data.inputs[0];
		AudioBusBuffers& outBus = data.outputs[0];

		for (int32 channel = 0; channel < outBus.numChannels; channel++)
		{
			float* src = nullptr;
			bool silent = true;
			float* dst = outBus.channelBuffers32[channel];
			if (!dst)
				continue;

			int inputChannel = mInputPinLookup[channel];
			if (inputChannel != -1)
			{
				silent = (inBus.silenceFlags & (1ll << inputChannel)) != 0;
				src = inBus.channelBuffers32[inputChannel];
			}

			if (mDelays[channel]->process (src, dst, data.numSamples, silent))
			{
				outBus.silenceFlags |= (1ll << channel);
			}
			else
			{
				outBus.silenceFlags = 0;
			}
		}
	}

	// clear all other outputs
	for (int32 outBusIndex = mMainIOBypass ? 1 : 0; outBusIndex < data.numOutputs; outBusIndex++)
	{
		AudioBusBuffers& outBus = data.outputs[outBusIndex];
		if (!outBus.channelBuffers32)
			continue;

		for (int32 channel = 0; channel < outBus.numChannels; channel++)
		{
			float* dst = outBus.channelBuffers32[channel];
			if (dst)
			{
				memset (dst, 0, data.numSamples * sizeof (float));
				outBus.silenceFlags |= 1ll << channel;
			}
		}
	}
}

//------------------------------------------------------------------------
void BypassProcessor::Delay::flush ()
{
	mDelayBuffer.clearAll ();

	mInPos = mOutPos = 0;
	if (hasDelay ())
		mOutPos = getBufferSamples () - mDelaySamples; // must be != inPos
}

//------------------------------------------------------------------------
bool BypassProcessor::Delay::process (float* src, float* dst, int32 numSamples, bool silentIn)
{
	bool silentOut = false;

	if (hasDelay () && src)
	{
		int32 bufferSize = getBufferSamples ();
		delay (numSamples, src, dst, mDelayBuffer, bufferSize, mInPos, mOutPos);

		// update inPos, outPos
		mInPos += numSamples;
		if (mInPos >= bufferSize)
			mInPos -= bufferSize;
		mOutPos += numSamples;
		if (mOutPos >= bufferSize)
			mOutPos -= bufferSize;
	}
	else
	{
		if (src != dst)
		{
			if (src && !silentIn)
			{
				memcpy (dst, src, numSamples * sizeof (float));
			}
			else
			{
				memset (dst, 0, numSamples * sizeof (float));
				silentOut = true;
			}
		}
		else
		{
			silentOut = silentIn;
		}
	}
	return silentOut;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
