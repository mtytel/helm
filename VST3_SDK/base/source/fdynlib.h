//------------------------------------------------------------------------
// Project     : SDK Base
// Version     : 1.0
//
// Category    : Helpers
// Filename    : base/source/fdynlib.h
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

//------------------------------------------------------------------------
/** @file base/source/fdynlib.h
	Platform independent dynamic library loading. */
//------------------------------------------------------------------------
#pragma once

#include "pluginterfaces/base/ftypes.h"
#include "base/source/fobject.h"

namespace Steinberg {

//------------------------------------------------------------------------
/** Platform independent dynamic library loader. */
//------------------------------------------------------------------------
class FDynLibrary : public FObject
{
public:
//------------------------------------------------------------------------
	/** Constructor.
	
		Loads the specified dynamic library.

		@param[in] name the path of the library to load.
		@param[in] addExtension if @c true append the platform dependent default extension to @c name.

		@remarks
		- If @c name specifies a full path, the FDynLibrary searches only that path for the library.
		- If @c name specifies a relative path or a name without path, 
		  FDynLibrary uses a standard search strategy of the current platform to find the library; 
		- If @c name is @c NULL the library is not loaded. 
		  - Use init() to load the library. */
	FDynLibrary (const tchar* name = 0, bool addExtension = true);
	
	/** Destructor.
		The destructor unloads the library.*/
	~FDynLibrary ();

	/** Loads the library if not already loaded.
		This function is normally called by FDynLibrary(). 
		@remarks If the library is already loaded, this call has no effect. */
	bool init (const tchar* name, bool addExtension = true);

	/** Returns the address of the procedure @c name */ 
	void* getProcAddress (const char* name);

	/** Returns true when the library was successfully loaded. */
	bool isLoaded () {return isloaded;} 

	/** Unloads the library if it is loaded.
		This function is called by ~FDynLibrary (). */
	bool unload ();
	
	/** Returns the platform dependent representation of the library instance. */
	void* getPlatformInstance () const { return instance; } 

#if MAC
	/** Returns @c true if the library is a bundle (Mac only). */
	bool isBundleLib () const {return isBundle;}
#endif

//------------------------------------------------------------------------
	OBJ_METHODS(FDynLibrary, FObject)
protected:	
	bool isloaded;

	void* instance;

#if MAC
	void* firstSymbol;
	bool isBundle;
#endif
};

//------------------------------------------------------------------------
} // namespace Steinberg
