//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/hosting/parameterchanges.cpp
// Created by  : Steinberg, 03/05/2008.
// Description : VST 3 parameter changes implementation
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

#include "parameterchanges.h"

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
IMPLEMENT_FUNKNOWN_METHODS (ParameterChanges, IParameterChanges, IParameterChanges::iid)
IMPLEMENT_FUNKNOWN_METHODS (ParameterValueQueue, IParamValueQueue, IParamValueQueue::iid)


const int32 kQueueReservedPoints = 5;

//-----------------------------------------------------------------------------
ParameterValueQueue::ParameterValueQueue (ParamID paramID) 
: paramID (paramID)
{ 
	values.reserve (kQueueReservedPoints);
	FUNKNOWN_CTOR
}

//-----------------------------------------------------------------------------
ParameterValueQueue::~ParameterValueQueue () 
{ 
	FUNKNOWN_DTOR 
}

//-----------------------------------------------------------------------------
void ParameterValueQueue::clear ()
{
	values.clear ();
}

//-----------------------------------------------------------------------------
int32 PLUGIN_API ParameterValueQueue::getPointCount () 
{ 
	return (int32)values.size ();
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API ParameterValueQueue::getPoint (int32 index, int32& sampleOffset, ParamValue& value)
{
	if (index < (int32)values.size ())
	{
		const ParameterQueueValue& queueValue = values[index];
		sampleOffset = queueValue.sampleOffset;
		value = queueValue.value;
		return kResultTrue;
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API ParameterValueQueue::addPoint (int32 sampleOffset, ParamValue value, int32& index)
{
	int32 destIndex = (int32)values.size ();
	for (uint32 i = 0; i < values.size (); i++)
	{
		if (values[i].sampleOffset == sampleOffset)
		{
			values[i].value = value;
			index = i;
			return kResultTrue;
		}
		else if (values[i].sampleOffset > sampleOffset)
		{
			destIndex = i;
			break;
		}
	}

	// need new point
	ParameterQueueValue queueValue (value, sampleOffset);
	if (destIndex == (int32)values.size ())
		values.push_back (queueValue);
	else
		values.insert (values.begin () + destIndex, queueValue);

	index = destIndex;

	return kResultTrue;
}

//-----------------------------------------------------------------------------
// ParameterChanges
//-----------------------------------------------------------------------------
ParameterChanges::ParameterChanges (int32 maxParameters)
: usedQueueCount (0)
{
	FUNKNOWN_CTOR
	setMaxParameters (maxParameters);
}

//-----------------------------------------------------------------------------
ParameterChanges::~ParameterChanges ()
{
	setMaxParameters (0);

	FUNKNOWN_DTOR
}

//-----------------------------------------------------------------------------
void ParameterChanges::setMaxParameters (int32 maxParameters)
{
	if (maxParameters < 0)
		return;

	while ((int32)queues.size () < maxParameters)
	{
		ParameterValueQueue* valueQueue = new ParameterValueQueue (0xffffffff);
		queues.push_back (valueQueue);
	}

	while ((int32)queues.size () > maxParameters)
	{
		queues.back ()->release ();
		queues.pop_back ();
	}
	if (usedQueueCount > maxParameters)
		usedQueueCount = maxParameters;
}

//-----------------------------------------------------------------------------
void ParameterChanges::clearQueue ()
{
	usedQueueCount = 0;
}

//-----------------------------------------------------------------------------
int32 PLUGIN_API ParameterChanges::getParameterCount ()
{
	return usedQueueCount;
}

//-----------------------------------------------------------------------------
IParamValueQueue* PLUGIN_API ParameterChanges::getParameterData (int32 index)
{
	if (index < usedQueueCount)
		return queues[index];
	return nullptr;
}

//-----------------------------------------------------------------------------
IParamValueQueue* PLUGIN_API ParameterChanges::addParameterData (const ParamID& pid, int32& index)
{
	for (int32 i = 0; i < usedQueueCount; i++)
	{
		if (queues[i]->getParameterId () == pid)
		{
			index = i;
			return queues[i];
		}
	}
	
	ParameterValueQueue* valueQueue = nullptr;
	if (usedQueueCount < (int32)queues.size ())
	{
		valueQueue = queues[usedQueueCount];
		valueQueue->setParamID (pid);
		valueQueue->clear ();
	}
	else
	{
		valueQueue = new ParameterValueQueue (pid);
		queues.push_back (valueQueue);
	}
	
	index = usedQueueCount;
	usedQueueCount++;
	return valueQueue;
}


//-----------------------------------------------------------------------------
// ParameterChangeTransfer
//-----------------------------------------------------------------------------
ParameterChangeTransfer::ParameterChangeTransfer (int32 maxParameters)
: size (0)
, changes (nullptr)
, readIndex (0)
, writeIndex (0)
{
	setMaxParameters (maxParameters);
}
	
//-----------------------------------------------------------------------------
ParameterChangeTransfer::~ParameterChangeTransfer ()
{
	setMaxParameters (0);
}

//-----------------------------------------------------------------------------
void ParameterChangeTransfer::setMaxParameters (int32 maxParameters)
{
	// reserve memory for twice the amount of all parameters
	int32 newSize = maxParameters * 2; 
	if (size != newSize)
	{
		if (changes)
			delete [] changes;
		changes = nullptr;
		size = newSize;
		if (size > 0)
			changes = new ParameterChange [size];
	}
}

//-----------------------------------------------------------------------------
void ParameterChangeTransfer::addChange (ParamID pid, ParamValue value, int32 sampleOffset)
{
	if (changes)
	{
		changes[writeIndex].id = pid;
		changes[writeIndex].value = value;
		changes[writeIndex].sampleOffset = sampleOffset;

		int32 newWriteIndex = writeIndex + 1;
		if (newWriteIndex >= size)
			newWriteIndex = 0;
		if (readIndex != newWriteIndex)
			writeIndex = newWriteIndex;
	}
}

//-----------------------------------------------------------------------------
bool ParameterChangeTransfer::getNextChange (ParamID& pid, ParamValue& value, int32& sampleOffset)
{
	if (!changes)
		return false;
	
	int32 currentWriteIndex = writeIndex;
	if (readIndex != currentWriteIndex)
	{
		pid = changes [readIndex].id;
		value = changes [readIndex].value;
		sampleOffset = changes [readIndex].sampleOffset;

		int32 newReadIndex = readIndex + 1;
		if (newReadIndex >= size)
			newReadIndex = 0;
		readIndex = newReadIndex;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
void ParameterChangeTransfer::transferChangesTo (ParameterChanges& dest)
{
	ParamID pid;
	ParamValue value;
	int32 sampleOffset;
	int32 index;
	
	while (getNextChange (pid, value, sampleOffset))
	{
		IParamValueQueue* queue = dest.addParameterData (pid, index);
		if (queue)
		{
			queue->addPoint (sampleOffset, value, index);
		}
	}
}

//-----------------------------------------------------------------------------
void ParameterChangeTransfer::transferChangesFrom (ParameterChanges& source)
{
	ParamValue value;
	int32 sampleOffset;
	for (int32 i = 0; i < source.getParameterCount (); i++)
	{
		IParamValueQueue* queue = source.getParameterData (i);
		if (queue)
		{
			for (int32 j = 0; j < queue->getPointCount (); j++)
			{
				if (queue->getPoint (j, sampleOffset, value) == kResultTrue)
				{
					addChange (queue->getParameterId (), value, sampleOffset);
				}
			}
		}
	}
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
