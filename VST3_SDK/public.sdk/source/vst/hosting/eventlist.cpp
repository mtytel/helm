//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/hosting/eventlist.cpp
// Created by  : Steinberg, 03/05/2008.
// Description : VST 3 event list implementation
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

#include "eventlist.h"

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
IMPLEMENT_FUNKNOWN_METHODS (EventList, IEventList, IEventList::iid)

//-----------------------------------------------------------------------------
EventList::EventList (int32 maxSize) : events (nullptr), maxSize (maxSize), fillCount (0)
{
	FUNKNOWN_CTOR
	setMaxSize (maxSize);
}

//-----------------------------------------------------------------------------
EventList::~EventList ()
{
	setMaxSize (0);
	FUNKNOWN_DTOR
}

//-----------------------------------------------------------------------------
void EventList::setMaxSize (int32 _maxSize)
{
	if (events)
	{
		delete[] events;
		events = nullptr;
	}
	if (_maxSize > 0)
		events = new Event[_maxSize];
	maxSize = _maxSize;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API EventList::getEvent (int32 index, Event& e)
{
	if (fillCount > index)
	{
		memcpy (&e, &events[index], sizeof (Event));
		return kResultTrue;
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API EventList::addEvent (Event& e)
{
	if (maxSize > fillCount)
	{
		memcpy (&events[fillCount], &e, sizeof (Event));
		fillCount++;
		return kResultTrue;
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
Event* EventList::getEventByIndex (int32 index)
{
	if (index < fillCount)
		return &events[index];
	return nullptr;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
