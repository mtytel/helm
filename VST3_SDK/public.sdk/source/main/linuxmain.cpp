//-----------------------------------------------------------------------------
// Project     : SDK Core
// Version     : 1.0
//
// Category    : Common Base Classes
// Filename    : public.sdk/source/main/linuxmain.cpp
// Created by  : Steinberg, 01/2004
// Description : Linux Component Entry
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

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define EXPORT	__attribute__ ((visibility ("default")))
#else
#define EXPORT
#endif

//------------------------------------------------------------------------
void* moduleHandle = 0;

//------------------------------------------------------------------------
bool InitModule ();		///< must be provided by Plug-in: called when the library is loaded
bool DeinitModule ();	///< must be provided by Plug-in: called when the library is unloaded

//------------------------------------------------------------------------
extern "C"
{
	EXPORT bool ModuleEntry (void*);
	EXPORT bool ModuleExit (void);
}

static int counter {0};

//------------------------------------------------------------------------
bool ModuleEntry (void* sharedLibraryHandle)
{
	if (++counter == 1)
	{
		moduleHandle = sharedLibraryHandle;
		return InitModule ();
	}
	return true;
}

//------------------------------------------------------------------------
bool ModuleExit (void)
{
	if (--counter == 0)
	{
		moduleHandle = nullptr;
		return DeinitModule ();
	}
	return true;
}
