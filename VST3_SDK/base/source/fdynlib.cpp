//------------------------------------------------------------------------
// Project     : SDK Base
// Version     : 1.0
//
// Category    : Helpers
// Filename    : base/source/fdynlib.cpp
// Created by  : Steinberg, 1998
// Description : Dynamic library loading
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

#include "base/source/fdynlib.h"
#include "pluginterfaces/base/fstrdefs.h"
#include "base/source/fstring.h"

#if WINDOWS
#include <windows.h>

#elif MAC
#include <mach-o/dyld.h>
#include <CoreFoundation/CoreFoundation.h>

#if !TARGET_OS_IPHONE
static const Steinberg::tchar kUnixDelimiter = STR ('/');
#endif
#endif

namespace Steinberg {

#if MAC
#include <dlfcn.h>

// we ignore for the moment that the NSAddImage functions are deprecated
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

static bool CopyProcessPath (Steinberg::String& name)
{
	Dl_info info;
	if (dladdr ((const void*)CopyProcessPath, &info))
	{
		if (info.dli_fname)
		{
			name.assign (info.dli_fname);
			#ifdef UNICODE
			name.toWideString ();
			#endif
			return true;
		}
	}
	return false;
}

#endif

//------------------------------------------------------------------------
// FDynLibrary
//------------------------------------------------------------------------
FDynLibrary::FDynLibrary (const tchar* n, bool addExtension)
: isloaded (false)
, instance (0)
{
	if (n)
		init (n, addExtension);
}

//------------------------------------------------------------------------
FDynLibrary::~FDynLibrary ()
{
	unload ();
}

//------------------------------------------------------------------------
bool FDynLibrary::init (const tchar* n, bool addExtension)
{
	if (isLoaded ())
		return true;

	Steinberg::String name (n);
		
#if WINDOWS
	if (addExtension)
		name.append (STR (".dll"));

	instance = LoadLibrary (name);
	if (instance)
		isloaded = true;
		
#elif MAC
	isBundle = false;
	// first check if it is a bundle
	if (addExtension)
		name.append (STR (".bundle"));
	if (name.getChar16 (0) != STR('/')) // no absoltue path
	{
		Steinberg::String p;
		if (CopyProcessPath (p))
		{
			Steinberg::int32 index = p.findLast (STR ('/'));
			p.remove (index+1);
			name = p + name;
		}
	}
	CFStringRef fsString = (CFStringRef)name.toCFStringRef ();
	CFURLRef url = CFURLCreateWithFileSystemPath (NULL, fsString, kCFURLPOSIXPathStyle, true);
	if (url)
	{
		CFBundleRef bundle = CFBundleCreate (NULL, url);
		if (bundle)
		{
			if (CFBundleLoadExecutable (bundle))
			{
				isBundle = true;
				instance = (void*)bundle;
			}
			else
				CFRelease (bundle);
		}
		CFRelease (url);
	}
	CFRelease (fsString);

	name.assign (n);

#if !TARGET_OS_IPHONE
	if (!isBundle)
	{
		// now we check for a dynamic library
		firstSymbol = NULL;
		if (addExtension)
		{
			name.append (STR (".dylib"));
		}
		// Only if name is a relative path we use the Process Path as root:
		if (name[0] != kUnixDelimiter)
		{
			Steinberg::String p;
			if (CopyProcessPath (p))
			{
				Steinberg::int32 index = p.findLast (STR ("/"));
				p.remove (index+1);
				p.append (name);
				p.toMultiByte (Steinberg::kCP_Utf8);
				instance = (void*) NSAddImage (p, NSADDIMAGE_OPTION_RETURN_ON_ERROR);
			}
		}
		// Last but not least let the system search for it
		// 
		if (instance == 0)
		{
			name.toMultiByte (Steinberg::kCP_Utf8);
			instance = (void*) NSAddImage (name, NSADDIMAGE_OPTION_RETURN_ON_ERROR);
		}
	}
#endif // !TARGET_OS_IPHONE

	if (instance)
		isloaded = true;

#endif

	return isloaded;
}

//------------------------------------------------------------------------
bool FDynLibrary::unload ()
{
	if (!isLoaded ())
		return false;

#if WINDOWS
	FreeLibrary ((HINSTANCE)instance);
	
#elif MAC
	if (isBundle)
	{
		if (CFGetRetainCount ((CFTypeRef)instance) == 1)
			CFBundleUnloadExecutable ((CFBundleRef)instance);
		CFRelease ((CFBundleRef)instance);
	}
	else
	{
		// we don't use this anymore as the darwin dyld can't unload dynamic libraries yet and may crash
/*		if (firstSymbol)
		{
			NSModule module = NSModuleForSymbol ((NSSymbol)firstSymbol);
			if (module)
				NSUnLinkModule (module, NSUNLINKMODULE_OPTION_NONE);
		}*/
	}
#endif
	instance = 0;
	isloaded = false;
	return true;
}


//------------------------------------------------------------------------
void* FDynLibrary::getProcAddress (const char* name)
{
	if (!isloaded)
		return 0;
	
#if WINDOWS
	return (void*)GetProcAddress ((HINSTANCE)instance, name);
	
#elif MAC
	if (isBundle)
	{
		CFStringRef functionName = CFStringCreateWithCString (NULL, name, kCFStringEncodingASCII);
		void* result = CFBundleGetFunctionPointerForName ((CFBundleRef)instance, functionName);
		CFRelease (functionName);
		return result;
	}
#if !TARGET_OS_IPHONE
	else
	{
		char* symbolName = (char*) malloc (strlen (name) + 2);
		strcpy (symbolName, "_");
		strcat (symbolName, name);
		NSSymbol symbol;
		symbol = NSLookupSymbolInImage ((const struct mach_header*)instance, symbolName, NSLOOKUPSYMBOLINIMAGE_OPTION_BIND_NOW | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR);
		free (symbolName);
		if (symbol)
		{
			if (firstSymbol == NULL)
				firstSymbol = symbol;
			return NSAddressOfSymbol (symbol);
		}
	}
#endif // !TARGET_OS_IPHONE

#endif

	return 0;
}

//------------------------------------------------------------------------
} // namespace Steinberg
