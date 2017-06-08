//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/hosting/eventlist.h
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

#pragma once

#include "pluginterfaces/vst/ivstevents.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
/** Implementation's example of IEventList.
\ingroup sdkBase
*/
class EventList : public IEventList
{
public:
	EventList (int32 maxSize = 50);
	virtual ~EventList ();

	void clear () { fillCount = 0; }

	int32 PLUGIN_API getEventCount () SMTG_OVERRIDE { return fillCount; }
	tresult PLUGIN_API getEvent (int32 index, Event& e) SMTG_OVERRIDE;
	tresult PLUGIN_API addEvent (Event& e) SMTG_OVERRIDE;

	Event* getEventByIndex (int32 index);
	void setMaxSize (int32 maxSize);

//------------------------------------------------------------------------
	DECLARE_FUNKNOWN_METHODS
protected:
	Event* events;
	int32 maxSize;
	int32 fillCount;
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
