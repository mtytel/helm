//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/vstbypassprocessor.h
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

#pragma once

#include "pluginterfaces/vst/ivstaudioprocessor.h"

namespace Steinberg {
namespace Vst {

#define kMaxChannelsSupported 64

//------------------------------------------------------------------------
// AudioBuffer
//------------------------------------------------------------------------
class AudioBuffer
{
public:
//------------------------------------------------------------------------
	AudioBuffer ();
	~AudioBuffer ();

	int32 getMaxSamples () const { return mMaxSamples; }
	void resize (int32 _maxSamples);
	void release () { resize (0); }
	void clear (int32 numSamples);
	void clearAll () { if (mMaxSamples > 0) clear (mMaxSamples); }

	operator float* () { return mBuffer; }
//------------------------------------------------------------------------
protected:
	float* mBuffer;
	int32 mMaxSamples;
};

//------------------------------------------------------------------------
// BypassProcessor
//------------------------------------------------------------------------
class BypassProcessor
{
public:
//------------------------------------------------------------------------
	BypassProcessor ();
	~BypassProcessor ();

	void setup (IAudioProcessor& audioProcessor, ProcessSetup& processSetup, int32 delaySamples);
	void reset ();

	bool isActive () const { return mActive; }
	void setActive (bool state);

	void process (ProcessData& data);
//------------------------------------------------------------------------
protected:
	bool mActive;
	bool mMainIOBypass;
	int32 mInputPinLookup[kMaxChannelsSupported];

	struct Delay
	{
		AudioBuffer mDelayBuffer;
		int32 mDelaySamples;
		int32 mInPos;
		int32 mOutPos;

		Delay (int32 maxSamplesPerBlock, int32 delaySamples)
		: mDelaySamples (delaySamples), mInPos (-1), mOutPos (-1)
		{
			if (mDelaySamples > 0)
				mDelayBuffer.resize (maxSamplesPerBlock + mDelaySamples);
		}

		bool hasDelay () const { return mDelaySamples > 0; }
		int32 getBufferSamples () const { return mDelayBuffer.getMaxSamples (); }

		bool process (float* src, float* dst, int32 numSamples, bool silentIn);
		void flush ();
	};

	Delay* mDelays[kMaxChannelsSupported];
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
