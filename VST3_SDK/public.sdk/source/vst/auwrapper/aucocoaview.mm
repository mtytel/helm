//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/auwrapper/aucocoaview.mm
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

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#import "aucocoaview.h"
#import "auwrapper.h"
#import "pluginterfaces/vst/ivsteditcontroller.h"
#import "pluginterfaces/gui/iplugview.h"
#import "base/source/fobject.h"

namespace Steinberg {
DEF_CLASS_IID(IPlugFrame)

//------------------------------------------------------------------------
class AUPlugFrame : public FObject, public IPlugFrame
//------------------------------------------------------------------------
{
public:
	AUPlugFrame (NSView* parent) : parent (parent) {}
	tresult PLUGIN_API resizeView (IPlugView* view, ViewRect* vr) SMTG_OVERRIDE
	{
		NSRect newSize = NSMakeRect ([parent frame].origin.x, [parent frame].origin.y, vr->right - vr->left, vr->bottom - vr->top);
		[parent setFrame:newSize];
		return kResultTrue;
	}
	
	OBJ_METHODS(AUPlugFrame, FObject)
	DEF_INTERFACES_1(IPlugFrame, FObject)
	REFCOUNT_METHODS(FObject)
protected:
	NSView* parent;
};
	
} // namespace Steinberg

using namespace Steinberg;

//------------------------------------------------------------------------
@interface SMTGCocoa_NSViewWrapperForAU : NSView {
//------------------------------------------------------------------------
	IPlugView* plugView;
	Vst::IEditController* editController;
	AudioUnit audioUnit;
	FObject* dynlib;
	AUPlugFrame* plugFrame;
	
	BOOL isAttached;
}

- (id) initWithEditController: (Vst::IEditController*) editController audioUnit: (AudioUnit) au preferredSize: (NSSize) size;

@end

//------------------------------------------------------------------------
// SMTG_AU_PLUGIN_NAMESPACE (SMTGAUPluginCocoaView)
//------------------------------------------------------------------------

//------------------------------------------------------------------------
@implementation SMTG_AU_PLUGIN_NAMESPACE (SMTGAUPluginCocoaView)

//------------------------------------------------------------------------
- (unsigned) interfaceVersion
{
    return 0;
}

//------------------------------------------------------------------------
- (NSString *) description
{
    return @"Cocoa View";
}

//------------------------------------------------------------------------
- (NSView *)uiViewForAudioUnit:(AudioUnit)inAU withSize:(NSSize)inPreferredSize
{
    Vst::IEditController* editController = 0;
    UInt32 size = sizeof (Vst::IEditController*);
    if (AudioUnitGetProperty (inAU, 64000, kAudioUnitScope_Global, 0, &editController, &size) != noErr)
        return nil;
    return [[[SMTGCocoa_NSViewWrapperForAU alloc] initWithEditController:editController audioUnit:inAU preferredSize:inPreferredSize] autorelease];
}

@end

//------------------------------------------------------------------------
@implementation SMTGCocoa_NSViewWrapperForAU
//------------------------------------------------------------------------
- (id) initWithEditController: (Vst::IEditController*) _editController audioUnit: (AudioUnit) au preferredSize: (NSSize) size
{
	self = [super initWithFrame: NSMakeRect (0, 0, size.width, size.height)];
	if (self)
	{
		editController = _editController;
		editController->addRef ();
		audioUnit = au;
		plugView = editController->createView (Vst::ViewType::kEditor);
		if (!plugView || plugView->isPlatformTypeSupported (kPlatformTypeNSView) != kResultTrue)
		{
			[self dealloc];
			return nil;
		}
		
		plugFrame = NEW AUPlugFrame (self);
		plugView->setFrame (plugFrame);
		
		if (plugView->attached (self, kPlatformTypeNSView) != kResultTrue)
		{
			[self dealloc];
			return nil;
		}
		ViewRect vr;
		if (plugView->getSize (&vr) == kResultTrue)
		{
			NSRect newSize = NSMakeRect (0, 0, vr.right - vr.left, vr.bottom - vr.top);
			[self setFrame:newSize];
		}

		isAttached = YES;
		UInt32 size = sizeof (FObject*);
		if (AudioUnitGetProperty (audioUnit, 64001, kAudioUnitScope_Global, 0, &dynlib, &size) == noErr)
			dynlib->addRef ();
	}
	return self;
}


//------------------------------------------------------------------------
- (void) setFrame: (NSRect) newSize
{
	[super setFrame: newSize];
	ViewRect viewRect (0, 0, newSize.size.width, newSize.size.height);
	
	if (plugView)
		plugView->onSize (&viewRect);
}


//------------------------------------------------------------------------
- (BOOL)isFlipped { return YES; }

//------------------------------------------------------------------------
- (void)viewDidMoveToSuperview
{
	if (plugView)
	{
		if ([self superview])
		{
			if (!isAttached)
			{
				isAttached = plugView->attached (self, kPlatformTypeNSView) == kResultTrue;
			}
		}
		else
		{
			if (isAttached)
			{
				plugView->removed ();
				isAttached = NO;
			}
		}
	}
}

//------------------------------------------------------------------------
- (void) dealloc
{
	if (plugView)
	{
		if (isAttached)
		{
			plugView->setFrame (0);
			plugView->removed ();
		}
		plugView->release ();
		if (plugFrame)
			plugFrame->release ();
		
		if (editController)
		{
			Steinberg::uint32 refCount= editController->addRef ();
			if (refCount == 2)
				editController->terminate ();
			
			editController->release ();
			editController->release ();
			editController = 0;
		}		
	}
	if (dynlib)
		dynlib->release ();
	[super dealloc];
}

@end

/// \endcond
