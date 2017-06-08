//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/auwrapper/aucarbonview.h
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

#pragma once

#if !__LP64__

#include "AUPublic/AUCarbonViewBase/AUCarbonViewBase.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "base/source/fobject.h"
#include "pluginterfaces/gui/iplugview.h"

namespace Steinberg {
namespace Vst {
class AUCarbonPlugFrame;

//------------------------------------------------------------------------
class AUCarbonView : public AUCarbonViewBase, public IPlugFrame, public FObject
{
public:
	AUCarbonView (AudioUnitCarbonView auv);
	~AUCarbonView ();

	OSStatus CreateUI (Float32 xoffset, Float32 yoffset);

	OBJ_METHODS(AUCarbonView, FObject)
	DEF_INTERFACES_1(IPlugFrame, FObject)
	REFCOUNT_METHODS(FObject)

protected:
	tresult PLUGIN_API resizeView (IPlugView* view, ViewRect* vr);

	static OSStatus HIViewAdded (EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void* inUserData);

	IEditController* editController;
	AUCarbonPlugFrame* plugFrame;
	IPlugView* plugView;
	HIViewRef hiPlugView;
	EventHandlerRef eventHandler;
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg

#endif // !__LP64__

/// \endcond
