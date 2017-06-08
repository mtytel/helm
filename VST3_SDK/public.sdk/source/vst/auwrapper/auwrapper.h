//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/auwrapper/auwrapper.h
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

/**
***************************
\page auwrapper AudioUnit Wrapper
***************************
\section AUIntroduction Introduction
***************************
The VST 3 SDK comes with an AudioUnit wrapper, which can wrap one VST 3 Audio Processor and Edit Controller as an AudioUnit effect/instrument.

The wrapper is a small dynamic library which loads the VST 3 Plug-in.
As AudioUnits store some important information in their resource fork, this library must be compiled for every VST 3 Plug-in.
\n\n
***************************
\section AUhowdoesitwork How does it work ?
***************************

- build the auwrapper project (public.sdk/source/vst/auwrapper/auwrapper.xcodeproj)
- create a copy of the again AU wrapper example project directory (public.sdk/source/vst/auwrapper/again/)
- rename the copy to your needs
- edit the target settings of the project and change
	- Product Name
	- Library search path so that it points to the directory where libauwrapper.a exists
	- architecture setting so that it only includes architectures the VST 3 Plug-in supports

- search in the project for AUWRAPPER_CHANGE and change the settings to your needs, especially in :
	- edit audiounitconfig.h see comments there
	- edit Info.plist see comments there
- edit the "Make Links Script" for easier debugging/development
- build your project
- done... that is all!

For the release version, you must place a copy or an alias of your VST 3 Plug-in in the resource folder of the bundle named "plugin.vst3"

 */
/// \cond ignore

#pragma once

#if CA_USE_AUDIO_PLUGIN_ONLY
#include "AudioUnits/AUPublic/AUBase/AUBase.h"
#define AUWRAPPER_BASE_CLASS AUBase
#else
#include "AudioUnits/AUPublic/OtherBases/MusicDeviceBase.h"
#define AUWRAPPER_BASE_CLASS MusicDeviceBase
#endif
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "pluginterfaces/vst/ivstprocesscontext.h"
#include "pluginterfaces/vst/ivstunits.h"
#include "public.sdk/source/vst/hosting/parameterchanges.h"
#include "public.sdk/source/vst/hosting/processdata.h"
#include "public.sdk/source/vst/hosting/eventlist.h"
#include "base/source/timer.h"
#include "base/source/fstring.h"
#include "base/source/flock.h"
#include <Cocoa/Cocoa.h>
#include <AudioToolbox/AudioToolbox.h>
#include <vector>
#include <map>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
typedef struct MIDIMessageInfoStruct {
	UInt8	status;
	UInt8	channel;
	UInt8	data1;
	UInt8	data2;
	UInt32	startFrame;
} MIDIMessageInfoStruct;

//------------------------------------------------------------------------
class MIDIOutputCallbackHelper 
{
public:
	MIDIOutputCallbackHelper ()
	{
		mMIDIMessageList.reserve (16);
		mMIDICallbackStruct.midiOutputCallback = NULL;
	}
	virtual	~MIDIOutputCallbackHelper () {};

	void SetCallbackInfo (AUMIDIOutputCallback callback, void* userData)
	{
		mMIDICallbackStruct.midiOutputCallback = callback;
		mMIDICallbackStruct.userData = userData;
	}

	void AddEvent (UInt8 status, UInt8 channel, UInt8 data1, UInt8 data2, UInt32 inStartFrame)
	{
		MIDIMessageInfoStruct info = {status, channel, data1, data2, inStartFrame};	
		mMIDIMessageList.push_back (info);
	}

	void FireAtTimeStamp (const AudioTimeStamp& inTimeStamp)
	{
		if (!mMIDIMessageList.empty () && mMIDICallbackStruct.midiOutputCallback != 0)
		{
			// synthesize the packet list and call the MIDIOutputCallback
			// iterate through the vector and get each item
			std::vector<MIDIMessageInfoStruct>::iterator myIterator;
			MIDIPacketList* pktlist = PacketList ();

			for (myIterator = mMIDIMessageList.begin (); myIterator != mMIDIMessageList.end (); myIterator++)
			{
				MIDIMessageInfoStruct item = *myIterator;
			
				MIDIPacket* pkt = MIDIPacketListInit (pktlist);
				bool tooBig = false;
				Byte data[3] = { item.status, item.data1, item.data2 };
				if ((pkt = MIDIPacketListAdd (pktlist, sizeof (mBuffersAllocated), pkt, item.startFrame, 4, const_cast<Byte*>(data))) == NULL)
					tooBig = true;

				if (tooBig)
				{	// send what we have and then clear the buffer and send again
					// issue the callback with what we got
					OSStatus result = mMIDICallbackStruct.midiOutputCallback (mMIDICallbackStruct.userData, &inTimeStamp, 0, pktlist);
					if (result != noErr)
						printf ("error calling output callback: %d", (int) result);

					// clear stuff we've already processed, and fire again
					mMIDIMessageList.erase (mMIDIMessageList.begin (), myIterator);
					this->FireAtTimeStamp (inTimeStamp);
					return;
				}
			}
			// fire callback
			OSStatus result = mMIDICallbackStruct.midiOutputCallback (mMIDICallbackStruct.userData, &inTimeStamp, 0, pktlist);
			if (result != noErr)
				printf ("error calling output callback: %d", (int) result);

			mMIDIMessageList.clear ();
		}
	}

protected:
	typedef std::vector<MIDIMessageInfoStruct> MIDIMessageList;

private:
	MIDIPacketList* PacketList () {return (MIDIPacketList*)mBuffersAllocated;}

