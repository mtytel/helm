//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/auwrapper/ausdk.mm
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

#ifndef MAC_OS_X_VERSION_10_7
	#define MAC_OS_X_VERSION_10_7 1070
#endif

#import "PublicUtility/CAAudioChannelLayout.cpp"
#import "PublicUtility/CABundleLocker.cpp"
#import "PublicUtility/CAHostTimeBase.cpp"
#import "PublicUtility/CAStreamBasicDescription.cpp"
#import "PublicUtility/CAVectorUnit.cpp"
#import "PublicUtility/CAAUParameter.cpp"

#import "AUPublic/AUBase/ComponentBase.cpp"
#import "AUPublic/AUBase/AUScopeElement.cpp"
#import "AUPublic/AUBase/AUOutputElement.cpp"
#import "AUPublic/AUBase/AUInputElement.cpp"
#import "AUPublic/AUBase/AUBase.cpp"

#if !__LP64__
	#import "AUPublic/AUCarbonViewBase/AUCarbonViewBase.cpp"
	#import "AUPublic/AUCarbonViewBase/AUCarbonViewControl.cpp"
	#import "AUPublic/AUCarbonViewBase/AUCarbonViewDispatch.cpp"
	#import "AUPublic/AUCarbonViewBase/AUControlGroup.cpp"
	#import "AUPublic/AUCarbonViewBase/CarbonEventHandler.cpp"
#endif

#import "AUPublic/Utility/AUTimestampGenerator.cpp"
#import "AUPublic/Utility/AUBuffer.cpp"
#import "AUPublic/Utility/AUBaseHelper.cpp"

#if MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_7
	#import "AUPublic/OtherBases/AUMIDIEffectBase.cpp"
	#import "AUPublic/Utility/AUDebugDispatcher.cpp"
#else
	#import "AUPublic/AUBase/AUPlugInDispatch.cpp"
#endif

#if !CA_USE_AUDIO_PLUGIN_ONLY
	#import "AUPublic/AUBase/AUDispatch.cpp"
	#import "AUPublic/OtherBases/MusicDeviceBase.cpp"
	#import "AUPublic/OtherBases/AUMIDIBase.cpp"
	#import "AUPublic/OtherBases/AUEffectBase.cpp"
#endif

/// \endcond
