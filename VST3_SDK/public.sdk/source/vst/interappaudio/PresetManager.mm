//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/interappaudio/PresetManager.mm
// Created by  : Steinberg, 10/2013
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

#import "PresetManager.h"
#import "public.sdk/source/vst/vstpresetfile.h"
#import "pluginterfaces/vst/ivstattributes.h"
#import "PresetBrowserViewController.h"
#import "PresetSaveViewController.h"

namespace Steinberg {
namespace Vst {
namespace InterAppAudio {

//-----------------------------------------------------------------------------
class PresetStream : public ReadOnlyBStream, public IStreamAttributes
{
public:
	PresetStream (IBStream* sourceStream, TSize sourceOffset, TSize sectionSize, const char* utf8Path)
	: ReadOnlyBStream (sourceStream, sourceOffset, sectionSize)
	, fileName (utf8Path)
	{
		fileName.toWideString (kCP_Utf8);
	}

	virtual tresult PLUGIN_API getFileName (String128 name)
	{
		if (fileName.length () > 0)
		{
			fileName.copyTo (name, 0, 128);
			return kResultTrue;
		}
		return kResultFalse;
	}
	virtual IAttributeList* PLUGIN_API getAttributes () { return 0; }

	DEF_INTERFACES_1(IStreamAttributes, ReadOnlyBStream)
	REFCOUNT_METHODS(ReadOnlyBStream)
protected:
	String fileName;
};

//-----------------------------------------------------------------------------
PresetManager::PresetManager (VST3Plugin* plugin, const TUID& cid)
: plugin (plugin)
, visiblePresetBrowserViewController (nil)
, visibleSavePresetViewController (nil)
, cid (cid)
{
	id obj = [[NSUserDefaults standardUserDefaults] objectForKey:@"PresetManager|lastPreset"];
	if (obj && [obj isKindOfClass:[NSString class]])
	{
		lastPreset = [obj UTF8String];
	}
}

//-----------------------------------------------------------------------------
NSArray* PresetManager::getPresetPaths (PresetPathType type)
{
	if (type == kFactory)
	{
		return [[NSBundle mainBundle] URLsForResourcesWithExtension:@"vstpreset" subdirectory:@"Presets"];
	}
	NSFileManager* fs = [NSFileManager defaultManager];
	NSURL* documentsUrl = [fs URLForDirectory:NSDocumentDirectory inDomain:NSUserDomainMask appropriateForURL:Nil create:YES error:NULL];
	if (documentsUrl)
	{
		NSMutableArray* userUrls = [NSMutableArray new];
		NSDirectoryEnumerator* enumerator = [fs enumeratorAtURL:documentsUrl includingPropertiesForKeys:nil options:NSDirectoryEnumerationSkipsSubdirectoryDescendants errorHandler:nil];
		for (NSURL* url in enumerator.allObjects)
		{
			if ([[url pathExtension] isEqualToString:@"vstpreset"])
			{
				[userUrls addObject:url];
			}
		}
		return [userUrls sortedArrayUsingComparator:^NSComparisonResult (NSURL* obj1, NSURL* obj2) {
					return [[obj1 lastPathComponent] caseInsensitiveCompare:[obj2 lastPathComponent]];
				}];
	}
	return nil;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PresetManager::runLoadPresetBrowser ()
{
	if (visiblePresetBrowserViewController != nil)
		return kResultFalse;

	addRef ();
	visiblePresetBrowserViewController = [[PresetBrowserViewController alloc] initWithCallback:[this](const char* path) {
											  loadPreset (path);
											  visiblePresetBrowserViewController = nil;
											  release ();
										  }];
	addRef ();
	dispatch_async (dispatch_get_global_queue (DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		if (visiblePresetBrowserViewController)
		{
			[visiblePresetBrowserViewController setFactoryPresets:getPresetPaths (kFactory) userPresets:getPresetPaths (kUser)];
		}
		release ();
	});
	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PresetManager::runSavePresetBrowser ()
{
	if (visibleSavePresetViewController != nil)
		return kResultFalse;

	addRef ();
	visibleSavePresetViewController = [[PresetSaveViewController alloc] initWithCallback:[this](const char* path) {
										   savePreset (path);
										   visibleSavePresetViewController = nil;
										   release ();
									   }];
	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PresetManager::loadNextPreset ()
{
	return loadPreset (true);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PresetManager::loadPreviousPreset ()
{
	return loadPreset (false);
}

//-----------------------------------------------------------------------------
tresult PresetManager::loadPreset (bool next)
{
	NSArray* presets = [[getPresetPaths (kFactory) arrayByAddingObjectsFromArray:getPresetPaths (kUser)] sortedArrayUsingComparator:^NSComparisonResult (NSURL* obj1, NSURL* obj2) {
		return [[obj1 lastPathComponent] caseInsensitiveCompare:[obj2 lastPathComponent]];
	}];

	__block NSUInteger index = NSNotFound;
	if (lastPreset.isEmpty() == false)
	{
		NSURL* lastUrl = [[NSURL fileURLWithPath:[NSString stringWithUTF8String:lastPreset]] fileReferenceURL];
		if (lastUrl)
		{
			[presets enumerateObjectsUsingBlock:^(NSURL* obj, NSUInteger idx, BOOL *stop) {
				if ([[obj fileReferenceURL] isEqual:lastUrl])
				{
					index = idx;
					*stop = YES;
				}
			}];
		}
	}
	if (index == NSNotFound)
	{
		if (next)
			index = [presets count] - 1;
		else
			index = 1;
	}
	if (index != NSNotFound)
	{
		if (next)
		{
			if (index >= [presets count] - 1)
				index = 0;
			else
				index++;
		}
		else
		{
			if (index == 0)
				index = [presets count] - 1;
			else
				index--;
		}
		return loadPreset ([[[presets objectAtIndex:index] path] UTF8String]);
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PresetManager::loadPreset (const char* path)
{
	if (path)
	{
		IPtr<IBStream> stream = owned (FileStream::open (path, "r"));
		if (stream)
		{
			[[NSUserDefaults standardUserDefaults] setObject:[NSString stringWithUTF8String:path] forKey:@"PresetManager|lastPreset"];
			lastPreset = path;
			FUnknownPtr<IComponent> component (plugin->getAudioProcessor ());
			IEditController* controller = plugin->getEditController ();
			if (component)
			{
				PresetFile pf (stream);
				if (!pf.readChunkList ())
					return kResultFalse;
				if (pf.getClassID () != cid)
					return kResultFalse;
				const PresetFile::Entry* e = pf.getEntry (kComponentState);
				if (e == 0)
					return kResultFalse;
				char* filename = strrchr (path, '/');
				if (filename)
					filename++;
				IPtr<PresetStream> readOnlyBStream = owned (new PresetStream (stream, e->offset, e->size, filename));
				tresult result = component->setState (readOnlyBStream);
				if ((result == kResultTrue || result == kNotImplemented) && controller)
				{
					readOnlyBStream->seek (0, IBStream::kIBSeekSet);
					controller->setComponentState (readOnlyBStream);
					if (pf.contains (kControllerState))
					{
						e = pf.getEntry (kControllerState);
						if (e)
						{
							readOnlyBStream = owned (new PresetStream (stream, e->offset, e->size, filename));
							controller->setState (readOnlyBStream);
						}
					}
				}
				return result;
			}
		}
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
void PresetManager::savePreset (const char* path)
{
	IBStream* stream = FileStream::open (path, "w");
	if (stream)
	{
		FUnknownPtr<IComponent> component (plugin->getAudioProcessor ());
		IEditController* controller = plugin->getEditController ();
		if (component)
		{
			PresetFile::savePreset (stream, cid, component, controller);
		}
		stream->release ();
		loadPreset (path);
	}
}

}}} // namespaces
