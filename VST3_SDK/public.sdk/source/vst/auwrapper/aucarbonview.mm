//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/auwrapper/aucarbonview.mm
// Created by  : Steinberg, 12/2007
// Description : VST 3 -> AU Wrapper
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

/// \cond ignore

#include "aucarbonview.h"

#if !__LP64__

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
AUCarbonView::AUCarbonView (AudioUnitCarbonView auv)
: AUCarbonViewBase (auv)
, editController (0)
, plugView (0)
, hiPlugView (0)
{
}

//------------------------------------------------------------------------
AUCarbonView::~AUCarbonView ()
{
	if (plugView)
	{
		plugView->setFrame (0);
		plugView->removed ();
		plugView->release ();
	}
}

//------------------------------------------------------------------------
OSStatus AUCarbonView::HIViewAdded (EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *inUserData)
{
	UInt32 eventClass = GetEventClass (inEvent);
	UInt32 eventKind = GetEventKind (inEvent);
	if (eventClass == kEventClassControl && eventKind == kEventControlAddedSubControl)
	{
		HIViewRef newControl;
		if (GetEventParameter (inEvent, kEventParamControlSubControl, typeControlRef, NULL, sizeof (HIViewRef) , NULL , &newControl) == noErr)
		{
			AUCarbonView* wrapper = (AUCarbonView*)inUserData;
			wrapper->hiPlugView = newControl;
			RemoveEventHandler (wrapper->eventHandler);
			wrapper->eventHandler = 0;
		}
	}
	return eventNotHandledErr;
}

//------------------------------------------------------------------------
OSStatus AUCarbonView::CreateUI (Float32 xoffset, Float32 yoffset)
{
	AudioUnit unit = GetEditAudioUnit ();
	if (unit)
	{
		if (!editController)
		{
			UInt32 size = sizeof (IEditController*);
			if (AudioUnitGetProperty (unit, 64000, kAudioUnitScope_Global, 0, &editController, &size) != noErr)
				return kAudioUnitErr_NoConnection;
		}
		if (editController)
		{
			plugView = editController->createView (ViewType::kEditor);
			if (!plugView)
				return kAudioUnitErr_NoConnection;

			HIViewRef contentView;
			const EventTypeSpec eventTypes[] = {
				{ kEventClassControl, kEventControlAddedSubControl },
			};
			OSStatus err = HIViewFindByID (HIViewGetRoot (GetCarbonWindow ()), kHIViewWindowContentID, &contentView);
			err = InstallControlEventHandler (contentView, HIViewAdded, 1, eventTypes, this, &eventHandler);

			plugView->setFrame (this);

			if (plugView->attached (GetCarbonWindow (), kPlatformTypeHIView) == kResultTrue)
			{
				HIViewRemoveFromSuperview (hiPlugView);
				EmbedControl (hiPlugView);
				HIViewMoveBy (hiPlugView, xoffset, yoffset);
				return noErr;
			}
			else
				plugView->setFrame (0);
		}
	}
	return kAudioUnitErr_NoConnection;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AUCarbonView::resizeView (IPlugView* view, ViewRect* vr)
{
	if (vr == 0 || view != plugView)
		return kInvalidArgument;

	HIViewRef hiView = GetCarbonPane ();
	if (hiView)
	{
		HIRect r;
		if (HIViewGetFrame (hiView, &r) != noErr)
			return kResultFalse;
		r.size.width = vr->right - vr->left;
		r.size.height = vr->bottom - vr->top;
		if (HIViewSetFrame (hiView, &r) != noErr)
			return kResultFalse;
			
		if (plugView)
			plugView->onSize (vr);
			
		return kResultTrue;
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
//COMPONENT_ENTRY(AUCarbonView)
//------------------------------------------------------------------------
extern "C" {
	ComponentResult AUCarbonViewEntry(ComponentParameters *params, AUCarbonView *obj);
	__attribute__ ((visibility ("default"))) ComponentResult AUCarbonViewEntry(ComponentParameters *params, AUCarbonView *obj) 
	{
		return ComponentEntryPoint<AUCarbonView>::Dispatch(params, obj);
	}
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
#endif

/// \endcond
