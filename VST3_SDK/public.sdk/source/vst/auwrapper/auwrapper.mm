//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/auwrapper/auwrapper.mm
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

/*

Things to do :
	- Parameter Mapping could be better (indexed stuff), but needs work if we want to have this
	- Speaker Arrangement -> Channel Layout (partially done, just doesn't work always in Logic 8)
	- dynamic Bus management

*/

#include "auwrapper.h"
#include "aucocoaview.h"
#include "NSDataIBStream.h"
#include "pluginterfaces/gui/iplugview.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/vst/vsttypes.h"
#include "pluginterfaces/vst/vstpresetkeys.h"
#include "public.sdk/source/vst/hosting/eventlist.h"
#include "public.sdk/source/vst/hosting/processdata.h"
#include "public.sdk/source/vst/hosting/parameterchanges.h"
#include "public.sdk/source/vst/hosting/hostclasses.h"
#include "public.sdk/source/vst/vsteditcontroller.h"
#include "base/source/fdynlib.h"
#include "base/source/fstring.h"
#include <AudioToolbox/AudioToolbox.h>
#if !__LP64__
#include <AudioUnit/AudioUnitCarbonView.h>
#endif
#include <dlfcn.h>
#include <algorithm>
#include <objc/runtime.h>
#if !CA_USE_AUDIO_PLUGIN_ONLY
#include "CAXException.h"
#endif

#define SMTG_MAKE_STRING_PRIVATE_DONT_USE(x) #x
#define SMTG_MAKE_STRING(x) SMTG_MAKE_STRING_PRIVATE_DONT_USE (x)

typedef bool (*bundleEntryPtr) (CFBundleRef);
typedef bool (*bundleExitPtr) (void);

#include <dlfcn.h>

// we ignore for the moment that the NSAddImage functions are deprecated
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

static bool CopyProcessPath (Steinberg::String& name)
{
	Dl_info info;
	if (dladdr ((const void*)CopyProcessPath, &info))
	{
		if (info.dli_fname)
		{
			name.assign (info.dli_fname);
#ifdef UNICODE
			name.toWideString ();
#endif
			return true;
		}
	}
	return false;
}

namespace Steinberg {

//------------------------------------------------------------------------
DEF_CLASS_IID (IPluginBase)
DEF_CLASS_IID (IBStream)
DEF_CLASS_IID (IPlugView)
DEF_CLASS_IID (IPluginFactory2)

//------------------------------------------------------------------------
class VST3DynLibrary : public FDynLibrary
{
public:
	VST3DynLibrary () : bundleEntryCalled (false) { gInstance = this; }

	~VST3DynLibrary ()
	{
		if (isLoaded ())
		{
			if (bundleEntryCalled)
			{
				if (bundleExitPtr bundleExit = (bundleExitPtr)getProcAddress ("bundleExit"))
					bundleExit ();
			}

#if defined(MAC_OS_X_VERSION_10_11)
			// workaround, because CFBundleCreate returns refcount == 2.
			if (CFBundleIsExecutableLoaded ((CFBundleRef)instance))
			{
				CFBundleUnloadExecutable ((CFBundleRef)instance);
				CFRelease ((CFBundleRef)instance);
			}
#else
			CFRelease ((CFBundleRef)instance);
#endif
			instance = 0;
			isloaded = false;
		}
		gInstance = 0;
	}

	bool init (const tchar* path)
	{
		if (isLoaded ())
			return true;

		isBundle = false;

		Steinberg::String name (path);

		if (name.getChar16 (0) != STR ('/')) // no absoltue path
		{
			Steinberg::String p;
			if (CopyProcessPath (p))
			{
				Steinberg::int32 index = p.findLast (STR ('/'));
				p.remove (index + 1);
				name = p + name;
			}
		}

		CFStringRef fsString = (CFStringRef)name.toCFStringRef ();
		CFURLRef url = CFURLCreateWithFileSystemPath (NULL, fsString, kCFURLPOSIXPathStyle, true);
		if (url)
		{
			CFBundleRef bundle = CFBundleCreate (NULL, url);
			if (bundle)
			{
				bundleEntryPtr bundleEntry = (bundleEntryPtr)CFBundleGetFunctionPointerForName (
				    bundle, CFSTR ("bundleEntry"));
				if (bundleEntry)
					bundleEntryCalled = bundleEntry (bundle);

				if (bundleEntryCalled)
				{
					isBundle = true;
					isloaded = true;
					instance = (void*)bundle;
				}
				else
					CFRelease (bundle);
			}
			CFRelease (url);
		}
		CFRelease (fsString);

		return isLoaded ();
	}

	static VST3DynLibrary* gInstance;

protected:
	bool bundleEntryCalled;
};

VST3DynLibrary* VST3DynLibrary::gInstance = 0;

namespace Vst {

const ParamID kNoParamId = std::numeric_limits<ParamID>::max ();

//------------------------------------------------------------------------
DEF_CLASS_IID (IEventList)
DEF_CLASS_IID (IHostApplication)
DEF_CLASS_IID (IParameterChanges)
DEF_CLASS_IID (IParamValueQueue)
DEF_CLASS_IID (IMessage)
DEF_CLASS_IID (IAttributeList)
DEF_CLASS_IID (IComponent)
DEF_CLASS_IID (IComponentHandler)
DEF_CLASS_IID (IAudioProcessor)
DEF_CLASS_IID (IEditController)
DEF_CLASS_IID (IMidiMapping)
DEF_CLASS_IID (IUnitInfo)
DEF_CLASS_IID (IConnectionPoint)
DEF_CLASS_IID (IVst3ToVst2Wrapper)
DEF_CLASS_IID (IVst3ToAUWrapper)

//--------------------------------------------------------------------------------------------
class SpeakerArrangementBase
{
public:
	SpeakerArrangementBase ()
	{
		channelLayout.mChannelBitmap = 0;
		channelLayout.mChannelLayoutTag = 0;
		channelLayout.mNumberChannelDescriptions = 0;
	}

	OSStatus setNumChannels (int32 numChannels)
	{
		switch (numChannels)
		{
			case 1: channelLayout.mChannelLayoutTag = kAudioChannelLayoutTag_Mono; break;
			case 2: channelLayout.mChannelLayoutTag = kAudioChannelLayoutTag_Stereo; break;
			case 6: channelLayout.mChannelLayoutTag = kAudioChannelLayoutTag_AudioUnit_5_1; break;
			default: return kAudioUnitErr_InvalidProperty;
		}
		return noErr;
	}

protected:
	AudioChannelLayout channelLayout;
};

//--------------------------------------------------------------------------------------------
class MultiChannelOutputElement : public AUOutputElement, public SpeakerArrangementBase
{
public:
	MultiChannelOutputElement (AUBase* audioUnit) : AUOutputElement (audioUnit) {}

	UInt32 GetChannelLayoutTags (AudioChannelLayoutTag* outLayoutTagsPtr)
	{
		if (AudioChannelLayoutTag_GetNumberOfChannels (channelLayout.mChannelLayoutTag) <= 2)
			return 0;

		if (outLayoutTagsPtr)
			*outLayoutTagsPtr = channelLayout.mChannelLayoutTag;
		return 1;
	}

	UInt32 GetAudioChannelLayout (AudioChannelLayout* outMapPtr, Boolean& outWritable)
	{
		if (AudioChannelLayoutTag_GetNumberOfChannels (channelLayout.mChannelLayoutTag) <= 2)
			return 0;

		if (outMapPtr)
			*outMapPtr = channelLayout;
		outWritable = false;
		return sizeof (AudioChannelLayout);
	}

	OSStatus SetAudioChannelLayout (const AudioChannelLayout& layout)
	{
		return kAudioUnitErr_InvalidProperty;
	}

	OSStatus SetStreamFormat (const CAStreamBasicDescription& desc)
	{
		OSStatus err = setNumChannels (desc.NumberChannels ());
		if (err != noErr)
			return err;
		return AUOutputElement::SetStreamFormat (desc);
	}
};

//--------------------------------------------------------------------------------------------
class MultiChannelInputElement : public AUInputElement, public SpeakerArrangementBase
{
public:
	MultiChannelInputElement (AUBase* audioUnit) : AUInputElement (audioUnit) {}

	UInt32 GetChannelLayoutTags (AudioChannelLayoutTag* outLayoutTagsPtr)
	{
		if (AudioChannelLayoutTag_GetNumberOfChannels (channelLayout.mChannelLayoutTag) <= 2)
			return 0;

		if (outLayoutTagsPtr)
			*outLayoutTagsPtr = channelLayout.mChannelLayoutTag;
		return 1;
	}

	UInt32 GetAudioChannelLayout (AudioChannelLayout* outMapPtr, Boolean& outWritable)
	{
		if (AudioChannelLayoutTag_GetNumberOfChannels (channelLayout.mChannelLayoutTag) <= 2)
			return 0;

		if (outMapPtr)
			memcpy (outMapPtr, &channelLayout, sizeof (AudioChannelLayout));
		outWritable = false;
		return sizeof (AudioChannelLayout);
	}

	OSStatus SetAudioChannelLayout (const AudioChannelLayout& layout)
	{
		return kAudioUnitErr_InvalidProperty;
	}

	OSStatus SetStreamFormat (const CAStreamBasicDescription& desc)
	{
		OSStatus err = setNumChannels (desc.NumberChannels ());
		if (err != noErr)
			return err;
		return AUInputElement::SetStreamFormat (desc);
	}
};

//------------------------------------------------------------------------
static CFStringRef createCFStringFromString128 (const String128& string)
{
	UString128 str (string);
	return CFStringCreateWithCharacters (0, (const UniChar*)string, str.getLength ());
}

//------------------------------------------------------------------------
static void createString128FromCFString (CFStringRef inString, String128& outString)
{
	CFStringGetCharacters (inString,
	                       CFRangeMake (0, std::max<CFIndex> (128, CFStringGetLength (inString))),
	                       (UniChar*)outString);
}

//------------------------------------------------------------------------
static CFBundleRef GetBundleFromExecutable (const char* filepath)
{
	AutoreleasePool ap;
	NSString* execStr = [NSString stringWithCString:filepath encoding:NSUTF8StringEncoding];
	NSString* macOSStr = [execStr stringByDeletingLastPathComponent];
	NSString* contentsStr = [macOSStr stringByDeletingLastPathComponent];
	NSString* bundleStr = [contentsStr stringByDeletingLastPathComponent];
	return CFBundleCreate (0, (CFURLRef)[NSURL fileURLWithPath:bundleStr isDirectory:YES]);
}

//------------------------------------------------------------------------
static CFBundleRef GetCurrentBundle ()
{
	Dl_info info;
	if (dladdr ((const void*)GetCurrentBundle, &info))
	{
		if (info.dli_fname)
		{
			return GetBundleFromExecutable (info.dli_fname);
		}
	}
	return 0;
}

//------------------------------------------------------------------------
CFBundleRef AUWrapper::gBundleRef = 0;
static CFIndex gBundleRefCount = 0;
static AUChannelInfo* channelInfos = 0;

//------------------------------------------------------------------------
static void initBundleRef ()
{
	if (AUWrapper::gBundleRef == 0)
	{
		AUWrapper::gBundleRef = GetCurrentBundle ();
		gBundleRefCount = CFGetRetainCount (AUWrapper::gBundleRef);
	}
	else
		CFRetain (AUWrapper::gBundleRef);
}

//------------------------------------------------------------------------
static void releaseBundleRef ()
{
	CFIndex currentCount = CFGetRetainCount (AUWrapper::gBundleRef);
	CFRelease (AUWrapper::gBundleRef);
	if (currentCount == gBundleRefCount)
		AUWrapper::gBundleRef = 0;
}

//------------------------------------------------------------------------
class AUHostApplication : public HostApplication, public IVst3ToAUWrapper
{
public:
	virtual tresult PLUGIN_API getName (String128 name) SMTG_OVERRIDE
	{
		String str ("VST3-AU Wrapper");
		str.copyTo (name, 0, 127);
		return kResultTrue;
	}

