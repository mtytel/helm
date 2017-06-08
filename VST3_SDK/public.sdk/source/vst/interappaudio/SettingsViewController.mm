//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/interappaudio/SettingsViewController.mm
// Created by  : Steinberg, 09/2013
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

#import "SettingsViewController.h"
#import "MidiIO.h"
#import "AudioIO.h"
#import <CoreMIDI/MIDINetworkSession.h>

using namespace Steinberg::Vst::InterAppAudio;

static const NSTimeInterval kAnimationTime = 0.2;
static const NSUInteger kMinTempo = 30;

//------------------------------------------------------------------------
@interface SettingsViewController ()
//------------------------------------------------------------------------
{
	IBOutlet UIView* containerView;
	IBOutlet UISwitch* midiOnSwitch;
	IBOutlet UIPickerView* tempoView;
}
@end

//------------------------------------------------------------------------
@implementation SettingsViewController
//------------------------------------------------------------------------

//------------------------------------------------------------------------
- (id)init
{
    self = [super initWithNibName:@"SettingsView" bundle:nil];
    if (self)
	{
        // Custom initialization
    }
    return self;
}

//------------------------------------------------------------------------
- (void)viewDidLoad
{
    [super viewDidLoad];

	containerView.layer.shadowOpacity = 0.5;
	containerView.layer.shadowOffset = CGSizeMake (5, 5);
	containerView.layer.shadowRadius = 5;

	midiOnSwitch.on = MidiIO::instance ().isEnabled ();

	Float64 tempo = AudioIO::instance()->getStaticFallbackTempo ();
	[tempoView selectRow:tempo - kMinTempo inComponent:0 animated:YES];
}

//------------------------------------------------------------------------
- (IBAction)enableMidi:(id)sender
{
	BOOL state = midiOnSwitch.on;
	MidiIO::instance().setEnabled (state);
}

//------------------------------------------------------------------------
- (IBAction)close:(id)sender
{
	[UIView animateWithDuration:kAnimationTime animations:^{
		self.view.alpha = 0.;
	} completion:^(BOOL finished) {
		[self.view removeFromSuperview];
		[self removeFromParentViewController];
	}];
}

//------------------------------------------------------------------------
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
	AudioIO::instance()->setStaticFallbackTempo (row + kMinTempo);
}

//------------------------------------------------------------------------
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
	return 1;
}

//------------------------------------------------------------------------
- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
	return 301 - kMinTempo;
}

//------------------------------------------------------------------------
- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
	return [@(row + kMinTempo) stringValue];
}

@end

//------------------------------------------------------------------------
void showIOSettings ()
{
	SettingsViewController* controller = [[SettingsViewController alloc] init];
	controller.view.alpha = 0.;
	
	UIViewController* rootViewController = [[UIApplication sharedApplication].windows[0] rootViewController];
	[rootViewController addChildViewController:controller];
	[rootViewController.view addSubview:controller.view];
	
	[UIView animateWithDuration:kAnimationTime animations:^{
		controller.view.alpha = 1.;
	}];
}
