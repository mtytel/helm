//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Common Base Classes
// Filename    : public.sdk/source/main/pluginfactoryvst3.cpp
// Created by  : Steinberg, 01/2004
// Description : Standard Plug-in Factory
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

#include "pluginfactoryvst3.h"
#include "pluginterfaces/gui/iplugview.h"
#include "pluginterfaces/base/ibstream.h"

#include <stdlib.h>

namespace Steinberg {

CPluginFactory* gPluginFactory = 0;

DEF_CLASS_IID (IPluginBase)
DEF_CLASS_IID (IPlugView)
DEF_CLASS_IID (IPlugViewIdleHandler)
DEF_CLASS_IID (IPlugFrame)
DEF_CLASS_IID (IPlugFrameIdle)
DEF_CLASS_IID (IBStream)
DEF_CLASS_IID (IPluginFactory)
DEF_CLASS_IID (IPluginFactory2)
DEF_CLASS_IID (IPluginFactory3)
	
//------------------------------------------------------------------------
//  CPluginFactory implementation
//------------------------------------------------------------------------
CPluginFactory::CPluginFactory (const PFactoryInfo& info)
: classes (0)
, classCount (0)
, maxClassCount (0)
{
	FUNKNOWN_CTOR

	factoryInfo = info;
}

//------------------------------------------------------------------------
CPluginFactory::~CPluginFactory ()
{
	if (gPluginFactory == this)
		gPluginFactory = 0;

	if (classes)
		free (classes);

	FUNKNOWN_DTOR
}

//------------------------------------------------------------------------
IMPLEMENT_REFCOUNT (CPluginFactory)

//------------------------------------------------------------------------
tresult PLUGIN_API CPluginFactory::queryInterface (FIDString _iid, void** obj)
{
	QUERY_INTERFACE (_iid, obj, IPluginFactory::iid, IPluginFactory)
	QUERY_INTERFACE (_iid, obj, IPluginFactory2::iid, IPluginFactory2)
	QUERY_INTERFACE (_iid, obj, IPluginFactory3::iid, IPluginFactory3)
	QUERY_INTERFACE (_iid, obj, FUnknown::iid, IPluginFactory)
	*obj = 0;
	return kNoInterface;
}

//------------------------------------------------------------------------
bool CPluginFactory::registerClass (const PClassInfo* info,
									FUnknown* (*createFunc)(void*), void* context)
{
	if (!info || !createFunc)
		return false;

	PClassInfo2 info2;
	memcpy (&info2, info, sizeof (PClassInfo));
	return registerClass (&info2, createFunc, context);
}

//------------------------------------------------------------------------
bool CPluginFactory::registerClass (const PClassInfo2* info,
									FUnknown* (*createFunc)(void*), void* context)
{
	if (!info || !createFunc)
		return false;

	if (classCount >= maxClassCount)
	{
		if (!growClasses ())
			return false;
	}

	PClassEntry& entry = classes[classCount];
	entry.info8 = *info;
	entry.info16.fromAscii (*info);
	entry.createFunc = createFunc;
	entry.context = context;
	entry.isUnicode = false;

	classCount++;
	return true;
}

//------------------------------------------------------------------------
bool CPluginFactory::registerClass (const PClassInfoW* info,
								    FUnknown* (*createFunc)(void*), void* context)
{
	if (!info || !createFunc)
		return false;

	if (classCount >= maxClassCount)
	{
		if (!growClasses ())
			return false;
	}

	PClassEntry& entry = classes[classCount];
	entry.info16 = *info;
	entry.createFunc = createFunc;
	entry.context = context;
	entry.isUnicode = true;

	classCount++;
	return true;
}

//------------------------------------------------------------------------
bool CPluginFactory::growClasses ()
{
	static const int32 delta = 10;

	size_t size = (maxClassCount + delta) * sizeof (PClassEntry);
	void* memory = classes;

	if (!memory)
		memory = malloc (size);
	else
		memory = realloc (memory, size);

	if (!memory)
		return false;

	classes = (PClassEntry*)memory;
	maxClassCount += delta;
	return true;
}

//------------------------------------------------------------------------
bool CPluginFactory::isClassRegistered (const FUID& cid)
{
	for (int32 i = 0; i < classCount; i++)
	{
		if (cid == classes[i].info16.cid)
			return true;
	}
	return false;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CPluginFactory::getFactoryInfo (PFactoryInfo* info)
{
	if (info)
		memcpy (info, &factoryInfo, sizeof (PFactoryInfo));
	return kResultOk;
}

//------------------------------------------------------------------------
int32 PLUGIN_API CPluginFactory::countClasses ()
{
	return classCount;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CPluginFactory::getClassInfo (int32 index, PClassInfo* info)
{
	if (info && (index >= 0 && index < classCount))
	{
		if (classes[index].isUnicode)
		{
			memset (info, 0, sizeof (PClassInfo));
			return kResultFalse;
		}

		memcpy (info, &classes[index].info8, sizeof (PClassInfo));
		return kResultOk;
	}
	return kInvalidArgument;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CPluginFactory::getClassInfo2 (int32 index, PClassInfo2* info)
{
	if (info && (index >= 0 && index < classCount))
	{
		if (classes[index].isUnicode)
		{
			memset (info, 0, sizeof (PClassInfo2));
			return kResultFalse;
		}

		memcpy (info, &classes[index].info8, sizeof (PClassInfo2));
		return kResultOk;
	}
	return kInvalidArgument;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CPluginFactory::getClassInfoUnicode (int32 index, PClassInfoW* info)
{
	if (info && (index >= 0 && index < classCount))
	{
		memcpy (info, &classes[index].info16, sizeof (PClassInfoW));
		return kResultOk;
	}
	return kInvalidArgument;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CPluginFactory::createInstance (FIDString cid, FIDString _iid, void** obj)
{
	for (int32 i = 0; i < classCount; i++)
	{
		if (memcmp (classes[i].info16.cid, cid, sizeof (TUID)) == 0)
		{
			FUnknown* instance = classes[i].createFunc (classes[i].context);
			if (instance)
			{
				if (instance->queryInterface (_iid, obj) == kResultOk)
				{
					instance->release ();
					return kResultOk;
				}
				else
					instance->release ();
			}
			break;
		}
	}

	*obj = 0;
	return kNoInterface;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CPluginFactory::setHostContext (FUnknown* /*context*/)
{
	return kNotImplemented;
}

} // namespace Steinberg
