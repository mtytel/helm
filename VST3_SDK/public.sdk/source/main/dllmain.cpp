//-----------------------------------------------------------------------------
// Project     : SDK Core
// Version     : 1.0
//
// Category    : Common Base Classes
// Filename    : public.sdk/source/main/dllmain.cpp
// Created by  : Steinberg, 01/2004
// Description : Windows DLL Entry
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

#include "pluginterfaces/base/ftypes.h"

#include <windows.h>

#if defined (_MSC_VER) && defined (DEVELOPMENT)
	#include <crtdbg.h>
#endif

#ifdef UNICODE
#define tstrrchr wcsrchr
#else
#define tstrrchr strrchr
#endif

//------------------------------------------------------------------------
HINSTANCE ghInst = 0;
void* moduleHandle = 0;
Steinberg::tchar gPath[MAX_PATH] = {0};

//------------------------------------------------------------------------
#define DllExport __declspec( dllexport )

//------------------------------------------------------------------------
extern bool InitModule ();		///< must be provided by Plug-in: called when the library is loaded
extern bool DeinitModule ();	///< must be provided by Plug-in: called when the library is unloaded

//------------------------------------------------------------------------
#ifdef __cplusplus 
extern "C" {
#endif
	bool DllExport InitDll () ///< must be called from host right after loading dll
	{ 		
		return InitModule (); 
	} 
	bool DllExport ExitDll ()  ///< must be called from host right before unloading dll
	{ 
		return DeinitModule (); 
	}
#ifdef __cplusplus 
} // extern "C"
#endif

//------------------------------------------------------------------------
BOOL WINAPI DllMain (HINSTANCE hInst, DWORD dwReason, LPVOID /*lpvReserved*/)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
	#if defined (_MSC_VER) && defined (DEVELOPMENT)
		_CrtSetReportMode ( _CRT_WARN, _CRTDBG_MODE_DEBUG );
		_CrtSetReportMode ( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
		_CrtSetReportMode ( _CRT_ASSERT, _CRTDBG_MODE_DEBUG );
		int flag = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG);
		_CrtSetDbgFlag (flag | _CRTDBG_LEAK_CHECK_DF);
	#endif

		moduleHandle = ghInst = hInst;

		// gets the path of the component
		if (GetModuleFileName (ghInst, gPath, MAX_PATH) > 0)
		{
			Steinberg::tchar* bkslash = tstrrchr (gPath, TEXT ('\\'));
			if (bkslash)
				gPath[bkslash - gPath + 1] = 0;
		}
	}

	return TRUE;
}
