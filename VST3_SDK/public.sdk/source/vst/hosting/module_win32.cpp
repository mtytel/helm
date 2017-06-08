//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    :
// Filename    : public.sdk/source/vst/hosting/module_win32.cpp
// Created by  : Steinberg, 08/2016
// Description : hosting module classes (win32 implementation)
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

#include "stringconvert.h"
#include "module.h"
#include <Windows.h>
#include <Shlobj.h>
#include <algorithm>
#include <experimental/filesystem>

#pragma comment(lib, "Shell32")

//------------------------------------------------------------------------
extern "C" {
typedef bool (PLUGIN_API* InitModuleFunc) ();
typedef bool (PLUGIN_API* ExitModuleFunc) ();
}

using namespace std::experimental;

//------------------------------------------------------------------------
namespace VST3 {
namespace Hosting {

//------------------------------------------------------------------------
namespace {

//------------------------------------------------------------------------
class Win32Module : public Module
{
public:
	template <typename T>
	T getFunctionPointer (const char* name)
	{
		return reinterpret_cast<T> (GetProcAddress (module, name));
	}

	~Win32Module ()
	{
		factory = PluginFactory (nullptr);

		if (module)
		{
			if (auto dllExit = getFunctionPointer<ExitModuleFunc> ("ExitDll"))
				dllExit ();

			FreeLibrary ((HMODULE)module);
		}
	}

	bool load (const std::string& inPath, std::string& errorDescription) override
	{
		auto wideStr = StringConvert::convert (inPath);
		module = LoadLibraryW (reinterpret_cast<LPCWSTR> (wideStr.data ()));
		if (!module)
		{
			// TODO: is there an API to get more information about the failure ?
			errorDescription = "LoadLibray failed.";
			return false;
		}

		auto dllEntry = getFunctionPointer<InitModuleFunc> ("InitDll");
		auto factoryProc = getFunctionPointer<GetFactoryProc> ("GetPluginFactory");
		if (!factoryProc)
		{
			errorDescription = "dll does not export the required 'GetPluginFactory' function";
			return false;
		}
		if (dllEntry && !dllEntry ())
		{
			errorDescription = "Calling 'InitDll' failed";
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

	HINSTANCE module {nullptr};
};

//------------------------------------------------------------------------
Optional<std::string> getKnownFolder (REFKNOWNFOLDERID folderID)
{
	PWSTR wideStr {};
	if (FAILED (SHGetKnownFolderPath (folderID, 0, nullptr, &wideStr)))
		return {};
	return StringConvert::convert (wideStr);
}

//------------------------------------------------------------------------
void findFilesWithExt (const std::string& path, const std::string& ext, Module::PathList& pathList)
{
	for (auto& p : filesystem::recursive_directory_iterator (path))
	{
		if (p.path ().extension () == ext)
		{
			pathList.push_back (p.path ().generic_u8string ());
		}
	}
}

//------------------------------------------------------------------------
void findModules (const std::string& path, Module::PathList& pathList)
{
	findFilesWithExt (path, ".vst3", pathList);
}

//------------------------------------------------------------------------
} // anonymous

//------------------------------------------------------------------------
Module::Ptr Module::create (const std::string& path, std::string& errorDescription)
{
	auto module = std::make_shared<Win32Module> ();
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
	// find plug-ins located in common/VST3
	PathList list;
	if (auto knownFolder = getKnownFolder (FOLDERID_ProgramFilesCommon))
	{
		filesystem::path p (*knownFolder);
		p.append ("VST3");
		findModules (p.generic_u8string (), list);
	}

	// find plug-ins located in VST3 (application folder)
	WCHAR modulePath[MAX_PATH + 1];
	GetModuleFileNameW (nullptr, modulePath, MAX_PATH);
	auto appPath = StringConvert::convert (modulePath);
	filesystem::path path (appPath);
	path = path.parent_path ();
	path = path.append ("VST3");
	findModules (path.generic_u8string (), list);

	return list;
}

//------------------------------------------------------------------------
} // Hosting
} // VST3
