//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/hosting/module_mac.mm
// Created by  : Steinberg, 08/2016
// Description : hosting module classes (macOS implementation)
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

#import "module.h"

#import <Cocoa/Cocoa.h>
#import <CoreFoundation/CoreFoundation.h>

//------------------------------------------------------------------------
extern "C" {
typedef bool (*BundleEntryFunc) (CFBundleRef);
typedef bool (*BundleExitFunc) ();
}

//------------------------------------------------------------------------
namespace VST3 {
namespace Hosting {

//------------------------------------------------------------------------
namespace {

//------------------------------------------------------------------------
template <typename T>
class CFPtr
{
public:
	inline CFPtr (const T& obj = nullptr) : obj (obj) {}
	inline CFPtr (CFPtr&& other) { *this = other; }
	inline ~CFPtr ()
	{
		if (obj)
			CFRelease (obj);
	}

	inline CFPtr& operator= (CFPtr&& other)
	{
		obj = other.obj;
		other.obj = nullptr;
		return *this;
	}
	inline CFPtr& operator= (const T& o)
	{
		if (obj)
			CFRelease (obj);
		obj = o;
		return *this;
	}
	inline operator T () const { return obj; } // act as T
private:
	CFPtr (const CFPtr& other) = delete;
	CFPtr& operator= (const CFPtr& other) = delete;

	T obj = nullptr;
};

//------------------------------------------------------------------------
class MacModule : public Module
{
public:
	template <typename T>
	T getFunctionPointer (const char* name)
	{
		assert (bundle);
		CFPtr<CFStringRef> functionName {
		    CFStringCreateWithCString (kCFAllocatorDefault, name, kCFStringEncodingASCII)};
		return reinterpret_cast<T> (CFBundleGetFunctionPointerForName (bundle, functionName));
	}

	bool loadInternal (const std::string& path, std::string& errorDescription)
	{
		CFPtr<CFURLRef> url (CFURLCreateFromFileSystemRepresentation (
		    kCFAllocatorDefault, reinterpret_cast<const UInt8*> (path.data ()), path.length (),
		    true));
		if (!url)
			return false;
		bundle = CFBundleCreate (kCFAllocatorDefault, url);
		CFErrorRef error = nullptr;
		if (!bundle || !CFBundleLoadExecutableAndReturnError (bundle, &error))
		{
			if (error)
			{
				CFPtr<CFStringRef> errorString (CFErrorCopyDescription (error));
				if (errorString)
				{
					auto stringLength = CFStringGetLength (errorString);
					auto maxSize =
					    CFStringGetMaximumSizeForEncoding (stringLength, kCFStringEncodingUTF8);
					auto buffer = std::make_unique<char[]> (maxSize);
					if (CFStringGetCString (errorString, buffer.get (), maxSize,
					                        kCFStringEncodingUTF8))
						errorDescription = buffer.get ();
					CFRelease (error);
				}
			}
			else
			{
				errorDescription = "Could not create Bundle for path: " + path;
			}
			return false;
		}
		auto bundleEntry = getFunctionPointer<BundleEntryFunc> ("bundleEntry");
		if (!bundleEntry)
		{
			errorDescription = "Bundle does not export the required 'bundleEntry' function";
			return false;
		}
		auto bundleExit = getFunctionPointer<BundleExitFunc> ("bundleExit");
		if (!bundleExit)
		{
			errorDescription = "Bundle does not export the required 'bundleExit' function";
			return false;
		}
		auto factoryProc = getFunctionPointer<GetFactoryProc> ("GetPluginFactory");
		if (!factoryProc)
		{
			errorDescription = "Bundle does not export the required 'GetPluginFactory' function";
			return false;
		}
		if (!bundleEntry (bundle))
		{
			errorDescription = "Calling 'bundleEntry' failed";
			return false;
		}
		auto f = Steinberg::FUnknownPtr<Steinberg::IPluginFactory> (factoryProc ());
		if (!f)
		{
			errorDescription = "Calling 'GetPluginFactory' returned nullptr";
			return false;
		}
		factory = PluginFactory (f);
		return true;
	}

