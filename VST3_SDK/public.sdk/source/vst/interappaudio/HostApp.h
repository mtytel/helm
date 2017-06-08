//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/interappaudio/HostApp.h
// Created by  : Steinberg, 08/2013.
// Description : VST 3 InterAppAudio
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

/// \cond ignore

#import "base/source/fobject.h"
#import "public.sdk/source/vst/hosting/hostclasses.h"
#import "pluginterfaces/vst/ivstinterappaudio.h"

namespace Steinberg {
namespace Vst {
namespace InterAppAudio {

class VST3Plugin;

//-----------------------------------------------------------------------------
class InterAppAudioHostApp : public FObject, public HostApplication, public IInterAppAudioHost
{
public:
//-----------------------------------------------------------------------------
	static InterAppAudioHostApp* instance ();

	void setPlugin (VST3Plugin* plugin);
	VST3Plugin* getPlugin () const { return plugin; }

//-----------------------------------------------------------------------------
//	IInterAppAudioHost
	tresult PLUGIN_API getScreenSize (ViewRect* size, float* scale) override;
	tresult PLUGIN_API connectedToHost () override;
	tresult PLUGIN_API switchToHost () override;
	tresult PLUGIN_API sendRemoteControlEvent (uint32 event) override;
	tresult PLUGIN_API getHostIcon (void** icon) override;
	tresult PLUGIN_API scheduleEventFromUI (Event& event) override;
	IInterAppAudioPresetManager* PLUGIN_API createPresetManager (const TUID& cid) override;
	tresult PLUGIN_API showSettingsView () override;

//-----------------------------------------------------------------------------
//	HostApplication
	tresult PLUGIN_API getName (String128 name) override;

	OBJ_METHODS(InterAppAudioHostApp, FObject)
	REFCOUNT_METHODS(FObject)
	DEFINE_INTERFACES
		DEF_INTERFACE(IHostApplication)
		DEF_INTERFACE(IInterAppAudioHost)
	END_DEFINE_INTERFACES(FObject)
protected:
	InterAppAudioHostApp ();
	
	VST3Plugin* plugin;
};

//------------------------------------------------------------------------
} // namespace InterAppAudio
} // namespace Vst
} // namespace Steinberg

/// \endcond
