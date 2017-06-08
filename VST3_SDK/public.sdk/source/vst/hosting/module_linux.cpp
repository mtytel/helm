//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    :
// Filename    : public.sdk/source/vst/hosting/module_linux.cpp
// Created by  : Steinberg, 08/2016
// Description : hosting module classes (linux implementation)
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
#include "optional.h"
#include <dlfcn.h>
#include <algorithm>
#include <experimental/filesystem>
#include <sys/utsname.h>
#include <sys/types.h>
#include <unistd.h>

//------------------------------------------------------------------------
extern "C" {
typedef bool (PLUGIN_API* ModuleEntryFunc) (void*);
typedef bool (PLUGIN_API* ModuleExitFunc) ();
}

//------------------------------------------------------------------------
namespace VST3 {
namespace Hosting {

//------------------------------------------------------------------------
namespace {

//------------------------------------------------------------------------
Optional<std::string> getCurrentMachineName ()
{
	struct utsname unameData;

	int res = uname (&unameData);
	if (res != 0)
		return {};

	return {unameData.machine};
}

using Path = std::experimental::filesystem::path;
//------------------------------------------------------------------------
Optional<Path> getApplicationPath ()
{
	std::string appPath = "";

	pid_t pid = getpid ();
	char buf[10];
	sprintf (buf, "%d", pid);
	std::string _link = "/proc/";
	_link.append (buf);
	_link.append ("/exe");
	char proc[1024];
	int ch = readlink (_link.c_str (), proc, 1024);
	if (ch == -1)
		return {};

	proc[ch] = 0;
	appPath = proc;
	std::string::size_type t = appPath.find_last_of ("/");
	appPath = appPath.substr (0, t);

	return Path {appPath};
}

//------------------------------------------------------------------------
class LinuxModule : public Module
{
public:
	template <typename T>
	T getFunctionPointer (const char* name)
	{
		return reinterpret_cast<T> (dlsym (module, name));
	}

	~LinuxModule ()
	{
		factory = PluginFactory (nullptr);

		if (module)
		{
			if (auto moduleExit = getFunctionPointer<ModuleExitFunc> ("ModuleExit"))
				moduleExit ();

			dlclose (module);
		}
	}

	static Optional<Path> getSOPath (const std::string& inPath)
	{
		using namespace std::experimental;

		Path modulePath {inPath};
		if (!filesystem::is_directory (modulePath))
			return {};

		auto stem = modulePath.stem ();

		modulePath /= "Contents";
		if (!filesystem::is_directory (modulePath))
			return {};

		auto machine = getCurrentMachineName ();
		if (!machine)
			return {};

		modulePath /= *machine + "-linux";
		if (!filesystem::is_directory (modulePath))
			return {};

		stem.replace_extension (".so");
		modulePath /= stem;
		return Optional<Path> (std::move (modulePath));
	}

	bool load (const std::string& inPath, std::string& errorDescription) override
	{
		auto modulePath = getSOPath (inPath);
		if (!modulePath)
		{
			errorDescription = inPath + " is not a module directory.";
			return false;
		}

		module = dlopen (modulePath->generic_u8string ().data (), RTLD_LAZY);
		if (!module)
		{
			errorDescription = "dlopen failed.\n";
			errorDescription += dlerror ();
			return false;
		}

		auto factoryProc = getFunctionPointer<GetFactoryProc> ("GetPluginFactory");
		if (!factoryProc)
		{
			errorDescription =
			    "The shared library does not export the required 'GetPluginFactory' function";
			return false;
		}
		auto moduleEntry = getFunctionPointer<ModuleEntryFunc> ("ModuleEntry");
		if (!moduleEntry)
		{
			errorDescription =
			    "The shared library does not export the required 'ModuleEntry' function";
			return false;
		}
		if (!moduleEntry (module))
		{
			errorDescription = "Calling 'ModuleEntry' failed";
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

	void* module {nullptr};
};

//------------------------------------------------------------------------
void findFilesWithExt (const std::string& path, const std::string& ext, Module::PathList& pathList)
{
	try
	{
		using namespace std::experimental;
		for (auto& p : filesystem::recursive_directory_iterator (path))
		{
			if (p.path ().extension () == ext && filesystem::is_directory (p))
			{
				pathList.push_back (p.path ().generic_u8string ());
			}
		}
	}
	catch (...)
	{
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
	auto module = std::make_shared<LinuxModule> ();
	if (module->load (path, errorDescription))
	{
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
	/* VST3 component locations on linux :
	    * User privately installed	: $HOME/.vst3/
	    * Distribution installed	: /usr/lib/vst3/
	    * Locally installed			: /usr/local/lib/vst3/
	    * Application				: /$APPFOLDER/vst3/
	*/

	const auto systemPaths = {"/usr/lib/vst3/", "/usr/local/lib/vst3/"};

	// 32bit Plug-ins on 64bit OS
	// const auto systemPaths = {"/usr/lib32/vst3/", "/usr/local/lib32/vst3/"};

	PathList list;
	if (auto homeDir = getenv ("HOME"))
	{
		std::experimental::filesystem::path homePath (homeDir);
		homePath /= ".vst3";
		findModules (homePath.generic_u8string (), list);
	}
	for (auto path : systemPaths)
		findModules (path, list);

	// application level
	auto appPath = getApplicationPath ();
	if (appPath)
	{
		*appPath /= "vst3";
		findModules (appPath->generic_u8string (), list);
	}

	return list;
}

//------------------------------------------------------------------------
} // Hosting
} // VST3
