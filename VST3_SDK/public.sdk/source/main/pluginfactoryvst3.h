//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Common Base Classes
// Filename    : public.sdk/source/main/pluginfactoryvst3.h
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

#pragma once

#include "pluginterfaces/base/ipluginbase.h"

namespace Steinberg {

//------------------------------------------------------------------------
/** Default Class Factory implementation.
\ingroup sdkBase
\see \ref classFactoryMacros */
//------------------------------------------------------------------------
class CPluginFactory : public IPluginFactory3
{
public:
//------------------------------------------------------------------------
	CPluginFactory (const PFactoryInfo& info);
	virtual ~CPluginFactory ();

//------------------------------------------------------------------------
	/** Registers a Plug-in class with classInfo version 1, returns true for success. */
	bool registerClass (const PClassInfo* info,
						FUnknown* (*createFunc)(void*),
						void* context = 0);

	/** Registers a Plug-in class with classInfo version 2, returns true for success. */
	bool registerClass (const PClassInfo2* info,
						FUnknown* (*createFunc)(void*),
						void* context = 0);

	/** Registers a Plug-in class with classInfo Unicode version, returns true for success. */
	bool registerClass (const PClassInfoW* info,
						FUnknown* (*createFunc)(void*),
						void* context = 0);


	/** Check if a class for a given classId is already registered. */
	bool isClassRegistered (const FUID& cid);

//------------------------------------------------------------------------
	DECLARE_FUNKNOWN_METHODS

	//---from IPluginFactory------
	tresult PLUGIN_API getFactoryInfo (PFactoryInfo* info) SMTG_OVERRIDE;
	int32 PLUGIN_API countClasses () SMTG_OVERRIDE;
	tresult PLUGIN_API getClassInfo (int32 index, PClassInfo* info) SMTG_OVERRIDE;
	tresult PLUGIN_API createInstance (FIDString cid, FIDString _iid, void** obj) SMTG_OVERRIDE;

	//---from IPluginFactory2-----
	tresult PLUGIN_API getClassInfo2 (int32 index, PClassInfo2* info) SMTG_OVERRIDE;

	//---from IPluginFactory3-----
	tresult PLUGIN_API getClassInfoUnicode (int32 index, PClassInfoW* info) SMTG_OVERRIDE;
	tresult PLUGIN_API setHostContext (FUnknown* context) SMTG_OVERRIDE;

//------------------------------------------------------------------------
protected:
	/// @cond
	struct PClassEntry
	{
	//-----------------------------------
		PClassInfo2 info8;
		PClassInfoW info16;

		FUnknown* (*createFunc)(void*);
		void* context;
		bool isUnicode;
	//-----------------------------------
	};
	/// @endcond

	PFactoryInfo factoryInfo;
	PClassEntry* classes;
	int32 classCount;
	int32 maxClassCount;

	bool growClasses ();
};

extern CPluginFactory* gPluginFactory;
//------------------------------------------------------------------------
} // namespace Steinberg


//------------------------------------------------------------------------
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
	#define EXPORT_FACTORY	__attribute__ ((visibility ("default")))
#else
	#define EXPORT_FACTORY
#endif

//------------------------------------------------------------------------
/** \defgroup classFactoryMacros Macros for defining the class factory
\ingroup sdkBase

\b Example - How to use the class factory macros:
\code
BEGIN_FACTORY ("Steinberg Technologies", 
			   "http://www.steinberg.de", 
			   "mailto:info@steinberg.de", 
			   PFactoryInfo::kNoFlags)

DEF_CLASS (INLINE_UID (0x00000000, 0x00000000, 0x00000000, 0x00000000),
			PClassInfo::kManyInstances,    
			"Service",
			"Test Service",
			TestService::newInstance)

END_FACTORY
\endcode 

@{*/

#define BEGIN_FACTORY(vendor,url,email,flags) using namespace Steinberg; \
	EXPORT_FACTORY IPluginFactory* PLUGIN_API GetPluginFactory () { \
	if (!gPluginFactory) \
	{	static PFactoryInfo factoryInfo (vendor,url,email,flags); \
		gPluginFactory = new CPluginFactory (factoryInfo); \

#define DEF_CLASS(cid,cardinality,category,name,createMethod) \
	{ TUID lcid = cid; static PClassInfo componentClass (lcid,cardinality,category,name); \
	gPluginFactory->registerClass (&componentClass,createMethod); }

#define DEF_CLASS1(cid,cardinality,category,name,createMethod) \
	{ static PClassInfo componentClass (cid,cardinality,category,name); \
	gPluginFactory->registerClass (&componentClass,createMethod); }

#define DEF_CLASS2(cid,cardinality,category,name,classFlags,subCategories,version,sdkVersion,createMethod) \
	{ TUID lcid = cid; static PClassInfo2 componentClass (lcid,cardinality,category,name,classFlags,subCategories, 0 ,version,sdkVersion);\
	gPluginFactory->registerClass (&componentClass,createMethod); }

#define DEF_CLASS_W(cid,cardinality,category,name,classFlags,subCategories,version,sdkVersion,createMethod) \
	{ TUID lcid = cid; static PClassInfoUnicode componentClass (lcid,cardinality,category,name,classFlags,subCategories, 0,version,sdkVersion);\
	gPluginFactory->registerClass (&componentClass,createMethod); }


#define END_FACTORY	} else gPluginFactory->addRef (); \
	return gPluginFactory; }

/** @} */
