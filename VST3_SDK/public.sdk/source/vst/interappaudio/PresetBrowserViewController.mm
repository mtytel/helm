//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/interappaudio/PresetBrowserViewController.mm
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


#import "PresetBrowserViewController.h"
#import "pluginterfaces/base/funknown.h"

static NSTimeInterval kAnimationTime = 0.2;

//------------------------------------------------------------------------
@interface PresetBrowserViewController ()
//------------------------------------------------------------------------
{
	IBOutlet UITableView* presetTableView;
	IBOutlet UIView* containerView;

	std::function<void (const char* presetPath)> callback;
	Steinberg::FUID uid;
}

@property (strong) NSArray* factoryPresets;
@property (strong) NSArray* userPresets;
@property (strong) NSArray* displayPresets;
@property (assign) BOOL editMode;

@end

//------------------------------------------------------------------------
@implementation PresetBrowserViewController
//------------------------------------------------------------------------

//------------------------------------------------------------------------
- (id)initWithCallback:(std::function<void (const char* presetPath)>)_callback
{
    self = [super initWithNibName:@"PresetBrowserView" bundle:nil];
    if (self)
	{
		callback = _callback;

		self.view.alpha = 0.;
		
		UIViewController* rootViewController = [[UIApplication sharedApplication].windows[0] rootViewController];
		[rootViewController addChildViewController:self];
		[rootViewController.view addSubview:self.view];
		
		[UIView animateWithDuration:kAnimationTime animations:^{
			self.view.alpha = 1.;
		}];
	}
	return self;
}

//------------------------------------------------------------------------
- (void)setFactoryPresets:(NSArray*)factoryPresets userPresets:(NSArray*)userPresets
{
	self.factoryPresets = factoryPresets;
	self.userPresets = userPresets;
	[self updatePresetArray];
	dispatch_async (dispatch_get_main_queue (), ^{
		[presetTableView reloadData];
	});
}

//------------------------------------------------------------------------
- (void)viewDidLoad
{
    [super viewDidLoad];
	
	containerView.layer.shadowOpacity = 0.5;
	containerView.layer.shadowOffset = CGSizeMake (5, 5);
	containerView.layer.shadowRadius = 5;
}

//------------------------------------------------------------------------
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

//------------------------------------------------------------------------
- (void)updatePresetArray
{
	if (self.userPresets)
	{
		self.displayPresets = [[self.factoryPresets arrayByAddingObjectsFromArray:self.userPresets] sortedArrayUsingComparator:^NSComparisonResult (NSURL* obj1, NSURL* obj2) {
			return [[obj1 lastPathComponent] caseInsensitiveCompare:[obj2 lastPathComponent]];
		}];
	}
	else
	{
		self.displayPresets = self.factoryPresets;
	}
}

//------------------------------------------------------------------------
- (void)removeSelf
{
	[UIView animateWithDuration:kAnimationTime animations:^{
		self.view.alpha = 0.;
	} completion:^(BOOL finished) {
		[self.view removeFromSuperview];
		[self removeFromParentViewController];
	}];
}

//------------------------------------------------------------------------
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	NSURL* url = [self.displayPresets objectAtIndex:indexPath.row];
	if (url)
	{
		callback ([[url path] UTF8String]);
	}
	[self removeSelf];
}

//------------------------------------------------------------------------
- (IBAction)toggleEditMode:(id)sender
{
	self.editMode = !self.editMode;
	if (self.editMode)
	{
		NSMutableArray* indexPaths = [NSMutableArray new];
		for (NSURL* url in self.factoryPresets)
		{
			NSUInteger index = [self.displayPresets indexOfObjectIdenticalTo:url];
			[indexPaths addObject:[NSIndexPath indexPathForRow:index inSection:0]];
		}
		[presetTableView deleteRowsAtIndexPaths:indexPaths withRowAnimation:UITableViewRowAnimationFade];
	}
	else
	{
		[self updatePresetArray];
		NSMutableArray* indexPaths = [NSMutableArray new];
		for (NSURL* url in self.factoryPresets)
		{
			NSUInteger index = [self.displayPresets indexOfObjectIdenticalTo:url];
			[indexPaths addObject:[NSIndexPath indexPathForRow:index inSection:0]];
		}
		[presetTableView insertRowsAtIndexPaths:indexPaths withRowAnimation:UITableViewRowAnimationFade];
	}
	[presetTableView setEditing:self.editMode animated:YES];
}

//------------------------------------------------------------------------
- (IBAction)cancel:(id)sender
{
	if (callback)
	{
		callback (0);
	}
	[self removeSelf];
}

//------------------------------------------------------------------------
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	if (self.editMode)
	{
		return [self.userPresets count];
	}
	return [self.displayPresets count];
}

//------------------------------------------------------------------------
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"PresetBrowserCell"];
	if (cell == nil)
	{
		cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:@"PresetBrowserCell"];
	}

	cell.backgroundColor = [UIColor clearColor];
	
	NSURL* presetUrl = nil;
	if (self.editMode)
	{
		presetUrl = [self.userPresets objectAtIndex:indexPath.row];
		cell.detailTextLabel.text = @"User";
	}
	else
	{
		presetUrl = [self.displayPresets objectAtIndex:indexPath.row];
		if ([self.factoryPresets indexOfObject:presetUrl] == NSNotFound)
		{
			cell.detailTextLabel.text = @"User";
		}
		else
		{
			cell.detailTextLabel.text = @"Factory";
		}
	}

	cell.textLabel.text = [[presetUrl lastPathComponent] stringByDeletingPathExtension];

	return cell;
}

//------------------------------------------------------------------------
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
	if (self.editMode)
	{
		return YES;
	}
	return NO;
}

//------------------------------------------------------------------------
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
	NSURL* presetUrl = [self.userPresets objectAtIndex:indexPath.row];
	if (presetUrl)
	{
		NSFileManager* fs = [NSFileManager defaultManager];
		NSError* error = nil;
		if ([fs removeItemAtURL:presetUrl error:&error] == NO)
		{
			UIAlertView *alert = [[UIAlertView alloc]
								  initWithTitle:[error localizedDescription]
								  message:[error localizedRecoverySuggestion]
								  delegate:nil
								  cancelButtonTitle:NSLocalizedString(@"Dismiss", @"")
								  otherButtonTitles:nil];
			
			[alert show];
		}
		else
		{
			NSMutableArray* newArray = [NSMutableArray arrayWithArray:self.userPresets];
			[newArray removeObject:presetUrl];
			self.userPresets = newArray;
			[presetTableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
		}
	}
}

@end
