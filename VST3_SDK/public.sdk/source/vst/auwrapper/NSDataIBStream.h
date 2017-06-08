//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/auwrapper/NSDataIBStream.h
// Created by  : Steinberg, 12/2007
// Description : VST 3 -> AU Wrapper
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

/// \cond ignore

#pragma once

#import <Foundation/Foundation.h>
#import "pluginterfaces/base/ibstream.h"
#import "public.sdk/source/vst/hosting/hostclasses.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
class NSDataIBStream : public IBStream, Vst::IStreamAttributes
{
public:
	NSDataIBStream (NSData* data, bool hideAttributes = false);
	virtual ~NSDataIBStream ();

	//---from IBStream-------------------
	tresult PLUGIN_API read  (void* buffer, int32 numBytes, int32* numBytesRead = 0) SMTG_OVERRIDE;
	tresult PLUGIN_API write (void* buffer, int32 numBytes, int32* numBytesWritten = 0) SMTG_OVERRIDE;
	tresult PLUGIN_API seek  (int64 pos, int32 mode, int64* result = 0) SMTG_OVERRIDE;
	tresult PLUGIN_API tell  (int64* pos) SMTG_OVERRIDE;

	//---from Vst::IStreamAttributes-----
	virtual tresult PLUGIN_API getFileName (String128 name) SMTG_OVERRIDE;
	virtual IAttributeList* PLUGIN_API getAttributes () SMTG_OVERRIDE;

	//------------------------------------------------------------------------
	DECLARE_FUNKNOWN_METHODS
protected:
	NSData* data;
	int64 currentPos;
	HostAttributeList attrList;
	bool hideAttributes;
};

//------------------------------------------------------------------------
class NSMutableDataIBStream : public NSDataIBStream
{
public:
	NSMutableDataIBStream (NSMutableData* data);
	virtual ~NSMutableDataIBStream ();

	tresult PLUGIN_API write (void* buffer, int32 numBytes, int32* numBytesWritten = 0);
//------------------------------------------------------------------------
protected:
	NSMutableData* mdata;
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg

/// \endcond
