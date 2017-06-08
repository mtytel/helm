//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/vstguieditor.h
// Created by  : Steinberg, 04/2005
// Description : VSTGUI Editor
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

#include "vsteditcontroller.h"
#include "vstgui/vstgui.h"

#if VSTGUI_VERSION_MAJOR < 4
#include "vstgui/cvstguitimer.h"
#define VSTGUI_INT32	long
#else
#define VSTGUI_INT32	int32_t
#endif

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
/** Base class for an edit view using VSTGUI.
\ingroup vstClasses  */
//------------------------------------------------------------------------
class VSTGUIEditor : public EditorView, public VSTGUIEditorInterface, public CBaseObject, public IPlugViewIdleHandler
{
public:
	/** Constructor. */
	VSTGUIEditor (void* controller, ViewRect* size = 0);

	/** Destructor. */
	virtual ~VSTGUIEditor ();

	//---Internal function-----
	/** Called when the editor will be opened. */
#if VSTGUI_VERSION_MAJOR >= 4 && VSTGUI_VERSION_MINOR >= 1
	virtual bool PLUGIN_API open (void* parent, const PlatformType& platformType = kDefaultNative) = 0;
#else
	virtual bool PLUGIN_API open (void* parent) = 0;
#endif
	/** Called when the editor will be closed. */
	virtual void PLUGIN_API close () = 0;

	/** Sets the idle rate controlling the parameter update rate. */
	void setIdleRate (int32 millisec);

	//---from CBaseObject---------------
	CMessageResult notify (CBaseObject* sender, const char* message) SMTG_OVERRIDE;
	void forget () SMTG_OVERRIDE { EditorView::release (); }
	void remember () SMTG_OVERRIDE { EditorView::addRef (); }
	VSTGUI_INT32 getNbReference () const SMTG_OVERRIDE { return refCount; }

	//---from IPlugView-------
	tresult PLUGIN_API isPlatformTypeSupported (FIDString type) SMTG_OVERRIDE;
	tresult PLUGIN_API onSize (ViewRect* newSize) SMTG_OVERRIDE;

	//---from VSTGUIEditorInterface-------
	/** Called from VSTGUI when a user begins editing.
		The default implementation calls performEdit of the EditController. */
	void beginEdit (VSTGUI_INT32 index) SMTG_OVERRIDE;
	/** Called from VSTGUI when a user ends editing.
		The default implementation calls endEdit of the EditController. */
	void endEdit (VSTGUI_INT32 index) SMTG_OVERRIDE;

	VSTGUI_INT32 getKnobMode () const SMTG_OVERRIDE;

	OBJ_METHODS (VSTGUIEditor, EditorView)
	DEFINE_INTERFACES
		DEF_INTERFACE (IPlugViewIdleHandler)
	END_DEFINE_INTERFACES (EditorView)
	REFCOUNT_METHODS(EditorView)
private:
	//---from IPlugView-------
	tresult PLUGIN_API attached (void* parent, FIDString type) SMTG_OVERRIDE;
	tresult PLUGIN_API removed () SMTG_OVERRIDE;
	tresult PLUGIN_API onKeyDown (char16 key, int16 keyMsg, int16 modifiers) SMTG_OVERRIDE;
	tresult PLUGIN_API onKeyUp (char16 key, int16 keyMsg, int16 modifiers) SMTG_OVERRIDE;
	tresult PLUGIN_API onWheel (float distance) SMTG_OVERRIDE;
	tresult PLUGIN_API setFrame (IPlugFrame* frame) SMTG_OVERRIDE;

	//---from IPlugViewIdle-------
	void PLUGIN_API onPlugViewIdle () SMTG_OVERRIDE;

	CVSTGUITimer* timer;
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
