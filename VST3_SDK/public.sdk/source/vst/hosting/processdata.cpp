//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/hosting/processdata.cpp
// Created by  : Steinberg, 10/2005
// Description : VST Hosting Utilities
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

#include "processdata.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// HostProcessData
//------------------------------------------------------------------------
HostProcessData::~HostProcessData ()
{
	unprepare ();
}

//------------------------------------------------------------------------
bool HostProcessData::prepare (IComponent& component, int32 bufferSamples,
                               int32 _symbolicSampleSize)
{
	if (checkIfReallocationNeeded (component, bufferSamples, _symbolicSampleSize))
	{
		unprepare ();

		symbolicSampleSize = _symbolicSampleSize;
		channelBufferOwner = bufferSamples > 0;

		numInputs = createBuffers (component, inputs, kInput, bufferSamples);
		numOutputs = createBuffers (component, outputs, kOutput, bufferSamples);
	}
	else
	{
		// reset silence flags
		for (int32 i = 0; i < numInputs; i++)
		{
			inputs[i].silenceFlags = 0;
		}
		for (int32 i = 0; i < numOutputs; i++)
		{
			outputs[i].silenceFlags = 0;
		}
	}

	return true;
}

//------------------------------------------------------------------------
void HostProcessData::unprepare ()
{
	destroyBuffers (inputs, numInputs);
	destroyBuffers (outputs, numOutputs);

	channelBufferOwner = false;
}

//------------------------------------------------------------------------
bool HostProcessData::checkIfReallocationNeeded (IComponent& component, int32 bufferSamples,
                                                 int32 _symbolicSampleSize)
{
	if (channelBufferOwner != (bufferSamples > 0))
		return true;
	if (symbolicSampleSize != _symbolicSampleSize)
		return true;

	int32 inBusCount = component.getBusCount (kAudio, kInput);
	if (inBusCount != numInputs)
		return true;

	int32 outBusCount = component.getBusCount (kAudio, kOutput);
	if (outBusCount != numOutputs)
		return true;

	for (int32 i = 0; i < inBusCount; i++)
	{
		BusInfo busInfo = {0};

		if (component.getBusInfo (kAudio, kInput, i, busInfo) == kResultTrue)
		{
			if (inputs[i].numChannels != busInfo.channelCount)
				return true;
		}
	}
	for (int32 i = 0; i < outBusCount; i++)
	{
		BusInfo busInfo = {0};

		if (component.getBusInfo (kAudio, kOutput, i, busInfo) == kResultTrue)
		{
			if (outputs[i].numChannels != busInfo.channelCount)
				return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
int32 HostProcessData::createBuffers (IComponent& component, AudioBusBuffers*& buffers,
                                      BusDirection dir, int32 bufferSamples)
{
	int32 busCount = component.getBusCount (kAudio, dir);
	if (busCount > 0)
	{
		buffers = new AudioBusBuffers[busCount];

		for (int32 i = 0; i < busCount; i++)
		{
			BusInfo busInfo = {0};

			if (component.getBusInfo (kAudio, dir, i, busInfo) == kResultTrue)
			{
				buffers[i].numChannels = busInfo.channelCount;

				// allocate for each channel
				if (busInfo.channelCount > 0)
				{
					if (symbolicSampleSize == kSample64)
						buffers[i].channelBuffers64 = new Sample64*[busInfo.channelCount];
					else
						buffers[i].channelBuffers32 = new Sample32*[busInfo.channelCount];

					for (int32 j = 0; j < busInfo.channelCount; j++)
					{
						if (symbolicSampleSize == kSample64)
						{
							if (bufferSamples > 0)
								buffers[i].channelBuffers64[j] = new Sample64[bufferSamples];
							else
								buffers[i].channelBuffers64[j] = 0;
						}
						else
						{
							if (bufferSamples > 0)
								buffers[i].channelBuffers32[j] = new Sample32[bufferSamples];
							else
								buffers[i].channelBuffers32[j] = 0;
						}
					}
				}
			}
		}
	}
	return busCount;
}

//-----------------------------------------------------------------------------
void HostProcessData::destroyBuffers (AudioBusBuffers*& buffers, int32& busCount)
{
	if (buffers)
	{
		for (int32 i = 0; i < busCount; i++)
		{
			if (channelBufferOwner)
			{
				for (int32 j = 0; j < buffers[i].numChannels; j++)
				{
					if (symbolicSampleSize == kSample64)
					{
						if (buffers[i].channelBuffers64[j])
							delete[] buffers[i].channelBuffers64[j];
					}
					else
					{
						if (buffers[i].channelBuffers32[j])
							delete[] buffers[i].channelBuffers32[j];
					}
				}
			}

			if (symbolicSampleSize == kSample64)
			{
				if (buffers[i].channelBuffers64)
					delete[] buffers[i].channelBuffers64;
			}
			else
			{
				if (buffers[i].channelBuffers32)
					delete[] buffers[i].channelBuffers32;
			}
		}

		delete[] buffers;
		buffers = 0;
	}
	busCount = 0;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
