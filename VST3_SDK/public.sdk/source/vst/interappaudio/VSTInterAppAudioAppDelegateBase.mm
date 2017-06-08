//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/interappaudio/VSTInterAppAudioAppDelegateBase.mm
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

#import "VSTInterAppAudioAppDelegateBase.h"

#import "public.sdk/source/vst/interappaudio/AudioIO.h"
#import "public.sdk/source/vst/interappaudio/MidiIO.h"
#import "public.sdk/source/vst/interappaudio/VST3Plugin.h"
#import "public.sdk/source/vst/interappaudio/VST3Editor.h"
#import "public.sdk/source/vst/interappaudio/HostApp.h"

using namespace Steinberg::Vst::InterAppAudio;

//------------------------------------------------------------------------
static OSType fourCharCodeToOSType (NSString* inCode)
{
	OSType rval = 0;
	NSData* data = [inCode dataUsingEncoding: NSMacOSRomanStringEncoding];
	[data getBytes:&rval length:sizeof(rval)];
	HTONL(rval);
	return rval;
}

//------------------------------------------------------------------------
@interface VSTInterAppAudioAppDelegateBase ()
//------------------------------------------------------------------------
{
	VST3Plugin plugin;
	VST3Editor editor;
	
	BOOL audioIOInitialized;
}
@end

//------------------------------------------------------------------------
@implementation VSTInterAppAudioAppDelegateBase
//------------------------------------------------------------------------

//------------------------------------------------------------------------
- (BOOL)initAudioIO
{
	id auArray = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"AudioComponents"];
	if (auArray)
	{
		id desc = [auArray objectAtIndex:0];
		if (desc)
		{
			NSString* typeStr = [desc objectForKey:@"type"];
			NSString* subtypeStr = [desc objectForKey:@"subtype"];
			NSString* manufacturerStr = [desc objectForKey:@"manufacturer"];
			NSString* nameStr = [desc objectForKey:@"name"];
			if (typeStr && subtypeStr && manufacturerStr && nameStr)
			{
				OSType type = fourCharCodeToOSType (typeStr);
				OSType subtype = fourCharCodeToOSType (subtypeStr);
				OSType manufacturer = fourCharCodeToOSType (manufacturerStr);
				AudioIO* audioIO = AudioIO::instance ();
				if (audioIO->init (type, subtype, manufacturer, (__bridge CFStringRef)nameStr) == Steinberg::kResultTrue)
				{
					if (plugin.init ())
					{
						InterAppAudioHostApp::instance ()->setPlugin (&plugin);
						audioIO->addProcessor (&plugin);
						audioIOInitialized = YES;
						return YES;
					}
				}
			}
		}
	}
	return NO;
}

//------------------------------------------------------------------------
- (BOOL)createUI
{
	if (audioIOInitialized)
	{
		[UIApplication sharedApplication].statusBarHidden = YES;
		self.window = [UIWindow new];
		self.window.backgroundColor = [UIColor redColor];
		CGRect screenSize = self.window.bounds;

		if (editor.init (screenSize))
		{
			self.window.rootViewController = editor.getViewController ();
			[self.window makeKeyAndVisible];
			if (editor.attach (plugin.getEditController ()) == false)
			{
				return NO;
			}
		}
		return YES;
	}
	return NO;
}

//------------------------------------------------------------------------
- (void)savePluginState:(NSCoder*)coder
{
	NSData* processorState = plugin.getProcessorState ();
	NSData* controllerState = plugin.getControllerState ();
	if (processorState)
		[coder encodeObject:processorState forKey:@"VST3ProcessorState"];
	if (controllerState)
		[coder encodeObject:controllerState forKey:@"VST3ControllerState"];
}

//------------------------------------------------------------------------
- (void)restorePluginState:(NSCoder*)coder
{
	NSData* processorState = [coder decodeObjectForKey:@"VST3ProcessorState"];
	if (processorState)
	{
		plugin.setProcessorState (processorState);
	}
	NSData* controllerState = [coder decodeObjectForKey:@"VST3ControllerState"];
	if (controllerState)
	{
		plugin.setControllerState (controllerState);
	}
}

//------------------------------------------------------------------------
// UIApplicationDelegate methods
//------------------------------------------------------------------------
- (BOOL)application:(UIApplication *)application willFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	return [self initAudioIO];
}

//------------------------------------------------------------------------
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	BOOL result = [self createUI];
	if (result)
	{
		AudioIO::instance ()->start ();
	}
	return result;
}

//------------------------------------------------------------------------
- (BOOL)application:(UIApplication *)application shouldSaveApplicationState:(NSCoder *)coder
{
	[self savePluginState:coder];
	[coder encodeBool:MidiIO::instance ().isEnabled () forKey:@"MIDI Enabled"];
	return YES;
}

//------------------------------------------------------------------------
- (BOOL)application:(UIApplication *)application shouldRestoreApplicationState:(NSCoder *)coder
{
	[self restorePluginState:coder];
	BOOL midiEnabled = [coder decodeBoolForKey:@"MIDI Enabled"];
	MidiIO::instance ().setEnabled (midiEnabled);
	return YES;
}

//------------------------------------------------------------------------
- (void)applicationDidBecomeActive:(UIApplication *)application
{
	AudioIO* audioIO = AudioIO::instance ();
	audioIO->start ();
}

//------------------------------------------------------------------------
- (void)applicationWillResignActive:(UIApplication *)application
{
	AudioIO* audioIO = AudioIO::instance ();
	if (audioIO->getInterAppAudioConnected () == false && MidiIO::instance().isEnabled () == false)
	{
		audioIO->stop ();
	}
}

@end