	bool load (const std::string& path, std::string& errorDescription) override
	{
		if (!path.empty () && path[0] != '/')
		{
			auto buffer = std::make_unique<char[]> (PATH_MAX);
			auto workDir = getcwd (buffer.get (), PATH_MAX);
			if (workDir)
			{
				std::string wd (workDir);
				wd += "/";
				return loadInternal (wd + path, errorDescription);
			}
		}
		return loadInternal (path, errorDescription);
	}

	~MacModule ()
	{
        factory = PluginFactory (nullptr);
        
		if (bundle)
		{
			if (auto bundleExit = getFunctionPointer<BundleExitFunc> ("bundleExit"))
				bundleExit ();
            
            if (CFBundleIsExecutableLoaded ((CFBundleRef)bundle))
            {
                CFBundleUnloadExecutable ((CFBundleRef)bundle);
            }
		}
        
	}

	CFPtr<CFBundleRef> bundle;
};

//------------------------------------------------------------------------
void getModules (NSSearchPathDomainMask domain, Module::PathList& result)
{
	NSURL* libraryUrl = [[NSFileManager defaultManager] URLForDirectory:NSLibraryDirectory
	                                                           inDomain:domain
	                                                  appropriateForURL:nil
	                                                             create:NO
	                                                              error:nil];
	if (libraryUrl == nil)
		return;
	NSURL* audioUrl = [libraryUrl URLByAppendingPathComponent:@"Audio"];
	if (audioUrl == nil)
		return;
	NSURL* plugInsUrl = [audioUrl URLByAppendingPathComponent:@"Plug-Ins"];
	if (plugInsUrl == nil)
		return;
	NSURL* vst3Url =
	    [[plugInsUrl URLByAppendingPathComponent:@"VST3"] URLByResolvingSymlinksInPath];
	if (vst3Url == nil)
		return;
	NSDirectoryEnumerator* enumerator = [[NSFileManager defaultManager]
	               enumeratorAtURL:[vst3Url URLByResolvingSymlinksInPath]
	    includingPropertiesForKeys:nil
	                       options:NSDirectoryEnumerationSkipsPackageDescendants
	                  errorHandler:nil];
	for (NSURL* url in enumerator)
	{
		if ([[[url lastPathComponent] pathExtension] isEqualToString:@"vst3"])
		{
			
			result.emplace_back ([url.path UTF8String]);
		}
	}
}

//------------------------------------------------------------------------
void getApplicationModules (Module::PathList& result)
{
	auto bundle = CFBundleGetMainBundle ();
	if (!bundle)
		return;
	auto bundleUrl = static_cast<NSURL*> (CFBundleCopyBundleURL (bundle));
	if (!bundleUrl)
		return;
	auto resUrl = [bundleUrl URLByAppendingPathComponent:@"Contents"];
	if (!resUrl)
		return;
	auto vst3Url = [resUrl URLByAppendingPathComponent:@"VST3"];
	if (!vst3Url)
		return;
	NSDirectoryEnumerator* enumerator = [[NSFileManager defaultManager]
	               enumeratorAtURL:[vst3Url URLByResolvingSymlinksInPath]
	    includingPropertiesForKeys:nil
	                       options:NSDirectoryEnumerationSkipsPackageDescendants
	                  errorHandler:nil];
	for (NSURL* url in enumerator)
	{
		if ([[[url lastPathComponent] pathExtension] isEqualToString:@"vst3"])
		{
			result.emplace_back ([url.path UTF8String]);
		}
	}
}

//------------------------------------------------------------------------
} // anonymous

//------------------------------------------------------------------------
Module::Ptr Module::create (const std::string& path, std::string& errorDescription)
{
	auto module = std::make_shared<MacModule> ();
	if (module->load (path, errorDescription))
	{
		module->path = path;
		auto it = std::find_if (path.rbegin (), path.rend (),
		                        [] (const std::string::value_type& c) { return c == '/'; });
		if (it != path.rend ())
			module->name = {it.base (), path.end ()};
		return module;
	}
	return nullptr;
}

//------------------------------------------------------------------------
Module::PathList Module::getModulePaths ()
{
	PathList list;
	getModules (NSUserDomainMask, list);
	getModules (NSLocalDomainMask, list);
	// TODO getModules (NSNetworkDomainMask, list);
	getApplicationModules (list);
	return list;
}

//------------------------------------------------------------------------
} // Hosting
} // VST3