	Byte mBuffersAllocated[1024];
	AUMIDIOutputCallbackStruct mMIDICallbackStruct;
	MIDIMessageList	mMIDIMessageList;
};


//------------------------------------------------------------------------
//------------------------------------------------------------------------
class AUWrapper
: public AUWRAPPER_BASE_CLASS
, public IComponentHandler
, public ITimerCallback
{
public:
	AUWrapper (ComponentInstanceRecord* ci);
	~AUWrapper ();

	//---ComponentBase---------------------
	ComponentResult	Version () SMTG_OVERRIDE;
	void PostConstructor () SMTG_OVERRIDE;

	//---AUBase-----------------------------
	void Cleanup () SMTG_OVERRIDE;
	ComponentResult Initialize () SMTG_OVERRIDE;
	AUElement* CreateElement (AudioUnitScope scope, AudioUnitElement element) SMTG_OVERRIDE;
	UInt32 SupportedNumChannels (const AUChannelInfo** outInfo) SMTG_OVERRIDE;
	bool StreamFormatWritable (AudioUnitScope scope, AudioUnitElement element) SMTG_OVERRIDE;
	ComponentResult ChangeStreamFormat (AudioUnitScope inScope, AudioUnitElement inElement, const CAStreamBasicDescription& inPrevFormat, const CAStreamBasicDescription& inNewFormat) SMTG_OVERRIDE;
	ComponentResult SetConnection (const AudioUnitConnection& inConnection) SMTG_OVERRIDE;
	ComponentResult GetParameterInfo (AudioUnitScope inScope, AudioUnitParameterID inParameterID, AudioUnitParameterInfo& outParameterInfo) SMTG_OVERRIDE;
	ComponentResult SetParameter (AudioUnitParameterID inID, AudioUnitScope inScope, AudioUnitElement inElement, AudioUnitParameterValue inValue, UInt32 inBufferOffsetInFrames) SMTG_OVERRIDE;

	ComponentResult SaveState (CFPropertyListRef* outData) SMTG_OVERRIDE;
	ComponentResult RestoreState (CFPropertyListRef inData) SMTG_OVERRIDE;

	ComponentResult Render (AudioUnitRenderActionFlags &ioActionFlags, const AudioTimeStamp &inTimeStamp, UInt32 inNumberFrames) SMTG_OVERRIDE;
	void processOutputEvents (const AudioTimeStamp &inTimeStamp);

	int GetNumCustomUIComponents () SMTG_OVERRIDE;
	void GetUIComponentDescs (ComponentDescription* inDescArray) SMTG_OVERRIDE;

	ComponentResult GetPropertyInfo (AudioUnitPropertyID inID, AudioUnitScope inScope, AudioUnitElement inElement, UInt32 &outDataSize, Boolean &outWritable) SMTG_OVERRIDE;
	ComponentResult GetProperty (AudioUnitPropertyID inID, AudioUnitScope inScope, AudioUnitElement inElement, void* outData) SMTG_OVERRIDE;
	ComponentResult SetProperty (AudioUnitPropertyID inID, AudioUnitScope inScope, AudioUnitElement inElement, const void* inData, UInt32 inDataSize) SMTG_OVERRIDE;
	bool CanScheduleParameters() const; // Not in the base class anymore in newer CoreAudio SDKs

	Float64 GetLatency () SMTG_OVERRIDE;
	Float64 GetTailTime () SMTG_OVERRIDE;

	//---Factory presets
	OSStatus GetPresets (CFArrayRef* outData) const SMTG_OVERRIDE;
	OSStatus NewFactoryPresetSet (const AUPreset& inNewFactoryPreset) SMTG_OVERRIDE;

	//---MusicDeviceBase-------------------------
	ComponentResult StartNote (MusicDeviceInstrumentID inInstrument, MusicDeviceGroupID inGroupID, NoteInstanceID* outNoteInstanceID, UInt32 inOffsetSampleFrame, const MusicDeviceNoteParams &inParams) SMTG_OVERRIDE;
	ComponentResult StopNote (MusicDeviceGroupID inGroupID, NoteInstanceID inNoteInstanceID, UInt32 inOffsetSampleFrame) SMTG_OVERRIDE;
	OSStatus GetInstrumentCount (UInt32 &outInstCount) const SMTG_OVERRIDE;

	//---AUMIDIBase------------------------------
	OSStatus HandleNonNoteEvent (UInt8 status, UInt8 channel, UInt8	data1, UInt8 data2, UInt32 inStartFrame) SMTG_OVERRIDE;
	
	//---custom----------------------------------
	void setControllerParameter (ParamID pid, ParamValue value);

	// return for a given midiChannel the unitID and the ProgramListID
	bool getProgramListAndUnit (int32 midiChannel, UnitID& unitId, ProgramListID& programListId);

	// restore preset state, add StateType "Project" to stream if loading from project
	ComponentResult restoreState (CFPropertyListRef inData, bool fromProject);

	//------------------------------------------------------------------------
#if !CA_USE_AUDIO_PLUGIN_ONLY
	static ComponentResult ComponentEntryDispatch (ComponentParameters* params, AUWrapper* This);
#endif
	//------------------------------------------------------------------------
	static CFBundleRef gBundleRef;
	//------------------------------------------------------------------------
	DECLARE_FUNKNOWN_METHODS

protected:
	//---from IComponentHandler-------------------
	tresult PLUGIN_API beginEdit (ParamID tag) SMTG_OVERRIDE;
	tresult PLUGIN_API performEdit (ParamID tag, ParamValue valueNormalized) SMTG_OVERRIDE;
	tresult PLUGIN_API endEdit (ParamID tag) SMTG_OVERRIDE;
	tresult PLUGIN_API restartComponent (int32 flags) SMTG_OVERRIDE;

	//---from ITimerCallback----------------------
	void onTimer (Timer* timer) SMTG_OVERRIDE;

	// internal helpers
	double getSampleRate () const { return sampleRate; }
	void updateProcessContext ();
	void syncParameterValues ();
	void cacheParameterValues ();
	void clearParameterValueCache ();
	
	virtual IPluginFactory* getFactory ();
	void loadVST3Module ();
	void unloadVST3Module ();
	bool validateChannelPair (int inChannelsIn, int inChannelsOut, const AUChannelInfo* info, UInt32 numChanInfo) const;


	IAudioProcessor* audioProcessor;
	IEditController* editController;
	IMidiMapping* midiMapping;

	Timer* timer;

	HostProcessData processData;
	ParameterChanges processParamChanges;
	ParameterChanges outputParamChanges;
	ParameterChangeTransfer transferParamChanges;
	ParameterChangeTransfer outputParamTransfer;
	ProcessContext processContext;
	EventList eventList;

	typedef std::map<uint32, AudioUnitParameterInfo> CachedParameterInfoMap;
	typedef std::map<UnitID, UnitInfo> UnitInfoMap;
	typedef std::vector<String> ClumpGroupVector;

	UnitInfoMap unitInfos;
	ClumpGroupVector clumpGroups;
	CachedParameterInfoMap cachedParameterInfos;
	FLock parameterCacheChanging;

	NoteInstanceID noteCounter;
	double sampleRate;
	ParamID bypassParamID;

	AUPreset* presets;
	int32 numPresets;
	ParamID factoryProgramChangedID;

	bool isInstrument;
	bool isBypassed;

	AUParameterListenerRef paramListenerRef;
	static const int32 kMaxProgramChangeParameters = 16;
	ParamID programChangeParameters[kMaxProgramChangeParameters]; // for each midi channel

	int32 midiOutCount; // currently only 0 or 1 supported
	MIDIOutputCallbackHelper mCallbackHelper;
	EventList outputEvents;
private:
	void buildUnitInfos (IUnitInfo* unitInfoController, UnitInfoMap& units) const;
};

//------------------------------------------------------------------------
class AutoreleasePool
{
public:
	AutoreleasePool () { ap = [[NSAutoreleasePool alloc] init]; }
	~AutoreleasePool () { [ap drain]; }
//------------------------------------------------------------------------
protected:
	NSAutoreleasePool* ap;
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg

/// \endcond