	DEFINE_INTERFACES
		DEF_INTERFACE (Vst::IVst3ToAUWrapper)
	END_DEFINE_INTERFACES (HostApplication)
	REFCOUNT_METHODS (HostApplication)
};

static AUHostApplication gHostApp;

//------------------------------------------------------------------------
static void paramChangedListenerProc (void* inRefCon, void* inObject,
                                      const AudioUnitParameter* inParameter,
                                      AudioUnitParameterValue inValue)
{
	AUWrapper* auWrapper = (AUWrapper*)inRefCon;
	if (inParameter && inObject != inRefCon)
		auWrapper->setControllerParameter (inParameter->mParameterID, inValue);
}

//------------------------------------------------------------------------
IMPLEMENT_FUNKNOWN_METHODS (AUWrapper, IComponentHandler, IComponentHandler::iid)
//------------------------------------------------------------------------
AUWrapper::AUWrapper (ComponentInstanceRecord* ci)
: AUWRAPPER_BASE_CLASS (ci, 0, 0)
, audioProcessor (0)
, editController (0)
, midiMapping (0)
, timer (0)
, noteCounter (0)
, sampleRate (44100)
, bypassParamID (-1)
, presets (0)
, numPresets (0)
, factoryProgramChangedID (-1)
, isInstrument (false)
, isBypassed (false)
, paramListenerRef (0)
, midiOutCount (0)
{
	FUNKNOWN_CTOR
	AutoreleasePool ap;
	initBundleRef ();

	for (int32 i = 0; i < kMaxProgramChangeParameters; i++)
		programChangeParameters[i] = kNoParamId;

	processData.processContext = &processContext;
	processData.inputParameterChanges = &processParamChanges;
	processData.outputParameterChanges = &outputParamChanges;
	eventList.setMaxSize (128);
	outputEvents.setMaxSize (128);
	processData.inputEvents = &eventList;
	processData.outputEvents = &outputEvents;

	loadVST3Module ();

	FUnknownPtr<IPluginFactory2> factory (owned (getFactory ()));
	if (factory)
	{
		// find first audio processor class
		for (int32 i = 0; i < factory->countClasses (); i++)
		{
			PClassInfo2 ci;
			if (factory->getClassInfo2 (i, &ci) == kResultTrue)
			{
				if (strcmp (ci.category, kVstAudioEffectClass) == 0)
				{
					if (factory->createInstance (ci.cid, IAudioProcessor::iid,
					                             (void**)&audioProcessor) == kResultTrue)
					{
						ConstString plugCategory (ci.subCategories);
						if (plugCategory.findFirst ("Instrument", -1,
						                            ConstString::kCaseInsensitive) >= 0)
							isInstrument = true;
						break;
					}
				}
			}
		}
	}
	if (audioProcessor)
	{
		if (FUnknownPtr<IPluginBase> (audioProcessor)->initialize ((HostApplication*)&gHostApp) !=
		    kResultTrue)
			return;

		if (audioProcessor->queryInterface (IEditController::iid, (void**)&editController) !=
		    kResultTrue)
		{
			FUnknownPtr<IComponent> component (audioProcessor);
			if (component)
			{
				TUID ccid;
				if (component->getControllerClassId (ccid) == kResultTrue)
				{
					if (factory->createInstance (ccid, IEditController::iid,
					                             (void**)&editController) == kResultTrue)
					{
						if (editController->initialize ((HostApplication*)&gHostApp) != kResultTrue)
						{
							editController->release ();
							editController = 0;
							return;
						}

						FUnknownPtr<IConnectionPoint> controllerConnectionPoint (editController);
						FUnknownPtr<IConnectionPoint> processorConnectionPoint (audioProcessor);
						if (controllerConnectionPoint && processorConnectionPoint)
						{
							controllerConnectionPoint->connect (processorConnectionPoint);
							processorConnectionPoint->connect (controllerConnectionPoint);
						}

						NSMutableData* processorData = [[[NSMutableData alloc] init] autorelease];
						NSMutableDataIBStream stream (processorData);
						if (FUnknownPtr<IComponent> (audioProcessor)->getState (&stream) ==
						    kResultTrue)
						{
							stream.seek (0, IBStream::kIBSeekSet);
							editController->setComponentState (&stream);
						}
					}
				}
			}
		}
		if (editController)
		{
			editController->setComponentHandler (this);
			// initialize buses
			FUnknownPtr<IComponent> component (audioProcessor);
			int32 inputBusCount = component->getBusCount (kAudio, kInput);
			int32 outputBusCount = component->getBusCount (kAudio, kOutput);

			CreateElements ();
			Inputs ().SetNumberOfElements (inputBusCount);
			Outputs ().SetNumberOfElements (outputBusCount);

			SpeakerArrangement sa;
			for (int32 inputNo = 0; inputNo < inputBusCount; inputNo++)
			{
				MultiChannelInputElement* element =
				    dynamic_cast<MultiChannelInputElement*> (Inputs ().GetIOElement (inputNo));
				if (element == 0)
					continue;
				if (audioProcessor->getBusArrangement (kInput, inputNo, sa) == kResultTrue)
				{
					CAStreamBasicDescription streamDesc (element->GetStreamFormat ());
					streamDesc.ChangeNumberChannels (SpeakerArr::getChannelCount (sa), false);
					element->SetStreamFormat (streamDesc);
				}
				BusInfo info = {0};
				if (component->getBusInfo (kAudio, kInput, inputNo, info) == kResultTrue)
				{
					String busName (info.name);
					CFStringRef busNameString = (CFStringRef)busName.toCFStringRef ();
					element->SetName (busNameString);
					CFRelease (busNameString);
				}
				component->activateBus (kAudio, kInput, inputNo, true);
			}
			for (int32 outputNo = 0; outputNo < outputBusCount; outputNo++)
			{
				MultiChannelOutputElement* element =
				    dynamic_cast<MultiChannelOutputElement*> (Outputs ().GetIOElement (outputNo));
				if (element == 0)
					continue;
				if (audioProcessor->getBusArrangement (kOutput, outputNo, sa) == kResultTrue)
				{
					CAStreamBasicDescription streamDesc (element->GetStreamFormat ());
					streamDesc.ChangeNumberChannels (SpeakerArr::getChannelCount (sa), false);
					element->SetStreamFormat (streamDesc);
				}
				BusInfo info = {0};
				if (component->getBusInfo (kAudio, kOutput, outputNo, info) == kResultTrue)
				{
					String busName (info.name);
					CFStringRef busNameString = (CFStringRef)busName.toCFStringRef ();
					element->SetName (busNameString);
					CFRelease (busNameString);
				}
				component->activateBus (kAudio, kOutput, outputNo, true);
			}
			processData.prepare (*component);

			// initialize parameters
			syncParameterValues ();
			cacheParameterValues ();

			midiOutCount = component->getBusCount (kEvent, kOutput);

			editController->queryInterface (IMidiMapping::iid, (void**)&midiMapping);

			transferParamChanges.setMaxParameters (500);
			outputParamTransfer.setMaxParameters (500);

			timer = Timer::create (this, 20);
		}
	}
}

//------------------------------------------------------------------------
AUWrapper::~AUWrapper ()
{
	AutoreleasePool ap;
	if (timer)
		timer->release ();
	if (paramListenerRef)
		AUListenerDispose (paramListenerRef);
	if (audioProcessor)
	{
		FUnknownPtr<IEditController> combined (audioProcessor);
		if (!combined)
		{
			FUnknownPtr<IConnectionPoint> controllerConnectionPoint (editController);
			FUnknownPtr<IConnectionPoint> processorConnectionPoint (audioProcessor);
			if (controllerConnectionPoint && processorConnectionPoint)
			{
				controllerConnectionPoint->disconnect (processorConnectionPoint);
				processorConnectionPoint->disconnect (controllerConnectionPoint);
			}
		}
	}
	if (midiMapping)
	{
		midiMapping->release ();
		midiMapping = 0;
	}
	if (editController)
	{
		editController->setComponentHandler (0);
		uint32 refCount = editController->addRef ();
		if (refCount == 2)
			editController->terminate ();

		editController->release ();
		editController->release ();
		editController = 0;
	}

	clearParameterValueCache ();

	if (audioProcessor)
	{
		FUnknownPtr<IPluginBase> (audioProcessor)->terminate ();
		audioProcessor->release ();
		audioProcessor = 0;
	}
	unloadVST3Module ();
	releaseBundleRef ();

	if (presets)
		delete presets;

	FUNKNOWN_DTOR
}

//------------------------------------------------------------------------
void AUWrapper::loadVST3Module ()
{
	if (VST3DynLibrary::gInstance == 0)
	{
		if (gBundleRef)
		{
			String pluginPath;
			CFStringRef pluginNameStr = (CFStringRef)CFBundleGetValueForInfoDictionaryKey (
			    gBundleRef, CFSTR ("VST3 Plug-in"));
			if (!pluginNameStr)
			{
				NSURL* pluginUrl = (NSURL*)CFBundleCopyResourceURL (gBundleRef, CFSTR ("plugin"),
				                                                    CFSTR ("vst3"), NULL);
				if (!pluginUrl)
				{
					NSLog (
					    @"VST3 Plug-in not defined in Info Dictionary and not included in bundle");
					return;
				}
				pluginPath.fromCFStringRef ((CFStringRef)[pluginUrl path]);
				[pluginUrl release];
			}
			else
			{
				pluginPath.fromCFStringRef (pluginNameStr);
				if (!pluginPath.getChar (0) != STR ('/'))
				{
					FSRef fsRef;
					if (FSFindFolder (kLocalDomain, kAudioPlugInsFolderType, false, &fsRef) ==
					    noErr)
					{
						NSURL* url = (NSURL*)CFURLCreateFromFSRef (0, &fsRef);
						if (url)
						{
							String basePath ([[url path] UTF8String]);
							basePath.toWideString (kCP_Utf8);
							pluginPath.insertAt (0, STR ("/VST3/"));
							pluginPath.insertAt (0, basePath);
							[url release];
						}
					}
				}
			}

			new VST3DynLibrary ();
			if (pluginPath.isEmpty () || !VST3DynLibrary::gInstance->init (pluginPath))
			{
				VST3DynLibrary::gInstance->release ();
				return;
			}
		}
	}
	else
		VST3DynLibrary::gInstance->addRef ();
}

//------------------------------------------------------------------------
void AUWrapper::unloadVST3Module ()
{
	if (VST3DynLibrary::gInstance)
	{
		VST3DynLibrary::gInstance->release ();
	}
}

//------------------------------------------------------------------------
IPluginFactory* AUWrapper::getFactory ()
{
	if (VST3DynLibrary::gInstance)
	{
		GetFactoryProc getPlugFactory =
		    (GetFactoryProc)VST3DynLibrary::gInstance->getProcAddress ("GetPluginFactory");
		if (getPlugFactory)
			return getPlugFactory ();
	}
	return 0;
}

//------------------------------------------------------------------------
// ComponentBase
ComponentResult AUWrapper::Version ()
{
	AutoreleasePool ap;
	NSString* versionString =
	    (NSString*)CFBundleGetValueForInfoDictionaryKey (gBundleRef, CFSTR ("AudioUnit Version"));
	if (versionString)
	{
		int version = 0;
		if (sscanf ([versionString UTF8String], "%x", &version) == 1)
			return version;
	}
	return 0xFFFFFFFF;
}

//------------------------------------------------------------------------
void AUWrapper::PostConstructor ()
{
	AUWRAPPER_BASE_CLASS::PostConstructor ();
}

//------------------------------------------------------------------------
static SpeakerArrangement numChannelsToSpeakerArrangement (UInt32 numChannels)
{
	switch (numChannels)
	{
		case 1: return SpeakerArr::kMono;
		case 2: return SpeakerArr::kStereo;
		case 6: return SpeakerArr::k51;
	}
	return 0;
}

//------------------------------------------------------------------------
ComponentResult AUWrapper::Initialize ()
{
	if (audioProcessor && editController)
	{
		// match speaker arrangement with AU stream format
		FUnknownPtr<IComponent> component (audioProcessor);
		int32 inputBusCount = component->getBusCount (kAudio, kInput);
		int32 outputBusCount = component->getBusCount (kAudio, kOutput);

		SupportedNumChannels (0); // initialize channelInfos
		if (channelInfos)
		{
			int32 maxBusCount = std::max (inputBusCount, outputBusCount);
			for (int32 i = 0; i < maxBusCount; i++)
			{
				int32 inChannelCount = 0;
				int32 outChannelCount = 0;
				if (inputBusCount > i)
					inChannelCount =
					    Inputs ().GetIOElement (i)->GetStreamFormat ().NumberChannels ();
				if (outputBusCount > i)
					outChannelCount =
					    Outputs ().GetIOElement (i)->GetStreamFormat ().NumberChannels ();
				if (!validateChannelPair (inChannelCount, outChannelCount, channelInfos,
				                          SupportedNumChannels (0)))
					return kAudioUnitErr_FailedInitialization;
			}
		}

		SpeakerArrangement inputs[inputBusCount];
		SpeakerArrangement outputs[outputBusCount];
		for (int32 element = 0; element < inputBusCount; element++)
		{
			const CAStreamBasicDescription desc =
			    Inputs ().GetIOElement (element)->GetStreamFormat ();
			inputs[element] = numChannelsToSpeakerArrangement (desc.NumberChannels ());
		}
		for (int32 element = 0; element < outputBusCount; element++)
		{
			const CAStreamBasicDescription desc =
			    Outputs ().GetIOElement (element)->GetStreamFormat ();
			outputs[element] = numChannelsToSpeakerArrangement (desc.NumberChannels ());
		}
		if (audioProcessor->setBusArrangements (inputs, inputBusCount, outputs, outputBusCount) !=
		    kResultTrue)
		{
			return kAudioUnitErr_FailedInitialization;
		}

		// After set Bus Arrangement, the channelbuffers may need to be reallocated -> hence the
		// second prepare!
		processData.prepare (*component);

		ProcessSetup ps;
		ps.sampleRate = getSampleRate ();
		ps.maxSamplesPerBlock = GetMaxFramesPerSlice ();
		ps.symbolicSampleSize = kSample32;
		ps.processMode = kRealtime;
		audioProcessor->setupProcessing (ps);

		component->setActive (true);

		audioProcessor->setProcessing (true);

		if (paramListenerRef == 0)
		{
			OSStatus status =
			    AUListenerCreate (paramChangedListenerProc, this, CFRunLoopGetCurrent (),
			                      kCFRunLoopDefaultMode, 0.2, &paramListenerRef);
			ASSERT (status == noErr)
			if (paramListenerRef)
			{
				AudioUnitParameter sPar;
				sPar.mAudioUnit = GetComponentInstance ();
				sPar.mParameterID = 0;
				sPar.mElement = 0;
				sPar.mScope = kAudioUnitScope_Global;

				std::vector<ParameterInfo> programParameters;

				// for each VST3 parameter
				for (int32 i = 0; i < editController->getParameterCount (); i++)
				{
					ParameterInfo pi = {0};
					editController->getParameterInfo (i, pi);

					// do not register bypass
					if ((pi.flags & ParameterInfo::kIsBypass) != 0)
					{
						continue;
					}

					sPar.mParameterID = pi.id;
					status = AUListenerAddParameter (paramListenerRef, 0, &sPar);
					ASSERT (status == noErr)

					if ((pi.flags & ParameterInfo::kIsProgramChange) != 0)
					{
						programParameters.push_back (pi);
					}
				}

				// assign programChanges
				for (int32 midiChannel = 0; midiChannel < kMaxProgramChangeParameters;
				     midiChannel++)
				{
					programChangeParameters[midiChannel] = kNoParamId;
					UnitID unitId;
					ProgramListID programListId;
					if (getProgramListAndUnit (midiChannel, unitId, programListId))
					{
						for (int32 i = 0; i < (int32)programParameters.size (); i++)
						{
							const ParameterInfo& paramInfo = programParameters.at (i);
							if (paramInfo.unitId == unitId)
							{
								programChangeParameters[midiChannel] = paramInfo.id;
								break;
							}
						}
					}
				}

				IUnitInfo* unitInfoController = NULL;
				// let's see if there's a preset list (take the first one)
				if (editController->queryInterface (IUnitInfo::iid, (void**)&unitInfoController) ==
				        kResultTrue &&
				    unitInfoController)
				{
					ProgramListInfo programListInfo;
					if (unitInfoController->getProgramListInfo (0, programListInfo) == kResultTrue)
					{
						factoryProgramChangedID = -1;
						numPresets = programListInfo.programCount;
						if (programListInfo.programCount > 0)
						{
							UnitID unitId = -1;

							// find the unit supporting this ProgramList
							IUnitInfo* unitInfo = NULL;
							if (editController->queryInterface (IUnitInfo::iid,
							                                    (void**)&unitInfo) == kResultTrue &&
							    unitInfo)
							{
								int32 unitCount = unitInfo->getUnitCount ();
								for (int32 i = 0; i < unitCount; i++)
								{
									UnitInfo unitInfoStruct = {0};
									if (unitInfo->getUnitInfo (i, unitInfoStruct) == kResultTrue)
									{
										if (programListInfo.id == unitInfoStruct.programListId)
										{
											unitId = unitInfoStruct.id;
											break;
										}
									}
								}

								unitInfo->release ();
							}
							if (unitId != -1)
							{
								// find the associated ProgramChange parameter ID
								for (int32 i = 0; i < (int32)programParameters.size (); i++)
								{
									const ParameterInfo& paramInfo = programParameters.at (i);
									if (paramInfo.unitId == unitId)
									{
										factoryProgramChangedID = paramInfo.id;
										break;
									}
								}
								if (factoryProgramChangedID != -1)
								{
									presets = new AUPreset[programListInfo.programCount];
									for (int32 i = 0; i < programListInfo.programCount; i++)
									{
										String128 name;
										unitInfoController->getProgramName (programListInfo.id, i,
										                                    name);
										presets[i].presetNumber = i;
										presets[i].presetName = createCFStringFromString128 (name);
									}
								}
							}
						}
					}
					unitInfoController->release ();
				}
			}
		}

		return AUWRAPPER_BASE_CLASS::Initialize ();
	}
	return -1;
}

//------------------------------------------------------------------------
void AUWrapper::Cleanup ()
{
	if (audioProcessor)
	{
		audioProcessor->setProcessing (false);

		FUnknownPtr<IComponent> component (audioProcessor);
		component->setActive (false);
	}
}

//------------------------------------------------------------------------
// AUBase
//--------------------------------------------------------------------------------------------
AUElement* AUWrapper::CreateElement (AudioUnitScope scope, AudioUnitElement element)
{
	switch (scope)
	{
		case kAudioUnitScope_Output: return new MultiChannelOutputElement (this);
		case kAudioUnitScope_Input: return new MultiChannelInputElement (this);
	}

	return AUWRAPPER_BASE_CLASS::CreateElement (scope, element);
}

//------------------------------------------------------------------------
UInt32 AUWrapper::SupportedNumChannels (const AUChannelInfo** outInfo)
{
	static UInt32 numChannelInfos = 0;
	static bool once = true;
	if (once && gBundleRef)
	{
		once = false;
		char buffer[128];

		CFStringRef processName = 0;
		ProcessSerialNumber psn;
		OSErr err = GetCurrentProcess (&psn);
		ASSERT (err == noErr);
		OSStatus stat = CopyProcessName (&psn, &processName);
		ASSERT (stat == noErr);
		CFStringGetCString (processName, buffer, sizeof (buffer), kCFStringEncodingUTF8);
		CFRelease (processName);
		strncat (buffer, " SupportedNumChannels", sizeof (buffer) - strlen (buffer) - 1);
		CFStringRef dictName =
		    CFStringCreateWithCString (kCFAllocatorDefault, buffer, kCFStringEncodingUTF8);

		NSArray* supportedNumChannelsArray =
		    (NSArray*)CFBundleGetValueForInfoDictionaryKey (gBundleRef, dictName);
		if (!supportedNumChannelsArray)
			supportedNumChannelsArray = (NSArray*)CFBundleGetValueForInfoDictionaryKey (
			    gBundleRef, CFSTR ("AudioUnit SupportedNumChannels"));
		if (supportedNumChannelsArray)
		{
			numChannelInfos = [supportedNumChannelsArray count];
			if (numChannelInfos > 0)
			{
				channelInfos = new AUChannelInfo[numChannelInfos];
				int i = 0;
				for (NSDictionary* dict in supportedNumChannelsArray)
				{
					NSInteger inputs = [[dict objectForKey:@"Inputs"] integerValue];
					NSInteger outputs = [[dict objectForKey:@"Outputs"] integerValue];
					channelInfos[i].inChannels = inputs;
					channelInfos[i++].outChannels = outputs;
				}
			}
		}
		CFRelease (dictName);
	}
	if (outInfo)
		*outInfo = channelInfos;
	return numChannelInfos;
}

//------------------------------------------------------------------------
bool AUWrapper::StreamFormatWritable (AudioUnitScope scope, AudioUnitElement element)
{
	return IsInitialized () ? false : true;
}

//------------------------------------------------------------------------
ComponentResult AUWrapper::ChangeStreamFormat (AudioUnitScope inScope, AudioUnitElement inElement,
                                               const CAStreamBasicDescription& inPrevFormat,
                                               const CAStreamBasicDescription& inNewFormat)
{
	if (inPrevFormat.NumberChannels () == inNewFormat.NumberChannels ())
	{
		// sample rate change
		ComponentResult res = AUWRAPPER_BASE_CLASS::ChangeStreamFormat (inScope, inElement,
		                                                                inPrevFormat, inNewFormat);
		if (res == noErr)
			sampleRate = inNewFormat.mSampleRate;
		return res;
	}
	else if (inPrevFormat.mSampleRate != inNewFormat.mSampleRate)
		sampleRate = inNewFormat.mSampleRate;
	else if (IsInitialized () || SupportedNumChannels (0) == 0)
		return kAudioUnitErr_Initialized;

	AUIOElement* element;

	switch (inScope)
	{
		case kAudioUnitScope_Input: element = Inputs ().GetIOElement (inElement); break;
		case kAudioUnitScope_Output: element = Outputs ().GetIOElement (inElement); break;
		case kAudioUnitScope_Global: element = Outputs ().GetIOElement (0); break;
		default: COMPONENT_THROW (kAudioUnitErr_InvalidScope);
	}
	OSStatus err = element->SetStreamFormat (inNewFormat);
	if (err == noErr)
		PropertyChanged (kAudioUnitProperty_StreamFormat, inScope, inElement);
	return err;
}

//------------------------------------------------------------------------
ComponentResult AUWrapper::SetConnection (const AudioUnitConnection& inConnection)
{
	ComponentResult result = AUWRAPPER_BASE_CLASS::SetConnection (inConnection);
	if (result == noErr)
	{
		int32 busIndex = inConnection.destInputNumber;
		bool active = GetInput (busIndex)->IsActive ();
		FUnknownPtr<IComponent> component (audioProcessor);
		component->activateBus (kAudio, kInput, busIndex, active);
	}
	return result;
}

static const UnitInfo kNoUnitInfo = {0};
//------------------------------------------------------------------------
void AUWrapper::buildUnitInfos (IUnitInfo* unitInfoController, UnitInfoMap& units) const
{
	units.clear ();
	if (unitInfoController)
	{
		int32 numUnits = unitInfoController->getUnitCount ();
		for (int32 i = 0; i < numUnits; i++)
		{
			UnitInfo ui;
			if (unitInfoController->getUnitInfo (i, ui) == kResultTrue)
				units[ui.id] = ui;
		}
	}
}

//------------------------------------------------------------------------
ComponentResult AUWrapper::GetParameterInfo (AudioUnitScope inScope,
                                             AudioUnitParameterID inParameterID,
                                             AudioUnitParameterInfo& outParameterInfo)
{
	if (inScope != kAudioUnitScope_Global)
		return kAudioUnitErr_InvalidScope;

	FGuard guard (parameterCacheChanging);
	CachedParameterInfoMap::const_iterator it = cachedParameterInfos.find (inParameterID);
	if (it == cachedParameterInfos.end ())
		return kAudioUnitErr_InvalidParameter;

	memcpy (&outParameterInfo, &it->second, sizeof (AudioUnitParameterInfo));
	if (outParameterInfo.cfNameString)
		CFRetain (outParameterInfo.cfNameString);
	if (outParameterInfo.unitName)
		CFRetain (outParameterInfo.unitName);

	return noErr;
}

//------------------------------------------------------------------------
ComponentResult AUWrapper::SetParameter (AudioUnitParameterID inID, AudioUnitScope inScope,
                                         AudioUnitElement inElement,
                                         AudioUnitParameterValue inValue,
                                         UInt32 inBufferOffsetInFrames)
{
	if (inScope == kAudioUnitScope_Global)
	{
		if (inID == factoryProgramChangedID)
		{
			SetAFactoryPresetAsCurrent (presets[(int)((float)inValue * (float)numPresets)]);
			PropertyChanged (kAudioUnitProperty_PresentPreset, kAudioUnitScope_Global, 0);
			PropertyChanged (kAudioUnitProperty_CurrentPreset, kAudioUnitScope_Global, 0);
		}

		transferParamChanges.addChange (inID, inValue, inBufferOffsetInFrames);
	}
	return AUWRAPPER_BASE_CLASS::SetParameter (inID, inScope, inElement, inValue,
	                                           inBufferOffsetInFrames);
}

//------------------------------------------------------------------------
void AUWrapper::setControllerParameter (ParamID pid, ParamValue value)
{
	if (editController && pid != factoryProgramChangedID)
		editController->setParamNormalized (pid, value);
}

//------------------------------------------------------------------------
ComponentResult AUWrapper::SaveState (CFPropertyListRef* outData)
{
	ComponentResult result = AUWRAPPER_BASE_CLASS::SaveState (outData);
	if (result != noErr)
		return result;

	NSMutableDictionary* dict = (NSMutableDictionary*)*outData;

	AutoreleasePool ap;
	NSMutableData* processorData = [[[NSMutableData alloc] init] autorelease];
	NSMutableData* controllerData = [[[NSMutableData alloc] init] autorelease];
	if (audioProcessor)
	{
		NSMutableDataIBStream stream (processorData);
		if (FUnknownPtr<IComponent> (audioProcessor)->getState (&stream) != kResultTrue)
		{
			[processorData setLength:0];
		}
	}
	if (editController)
	{
		NSMutableDataIBStream stream (controllerData);
		if (editController->getState (&stream) != kResultTrue)
		{
			[controllerData setLength:0];
		}
	}
	[dict setValue:processorData forKey:@"Processor State"];
	[dict setValue:controllerData forKey:@"Controller State"];
	*outData = dict;
	return result;
}

//------------------------------------------------------------------------
ComponentResult AUWrapper::RestoreState (CFPropertyListRef inData)
{
	return restoreState (inData, false);
}

//------------------------------------------------------------------------
ComponentResult AUWrapper::restoreState (CFPropertyListRef inData, bool fromProject)
{
	AutoreleasePool ap;

	if (CFGetTypeID (inData) != CFDictionaryGetTypeID ())
		return kAudioUnitErr_InvalidPropertyValue;

	AudioComponentDescription desc = GetComponentDescription ();

	CFDictionaryRef dict = static_cast<CFDictionaryRef> (inData);

	if (CFDictionaryContainsKey (
	        dict, [NSString stringWithCString:kAUPresetPartKey encoding:NSASCIIStringEncoding]))
		return kAudioUnitErr_InvalidPropertyValue;

#define kCurrentSavedStateVersion 0

	CFNumberRef cfnum = reinterpret_cast<CFNumberRef> (CFDictionaryGetValue (
	    dict, [NSString stringWithCString:kAUPresetVersionKey encoding:NSASCIIStringEncoding]));
	if (cfnum == NULL)
		return kAudioUnitErr_InvalidPropertyValue;
	SInt32 value;
	CFNumberGetValue (cfnum, kCFNumberSInt32Type, &value);
	if (value != kCurrentSavedStateVersion)
		return kAudioUnitErr_InvalidPropertyValue;

	cfnum = reinterpret_cast<CFNumberRef> (CFDictionaryGetValue (
	    dict, [NSString stringWithCString:kAUPresetSubtypeKey encoding:NSASCIIStringEncoding]));
	if (cfnum == NULL)
		return kAudioUnitErr_InvalidPropertyValue;
	CFNumberGetValue (cfnum, kCFNumberSInt32Type, &value);
	if (UInt32 (value) != desc.componentSubType)
		return kAudioUnitErr_InvalidPropertyValue;

	cfnum = reinterpret_cast<CFNumberRef> (
	    CFDictionaryGetValue (dict, [NSString stringWithCString:kAUPresetManufacturerKey
	                                                   encoding:NSASCIIStringEncoding]));
	if (cfnum == NULL)
		return kAudioUnitErr_InvalidPropertyValue;
	CFNumberGetValue (cfnum, kCFNumberSInt32Type, &value);
	if (UInt32 (value) != desc.componentManufacturer)
		return kAudioUnitErr_InvalidPropertyValue;

	ComponentResult result = noErr;

	if (result == noErr && audioProcessor && editController &&
	    CFGetTypeID (inData) == CFDictionaryGetTypeID ())
	{
		// Note: the bypass state is not part of the AU state
		bool wasBypassed = false;
		if (bypassParamID != -1)
		{
			wasBypassed = editController->getParamNormalized (bypassParamID) >= 0.5 ? true : false;
		}

		NSDictionary* dict = (NSDictionary*)inData;

		NSData* processorData = [dict valueForKey:@"Processor State"];
		int processLen = [processorData length];
		if (processLen == 0)
			return kAudioUnitErr_InvalidPropertyValue;

		if (processorData)
		{
			NSDataIBStream stream (processorData);
			if (fromProject)
				stream.getAttributes ()->setString (Vst::PresetAttributes::kStateType,
				                                    String (Vst::StateType::kProject));
			FUnknownPtr<IComponent> (audioProcessor)->setState (&stream);
		}

		NSData* controllerData = [dict valueForKey:@"Controller State"];
		if (controllerData)
		{
			NSDataIBStream processorStream (processorData);
			editController->setComponentState (&processorStream);

			NSDataIBStream stream (controllerData);
			if (fromProject)
				stream.getAttributes ()->setString (Vst::PresetAttributes::kStateType,
				                                    String (Vst::StateType::kProject));
			editController->setState (&stream);
			syncParameterValues ();
			cacheParameterValues ();
		}
		if (bypassParamID != -1)
		{
			transferParamChanges.addChange (bypassParamID, wasBypassed ? 1 : 0, 0);
			editController->setParamNormalized (bypassParamID, wasBypassed ? 1 : 0);
		}
	}
	return result;
}

//------------------------------------------------------------------------
OSStatus AUWrapper::GetPresets (CFArrayRef* outData) const
{
	if (presets && numPresets > 0)
	{
		if (outData != 0)
		{
			CFMutableArrayRef presetsArray = CFArrayCreateMutable (NULL, numPresets, NULL);
			for (int32 i = 0; i < numPresets; i++)
			{
				CFArrayAppendValue (presetsArray, &presets[i]);
			}
			*outData = (CFArrayRef)presetsArray;
		}
		return noErr;
	}
	return kAudioUnitErr_InvalidProperty;
}

//------------------------------------------------------------------------
OSStatus AUWrapper::NewFactoryPresetSet (const AUPreset& inNewFactoryPreset)
{
	if (numPresets > 0)
	{
		float normalizedValue = (float)inNewFactoryPreset.presetNumber / (float)numPresets;
		transferParamChanges.addChange (factoryProgramChangedID, normalizedValue, 0);
		editController->setParamNormalized (factoryProgramChangedID, normalizedValue);

		if (inNewFactoryPreset.presetNumber < numPresets)
			SetAFactoryPresetAsCurrent (presets[inNewFactoryPreset.presetNumber]);
		return noErr;
	}
	return kAudioUnitErr_InvalidProperty;
}

//------------------------------------------------------------------------
ComponentResult AUWrapper::Render (AudioUnitRenderActionFlags& ioActionFlags,
                                   const AudioTimeStamp& inTimeStamp, UInt32 inNumberFrames)
{
	updateProcessContext ();
	processContext.systemTime = inTimeStamp.mHostTime;

	processParamChanges.clearQueue ();
	transferParamChanges.transferChangesTo (processParamChanges);
	processData.numSamples = inNumberFrames;

	for (int32 i = 0; i < Inputs ().GetNumberOfElements (); i++)
	{
		AUInputElement* input = GetInput (i);
		if (input->IsActive ())
			input->PullInput (ioActionFlags, inTimeStamp, i, inNumberFrames);
		processData.inputs[i].numChannels = input->GetStreamFormat ().NumberChannels ();
		for (int32 channel = 0; channel < input->GetStreamFormat ().NumberChannels (); channel++)
		{
			processData.inputs[i].channelBuffers32[channel] =
			    input->IsActive () ? (Sample32*)input->GetBufferList ().mBuffers[channel].mData : 0;
		}
	}
	for (int32 i = 0; i < Outputs ().GetNumberOfElements (); i++)
	{
		AUOutputElement* output = GetOutput (i);
		output->PrepareBuffer (inNumberFrames);
		processData.outputs[i].numChannels = output->GetStreamFormat ().NumberChannels ();
		for (int32 channel = 0; channel < output->GetStreamFormat ().NumberChannels (); channel++)
		{
			processData.outputs[i].channelBuffers32[channel] =
			    (Sample32*)output->GetBufferList ().mBuffers[channel].mData;
		}
	}
	audioProcessor->process (processData);

	outputParamTransfer.transferChangesFrom (outputParamChanges);
	outputParamChanges.clearQueue ();
	eventList.clear ();

	ioActionFlags &= ~kAudioUnitRenderAction_OutputIsSilence;

	processOutputEvents (inTimeStamp);

	return noErr;
}

const uint8 kNoteOff = 0x80; ///< note, off velocity
const uint8 kNoteOn = 0x90; ///< note, on velocity
const uint8 kPolyPressure = 0xA0; ///< note, pressure
const uint8 kController = 0xB0; ///< controller, value
const uint8 kProgramChangeStatus = 0xC0; ///< program change
const uint8 kAfterTouchStatus = 0xD0; ///< channel pressure
const uint8 kPitchBendStatus = 0xE0; ///< lsb, msb

const float kMidiScaler = 1.f / 127.f;
static const uint8 kChannelMask = 0x0F;
static const uint8 kStatusMask = 0xF0;
static const uint32 kDataMask = 0x7F;

//------------------------------------------------------------------------
inline void AUWrapper::processOutputEvents (const AudioTimeStamp& inTimeStamp)
{
	if (midiOutCount > 0 && outputEvents.getEventCount () > 0)
	{
		int32 total = outputEvents.getEventCount ();
		Event e = {0};

		for (int32 i = 0; i < total; i++)
		{
			if (outputEvents.getEvent (i, e) != kResultOk)
				break;

			//--- SYSEX ----------------
			if (e.type == Event::kDataEvent && e.data.type == DataEvent::kMidiSysEx)
			{
			}
			else
			{
				switch (e.type)
				{
					case Event::kNoteOnEvent:
					{
						UInt8 status = (UInt8) (kNoteOn | (e.noteOn.channel & kChannelMask));
						UInt8 data1 = (UInt8) (e.noteOn.pitch & kDataMask);
						UInt8 data2 =
						    (UInt8) ((int32) (e.noteOn.velocity * 127.f + 0.4999999f) & kDataMask);
						UInt8 channel = e.noteOn.channel;
						if (data2 == 0) // zero velocity => note off
							status = (char)(kNoteOff | (e.noteOn.channel & kChannelMask));

						mCallbackHelper.AddEvent (status, channel, data1, data2, e.sampleOffset);
					}
					break;
					case Event::kNoteOffEvent:
					{
						UInt8 status = (UInt8) (kNoteOff | (e.noteOff.channel & kChannelMask));
						UInt8 data1 = e.noteOff.pitch;
						UInt8 data2 =
						    (UInt8) ((int32) (e.noteOff.velocity * 127.f + 0.4999999f) & kDataMask);
						UInt8 channel = e.noteOff.channel;

						mCallbackHelper.AddEvent (status, channel, data1, data2, e.sampleOffset);
					}
					break;
				}
			}
		}

		outputEvents.clear ();
		mCallbackHelper.FireAtTimeStamp (inTimeStamp);
	}
}

//--------------------------------------------------------------------------------------------
ComponentResult AUWrapper::GetPropertyInfo (AudioUnitPropertyID inID, AudioUnitScope inScope,
                                            AudioUnitElement inElement, UInt32& outDataSize,
                                            Boolean& outWritable)
{
	switch (inID)
	{
    //--------------------------
		case kAudioUnitProperty_BypassEffect:
		{
			if (inScope == kAudioUnitScope_Global && bypassParamID != -1)
			{
				outWritable = true;
				outDataSize = sizeof (UInt32);
				return noErr;
			}
			return kAudioUnitErr_InvalidProperty;
		}
    //--------------------------
		case kAudioUnitProperty_ParameterClumpName:
		{
			if (bypassParamID != -1)
			{
				outWritable = false;
				outDataSize = sizeof (AudioUnitParameterNameInfo);
				return noErr;
			}
			break;
		}
    //--------------------------
		case kAudioUnitProperty_ParameterStringFromValue:
		{
			if (inScope == kAudioUnitScope_Global)
			{
				outDataSize = sizeof (AudioUnitParameterStringFromValue);
				outWritable = false;
				return noErr;
			}
			return kAudioUnitErr_InvalidProperty;
		}
    //--------------------------
		case kAudioUnitProperty_ParameterValueFromString:
		{
			if (inScope == kAudioUnitScope_Global)
			{
				outDataSize = sizeof (AudioUnitParameterValueFromString);
				outWritable = false;
				return noErr;
			}
			return kAudioUnitErr_InvalidProperty;
		}
    //--------------------------
		case kAudioUnitProperty_ElementName:
		{
			if (inScope == kAudioUnitScope_Input || inScope == kAudioUnitScope_Output)
			{
				outDataSize = sizeof (CFStringRef);
				return noErr;
			}
			break;
		}
    //--------------------------
		case kAudioUnitProperty_CocoaUI:
		{
			outWritable = false;
			outDataSize = sizeof (AudioUnitCocoaViewInfo);
			return noErr;
		}
    //--------------------------
		case kAudioUnitProperty_MIDIOutputCallbackInfo:
		{
			if (inScope == kAudioUnitScope_Global && midiOutCount > 0)
			{
				outDataSize = sizeof (CFArrayRef);
				outWritable = false;
				return noErr;
			}
			break;
		}
    //--------------------------
		case kAudioUnitProperty_MIDIOutputCallback:
		{
			if (inScope == kAudioUnitScope_Global && midiOutCount > 0)
			{
				outDataSize = sizeof (AUMIDIOutputCallbackStruct);
				outWritable = true;
				return noErr;
			}
			break;
		}
    //--------------------------
		case 64000:
		{
			if (editController)
			{
				outDataSize = sizeof (TPtrInt);
				outWritable = false;
				return noErr;
			}
			return kAudioUnitErr_InvalidProperty;
		}
    //--------------------------
		case 64001:
		{
			if (VST3DynLibrary::gInstance)
			{
				outDataSize = sizeof (TPtrInt);
				outWritable = false;
				return noErr;
			}
			return kAudioUnitErr_InvalidProperty;
		}
	}
	return AUWRAPPER_BASE_CLASS::GetPropertyInfo (inID, inScope, inElement, outDataSize,
	                                              outWritable);
}

//--------------------------------------------------------------------------------------------
ComponentResult AUWrapper::SetProperty (AudioUnitPropertyID inID, AudioUnitScope inScope,
                                        AudioUnitElement inElement, const void* inData,
                                        UInt32 inDataSize)
{
	switch (inID)
	{
    //--------------------------
		case kAudioUnitProperty_BypassEffect:
		{
			if (inScope == kAudioUnitScope_Global && bypassParamID != -1)
			{
				bool tempNewSetting = *((UInt32*)inData) != 0;
				transferParamChanges.addChange (bypassParamID, tempNewSetting ? 1 : 0, 0);
				editController->setParamNormalized (bypassParamID, tempNewSetting ? 1 : 0);
				isBypassed = tempNewSetting;
				return noErr;
			}
			break;
		}
    //--------------------------
		case kAudioUnitProperty_MIDIOutputCallback:
		{
			if (inScope == kAudioUnitScope_Global && midiOutCount > 0)
			{
				if (inDataSize < sizeof (AUMIDIOutputCallbackStruct))
					return kAudioUnitErr_InvalidPropertyValue;

				AUMIDIOutputCallbackStruct* callbackStruct = (AUMIDIOutputCallbackStruct*)inData;
				mCallbackHelper.SetCallbackInfo (callbackStruct->midiOutputCallback,
				                                 callbackStruct->userData);
				return noErr;
			}
			break;
		}
	}
	return AUWRAPPER_BASE_CLASS::SetProperty (inID, inScope, inElement, inData, inDataSize);
}

//--------------------------------------------------------------------------------------------
bool AUWrapper::CanScheduleParameters () const
{
	return false;
}

//--------------------------------------------------------------------------------------------
ComponentResult AUWrapper::GetProperty (AudioUnitPropertyID inID, AudioUnitScope inScope,
                                        AudioUnitElement inElement, void* outData)
{
	switch (inID)
	{
    //--------------------------
		case kAudioUnitProperty_BypassEffect:
		{
			if (inScope == kAudioUnitScope_Global && bypassParamID != -1)
			{
				UInt32 bypass = editController->getParamNormalized (bypassParamID) >= 0.5 ? 1 : 0;
				*((UInt32*)outData) = bypass;
				return noErr;
			}
			break;
		}
    //--------------------------
		case kAudioUnitProperty_ParameterClumpName:
		{
			AudioUnitParameterNameInfo* parameterNameInfo = (AudioUnitParameterNameInfo*)outData;
			int32 clumpId = parameterNameInfo->inID;

			if (clumpId < 1 || clumpId > clumpGroups.size ())
				return kAudioUnitErr_PropertyNotInUse;

			const String& clumpGroup = clumpGroups.at (clumpId - 1);
			parameterNameInfo->outName = (CFStringRef)clumpGroup.toCFStringRef ();

			return noErr;
		}
    //--------------------------
		case kAudioUnitProperty_ParameterStringFromValue:
		{
			if (inScope == kAudioUnitScope_Global)
			{
				AudioUnitParameterStringFromValue* ps = (AudioUnitParameterStringFromValue*)outData;
				String128 paramString;
				if (editController->getParamStringByValue (
				        ps->inParamID,
				        ps->inValue ? *ps->inValue :
				                      editController->getParamNormalized (ps->inParamID),
				        paramString) == kResultTrue)
					ps->outString = createCFStringFromString128 (paramString);
				else
					ps->outString = CFStringCreateWithCString (0, "", kCFStringEncodingUTF8);
				return noErr;
			}
			return kAudioUnitErr_InvalidProperty;
		}
    //--------------------------
		case kAudioUnitProperty_ParameterValueFromString:
		{
			if (inScope == kAudioUnitScope_Global)
			{
				AudioUnitParameterValueFromString* ps = (AudioUnitParameterValueFromString*)outData;
				String128 paramString;
				createString128FromCFString (ps->inString, paramString);
				ParamValue value;
				if (editController->getParamValueByString (ps->inParamID, paramString, value) ==
				    kResultTrue)
				{
					ps->outValue = value;
					return noErr;
				}
				return -1;
			}
			return kAudioUnitErr_InvalidProperty;
		}
    //--------------------------
		case kAudioUnitProperty_ElementName:
		{
			if (inScope == kAudioUnitScope_Input || inScope == kAudioUnitScope_Output)
			{
				FUnknownPtr<IComponent> component (audioProcessor);
				BusInfo busInfo;
				if (component->getBusInfo (kAudio,
				                           inScope == kAudioUnitScope_Input ? kInput : kOutput,
				                           inElement, busInfo) == kResultTrue)
				{
					outData = (void*)createCFStringFromString128 (busInfo.name);
					return noErr;
				}
			}
			return kAudioUnitErr_InvalidProperty;
		}
    //--------------------------
		case kAudioUnitProperty_CocoaUI:
		{
			AutoreleasePool ap;

			CFStringRef className = (CFStringRef)[[NSString alloc]
			    initWithCString:SMTG_MAKE_STRING (SMTG_AU_PLUGIN_NAMESPACE (SMTGAUPluginCocoaView))
			           encoding:NSUTF8StringEncoding];
			const char* image = class_getImageName (objc_getClass (
			    SMTG_MAKE_STRING (SMTG_AU_PLUGIN_NAMESPACE (SMTGAUPluginCocoaView))));
			CFBundleRef bundle = GetBundleFromExecutable (image);

			CFURLRef url = CFBundleCopyBundleURL (bundle);
			CFRelease (bundle);
			AudioUnitCocoaViewInfo cocoaInfo = {url, {className}};
			*((AudioUnitCocoaViewInfo*)outData) = cocoaInfo;
			return noErr;
		}
    //--------------------------
		case kAudioUnitProperty_MIDIOutputCallbackInfo:
		{
			if (inScope == kAudioUnitScope_Global && midiOutCount > 0)
			{
				CFStringRef strs[1];
				strs[0] = CFSTR ("MIDI Callback");

				CFArrayRef callbackArray =
				    CFArrayCreate (NULL, (const void**)strs, 1, &kCFTypeArrayCallBacks);
				*(CFArrayRef*)outData = callbackArray;

				return noErr;
			}
			return kAudioUnitErr_InvalidProperty;
		}
    //--------------------------
		case 64000:
		{
			if (editController)
			{
				TPtrInt ptr = (TPtrInt)editController;
				*((TPtrInt*)outData) = ptr;
				return noErr;
			}
			else
				*((TPtrInt*)outData) = 0;
			return kAudioUnitErr_InvalidProperty;
		}
    //--------------------------
		case 64001:
		{
			if (VST3DynLibrary::gInstance)
			{
				TPtrInt ptr = (TPtrInt)VST3DynLibrary::gInstance;
				*((TPtrInt*)outData) = ptr;
				return noErr;
			}
			else
				*((TPtrInt*)outData) = 0;
			return kAudioUnitErr_InvalidProperty;
		}
	}
	return AUWRAPPER_BASE_CLASS::GetProperty (inID, inScope, inElement, outData);
}

//------------------------------------------------------------------------
int AUWrapper::GetNumCustomUIComponents ()
{
	if (editController)
	{
		static int numUIComponents = 0;
		static bool once = true;
		if (once)
		{
			AutoreleasePool ap;
			once = false;
			IPlugView* plugView = editController->createView (ViewType::kEditor);
			if (plugView)
			{

				if (plugView->isPlatformTypeSupported (kPlatformTypeHIView) == kResultTrue)
					numUIComponents = 1;
				plugView->release ();
			}
		}
		return numUIComponents;
	}
	return 0;
}

//------------------------------------------------------------------------
void AUWrapper::GetUIComponentDescs (ComponentDescription* inDescArray)
{
#if !__LP64__
	if (editController && GetNumCustomUIComponents () > 0)
	{
		static OSType subType = 0;
		static OSType manuf = 0;
		static bool once = true;
		if (once)
		{
			once = false;
			short prevResFile = CurResFile ();
			CFBundleRef bundle = GetCurrentBundle ();
			if (!bundle)
				return;

			short resourceFileID = CFBundleOpenBundleResourceMap (bundle);
			UseResFile (resourceFileID);
			Handle h = Get1Resource ('thng', 9000);
			if (h)
			{
				HLock (h);
				OSType* hPtr = (OSType*)*h;
				subType = hPtr[1];
				manuf = hPtr[2];
				HUnlock (h);
			}
			UseResFile (prevResFile);
			CFRelease (bundle);
		}
		inDescArray[0].componentType = kAudioUnitCarbonViewComponentType;
		inDescArray[0].componentSubType = subType;
		inDescArray[0].componentManufacturer = manuf;
		inDescArray[0].componentFlags = 0;
		inDescArray[0].componentFlagsMask = 0;
	}
#endif
}

//------------------------------------------------------------------------
Float64 AUWrapper::GetLatency ()
{
	Float64 latencyInSeconds = 0.0;
	if (audioProcessor)
		latencyInSeconds = audioProcessor->getLatencySamples () / getSampleRate ();
	return latencyInSeconds;
}

//------------------------------------------------------------------------
Float64 AUWrapper::GetTailTime ()
{
	Float64 tailTimeInSeconds = 0.0;
	if (audioProcessor)
		tailTimeInSeconds = audioProcessor->getTailSamples () / getSampleRate ();
	return tailTimeInSeconds;
}

//------------------------------------------------------------------------
// MusicDeviceBase
//------------------------------------------------------------------------
ComponentResult AUWrapper::StartNote (MusicDeviceInstrumentID inInstrument,
                                      MusicDeviceGroupID inGroupID,
                                      NoteInstanceID* outNoteInstanceID, UInt32 inOffsetSampleFrame,
                                      const MusicDeviceNoteParams& inParams)
{
	NoteInstanceID noteID = ((UInt8)inParams.mPitch) | ((noteCounter++) << 8);

	Event e = {0};

	e.type = Event::kNoteOnEvent;
	e.noteOn.pitch = inParams.mPitch;
	e.noteOn.velocity = inParams.mVelocity / 127.;
	e.noteOn.noteId = noteID;
	e.sampleOffset = inOffsetSampleFrame;

	// The Group ID is the channel with a standard midi device
	e.noteOn.channel = inGroupID;

	eventList.addEvent (e);
	if (outNoteInstanceID)
		*outNoteInstanceID = noteID;
	return noErr;
}

//------------------------------------------------------------------------
ComponentResult AUWrapper::StopNote (MusicDeviceGroupID inGroupID, NoteInstanceID inNoteInstanceID,
                                     UInt32 inOffsetSampleFrame)
{
	UInt32 pitch = inNoteInstanceID & 0xFF;
	Event e = {0};
	e.type = Event::kNoteOffEvent;
	e.noteOn.pitch = pitch;
	e.noteOn.velocity = 0;
	e.noteOn.noteId = inNoteInstanceID;
	e.sampleOffset = inOffsetSampleFrame;

	// The Group ID is the channel with a standard midi device
	e.noteOff.channel = inGroupID;

	eventList.addEvent (e);
	return noErr;
}

//--------------------------------------------------------------------------------------------
OSStatus AUWrapper::GetInstrumentCount (UInt32& outInstCount) const
{
	outInstCount = 1;
	return noErr;
}

//------------------------------------------------------------------------
// AUMIDIBase
//------------------------------------------------------------------------
OSStatus AUWrapper::HandleNonNoteEvent (UInt8 status, UInt8 channel, UInt8 data1, UInt8 data2,
                                        UInt32 inStartFrame)
{
	OSStatus result = noErr;

	if (status == kPolyPressure) // kMidiMessage_PolyPressure
	{
		Event e = {0};
		e.type = Event::kPolyPressureEvent;
		e.polyPressure.channel = channel;
		e.polyPressure.pitch = data1;
		e.polyPressure.pressure = data2 / 127.;
		e.sampleOffset = inStartFrame;

		eventList.addEvent (e);
		return result;
	}

	if (!midiMapping)
		return result;

	ParamID pid = 0;
	ParamValue value = 0;
	CtrlNumber cn = -1;
	bool prgChange = false;

	switch (status)
	{
		case kPitchBendStatus: // kMidiMessage_PitchWheel
		{
			cn = kPitchBend;
			unsigned short _14bit;
			_14bit = (unsigned short)data2;
			_14bit <<= 7;
			_14bit |= (unsigned short)data1;
			value = (double)_14bit / (double)0x3FFF;
			break;
		}
		case kAfterTouchStatus: // kMidiMessage_ChannelPressure
		{
			cn = kAfterTouch;
			value = data1 / 127.f;
			break;
		}
		case kController: // kMidiMessage_ControlChange
		{
			cn = data1;
			value = data2 / 127.f;
			break;
		}
		case kProgramChangeStatus: // kMidiMessage_ProgramChange
		{
			pid = programChangeParameters[channel];
			if (pid != kNoParamId)
			{
				ParameterInfo paramInfo = {0};
				if (editController->getParameterInfo (pid, paramInfo) == kResultTrue)
				{
					if (paramInfo.stepCount > 0 && data1 <= paramInfo.stepCount)
					{
						value = (ParamValue)data1 / (ParamValue)paramInfo.stepCount;
						prgChange = true;
					}
				}
			}
		}
	}
	if (prgChange || (cn >= 0 && (midiMapping->getMidiControllerAssignment (0, channel, cn, pid) ==
	                              kResultTrue)))
	{
		beginEdit (pid);
		performEdit (pid, value);
		endEdit (pid);

		// make sure that our edit controller get the changes
		int32 index;
		IParamValueQueue* vq = outputParamChanges.addParameterData (pid, index);
		if (vq)
			vq->addPoint (inStartFrame, value, index);
	}
	return result;
}

//------------------------------------------------------------------------
// IComponentHandler
//------------------------------------------------------------------------
tresult PLUGIN_API AUWrapper::beginEdit (ParamID tag)
{
	AudioUnitEvent auEvent;
	auEvent.mArgument.mParameter.mAudioUnit = GetComponentInstance ();
	auEvent.mArgument.mParameter.mParameterID = tag;
	auEvent.mArgument.mParameter.mScope = kAudioUnitScope_Global;
	auEvent.mArgument.mParameter.mElement = 0;
	auEvent.mEventType = kAudioUnitEvent_BeginParameterChangeGesture;
	AUEventListenerNotify (paramListenerRef, NULL, &auEvent);

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AUWrapper::performEdit (ParamID tag, ParamValue valueNormalized)
{
	AudioUnitParameter sPar = {GetComponentInstance (), tag, kAudioUnitScope_Global, 0};
	AUParameterSet (paramListenerRef, NULL, &sPar, valueNormalized, 0);

	AudioUnitEvent auEvent;
	auEvent.mArgument.mParameter.mAudioUnit = GetComponentInstance ();
	auEvent.mArgument.mParameter.mParameterID = tag;
	auEvent.mArgument.mParameter.mScope = kAudioUnitScope_Global;
	auEvent.mArgument.mParameter.mElement = 0;
	auEvent.mEventType = kAudioUnitEvent_ParameterValueChange;
	AUEventListenerNotify (paramListenerRef, NULL, &auEvent);

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AUWrapper::endEdit (ParamID tag)
{
	AudioUnitEvent auEvent;
	auEvent.mArgument.mParameter.mAudioUnit = GetComponentInstance ();
	auEvent.mArgument.mParameter.mParameterID = tag;
	auEvent.mArgument.mParameter.mScope = kAudioUnitScope_Global;
	auEvent.mArgument.mParameter.mElement = 0;
	auEvent.mEventType = kAudioUnitEvent_EndParameterChangeGesture;
	AUEventListenerNotify (paramListenerRef, NULL, &auEvent);

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AUWrapper::restartComponent (int32 flags)
{
	tresult result = kNotImplemented;

	if (flags & kParamValuesChanged)
	{
		syncParameterValues ();
		result = kResultTrue;
	}

	if (flags & kParamTitlesChanged)
	{
		cacheParameterValues ();
		result = kResultTrue;
		PropertyChanged (kAudioUnitProperty_ParameterList, kAudioUnitScope_Global, 0);
	}

	if (flags & kLatencyChanged)
	{
		AudioUnitEvent auEvent;
		auEvent.mArgument.mProperty.mAudioUnit = GetComponentInstance ();
		auEvent.mArgument.mProperty.mPropertyID = kAudioUnitProperty_Latency;
		auEvent.mArgument.mProperty.mScope = kAudioUnitScope_Global;
		auEvent.mArgument.mProperty.mElement = 0;
		auEvent.mEventType = kAudioUnitEvent_PropertyChange;
		AUEventListenerNotify (paramListenerRef, NULL, &auEvent);
		result = kResultTrue;
	}
	// TODO: finish restartComponent implementation
	return result;
}

//------------------------------------------------------------------------
// Helpers
//------------------------------------------------------------------------
void AUWrapper::syncParameterValues ()
{
	if (editController)
	{
		for (int32 i = 0; i < editController->getParameterCount (); i++)
		{
			ParameterInfo pi;
			editController->getParameterInfo (i, pi);
			if (pi.flags & ParameterInfo::kIsBypass)
			{
				bypassParamID = pi.id;
			}
			else
			{
				ParamValue value = editController->getParamNormalized (pi.id);
				GlobalScope ().GetElement (0)->SetParameter (pi.id, value);
			}
		}
	}
}

//------------------------------------------------------------------------
void AUWrapper::cacheParameterValues ()
{
	clearParameterValueCache ();

	if (editController)
	{
		FGuard guard (parameterCacheChanging);

		FUnknownPtr<IUnitInfo> unitInfoController (editController);
		if (unitInfoController)
		{
			if (unitInfos.empty ())
				buildUnitInfos (unitInfoController, unitInfos);
		}

		// This must be cached!
		for (int32 i = 0; i < editController->getParameterCount (); i++)
		{
			AudioUnitParameterInfo auInfo;
			ParameterInfo pi;
			editController->getParameterInfo (i, pi);

			int32 clumpIdx = 0; // Zero should be reserved for values that are not clumped

			// Build clump group if possible
			if (unitInfoController)
			{			
				String fullUnitName;
				ConstString separator (STR ("."));
				bool insertSeparator = false;

				UnitInfoMap::const_iterator it = unitInfos.find (pi.unitId);
				while (it != unitInfos.end () && it->second.id != kRootUnitId)
				{
					ConstString unitName (it->second.name);
					if (unitName.length () > 0)
					{
						if (insertSeparator)
							fullUnitName.insertAt (0, separator);
						insertSeparator = true;
						fullUnitName.insertAt (0, unitName);
					}
					it = unitInfos.find (it->second.parentUnitId);
				}
				if (!fullUnitName.isEmpty ())
				{
					bool insertClump = true;
					clumpIdx = 1;
					for (int32 i = 0; i < clumpGroups.size (); i++)
					{
						const String& str = clumpGroups.at (i);
						if (str.compare (fullUnitName) == 0)
						{
							insertClump = false;
							break;
						}

						++clumpIdx;
					}

					if (insertClump)
						clumpGroups.push_back (fullUnitName);
				}
			}

			auInfo.name[0] = 0;
			auInfo.cfNameString = createCFStringFromString128 (pi.title);
			auInfo.defaultValue = pi.defaultNormalizedValue;
			auInfo.minValue = 0;
			auInfo.maxValue = 1;
			auInfo.unit = kAudioUnitParameterUnit_CustomUnit;
			auInfo.unitName = createCFStringFromString128 (pi.units);

			// We release this ourselved when emptying the cache
			auInfo.flags =
			    /*kAudioUnitParameterFlag_CFNameRelease |*/ kAudioUnitParameterFlag_HasCFNameString |
			    kAudioUnitParameterFlag_IsReadable | kAudioUnitParameterFlag_HasName;

			if (clumpIdx > 0)
				AUBase::HasClump (auInfo, clumpIdx);

			if (!(pi.flags & ParameterInfo::kIsReadOnly))
				auInfo.flags |= kAudioUnitParameterFlag_IsWritable;
			if (!(pi.flags & ParameterInfo::kCanAutomate))
				auInfo.flags |= kAudioUnitParameterFlag_NonRealTime;
			if (pi.stepCount == 1)
				auInfo.unit = kAudioUnitParameterUnit_Boolean;

			cachedParameterInfos[pi.id] = auInfo;
		}
	}
}

//------------------------------------------------------------------------
void AUWrapper::clearParameterValueCache ()
{
	FGuard guard (parameterCacheChanging);

	for (CachedParameterInfoMap::const_iterator it = cachedParameterInfos.begin (),
	                                            end = cachedParameterInfos.end ();
	     it != end; ++it)
	{
		CFRelease (it->second.cfNameString);
		CFRelease (it->second.unitName);
	}
	cachedParameterInfos.clear ();
}

//------------------------------------------------------------------------
void AUWrapper::updateProcessContext ()
{
	memset (&processContext, 0, sizeof (ProcessContext));
	processContext.sampleRate = getSampleRate ();
	Float64 beat = 0;
	Float64 tempo = 0;
	if (CallHostBeatAndTempo (&beat, &tempo) == noErr)
	{
		processContext.state |=
		    ProcessContext::kTempoValid | ProcessContext::kProjectTimeMusicValid;
		processContext.tempo = tempo;
		processContext.projectTimeMusic = beat;
	}
	UInt32 deltaSampleOffsetToNextBeat = 0;
	Float32 timeSigNumerator = 0;
	UInt32 timeSigDenominator = 0;
	Float64 currentMeasureDownBeat = 0;
	if (CallHostMusicalTimeLocation (&deltaSampleOffsetToNextBeat, &timeSigNumerator,
	                                 &timeSigDenominator, &currentMeasureDownBeat) == noErr)
	{
		processContext.state |= ProcessContext::kTimeSigValid | ProcessContext::kBarPositionValid |
		                        ProcessContext::kClockValid;
		processContext.timeSigNumerator = timeSigNumerator;
		processContext.timeSigDenominator = timeSigDenominator;
		processContext.samplesToNextClock = deltaSampleOffsetToNextBeat;
		processContext.barPositionMusic = currentMeasureDownBeat;
	}
	Boolean isPlaying;
	Boolean transportStateChanged;
	Float64 currentSampleInTimeLine;
	Boolean isCycling;
	Float64 cycleStartBeat;
	Float64 cycleEndBeat;
	if (CallHostTransportState (&isPlaying, &transportStateChanged, &currentSampleInTimeLine,
	                            &isCycling, &cycleStartBeat, &cycleEndBeat) == noErr)
	{
		processContext.state |= ProcessContext::kCycleValid;
		processContext.cycleStartMusic = cycleStartBeat;
		processContext.cycleEndMusic = cycleEndBeat;
		processContext.projectTimeSamples = currentSampleInTimeLine;
		if (isPlaying)
			processContext.state |= ProcessContext::kPlaying;
		if (isCycling)
			processContext.state |= ProcessContext::kCycleActive;
	}
}

//------------------------------------------------------------------------
void AUWrapper::onTimer (Timer* timer)
{
	ParamID pid;
	ParamValue value;
	int32 sampleOffset;
	while (outputParamTransfer.getNextChange (pid, value, sampleOffset))
	{
		GlobalScope ().GetElement (0)->SetParameter (pid, value);
		AudioUnitParameter auParam = {0};
		auParam.mAudioUnit = GetComponentInstance ();
		auParam.mElement = 0;
		auParam.mScope = kAudioUnitScope_Global;
		auParam.mParameterID = pid;
		AUParameterListenerNotify (paramListenerRef, 0, &auParam);
		editController->setParamNormalized (pid, value);
	}
	if (isBypassed)
	{
		ProcessData bypassData = processData;
		bypassData.numSamples = 0;
		processParamChanges.clearQueue ();
		transferParamChanges.transferChangesTo (processParamChanges);
		if (processParamChanges.getParameterCount () > 0)
		{
			audioProcessor->process (bypassData);
			outputParamTransfer.transferChangesFrom (outputParamChanges);
			outputParamChanges.clearQueue ();
		}
	}
}

//------------------------------------------------------------------------
bool AUWrapper::validateChannelPair (int inChannelsIn, int inChannelsOut, const AUChannelInfo* info,
                                     UInt32 numChanInfo) const
{
	// we've the following cases (some combinations) to test here:
	/*
	>0		An explicit number of channels on either side
	0		that side (generally input!) has no elements
	-1		wild card:
	-1,-1	any num channels as long as same channels on in and out
	-1,-2	any num channels channels on in and out - special meaning
	-2+ 	indicates total num channs AU can handle
	- elements configurable to any num channels,
	- element count in scope must be writable
	*/

	// now chan layout can contain -1 for either scope (ie. doesn't care)
	for (unsigned int i = 0; i < numChanInfo; ++i)
	{
		// less than zero on both sides - check for special attributes
		if ((info[i].inChannels < 0) && (info[i].outChannels < 0))
		{
			// these are our wild card matches
			if (info[i].inChannels == -1 && info[i].outChannels == -1)
			{
				if (inChannelsIn && inChannelsOut)
				{
					if (inChannelsOut == inChannelsIn)
						return true;
				}
				else
					return true; // if one of these is zero, then a -1 means any
			}
			else if ((info[i].inChannels == -1 && info[i].outChannels == -2) ||
			         (info[i].inChannels == -2 && info[i].outChannels == -1))
			{
				return true;
			}
			// these are our total num channels matches
			// element count MUST be writable
			else
			{
				bool outWrite = false;
				bool inWrite = false;
				// IsElementCountWritable (kAudioUnitScope_Output, outWrite);
				// IsElementCountWritable (kAudioUnitScope_Input, inWrite);
				if (inWrite && outWrite)
				{
					if ((inChannelsOut <= abs (info[i].outChannels)) &&
					    (inChannelsIn <= abs (info[i].inChannels)))
					{
						return true;
					}
				}
			}
		}

		// special meaning on input, specific num on output
		else if (info[i].inChannels < 0)
		{
			if (info[i].outChannels == inChannelsOut)
			{
				// can do any in channels
				if (info[i].inChannels == -1)
				{
					return true;
				}
				// total chans on input
				else
				{
					bool inWrite = false;
					// IsElementCountWritable (kAudioUnitScope_Input, inWrite);
					if (inWrite && (inChannelsIn <= abs (info[i].inChannels)))
					{
						return true;
					}
				}
			}
		}

		// special meaning on output, specific num on input
		else if (info[i].outChannels < 0)
		{
			if (info[i].inChannels == inChannelsIn)
			{
				// can do any out channels
				if (info[i].outChannels == -1)
				{
					return true;
				}
				// total chans on output
				else
				{
					bool outWrite = false;
					// IsElementCountWritable (kAudioUnitScope_Output, outWrite);
					if (outWrite && (inChannelsOut <= abs (info[i].outChannels)))
					{
						return true;
					}
				}
			}
		}

		// both chans in struct >= 0 - thus has to explicitly match
		else if ((info[i].inChannels == inChannelsIn) && (info[i].outChannels == inChannelsOut))
		{
			return true;
		}

		// now check to see if a wild card on the args (inChannelsIn or inChannelsOut chans is zero)
		// is found
		// tells us to match just one side of the scopes
		else if (inChannelsIn == 0)
		{
			if (info[i].outChannels == inChannelsOut)
			{
				return true;
			}
		}
		else if (inChannelsOut == 0)
		{
			if (info[i].inChannels == inChannelsIn)
			{
				return true;
			}
		}
	}

	return false;
}

//------------------------------------------------------------------------
bool AUWrapper::getProgramListAndUnit (int32 midiChannel, UnitID& unitId,
                                       ProgramListID& programListId)
{
	programListId = kNoProgramListId;
	unitId = -1;

	IUnitInfo* unitInfo = NULL;

	if (editController &&
	    editController->queryInterface (IUnitInfo::iid, (void**)&unitInfo) == kResultTrue &&
	    unitInfo)
	{
		if (unitInfo->getUnitByBus (kEvent, kInput, 0, midiChannel, unitId) == kResultTrue)
		{
			int32 unitCount = unitInfo->getUnitCount ();
			for (int32 i = 0; i < unitCount; i++)
			{
				UnitInfo unitInfoStruct = {0};
				if (unitInfo->getUnitInfo (i, unitInfoStruct) == kResultTrue)
				{
					if (unitId == unitInfoStruct.id)
					{
						programListId = unitInfoStruct.programListId;
						unitInfo->release ();
						return programListId != kNoProgramListId;
					}
				}
			}
		}

		unitInfo->release ();
	}
	return false;
}

#if !CA_USE_AUDIO_PLUGIN_ONLY

// copied from AUDispatch.cpp
#if __LP64__
// comp instance, parameters in forward order
#define PARAM(_typ, _name, _index, _nparams) _typ _name = *(_typ*)&params->params[_index + 1];
#else
// parameters in reverse order, then comp instance
#define PARAM(_typ, _name, _index, _nparams) \
	_typ _name = *(_typ*)&params->params[_nparams - 1 - _index];
#endif

//------------------------------------------------------------------------
ComponentResult AUWrapper::ComponentEntryDispatch (ComponentParameters* params, AUWrapper* This)
{
	OSStatus result = noErr;

	switch (params->what)
	{
		case kAudioUnitSetPropertySelect:
		{
			PARAM (AudioUnitPropertyID, inID, 0, 5);

			if (inID == kAudioUnitProperty_ClassInfoFromDocument)
			{
				PARAM (AudioUnitScope, inScope, 1, 5);
				PARAM (AudioUnitElement, inElement, 2, 5);
				PARAM (const void*, inData, 3, 5);
				PARAM (UInt32, inDataSize, 4, 5);

				ca_require (inDataSize == sizeof (CFPropertyListRef*), InvalidPropertyValue);
				ca_require (inScope == kAudioUnitScope_Global, InvalidScope);
				result = This->restoreState (*(CFPropertyListRef*)inData, true);
				return result;
			}
			break;
		}
	}
	return AUWRAPPER_BASE_CLASS::ComponentEntryDispatch (params, This);

InvalidScope:
	return kAudioUnitErr_InvalidScope;
InvalidPropertyValue:
	return kAudioUnitErr_InvalidPropertyValue;
}
#endif

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
class CleanupMemory
{
public:
	CleanupMemory () {}
	~CleanupMemory ()
	{
		if (channelInfos)
		{
			delete[] channelInfos;
			channelInfos = 0;
		}
	}
};

CleanupMemory _gCleanupMemory;
CleanupMemory* gCleanupMemory = &_gCleanupMemory;

#if __MAC_OS_X_VERSION_MAX_ALLOWED == 1060 // if compiling against the 10.6 sdk
//------------------------------------------------------------------------
// COMPONENT_ENTRY(AUWrapper)
//------------------------------------------------------------------------
extern "C" {
ComponentResult AUWrapperEntry (ComponentParameters* params, AUWrapper* obj);
__attribute__ ((visibility ("default"))) ComponentResult AUWrapperEntry (
    ComponentParameters* params, AUWrapper* obj)
{
	return ComponentEntryPoint<AUWrapper>::Dispatch (params, obj);
}
}

#else
struct AUWrapperLookup
{
	static AudioComponentMethod Lookup (SInt16 selector)
	{
		// ProcessBufferLists not supported, so don't try any of the process methods
		if (selector == kAudioUnitProcessSelect || selector == kAudioUnitProcessMultipleSelect)
			return NULL;
		AudioComponentMethod method = AUBaseProcessLookup::Lookup (selector);
		if (method)
			return method;
		method = AUMusicLookup::Lookup (selector);
		if (method)
			return method;
		method = AUBaseProcessMultipleLookup::Lookup (selector);
		if (method)
			return method;
		return NULL;
	}
};
template <class Implementor>
class AUFactory : public APFactory<AUWrapperLookup, Implementor>
{
};

//------------------------------------------------------------------------
// new entry method
extern "C" {
void* AUWrapperFactory (const AudioComponentDescription* inDesc);
__attribute__ ((visibility ("default"))) void* AUWrapperFactory (
    const AudioComponentDescription* inDesc)
{
	return AUFactory<AUWrapper>::Factory (inDesc);
}
} // extern "C"

#if !CA_USE_AUDIO_PLUGIN_ONLY
//------------------------------------------------------------------------
// old entry method
extern "C" {
ComponentResult AUWrapperEntry (ComponentParameters* params, AUWrapper* obj);
__attribute__ ((visibility ("default"))) ComponentResult AUWrapperEntry (
    ComponentParameters* params, AUWrapper* obj)
{
	return ComponentEntryPoint<AUWrapper>::Dispatch (params, obj);
}
}
#endif
#endif

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg

/// \endcond
