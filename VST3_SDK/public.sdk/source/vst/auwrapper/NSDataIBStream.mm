//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/auwrapper/NSDataIBStream.mm
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

#include "NSDataIBStream.h"

#include "pluginterfaces/vst/ivstattributes.h"

#include <algorithm>

#if __clang__
#if __has_feature(objc_arc) && __clang_major__ >= 3
#define ARC_ENABLED 1
#endif // __has_feature(objc_arc)
#endif // __clang__

namespace Steinberg {
namespace Vst {

DEF_CLASS_IID(IStreamAttributes);

//------------------------------------------------------------------------
NSDataIBStream::NSDataIBStream (NSData* data, bool hideAttributes)
: data (data)
, currentPos (0)
, hideAttributes (hideAttributes)
{
	FUNKNOWN_CTOR
#if !ARC_ENABLED
	[data retain];
#endif
}

//------------------------------------------------------------------------
NSDataIBStream::~NSDataIBStream ()
{
#if !ARC_ENABLED
	[data release];
#endif
	FUNKNOWN_DTOR
}

//------------------------------------------------------------------------
IMPLEMENT_REFCOUNT (NSDataIBStream)

//------------------------------------------------------------------------
tresult PLUGIN_API NSDataIBStream::queryInterface (const TUID iid, void** obj)
{
	QUERY_INTERFACE (iid, obj, FUnknown::iid, IBStream)
	QUERY_INTERFACE (iid, obj, IBStream::iid, IBStream)
	if (!hideAttributes)
		QUERY_INTERFACE (iid, obj, IStreamAttributes::iid, IStreamAttributes)
	*obj = 0;
	return kNoInterface;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NSDataIBStream::read (void* buffer, int32 numBytes, int32* numBytesRead)
{
	int32 useBytes = std::min (numBytes, (int32)([data length] - currentPos));
	if (useBytes > 0)
	{
		[data getBytes: buffer range: NSMakeRange (currentPos, useBytes)];
		if (numBytesRead)
			*numBytesRead = useBytes;
		currentPos += useBytes;
		return kResultTrue;
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NSDataIBStream::write (void* buffer, int32 numBytes, int32* numBytesWritten)
{
	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NSDataIBStream::seek (int64 pos, int32 mode, int64* result)
{
	switch (mode)
	{
		case kIBSeekSet:
		{
			if (pos <= [data length])
			{
				currentPos = pos;
				if (result)
					tell (result);
				return kResultTrue;
			}
			break;
		}
		case kIBSeekCur:
		{
			if (currentPos + pos <= [data length])
			{
				currentPos += pos;
				if (result)
					tell (result);
				return kResultTrue;
			}
			break;
		}
		case kIBSeekEnd:
		{
			if ([data length] + pos <= [data length])
			{
				currentPos = [data length] + pos;
				if (result)
					tell (result);
				return kResultTrue;
			}
			break;
		}
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NSDataIBStream::tell (int64* pos)
{
	if (pos)
	{
		*pos = currentPos;
		return kResultTrue;
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NSDataIBStream::getFileName (String128 name)
{
	return kNotImplemented;
}

//------------------------------------------------------------------------
IAttributeList* PLUGIN_API NSDataIBStream::getAttributes ()
{
	return hideAttributes ? 0 : &attrList;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
NSMutableDataIBStream::NSMutableDataIBStream (NSMutableData* data)
: NSDataIBStream (data, true)
, mdata (data)
{
}

//------------------------------------------------------------------------
NSMutableDataIBStream::~NSMutableDataIBStream ()
{
	[mdata setLength:currentPos];
}

//------------------------------------------------------------------------
tresult PLUGIN_API NSMutableDataIBStream::write (void* buffer, int32 numBytes, int32* numBytesWritten)
{
	[mdata replaceBytesInRange:NSMakeRange (currentPos, numBytes) withBytes:buffer];
	if (numBytesWritten)
		*numBytesWritten = numBytes;
	currentPos += numBytes;
	return kResultTrue;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg

/// \endcond
