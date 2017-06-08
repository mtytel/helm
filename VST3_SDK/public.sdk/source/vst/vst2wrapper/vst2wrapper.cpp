//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/vst2wrapper/vst2wrapper.cpp
// Created by  : Steinberg, 01/2009
// Description : VST 3 -> VST 2 Wrapper
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

#include "public.sdk/source/vst/vst2wrapper/vst2wrapper.h"

#include "public.sdk/source/vst/hosting/hostclasses.h"
#include "public.sdk/source/vst2.x/aeffeditor.h"

#include "pluginterfaces/gui/iplugview.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/vst/ivstmessage.h"
#include "pluginterfaces/vst/vstpresetkeys.h"
#include "pluginterfaces/base/futils.h"
#include "pluginterfaces/base/keycodes.h"

#include "base/source/fstreamer.h"

#include <cstdlib>
#include <cstdio>
#include <limits>

extern bool DeinitModule (); //! Called in Vst2Wrapper destructor

//------------------------------------------------------------------------
// some Defines
//------------------------------------------------------------------------
// should be kVstMaxParamStrLen if we want to respect the VST 2 specification!!!
#define kVstExtMaxParamStrLen 32

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//! The parameter's name contains the unit path (e.g. "Modulators.LFO 1.frequency")
bool vst2WrapperFullParameterPath = true;

//------------------------------------------------------------------------
// some Globals
//------------------------------------------------------------------------
// In order to speed up hasEditor function gPluginHasEditor can be set in EditController::initialize
enum
{
	kDontKnow = -1,
	kNoEditor = 0,
	kEditor,
};

// default: kDontKnow which uses createView to find out
typedef int32 EditorAvailability;
EditorAvailability gPluginHasEditor = kDontKnow;

// Set to 'true' in EditController::initialize
// default: VST 3 kIsProgramChange parameter will not be exported in VST 2
bool gExportProgramChangeParameters = false; 

//------------------------------------------------------------------------
// Vst2EditorWrapper Declaration
//------------------------------------------------------------------------
class Vst2EditorWrapper : public AEffEditor, public IPlugFrame
{
public:
//------------------------------------------------------------------------
	Vst2EditorWrapper (AudioEffect* effect, IEditController* controller);
	~Vst2EditorWrapper ();

	static bool hasEditor (IEditController* controller);

	virtual bool getRect (ERect** rect);
	virtual bool open (void* ptr);
	virtual void close ();
	virtual bool setKnobMode (VstInt32 val);

	///< Receives key down event. Return true only if key was really used!
	virtual bool onKeyDown (VstKeyCode& keyCode);
	///< Receives key up event. Return true only if key was really used!
	virtual bool onKeyUp (VstKeyCode& keyCode);
	///< Handles mouse wheel event, distance is positive or negative to indicate wheel direction.
	virtual bool onWheel (float distance);

	// IPlugFrame
	virtual tresult PLUGIN_API resizeView (IPlugView* view, ViewRect* newSize);
	virtual tresult PLUGIN_API queryInterface (const char* _iid, void** obj);
	virtual uint32 PLUGIN_API addRef () { return 1; }
	virtual uint32 PLUGIN_API release () { return 1; }
//------------------------------------------------------------------------
protected:
	void createView ();

	IEditController* mController;
	IPlugView* mView;

	ERect mERect;
};

//------------------------------------------------------------------------
// Vst2EditorWrapper Implementation
//------------------------------------------------------------------------
Vst2EditorWrapper::Vst2EditorWrapper (AudioEffect* effect, IEditController* controller)
: AEffEditor (effect), mController (controller), mView (0)
{
	if (mController)
		mController->addRef ();
}

//------------------------------------------------------------------------
Vst2EditorWrapper::~Vst2EditorWrapper ()
{
	if (mView)
		close ();

	if (mController)
		mController->release ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API Vst2EditorWrapper::queryInterface (const char* _iid, void** obj)
{
	QUERY_INTERFACE (_iid, obj, FUnknown::iid, FUnknown)
	QUERY_INTERFACE (_iid, obj, IPlugFrame::iid, IPlugFrame)

	*obj = 0;
	return kNoInterface;
}

//------------------------------------------------------------------------
tresult PLUGIN_API Vst2EditorWrapper::resizeView (IPlugView* view, ViewRect* newSize)
{
	tresult result = kResultFalse;
	if (view && newSize)
	{
		if (effect)
		{
			AudioEffectX* effectx = dynamic_cast<AudioEffectX*> (effect);
			if (effectx && effectx->sizeWindow (newSize->getWidth (), newSize->getHeight ()))
			{
#if MAC
				ViewRect nSize (0, 0, newSize->right - newSize->left, newSize->bottom - newSize->top);
				result = view->onSize (&nSize);
#else
				result = view->onSize (newSize);
#endif
			}
		}
	}

	return result;
}

//------------------------------------------------------------------------
bool Vst2EditorWrapper::hasEditor (IEditController* controller)
{
	/* Some Plug-ins might have large GUIs. In order to speed up hasEditor function while
	 * initializing the Plug-in gPluginHasEditor can be set in EditController::initialize
	 * beforehand. */
	bool result = false;
	if (gPluginHasEditor == kEditor)
	{
		result = true;
	}
	else if (gPluginHasEditor == kNoEditor)
	{
		result = false;
	}
	else
	{
		IPlugView* view = controller ? controller->createView (ViewType::kEditor) : 0;
		FReleaser viewRel (view);
		result = (view != 0);
	}

	return result;
}

//------------------------------------------------------------------------
void Vst2EditorWrapper::createView ()
{
	if (mView == 0 && mController != 0)
	{
		mView = mController->createView (ViewType::kEditor);
		mView->setFrame (this);

#if MAC
#if PLATFORM_64
		if (mView && mView->isPlatformTypeSupported (kPlatformTypeNSView) != kResultTrue)
#else
		if (mView && mView->isPlatformTypeSupported (kPlatformTypeHIView) != kResultTrue)
#endif
		{
			mView->release ();
			mView = nullptr;
			mController->release (); // do not try again
			mController = nullptr;
		}
#endif
	}
}

//------------------------------------------------------------------------
bool Vst2EditorWrapper::getRect (ERect** rect)
{
	createView ();
	if (mView)
	{
		ViewRect size;
		if (mView->getSize (&size) == kResultTrue)
		{
			mERect.left = (VstInt16)size.left;
			mERect.top = (VstInt16)size.top;
			mERect.right = (VstInt16)size.right;
			mERect.bottom = (VstInt16)size.bottom;

			if ((mERect.bottom - mERect.top) > 0 && (mERect.right - mERect.left) > 0)
			{
				*rect = &mERect;
				return true;
			}
		}
	}

	*rect = 0;
	return false;
}

//------------------------------------------------------------------------
bool Vst2EditorWrapper::open (void* ptr)
{
	AEffEditor::open (ptr);
	createView ();

	if (mView)
	{
#if WINDOWS
		FIDString type = kPlatformTypeHWND;
#elif MAC && PLATFORM_64
		FIDString type = kPlatformTypeNSView;
#elif MAC
		FIDString type = kPlatformTypeHIView;
#endif
		return mView->attached (ptr, type) == kResultTrue;
	}
	return false;
}

//------------------------------------------------------------------------
void Vst2EditorWrapper::close ()
{
	if (mView)
	{
		mView->setFrame (0);
		mView->removed ();
		mView->release ();
		mView = nullptr;
	}

	AEffEditor::close ();
}

//------------------------------------------------------------------------
bool Vst2EditorWrapper::setKnobMode (VstInt32 val)
{
	bool result = false;
	IEditController2* editController2;
	if (mController &&
	    mController->queryInterface (IEditController2::iid, (void**)&editController2) ==
	        kResultTrue)
	{
		switch (val)
		{
			// Circular
			case 0:
				result = editController2->setKnobMode (Vst::kCircularMode) == kResultTrue;
				break;

			// Relative Circular
			case 1:
				result = editController2->setKnobMode (kRelativCircularMode) == kResultTrue;
				break;

			// Linear
			case 2: result = editController2->setKnobMode (kLinearMode) == kResultTrue; break;
		}
		editController2->release ();
	}
	return result;
}

//------------------------------------------------------------------------
bool Vst2EditorWrapper::onKeyDown (VstKeyCode& keyCode)
{
	if (mView)
		return mView->onKeyDown (VirtualKeyCodeToChar (keyCode.virt), keyCode.virt,
		                         keyCode.modifier) == kResultTrue;
	return false;
}

//------------------------------------------------------------------------
bool Vst2EditorWrapper::onKeyUp (VstKeyCode& keyCode)
{
	if (mView)
		return mView->onKeyUp (VirtualKeyCodeToChar (keyCode.virt), keyCode.virt,
		                       keyCode.modifier) == kResultTrue;
	return false;
}

//------------------------------------------------------------------------
bool Vst2EditorWrapper::onWheel (float distance)
{
	if (mView)
		return mView->onWheel (distance) == kResultTrue;
	return false;
}

//------------------------------------------------------------------------
// Vst2MidiEventQueue Declaration
//------------------------------------------------------------------------
class Vst2MidiEventQueue
{
public:
//------------------------------------------------------------------------
	Vst2MidiEventQueue (int32 maxEventCount);
	~Vst2MidiEventQueue ();

	bool isEmpty () const { return eventList->numEvents == 0; }
	bool add (const VstMidiEvent& e);
	bool add (const VstMidiSysexEvent& e);
	void flush ();

	operator VstEvents* () { return eventList; }
//------------------------------------------------------------------------
protected:
	VstEvents* eventList;
	int32 maxEventCount;
};

//------------------------------------------------------------------------
// Vst2MidiEventQueue Implementation
//------------------------------------------------------------------------
Vst2MidiEventQueue::Vst2MidiEventQueue (int32 maxEventCount) : maxEventCount (maxEventCount)
{
	eventList = (VstEvents*)new char[sizeof (VstEvents) + (maxEventCount - 2) * sizeof (VstEvent*)];
	eventList->numEvents = 0;
	eventList->reserved = 0;

	int32 eventSize = sizeof (VstMidiSysexEvent) > sizeof (VstMidiEvent) ?
	                      sizeof (VstMidiSysexEvent) :
	                      sizeof (VstMidiEvent);

	for (int32 i = 0; i < maxEventCount; i++)
	{
		char* eventBuffer = new char[eventSize];
		memset (eventBuffer, 0, eventSize);
		eventList->events[i] = (VstEvent*)eventBuffer;
	}
}

//------------------------------------------------------------------------
Vst2MidiEventQueue::~Vst2MidiEventQueue ()
{
	for (int32 i = 0; i < maxEventCount; i++)
		delete[] (char*) eventList->events[i];

	delete[] (char*) eventList;
}

//------------------------------------------------------------------------
bool Vst2MidiEventQueue::add (const VstMidiEvent& e)
{
	if (eventList->numEvents >= maxEventCount)
		return false;

	VstMidiEvent* dst = (VstMidiEvent*)eventList->events[eventList->numEvents++];
	memcpy (dst, &e, sizeof (VstMidiEvent));
	dst->type = kVstMidiType;
	dst->byteSize = sizeof (VstMidiEvent);
	return true;
}

//------------------------------------------------------------------------
bool Vst2MidiEventQueue::add (const VstMidiSysexEvent& e)
{
	if (eventList->numEvents >= maxEventCount)
		return false;

	VstMidiSysexEvent* dst = (VstMidiSysexEvent*)eventList->events[eventList->numEvents++];
	memcpy (dst, &e, sizeof (VstMidiSysexEvent));
	dst->type = kVstSysExType;
	dst->byteSize = sizeof (VstMidiSysexEvent);
	return true;
}

//------------------------------------------------------------------------
void Vst2MidiEventQueue::flush ()
{
	eventList->numEvents = 0;
}

//------------------------------------------------------------------------
// MemoryStream with attributes to add information "preset or project"
//------------------------------------------------------------------------
class VstPresetStream : public MemoryStream, Vst::IStreamAttributes
{
public:
	VstPresetStream () {}
	VstPresetStream (void* memory, TSize memorySize) : MemoryStream (memory, memorySize) {}

	//---from Vst::IStreamAttributes-----
	virtual tresult PLUGIN_API getFileName (String128 name) SMTG_OVERRIDE { return kNotImplemented; }
	virtual IAttributeList* PLUGIN_API getAttributes () SMTG_OVERRIDE { return &attrList; }

	//------------------------------------------------------------------------
	DELEGATE_REFCOUNT (MemoryStream)
	virtual tresult PLUGIN_API queryInterface (const TUID iid, void** obj) SMTG_OVERRIDE
	{
		QUERY_INTERFACE (iid, obj, IStreamAttributes::iid, IStreamAttributes)
		return MemoryStream::queryInterface (iid, obj);
	}

protected:
	HostAttributeList attrList;
};

//------------------------------------------------------------------------
// Define the numeric max as "No ParamID"
const ParamID kNoParamId = std::numeric_limits<ParamID>::max ();
const int32 kMaxEvents = 2048;

//------------------------------------------------------------------------
// Vst2Wrapper
//------------------------------------------------------------------------
Vst2Wrapper::Vst2Wrapper (IAudioProcessor* processor, IEditController* controller,
                          audioMasterCallback audioMaster, const TUID vst3ComponentID,
                          VstInt32 vst2ID, IPluginFactory* factory)
: AudioEffectX (audioMaster, 0, 0)
, mVst2InputArrangement (nullptr)
, mVst2OutputArrangement (nullptr)
, mProcessor (processor)
, mComponent (nullptr)
, mController (controller)
, mUnitInfo (nullptr)
, mMidiMapping (nullptr)
, componentInitialized (false)
, controllerInitialized (false)
, componentsConnected (false)
, processing (false)
, hasEventInputBuses (false)
, hasEventOutputBuses (false)
, mVst3SampleSize (kSample32)
, mVst3processMode (kRealtime)
, mBypassParameterID (kNoParamId)
, mProgramParameterID (kNoParamId)
, mProgramParameterIdx (-1)
, mInputEvents (nullptr)
, mOutputEvents (nullptr)
, mVst2OutputEvents (nullptr)
, mMainAudioInputBuses (0)
, mMainAudioOutputBuses (0)
, mTimer (nullptr)
, mFactory (factory)
{
	memset (mName, 0, sizeof (mName));
	memset (mVendor, 0, sizeof (mVendor));
	memset (mSubCategories, 0, sizeof (mSubCategories));
	memset (&mProcessContext, 0, sizeof (ProcessContext));
	mVersion = 0;

	for (int32 b = 0; b < kMaxMidiMappingBusses; b++)
		for (int32 i = 0; i < 16; i++)
			mMidiCCMapping[b][i] = 0;

	// VST 2 stuff -----------------------------------------------
	setUniqueID (vst2ID); // identify
	mVst3EffectClassID = vst3ComponentID;
	canProcessReplacing (true); // supports replacing output
	programsAreChunks (true);

	for (int32 i = 0; i < kMaxProgramChangeParameters; i++)
	{
		mProgramChangeParameterIDs[i] = kNoParamId;
		mProgramChangeParameterIdxs[i] = -1;
	}
	if (factory)
		factory->addRef ();
}

//------------------------------------------------------------------------
bool Vst2Wrapper::init ()
{
	// VST 3 stuff -----------------------------------------------
	if (mProcessor)
		mProcessor->queryInterface (IComponent::iid, (void**)&mComponent);
	if (mController)
	{
		mController->queryInterface (IUnitInfo::iid, (void**)&mUnitInfo);
		mController->queryInterface (IMidiMapping::iid, (void**)&mMidiMapping);
	}

	//---init the processor component
	componentInitialized = true;
	if (mComponent->initialize ((IHostApplication*)this) != kResultTrue)
		return false;

	//---init the controller component
	if (mController)
	{
		// do not initialize 2 times the component if it is singleComponent
		if (FUnknownPtr<IEditController> (mComponent).getInterface () != mController)
		{
			controllerInitialized = true;
			if (mController->initialize ((IHostApplication*)this) != kResultTrue)
				return false;
		}

		//---set this class as Component Handler
		mController->setComponentHandler (this);

		//---connect the 2 components
		FUnknownPtr<IConnectionPoint> cp1 (mProcessor);
		FUnknownPtr<IConnectionPoint> cp2 (mController);
		if (cp1 && cp2)
		{
			cp1->connect (cp2);
			cp2->connect (cp1);

			componentsConnected = true;
		}

		//---inform the component "controller" with the component "processor" state
		MemoryStream stream;
		if (mComponent->getState (&stream) == kResultTrue)
		{
			stream.seek (0, IBStream::kIBSeekSet, 0);
			mController->setComponentState (&stream);
		}
	}

	// VST 2 stuff -----------------------------------------------
	if (mProcessor)
	{
		if (mProcessor->canProcessSampleSize (kSample64) == kResultTrue)
		{
			canDoubleReplacing (); // supports double precision processing

			// we use the 64 as default only if 32 bit not supported
			if (mProcessor->canProcessSampleSize (kSample32) != kResultTrue)
				mVst3SampleSize = kSample64;
			else
				mVst3SampleSize = kSample32;
		}

		// latency -------------------------------
		setInitialDelay (mProcessor->getLatencySamples ());

		if (mProcessor->getTailSamples () == kNoTail)
			noTail (true);

		setupProcessing (); // initialize vst3 component processing parameters
	}

	// parameters
	setupParameters ();

	// setup inputs and outputs
	setupBuses ();

	if (mController)
	{
		if (Vst2EditorWrapper::hasEditor (mController))
			setEditor (new Vst2EditorWrapper (this, mController));
	}

	// find out programs of root unit --------------------------
	numPrograms = cEffect.numPrograms = 0;
	if (mUnitInfo)
	{
		int32 programListCount = mUnitInfo->getProgramListCount ();
		if (programListCount > 0)
		{
			ProgramListID rootUnitProgramListId = kNoProgramListId;
			for (int32 i = 0; i < mUnitInfo->getUnitCount (); i++)
			{
				UnitInfo unit = {0};
				if (mUnitInfo->getUnitInfo (i, unit) == kResultTrue)
				{
					if (unit.id == kRootUnitId)
					{
						rootUnitProgramListId = unit.programListId;
						break;
					}
				}
			}

			if (rootUnitProgramListId != kNoProgramListId)
			{
				for (int32 i = 0; i < programListCount; i++)
				{
					ProgramListInfo progList = {0};
					if (mUnitInfo->getProgramListInfo (i, progList) == kResultTrue)
					{
						if (progList.id == rootUnitProgramListId)
						{
							numPrograms = cEffect.numPrograms = progList.programCount;
							break;
						}
					}
				}
			}
		}
	}

	if (mTimer == 0)
	{
		mTimer = Timer::create (this, 50);
	}

	initMidiCtrlerAssignment ();

	return true;
}

//------------------------------------------------------------------------
Vst2Wrapper::~Vst2Wrapper ()
{
	if (mTimer)
	{
		mTimer->release ();
		mTimer = nullptr;
	}

	mProcessData.unprepare ();

	if (mVst2InputArrangement)
		free (mVst2InputArrangement);
	if (mVst2OutputArrangement)
		free (mVst2OutputArrangement);

	//---Disconnect components
	if (componentsConnected)
	{
		FUnknownPtr<IConnectionPoint> cp1 (mProcessor);
		FUnknownPtr<IConnectionPoint> cp2 (mController);
		if (cp1 && cp2)
		{
			cp1->disconnect (cp2);
			cp2->disconnect (cp1);
		}
	}

	//---Terminate Controller Component
	if (mController)
	{
		mController->setComponentHandler (0);
		if (controllerInitialized)
			mController->terminate ();
		controllerInitialized = false;
	}

	//---Terminate Processor Component
	if (mComponent && componentInitialized)
	{
		mComponent->terminate ();
		componentInitialized = false;
	}

	if (mUnitInfo)
		mUnitInfo->release ();

	if (mMidiMapping)
		mMidiMapping->release ();
	if (mMidiCCMapping[0])
		for (int32 b = 0; b < kMaxMidiMappingBusses; b++)
			for (int32 i = 0; i < 16; i++)
				delete mMidiCCMapping[b][i];

	if (mController)
		mController->release ();

	//! editor needs to be destroyed BEFORE DeinitModule. Therefore destroy it here already
	//  instead of AudioEffect destructor
	delete editor;
	editor = nullptr;

	if (mComponent)
		mComponent->release ();

	if (mProcessor)
		mProcessor->release ();

	delete mInputEvents;
	delete mOutputEvents;
	delete mVst2OutputEvents;

	if (mFactory)
		mFactory->release ();

	DeinitModule ();
}

// VST 2
//------------------------------------------------------------------------
void Vst2Wrapper::suspend ()
{
	stopProcess ();

	if (mComponent)
		mComponent->setActive (false);
}

//------------------------------------------------------------------------
void Vst2Wrapper::resume ()
{
	AudioEffectX::resume ();
	mChunk.setSize (0);

	if (mComponent)
		mComponent->setActive (true);
}

//------------------------------------------------------------------------
VstInt32 Vst2Wrapper::startProcess ()
{
	if (mProcessor && processing == false)
	{
		processing = true;
		mProcessor->setProcessing (true);
	}
	return 0;
}

//------------------------------------------------------------------------
VstInt32 Vst2Wrapper::stopProcess ()
{
	if (mProcessor && processing)
	{
		mProcessor->setProcessing (false);
		processing = false;
	}
	return 0;
}

//------------------------------------------------------------------------
bool Vst2Wrapper::setupProcessing (int32 processModeOverwrite)
{
	if (!mProcessor)
		return false;

	ProcessSetup setup;
	if (processModeOverwrite >= 0)
		setup.processMode = processModeOverwrite;
	else
		setup.processMode = mVst3processMode;
	setup.maxSamplesPerBlock = blockSize;
	setup.sampleRate = sampleRate;
	setup.symbolicSampleSize = mVst3SampleSize;

	return mProcessor->setupProcessing (setup) == kResultTrue;
}

//------------------------------------------------------------------------
void Vst2Wrapper::setSampleRate (float newSamplerate)
{
	if (processing)
		return;

	if (newSamplerate != sampleRate)
	{
		AudioEffectX::setSampleRate (newSamplerate);
		setupProcessing ();
	}
}

//------------------------------------------------------------------------
void Vst2Wrapper::setBlockSize (VstInt32 newBlockSize)
{
	if (processing)
		return;

	if (blockSize != newBlockSize)
	{
		AudioEffectX::setBlockSize (newBlockSize);
		setupProcessing ();
	}
}

//------------------------------------------------------------------------
float Vst2Wrapper::getParameter (VstInt32 index)
{
	if (!mController)
		return 0.f;

	if (index < (int32)mParameterMap.size ())
	{
		ParamID id = mParameterMap.at (index).vst3ID;
		return (float)mController->getParamNormalized (id);
	}

	return 0.f;
}

//------------------------------------------------------------------------
void Vst2Wrapper::setParameter (VstInt32 index, float value)
{
	if (!mController)
		return;

	if (index < (int32)mParameterMap.size ())
	{
		ParamID id = mParameterMap.at (index).vst3ID;
		addParameterChange (id, (ParamValue)value, 0);
	}
}

//------------------------------------------------------------------------
void Vst2Wrapper::addParameterChange (ParamID id, ParamValue value, int32 sampleOffset)
{
	mGuiTransfer.addChange (id, value, sampleOffset);
	mInputTransfer.addChange (id, value, sampleOffset);
}

//------------------------------------------------------------------------
void Vst2Wrapper::setProgram (VstInt32 program)
{
	if (mProgramParameterID != kNoParamId && mController != 0 && mProgramParameterIdx != -1)
	{
		AudioEffectX::setProgram (program);

		ParameterInfo paramInfo = {0};
		if (mController->getParameterInfo (mProgramParameterIdx, paramInfo) == kResultTrue)
		{
			if (paramInfo.stepCount > 0 && program <= paramInfo.stepCount)
			{
				ParamValue normalized = (ParamValue)program / (ParamValue)paramInfo.stepCount;
				addParameterChange (mProgramParameterID, normalized, 0);
			}
		}
	}
}

//------------------------------------------------------------------------
void Vst2Wrapper::setProgramName (char* name)
{
	// not supported in VST 3
}

//------------------------------------------------------------------------
void Vst2Wrapper::getProgramName (char* name)
{
	// name of the current program. Limited to #kVstMaxProgNameLen.
	*name = 0;
	if (mUnitInfo)
	{
		ProgramListInfo listInfo = {0};
		if (mUnitInfo->getProgramListInfo (0, listInfo) == kResultTrue)
		{
			String128 tmp = {0};
			if (mUnitInfo->getProgramName (listInfo.id, curProgram, tmp) == kResultTrue)
			{
				String str (tmp);
				str.copyTo8 (name, 0, kVstMaxProgNameLen);
			}
		}
	}
}

//------------------------------------------------------------------------
bool Vst2Wrapper::getProgramNameIndexed (VstInt32, VstInt32 index, char* name)
{
	*name = 0;
	if (mUnitInfo)
	{
		ProgramListInfo listInfo = {0};
		if (mUnitInfo->getProgramListInfo (0, listInfo) == kResultTrue)
		{
			String128 tmp = {0};
			if (mUnitInfo->getProgramName (listInfo.id, index, tmp) == kResultTrue)
			{
				String str (tmp);
				str.copyTo8 (name, 0, kVstMaxProgNameLen);
				return true;
			}
		}
	}

	return false;
}

//------------------------------------------------------------------------
void Vst2Wrapper::getParameterLabel (VstInt32 index, char* label)
{
	// units in which parameter \e index is displayed (i.e. "sec", "dB", "type", etc...). Limited to
	// #kVstMaxParamStrLen.
	*label = 0;
	if (mController)
	{
		int32 vst3Index = mParameterMap.at (index).vst3Index;

		ParameterInfo paramInfo = {0};
		if (mController->getParameterInfo (vst3Index, paramInfo) == kResultTrue)
		{
			String str (paramInfo.units);
			str.copyTo8 (label, 0, kVstMaxParamStrLen);
		}
	}
}

//------------------------------------------------------------------------
/*!	Usually VST 2 hosts call setParameter (...) and getParameterDisplay (...) synchronously.
    In setParameter (...) param changes get queued (guiTransfer) and transfered in idle (::onTimer).
    The ::onTimer call almost always comes AFTER getParameterDisplay (...) and therefore returns an
   old
    value. To avoid sending back old values, getLastParamChange (...) returns the latest value
    from the guiTransfer queue. */
//------------------------------------------------------------------------
bool Vst2Wrapper::getLastParamChange (ParamID id, ParamValue& value)
{
	ParameterChanges changes;
	mGuiTransfer.transferChangesTo (changes);
	for (int32 i = 0; i < changes.getParameterCount (); ++i)
	{
		IParamValueQueue* queue = changes.getParameterData (i);
		if (queue)
		{
			if (queue->getParameterId () == id)
			{
				int32 points = queue->getPointCount ();
				if (points > 0)
				{
					mGuiTransfer.transferChangesFrom (changes);
					int32 sampleOffset = 0;
					return queue->getPoint (points - 1, sampleOffset, value) == kResultTrue;
				}
			}
		}
	}

	mGuiTransfer.transferChangesFrom (changes);
	return false;
}

//------------------------------------------------------------------------
void Vst2Wrapper::getParameterDisplay (VstInt32 index, char* text)
{
	// string representation ("0.5", "-3", "PLATE", etc...) of the value of parameter \e index.
	// Limited to #kVstMaxParamStrLen.
	*text = 0;
	if (mController)
	{
		int32 vst3Index = mParameterMap.at (index).vst3Index;

		ParameterInfo paramInfo = {0};
		if (mController->getParameterInfo (vst3Index, paramInfo) == kResultTrue)
		{
			String128 tmp = {0};
			ParamValue value = 0;
			if (!getLastParamChange (paramInfo.id, value))
				value = mController->getParamNormalized (paramInfo.id);

			if (mController->getParamStringByValue (paramInfo.id, value, tmp) == kResultTrue)
			{
				String str (tmp);
				str.copyTo8 (text, 0, kVstMaxParamStrLen);
			}
		}
	}
}

//------------------------------------------------------------------------
void Vst2Wrapper::getUnitPath (UnitID unitID, String& path)
{
	//! Build the unit path up to the root unit (e.g. "Modulators.LFO 1.". Separator is a ".")
	for (int32 unitIndex = 0; unitIndex < mUnitInfo->getUnitCount (); ++unitIndex)
	{
		UnitInfo info = {0};
		mUnitInfo->getUnitInfo (unitIndex, info);
		if (info.id == unitID)
		{
			String unitName (info.name);
			unitName.append (".");
			path.insertAt (0, unitName);
			if (info.parentUnitId != kRootUnitId)
				getUnitPath (info.parentUnitId, path);

			break;
		}
	}
}
//------------------------------------------------------------------------
void Vst2Wrapper::getParameterName (VstInt32 index, char* text)
{
	// name ("Time", "Gain", "RoomType", etc...) of parameter \e index. Limited to
	// #kVstExtMaxParamStrLen.
	*text = 0;
	if (mController && index < (int32)mParameterMap.size ())
	{
		int32 vst3Index = mParameterMap.at (index).vst3Index;

		ParameterInfo paramInfo = {0};
		if (mController->getParameterInfo (vst3Index, paramInfo) == kResultTrue)
		{
			String str;
			if (vst2WrapperFullParameterPath)
			{
				//! The parameter's name contains the unit path (e.g. "LFO 1.freq") as well
				if (mUnitInfo)
				{
					getUnitPath (paramInfo.unitId, str);
				}
			}
			str.append (paramInfo.title);

			if (str.length () > kVstExtMaxParamStrLen)
			{
				//! In case the string's length exceeds the limit, try parameter's title without
				// unit path.
				str = paramInfo.title;
			}
			if (str.length () > kVstExtMaxParamStrLen)
			{
				str = paramInfo.shortTitle;
			}
			str.copyTo8 (text, 0, kVstExtMaxParamStrLen);
		}
	}
}

//------------------------------------------------------------------------
bool Vst2Wrapper::canParameterBeAutomated (VstInt32 index)
{
	if (mController && index < (int32)mParameterMap.size ())
	{
		int32 vst3Index = mParameterMap.at (index).vst3Index;

		ParameterInfo paramInfo = {0};
		if (mController->getParameterInfo (vst3Index, paramInfo) == kResultTrue)
			return (paramInfo.flags & ParameterInfo::kCanAutomate) != 0;
	}
	return false;
}

//------------------------------------------------------------------------
bool Vst2Wrapper::string2parameter (VstInt32 index, char* text)
{
	if (mController && index < (int32)mParameterMap.size ())
	{
		int32 vst3Index = mParameterMap.at (index).vst3Index;

		ParameterInfo paramInfo = {0};
		if (mController->getParameterInfo (vst3Index, paramInfo) == kResultTrue)
		{
			TChar tString[1024] = {0};
			String tmp (text);
			tmp.copyTo16 (tString, 0, 1023);

			ParamValue valueNormalized = 0.0;

			if (mController->getParamValueByString (paramInfo.id, tString, valueNormalized))
			{
				setParameterAutomated (index, (float)valueNormalized);
				// TODO: check if setParameterAutomated is correct
			}
		}
	}
	return false;
}

//------------------------------------------------------------------------
bool Vst2Wrapper::getParameterProperties (VstInt32 index, VstParameterProperties* p)
{
	if (mController && index < (int32)mParameterMap.size ())
	{
		p->label[0] = 0;
		p->shortLabel[0] = 0;

		int32 vst3Index = mParameterMap.at (index).vst3Index;

		ParameterInfo paramInfo = {0};
		if (mController->getParameterInfo (vst3Index, paramInfo) == kResultTrue)
		{
			String str (paramInfo.title);
			str.copyTo8 (p->label, 0, kVstMaxLabelLen);

			String str2 (paramInfo.shortTitle);
			str.copyTo8 (p->shortLabel, 0, kVstMaxShortLabelLen);

			if (paramInfo.stepCount == 0) // continuous
			{
				p->flags |= kVstParameterCanRamp;
			}
			else if (paramInfo.stepCount == 1) // on / off
			{
				p->flags |= kVstParameterIsSwitch;
			}
			else
			{
				p->minInteger = 0;
				p->maxInteger = paramInfo.stepCount;
				p->flags |= kVstParameterUsesIntegerMinMax;
			}

			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------
VstInt32 Vst2Wrapper::getChunk (void** data, bool isPreset)
{
	// Host stores Plug-in state. Returns the size in bytes of the chunk (Plug-in allocates the data
	// array)
	MemoryStream componentStream;
	if (mComponent && mComponent->getState (&componentStream) != kResultTrue)
		componentStream.setSize (0);

	MemoryStream controllerStream;
	if (mController && mController->getState (&controllerStream) != kResultTrue)
		controllerStream.setSize (0);

	if (componentStream.getSize () + controllerStream.getSize () == 0)
		return 0;

	mChunk.setSize (0);
	IBStreamer acc (&mChunk, kLittleEndian);

	acc.writeInt64 (componentStream.getSize ());
	acc.writeInt64 (controllerStream.getSize ());

	acc.writeRaw (componentStream.getData (), (int32)componentStream.getSize ());
	acc.writeRaw (controllerStream.getData (), (int32)controllerStream.getSize ());

	int32 chunkSize = (int32)mChunk.getSize ();
	*data = mChunk.getData ();
	return chunkSize;
}

//------------------------------------------------------------------------
VstInt32 Vst2Wrapper::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{
	if (!mComponent)
		return 0;

	// throw away all previously queued parameter changes, they are obsolete
	mGuiTransfer.removeChanges ();
	mInputTransfer.removeChanges ();

	MemoryStream chunk (data, byteSize);
	IBStreamer acc (&chunk, kLittleEndian);

	int64 componentDataSize = 0;
	int64 controllerDataSize = 0;

	acc.readInt64 (componentDataSize);
	acc.readInt64 (controllerDataSize);

	VstPresetStream componentStream (((char*)data) + acc.tell (), componentDataSize);
	VstPresetStream controllerStream (((char*)data) + acc.tell () + componentDataSize,
	                                  controllerDataSize);

	mComponent->setState (&componentStream);
	componentStream.seek (0, IBStream::kIBSeekSet, 0);

	if (mController)
	{
		if (!isPreset)
		{
			if (Vst::IAttributeList* attr = componentStream.getAttributes ())
				attr->setString (Vst::PresetAttributes::kStateType,
				                 String (Vst::StateType::kProject));
			if (Vst::IAttributeList* attr = controllerStream.getAttributes ())
				attr->setString (Vst::PresetAttributes::kStateType, 
				                 String (Vst::StateType::kProject));
		}
		mController->setComponentState (&componentStream);
		mController->setState (&controllerStream);
	}

	return 0;
}

//------------------------------------------------------------------------
VstInt32 Vst2Wrapper::vst3ToVst2SpeakerArr (SpeakerArrangement vst3Arr)
{
	switch (vst3Arr)
	{
		case SpeakerArr::kMono: return kSpeakerArrMono;
		case SpeakerArr::kStereo: return kSpeakerArrStereo;
		case SpeakerArr::kStereoSurround: return kSpeakerArrStereoSurround;
		case SpeakerArr::kStereoCenter: return kSpeakerArrStereoCenter;
		case SpeakerArr::kStereoSide: return kSpeakerArrStereoSide;
		case SpeakerArr::kStereoCLfe: return kSpeakerArrStereoCLfe;
		case SpeakerArr::k30Cine: return kSpeakerArr30Cine;
		case SpeakerArr::k30Music: return kSpeakerArr30Music;
		case SpeakerArr::k31Cine: return kSpeakerArr31Cine;
		case SpeakerArr::k31Music: return kSpeakerArr31Music;
		case SpeakerArr::k40Cine: return kSpeakerArr40Cine;
		case SpeakerArr::k40Music: return kSpeakerArr40Music;
		case SpeakerArr::k41Cine: return kSpeakerArr41Cine;
		case SpeakerArr::k41Music: return kSpeakerArr41Music;
		case SpeakerArr::k50: return kSpeakerArr50;
		case SpeakerArr::k51: return kSpeakerArr51;
		case SpeakerArr::k60Cine: return kSpeakerArr60Cine;
		case SpeakerArr::k60Music: return kSpeakerArr60Music;
		case SpeakerArr::k61Cine: return kSpeakerArr61Cine;
		case SpeakerArr::k61Music: return kSpeakerArr61Music;
		case SpeakerArr::k70Cine: return kSpeakerArr70Cine;
		case SpeakerArr::k70Music: return kSpeakerArr70Music;
		case SpeakerArr::k71Cine: return kSpeakerArr71Cine;
		case SpeakerArr::k71Music: return kSpeakerArr71Music;
		case SpeakerArr::k80Cine: return kSpeakerArr80Cine;
		case SpeakerArr::k80Music: return kSpeakerArr80Music;
		case SpeakerArr::k81Cine: return kSpeakerArr81Cine;
		case SpeakerArr::k81Music: return kSpeakerArr81Music;
		case SpeakerArr::k102: return kSpeakerArr102;
	}

	return kSpeakerArrUserDefined;
}

//------------------------------------------------------------------------
SpeakerArrangement Vst2Wrapper::vst2ToVst3SpeakerArr (VstInt32 vst2Arr)
{
	switch (vst2Arr)
	{
		case kSpeakerArrMono: return SpeakerArr::kMono;
		case kSpeakerArrStereo: return SpeakerArr::kStereo;
		case kSpeakerArrStereoSurround: return SpeakerArr::kStereoSurround;
		case kSpeakerArrStereoCenter: return SpeakerArr::kStereoCenter;
		case kSpeakerArrStereoSide: return SpeakerArr::kStereoSide;
		case kSpeakerArrStereoCLfe: return SpeakerArr::kStereoCLfe;
		case kSpeakerArr30Cine: return SpeakerArr::k30Cine;
		case kSpeakerArr30Music: return SpeakerArr::k30Music;
		case kSpeakerArr31Cine: return SpeakerArr::k31Cine;
		case kSpeakerArr31Music: return SpeakerArr::k31Music;
		case kSpeakerArr40Cine: return SpeakerArr::k40Cine;
		case kSpeakerArr40Music: return SpeakerArr::k40Music;
		case kSpeakerArr41Cine: return SpeakerArr::k41Cine;
		case kSpeakerArr41Music: return SpeakerArr::k41Music;
		case kSpeakerArr50: return SpeakerArr::k50;
		case kSpeakerArr51: return SpeakerArr::k51;
		case kSpeakerArr60Cine: return SpeakerArr::k60Cine;
		case kSpeakerArr60Music: return SpeakerArr::k60Music;
		case kSpeakerArr61Cine: return SpeakerArr::k61Cine;
		case kSpeakerArr61Music: return SpeakerArr::k61Music;
		case kSpeakerArr70Cine: return SpeakerArr::k70Cine;
		case kSpeakerArr70Music: return SpeakerArr::k70Music;
		case kSpeakerArr71Cine: return SpeakerArr::k71Cine;
		case kSpeakerArr71Music: return SpeakerArr::k71Music;
		case kSpeakerArr80Cine: return SpeakerArr::k80Cine;
		case kSpeakerArr80Music: return SpeakerArr::k80Music;
		case kSpeakerArr81Cine: return SpeakerArr::k81Cine;
		case kSpeakerArr81Music: return SpeakerArr::k81Music;
		case kSpeakerArr102: return SpeakerArr::k102;
	}

	return 0;
}

//------------------------------------------------------------------------
VstInt32 Vst2Wrapper::vst3ToVst2Speaker (Vst::Speaker vst3Speaker)
{
	switch (vst3Speaker)
	{
		case Vst::kSpeakerM: return ::kSpeakerM;
		case Vst::kSpeakerL: return ::kSpeakerL;
		case Vst::kSpeakerR: return ::kSpeakerR;
		case Vst::kSpeakerC: return ::kSpeakerC;
		case Vst::kSpeakerLfe: return ::kSpeakerLfe;
		case Vst::kSpeakerLs: return ::kSpeakerLs;
		case Vst::kSpeakerRs: return ::kSpeakerRs;
		case Vst::kSpeakerLc: return ::kSpeakerLc;
		case Vst::kSpeakerRc: return ::kSpeakerRc;
		case Vst::kSpeakerS: return ::kSpeakerS;
		case Vst::kSpeakerSl: return ::kSpeakerSl;
		case Vst::kSpeakerSr: return ::kSpeakerSr;
		case Vst::kSpeakerTc: return ::kSpeakerTm;
		case Vst::kSpeakerTfl: return ::kSpeakerTfl;
		case Vst::kSpeakerTfc: return ::kSpeakerTfc;
		case Vst::kSpeakerTfr: return ::kSpeakerTfr;
		case Vst::kSpeakerTrl: return ::kSpeakerTrl;
		case Vst::kSpeakerTrc: return ::kSpeakerTrc;
		case Vst::kSpeakerTrr: return ::kSpeakerTrr;
		case Vst::kSpeakerLfe2: return ::kSpeakerLfe2;
	}
	return ::kSpeakerUndefined;
}

//------------------------------------------------------------------------
static const char* gSpeakerNames[] = {
    "M", ///< Mono (M)
    "L", ///< Left (L)
    "R", ///< Right (R)
    "C", ///< Center (C)
    "Lfe", ///< Subbass (Lfe)
    "Ls", ///< Left Surround (Ls)
    "Rs", ///< Right Surround (Rs)
    "Lc", ///< Left of Center (Lc)
    "Rc", ///< Right of Center (Rc)
    "Cs", ///< Center of Surround (Cs) = Surround (S)
    "Sl", ///< Side Left (Sl)
    "Sr", ///< Side Right (Sr)
    "Tm", ///< Top Middle (Tm)
    "Tfl", ///< Top Front Left (Tfl)
    "Tfc", ///< Top Front Center (Tfc)
    "Tfr", ///< Top Front Right (Tfr)
    "Trl", ///< Top Rear Left (Trl)
    "Trc", ///< Top Rear Center (Trc)
    "Trr", ///< Top Rear Right (Trr)
    "Lfe2" ///< Subbass 2 (Lfe2)
};
static const int32 kNumSpeakerNames = sizeof (gSpeakerNames) / sizeof (char*);

//------------------------------------------------------------------------
bool Vst2Wrapper::pinIndexToBusChannel (BusDirection dir, VstInt32 pinIndex, int32& busIndex,
                                        int32& busChannel)
{
	AudioBusBuffers* busBuffers = dir == kInput ? mProcessData.inputs : mProcessData.outputs;
	int32 busCount = dir == kInput ? mProcessData.numInputs : mProcessData.numOutputs;
	uint64 mainBusFlags = dir == kInput ? mMainAudioInputBuses : mMainAudioOutputBuses;

	int32 sourceIndex = 0;
	for (busIndex = 0; busIndex < busCount; busIndex++)
	{
		AudioBusBuffers& buffers = busBuffers[busIndex];
		if (mainBusFlags & (uint64 (1) << busIndex))
		{
			for (busChannel = 0; busChannel < buffers.numChannels; busChannel++)
			{
				if (pinIndex == sourceIndex)
				{
					return true;
				}
				sourceIndex++;
			}
		}
	}
	return false;
}

//------------------------------------------------------------------------
bool Vst2Wrapper::getPinProperties (BusDirection dir, VstInt32 pinIndex,
                                    VstPinProperties* properties)
{
	int32 busIndex = -1;
	int32 busChannelIndex = -1;

	if (pinIndexToBusChannel (dir, pinIndex, busIndex, busChannelIndex))
	{
		BusInfo busInfo = {0};
		if (mComponent && mComponent->getBusInfo (kAudio, dir, busIndex, busInfo) == kResultTrue)
		{
			properties->flags = kVstPinIsActive; // ????

			String name (busInfo.name);
			name.copyTo8 (properties->label, 0, kVstMaxLabelLen);

			if (busInfo.channelCount == 1)
			{
				properties->flags |= kVstPinUseSpeaker;
				properties->arrangementType = kSpeakerArrMono;
			}
			if (busInfo.channelCount == 2)
			{
				properties->flags |= kVstPinUseSpeaker;
				properties->flags |= kVstPinIsStereo;
				properties->arrangementType = kSpeakerArrStereo;
			}
			else if (busInfo.channelCount > 2)
			{
				Vst::SpeakerArrangement arr = 0;
				if (mProcessor && mProcessor->getBusArrangement (dir, busIndex, arr) == kResultTrue)
				{
					properties->flags |= kVstPinUseSpeaker;
					properties->arrangementType = vst3ToVst2SpeakerArr (arr);
				}
				else
				{
					return false;
				}
			}

			return true;
		}
	}

	return false;
}

//------------------------------------------------------------------------
bool Vst2Wrapper::getInputProperties (VstInt32 index, VstPinProperties* properties)
{
	return getPinProperties (kInput, index, properties);
}

//------------------------------------------------------------------------
bool Vst2Wrapper::getOutputProperties (VstInt32 index, VstPinProperties* properties)
{
	return getPinProperties (kOutput, index, properties);
}

//------------------------------------------------------------------------
bool Vst2Wrapper::setSpeakerArrangement (VstSpeakerArrangement* pluginInput,
                                         VstSpeakerArrangement* pluginOutput)
{
	if (!mProcessor || !mComponent)
		return false;

	Vst::SpeakerArrangement newInputArr = 0;
	Vst::SpeakerArrangement newOutputArr = 0;
	Vst::SpeakerArrangement outputArr = 0;
	Vst::SpeakerArrangement inputArr = 0;

	int32 inputBusCount = mComponent->getBusCount (kAudio, kInput);
	int32 outputBusCount = mComponent->getBusCount (kAudio, kOutput);

	if (inputBusCount > 0)
		if (mProcessor->getBusArrangement (kInput, 0, inputArr) != kResultTrue)
			return false;
	if (outputBusCount > 0)
		if (mProcessor->getBusArrangement (kOutput, 0, outputArr) != kResultTrue)
			return false;

	if (pluginInput)
	{
		newInputArr = vst2ToVst3SpeakerArr (pluginInput->type);
		if (newInputArr == 0)
			return false;
	}
	if (pluginOutput)
	{
		newOutputArr = vst2ToVst3SpeakerArr (pluginOutput->type);
		if (newOutputArr == 0)
			return false;
	}

	if (newInputArr == 0)
		newInputArr = inputArr;
	if (newOutputArr == 0)
		newOutputArr = outputArr;

	if (newInputArr != inputArr || newOutputArr != outputArr)
	{
		if (mProcessor->setBusArrangements (
		        &newInputArr, (newInputArr > 0 && inputBusCount > 0) ? 1 : 0, &newOutputArr,
		        (newOutputArr > 0 && outputBusCount > 0) ? 1 : 0) != kResultTrue)
			return false;

		restartComponent (kIoChanged);
	}

	return true;
}

//------------------------------------------------------------------------
void Vst2Wrapper::setupVst2Arrangement (VstSpeakerArrangement*& vst2arr,
                                        Vst::SpeakerArrangement vst3Arrangement)
{
	int32 numChannels = Vst::SpeakerArr::getChannelCount (vst3Arrangement);

	if (vst2arr && (numChannels == 0 || (numChannels > vst2arr->numChannels && numChannels > 8)))
	{
		free (vst2arr);
		vst2arr = 0;
		if (numChannels == 0)
			return;
	}

	if (vst2arr == 0)
	{
		int32 channelOverhead = numChannels > 8 ? numChannels - 8 : 0;
		uint32 structSize =
		    sizeof (VstSpeakerArrangement) + channelOverhead * sizeof (VstSpeakerProperties);
		vst2arr = (VstSpeakerArrangement*)malloc (structSize);
		memset (vst2arr, 0, structSize);
	}

	if (vst2arr)
	{
		vst2arr->type = vst3ToVst2SpeakerArr (vst3Arrangement);
		vst2arr->numChannels = numChannels;

		Speaker vst3TestSpeaker = 1;

		for (int32 i = 0; i < numChannels; i++)
		{
			VstSpeakerProperties& props = vst2arr->speakers[i];

			// find nextSpeaker in vst3 arrangement
			Speaker vst3Speaker = 0;
			while (vst3Speaker == 0 && vst3TestSpeaker != 0)
			{
				if (vst3Arrangement & vst3TestSpeaker)
					vst3Speaker = vst3TestSpeaker;

				vst3TestSpeaker <<= 1;
			}

			if (vst3Speaker)
			{
				props.type = vst3ToVst2Speaker (vst3Speaker);
				if (props.type >= 0 && props.type < kNumSpeakerNames)
					strncpy (props.name, gSpeakerNames[props.type], kVstMaxNameLen);
				else
					sprintf (props.name, "%d", i + 1);
			}
		}
	}
}

//------------------------------------------------------------------------
bool Vst2Wrapper::getSpeakerArrangement (VstSpeakerArrangement** pluginInput,
                                         VstSpeakerArrangement** pluginOutput)
{
	if (!mProcessor)
		return false;

	Vst::SpeakerArrangement inputArr = 0;
	Vst::SpeakerArrangement outputArr = 0;

	if (mProcessor->getBusArrangement (kInput, 0, inputArr) != kResultTrue)
		inputArr = 0;

	if (mProcessor->getBusArrangement (kOutput, 0, outputArr) != kResultTrue)
		outputArr = 0;

	setupVst2Arrangement (mVst2InputArrangement, inputArr);
	setupVst2Arrangement (mVst2OutputArrangement, outputArr);

	*pluginInput = mVst2InputArrangement;
	*pluginOutput = mVst2OutputArrangement;

	return mVst2InputArrangement != 0 && mVst2OutputArrangement != 0;
}

//------------------------------------------------------------------------
bool Vst2Wrapper::setBypass (bool onOff)
{
	if (mBypassParameterID != kNoParamId)
	{
		addParameterChange (mBypassParameterID, onOff ? 1.0 : 0.0, 0);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
bool Vst2Wrapper::setProcessPrecision (VstInt32 precision)
{
	int32 newVst3SampleSize = -1;

	if (precision == kVstProcessPrecision32)
		newVst3SampleSize = kSample32;
	else if (precision == kVstProcessPrecision64)
		newVst3SampleSize = kSample64;

	if (newVst3SampleSize != mVst3SampleSize)
	{
		if (mProcessor && mProcessor->canProcessSampleSize (newVst3SampleSize) == kResultTrue)
		{
			mVst3SampleSize = newVst3SampleSize;
			setupProcessing ();

			setupBuses ();

			return true;
		}
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
VstInt32 Vst2Wrapper::getNumMidiInputChannels ()
{
	if (!mComponent)
		return 0;

	int32 busCount = mComponent->getBusCount (kEvent, kInput);
	if (busCount > 0)
	{
		BusInfo busInfo = {0};
		if (mComponent->getBusInfo (kEvent, kInput, 0, busInfo) == kResultTrue)
		{
			return busInfo.channelCount;
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
VstInt32 Vst2Wrapper::getNumMidiOutputChannels ()
{
	if (!mComponent)
		return 0;

	int32 busCount = mComponent->getBusCount (kEvent, kOutput);
	if (busCount > 0)
	{
		BusInfo busInfo = {0};
		if (mComponent->getBusInfo (kEvent, kOutput, 0, busInfo) == kResultTrue)
		{
			return busInfo.channelCount;
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
VstInt32 Vst2Wrapper::getGetTailSize ()
{
	if (mProcessor)
		return mProcessor->getTailSamples ();

	return 0;
}

//-----------------------------------------------------------------------------
bool Vst2Wrapper::getEffectName (char* effectName)
{
	if (mName[0])
	{
		strncpy (effectName, mName, kVstMaxEffectNameLen);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
bool Vst2Wrapper::getVendorString (char* text)
{
	if (mVendor[0])
	{
		strncpy (text, mVendor, kVstMaxVendorStrLen);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
VstInt32 Vst2Wrapper::getVendorVersion ()
{
	return mVersion;
}

//-----------------------------------------------------------------------------
VstIntPtr Vst2Wrapper::vendorSpecific (VstInt32 lArg, VstIntPtr lArg2, void* ptrArg, float floatArg)
{
	switch (lArg)
	{
		case 'stCA':
		case 'stCa':
			switch (lArg2)
			{
				//--- -------
				case 'FUID':
					if (ptrArg && mVst3EffectClassID.isValid ())
					{
						memcpy ((char*)ptrArg, mVst3EffectClassID, 16);
						return 1;
					}
					break;
				//--- -------
				case 'Whee':
					if (editor)
						editor->onWheel (floatArg);
					return 1;
					break;
			}
	}

	return AudioEffectX::vendorSpecific (lArg, lArg2, ptrArg, floatArg);
}

//-----------------------------------------------------------------------------
VstPlugCategory Vst2Wrapper::getPlugCategory ()
{
	if (mSubCategories[0])
	{
		if (strstr (mSubCategories, "Analyzer"))
			return kPlugCategAnalysis;

		else if (strstr (mSubCategories, "Delay") || strstr (mSubCategories, "Reverb"))
			return kPlugCategRoomFx;

		else if (strstr (mSubCategories, "Dynamics") || strstr (mSubCategories, "Mastering"))
			return kPlugCategMastering;

		else if (strstr (mSubCategories, "Restoration"))
			return kPlugCategRestoration;

		else if (strstr (mSubCategories, "Generator"))
			return kPlugCategGenerator;

		else if (strstr (mSubCategories, "Spatial"))
			return kPlugCategSpacializer;

		else if (strstr (mSubCategories, "Fx"))
			return kPlugCategEffect;

		else if (strstr (mSubCategories, "Instrument"))
			return kPlugCategSynth;
	}
	return kPlugCategUnknown;
}

//-----------------------------------------------------------------------------
VstInt32 Vst2Wrapper::canDo (char* text)
{
	if (stricmp (text, "sendVstEvents") == 0)
	{
		return -1;
	}
	else if (stricmp (text, "sendVstMidiEvent") == 0)
	{
		return hasEventOutputBuses ? 1 : -1;
	}
	else if (stricmp (text, "receiveVstEvents") == 0)
	{
		return -1;
	}
	else if (stricmp (text, "receiveVstMidiEvent") == 0)
	{
		return hasEventInputBuses ? 1 : -1;
	}
	else if (stricmp (text, "receiveVstTimeInfo") == 0)
	{
		return 1;
	}
	else if (stricmp (text, "offline") == 0)
	{
		if (processing)
			return 0;
		if (mVst3processMode == kOffline)
			return 1;

		bool canOffline = setupProcessing (kOffline);
		setupProcessing ();
		return canOffline ? 1 : -1;
	}
	else if (stricmp (text, "midiProgramNames") == 0)
	{
		if (mUnitInfo)
		{
			UnitID unitId = -1;
			if (mUnitInfo->getUnitByBus (kEvent, kInput, 0, 0, unitId) == kResultTrue && unitId >= 0)
				return 1;
		}
		return -1;
	}
	else if (stricmp (text, "bypass") == 0)
	{
		return mBypassParameterID != kNoParamId ? 1 : -1;
	}
	return 0; // do not know
}

//-----------------------------------------------------------------------------
bool Vst2Wrapper::setupMidiProgram (int32 midiChannel, ProgramListID programListId,
									MidiProgramName& midiProgramName)
{
	if (mUnitInfo)
	{
		String128 string128 = {0};

		if (mUnitInfo->getProgramName (programListId, midiProgramName.thisProgramIndex, string128) ==
			kResultTrue)
		{
			String str (string128);
			str.copyTo8 (midiProgramName.name, 0, kVstMaxNameLen);

			midiProgramName.midiProgram = midiProgramName.thisProgramIndex;
			midiProgramName.midiBankMsb = -1;
			midiProgramName.midiBankLsb = -1;
			midiProgramName.parentCategoryIndex = -1;
			midiProgramName.flags = 0;

			if (mUnitInfo->getProgramInfo (programListId, midiProgramName.thisProgramIndex,
										  PresetAttributes::kInstrument, string128) == kResultTrue)
			{
				midiProgramName.parentCategoryIndex =
					lookupProgramCategory (midiChannel, string128);
			}
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
VstInt32 Vst2Wrapper::getMidiProgramName (VstInt32 channel, MidiProgramName* midiProgramName)
{
	UnitID unitId;
	ProgramListID programListId;
	if (mUnitInfo && getProgramListAndUnit (channel, unitId, programListId))
	{
		if (midiProgramName)
			setupMidiProgram (channel, programListId, *midiProgramName);

		ProgramListInfo programListInfo;
		if (getProgramListInfoByProgramListID (programListId, programListInfo))
			return programListInfo.programCount;
	}
	return 0;
}

//-----------------------------------------------------------------------------
VstInt32 Vst2Wrapper::getCurrentMidiProgram (VstInt32 channel, MidiProgramName* currentProgram)
{
	if (mUnitInfo && mController)
	{
		UnitID unitId;
		ProgramListID programListId;
		if (getProgramListAndUnit (channel, unitId, programListId))
		{
			// find program selector parameter
			int32 parameterCount = mController->getParameterCount ();
			for (int32 i = 0; i < parameterCount; i++)
			{
				ParameterInfo parameterInfo = {0};
				if (mController->getParameterInfo (i, parameterInfo) == kResultTrue)
				{
					if ((parameterInfo.flags & ParameterInfo::kIsProgramChange) != 0 &&
						parameterInfo.unitId == unitId)
					{
						ParamValue normalized = mController->getParamNormalized (parameterInfo.id);
						int32 discreteValue =
							Min<int32> ((int32) (normalized * (parameterInfo.stepCount + 1)),
										parameterInfo.stepCount);

						if (currentProgram)
						{
							currentProgram->thisProgramIndex = discreteValue;
							setupMidiProgram (channel, programListId, *currentProgram);
						}

						return discreteValue;
					}
				}
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
VstInt32 Vst2Wrapper::getMidiProgramCategory (VstInt32 channel, MidiProgramCategory* category)
{
	// try to rebuild it each time
	setupProgramCategories ();

	if (channel >= (VstInt32)mProgramCategories.size ())
		return 0;

	std::vector<ProgramCategory>& channelCategories = mProgramCategories[channel];
	if (category && category->thisCategoryIndex < (VstInt32)channelCategories.size ())
	{
		ProgramCategory& cat = channelCategories[category->thisCategoryIndex];
		if (cat.vst2Category.thisCategoryIndex == category->thisCategoryIndex)
			memcpy (category, &cat.vst2Category, sizeof (MidiProgramCategory));
	}
	return (VstInt32)channelCategories.size ();
}

//-----------------------------------------------------------------------------
bool Vst2Wrapper::hasMidiProgramsChanged (VstInt32 channel)
{
	// names of programs or program categories have changed
	return false;
}

//-----------------------------------------------------------------------------
bool Vst2Wrapper::getMidiKeyName (VstInt32 channel, MidiKeyName* keyName)
{
	UnitID unitId;
	ProgramListID programListId;
	if (mUnitInfo && getProgramListAndUnit (channel, unitId, programListId))
	{
		String128 string128 = {0};
		if (mUnitInfo->getProgramPitchName (programListId, keyName->thisProgramIndex,
										   keyName->thisKeyNumber, string128))
		{
			String str (string128);
			str.copyTo8 (keyName->keyName, 0, kVstMaxNameLen);
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
bool Vst2Wrapper::getProgramListAndUnit (int32 midiChannel, UnitID& unitId,
										 ProgramListID& programListId)
{
	programListId = kNoProgramListId;
	unitId = -1;

	// use the first input event bus (VST2 has only 1 bus for event)
	if (mUnitInfo && mUnitInfo->getUnitByBus (kEvent, kInput, 0, midiChannel, unitId) == kResultTrue)
	{
		for (int32 i = 0, unitCount = mUnitInfo->getUnitCount (); i < unitCount; i++)
		{
			UnitInfo unitInfoStruct = {0};
			if (mUnitInfo->getUnitInfo (i, unitInfoStruct) == kResultTrue)
			{
				if (unitId == unitInfoStruct.id)
				{
					programListId = unitInfoStruct.programListId;
					return programListId != kNoProgramListId;
				}
			}
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
bool Vst2Wrapper::getProgramListInfoByProgramListID (ProgramListID programListId,
													 ProgramListInfo& info)
{
	if (mUnitInfo)
	{
		int32 programListCount = mUnitInfo->getProgramListCount ();
		for (int32 i = 0; i < programListCount; i++)
		{
			memset (&info, 0, sizeof (ProgramListInfo));
			if (mUnitInfo->getProgramListInfo (i, info) == kResultTrue)
			{
				if (info.id == programListId)
				{
					return true;
				}
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
int32 Vst2Wrapper::lookupProgramCategory (int32 midiChannel, String128 instrumentAttribute)
{
	std::vector<ProgramCategory>& channelCategories = mProgramCategories[midiChannel];

	for (uint32 categoryIndex = 0; categoryIndex < channelCategories.size (); categoryIndex++)
	{
		ProgramCategory& cat = channelCategories[categoryIndex];
		if (memcmp (instrumentAttribute, cat.vst3InstrumentAttribute, sizeof (String128)) == 0)
			return categoryIndex;
	}

	return -1;
}

//-----------------------------------------------------------------------------
uint32 Vst2Wrapper::makeCategoriesRecursive (std::vector<ProgramCategory>& channelCategories,
											 String128 vst3Category)
{
	for (uint32 categoryIndex = 0; categoryIndex < channelCategories.size (); categoryIndex++)
	{
		ProgramCategory& cat = channelCategories[categoryIndex];
		if (memcmp (vst3Category, cat.vst3InstrumentAttribute, sizeof (String128)) == 0)
		{
			return categoryIndex;
		}
	}

	int32 parentCategorIndex = -1;

	String128 str;
	String strAcc (str);
	strAcc.copyTo16 (vst3Category, 0, 127);
	int32 len = strAcc.length ();
	String singleName;

	char16 divider = '|';
	for (int32 strIndex = len - 1; strIndex >= 0; strIndex--)
	{
		bool isDivider = str[strIndex] == divider;
		str[strIndex] = 0; // zero out rest
		if (isDivider)
		{
			singleName.assign (vst3Category + strIndex + 1);
			parentCategorIndex = makeCategoriesRecursive (channelCategories, str);
			break;
		}
	}

	// make new
	ProgramCategory cat = {0};
	memcpy (cat.vst3InstrumentAttribute, vst3Category, sizeof (String128));
	singleName.copyTo8 (cat.vst2Category.name, 0, kVstMaxNameLen);
	cat.vst2Category.parentCategoryIndex = parentCategorIndex;
	cat.vst2Category.thisCategoryIndex = (int32)channelCategories.size ();

	channelCategories.push_back (cat);

	return cat.vst2Category.thisCategoryIndex;
}

//-----------------------------------------------------------------------------
void Vst2Wrapper::setupProgramCategories ()
{
	mProgramCategories.clear ();
	if (mUnitInfo && mComponent)
	{
		if (mComponent->getBusCount (kEvent, kInput) > 0)
		{
			for (int32 channel = 0; channel < 16; channel++) // the 16 channels
			{
				// make vector for channel
				mProgramCategories.push_back (std::vector<ProgramCategory> ());
				std::vector<ProgramCategory>& channelCategories = mProgramCategories[channel];

				// scan program list of channel and find categories
				UnitID unitId;
				ProgramListID programListId;
				if (getProgramListAndUnit (channel, unitId, programListId))
				{
					ProgramListInfo programListInfo;
					if (getProgramListInfoByProgramListID (programListId, programListInfo))
					{
						for (int32 programIndex = 0; programIndex < programListInfo.programCount;
						     programIndex++)
						{
							String128 string128 = {0};
							if (mUnitInfo->getProgramInfo (programListId, programIndex,
							                               PresetAttributes::kInstrument,
							                               string128) == kResultTrue)
							{
								makeCategoriesRecursive (channelCategories, string128);
							}
						}
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
void Vst2Wrapper::setVendorName (char* name)
{
	memcpy (mVendor, name, sizeof (mVendor));
}

//-----------------------------------------------------------------------------
void Vst2Wrapper::setEffectName (char* effectName)
{
	memcpy (mName, effectName, sizeof (mName));
}

//-----------------------------------------------------------------------------
void Vst2Wrapper::setEffectVersion (char* version)
{
	if (!version)
		mVersion = 0;
	else
	{
		int32 major = 1;
		int32 minor = 0;
		int32 subminor = 0;
		int32 subsubminor = 0;
		int32 ret = sscanf (version, "%d.%d.%d.%d", &major, &minor, &subminor, &subsubminor);
		mVersion = (major & 0xff) << 24;
		if (ret > 3)
			mVersion += (subsubminor & 0xff);
		if (ret > 2)
			mVersion += (subminor & 0xff) << 8;
		if (ret > 1)
			mVersion += (minor & 0xff) << 16;
	}
}

//-----------------------------------------------------------------------------
void Vst2Wrapper::setSubCategories (char* string)
{
	memcpy (mSubCategories, string, sizeof (mSubCategories));

	if (strstr (mSubCategories, "Instrument"))
		isSynth (true);
}

//-----------------------------------------------------------------------------
void Vst2Wrapper::setupBuses ()
{
	if (!mComponent)
		return;

	mProcessData.prepare (*mComponent, 0, mVst3SampleSize);

	setNumInputs (countMainBusChannels (kInput, mMainAudioInputBuses));
	setNumOutputs (countMainBusChannels (kOutput, mMainAudioOutputBuses));

	hasEventInputBuses = mComponent->getBusCount (kEvent, kInput) > 0;
	hasEventOutputBuses = mComponent->getBusCount (kEvent, kOutput) > 0;

	if (hasEventInputBuses)
	{
		if (mInputEvents == 0)
			mInputEvents = new EventList (kMaxEvents);
	}
	else
	{
		if (mInputEvents)
		{
			delete mInputEvents;
			mInputEvents = nullptr;
		}
	}

	if (hasEventOutputBuses)
	{
		if (mOutputEvents == 0)
			mOutputEvents = new EventList (kMaxEvents);
		if (mVst2OutputEvents == 0)
			mVst2OutputEvents = new Vst2MidiEventQueue (kMaxEvents);
	}
	else
	{
		if (mOutputEvents)
		{
			delete mOutputEvents;
			mOutputEvents = nullptr;
		}
		if (mVst2OutputEvents)
		{
			delete mVst2OutputEvents;
			mVst2OutputEvents = nullptr;
		}
	}
}

//-----------------------------------------------------------------------------
void Vst2Wrapper::setupParameters ()
{
	mParameterMap.clear ();
	mParamIndexMap.clear ();
	mBypassParameterID = mProgramParameterID = kNoParamId;
	mProgramParameterIdx = -1;

	std::vector<ParameterInfo> programParameterInfos;
	std::vector<int32> programParameterIdxs;

	int32 paramCount = mController ? mController->getParameterCount () : 0;
    int32 vst2ParamID = 0;
    for (int32 i = 0; i < paramCount; i++)
	{
		ParameterInfo paramInfo = {0};
		if (mController->getParameterInfo (i, paramInfo) == kResultTrue)
		{
			//--- ------------------------------------------
			if ((paramInfo.flags & ParameterInfo::kIsBypass) != 0)
			{
				if (mBypassParameterID == kNoParamId)
					mBypassParameterID = paramInfo.id;
			}
			//--- ------------------------------------------
			else if ((paramInfo.flags & ParameterInfo::kIsProgramChange) != 0)
			{
				programParameterInfos.push_back (paramInfo);
				programParameterIdxs.push_back (i);
				if (paramInfo.unitId == kRootUnitId)
				{
					if (mProgramParameterID == kNoParamId)
					{
						mProgramParameterID = paramInfo.id;
						mProgramParameterIdx = i;
					}
				}

				if (gExportProgramChangeParameters == true)
				{
					ParamMapEntry entry = {paramInfo.id, i};
					mParameterMap.push_back (entry);
					mParamIndexMap[paramInfo.id] = vst2ParamID;
                    vst2ParamID++;
				}
			}
			//--- ------------------------------------------
			// do not export read only parameters to VST2
			else if ((paramInfo.flags & ParameterInfo::kIsReadOnly) == 0)
			{
				ParamMapEntry entry = {paramInfo.id, i};
				mParameterMap.push_back (entry);
				mParamIndexMap[paramInfo.id] = vst2ParamID;
                vst2ParamID++;
			}
		}
	}

	numParams = cEffect.numParams = (int32)mParameterMap.size ();

	mInputTransfer.setMaxParameters (paramCount);
	mOutputTransfer.setMaxParameters (paramCount);
	mGuiTransfer.setMaxParameters (paramCount);
	mInputChanges.setMaxParameters (paramCount);
	mOutputChanges.setMaxParameters (paramCount);

	for (int32 midiChannel = 0; midiChannel < kMaxProgramChangeParameters; midiChannel++)
	{
		mProgramChangeParameterIDs[midiChannel] = kNoParamId;
		mProgramChangeParameterIdxs[midiChannel] = -1;

		UnitID unitId;
		ProgramListID programListId;
		if (getProgramListAndUnit (midiChannel, unitId, programListId))
		{
			for (int32 i = 0; i < (int32)programParameterInfos.size (); i++)
			{
				const ParameterInfo& paramInfo = programParameterInfos.at (i);
				if (paramInfo.unitId == unitId)
				{
					mProgramChangeParameterIDs[midiChannel] = paramInfo.id;
					mProgramChangeParameterIdxs[midiChannel] = programParameterIdxs.at (i);
					break;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
void Vst2Wrapper::initMidiCtrlerAssignment ()
{
	if (!mMidiMapping || !mComponent)
		return;

	int32 busses = Min<int32> (mComponent->getBusCount (kEvent, kInput), kMaxMidiMappingBusses);

	if (!mMidiCCMapping[0][0])
	{
		for (int32 b = 0; b < busses; b++)
			for (int32 i = 0; i < 16; i++)
				mMidiCCMapping[b][i] = NEW int32[Vst::kCountCtrlNumber];
	}

	ParamID paramID;
	for (int32 b = 0; b < busses; b++)
	{
		for (int16 ch = 0; ch < 16; ch++)
		{
			for (int32 i = 0; i < Vst::kCountCtrlNumber; i++)
			{
				paramID = kNoParamId;
				if (mMidiMapping->getMidiControllerAssignment (b, ch, (CtrlNumber)i, paramID) ==
				    kResultTrue)
				{
					// TODO check if tag is associated to a parameter
					mMidiCCMapping[b][ch][i] = paramID;
				}
				else
					mMidiCCMapping[b][ch][i] = kNoParamId;
			}
		}
	}
}

//-----------------------------------------------------------------------------
int32 Vst2Wrapper::countMainBusChannels (BusDirection dir, uint64& mainBusBitset)
{
	int32 result = 0;
	mainBusBitset = 0;

	int32 busCount = mComponent->getBusCount (kAudio, dir);
	for (int32 i = 0; i < busCount; i++)
	{
		BusInfo busInfo = {0};
		if (mComponent->getBusInfo (kAudio, dir, i, busInfo) == kResultTrue)
		{
			if (busInfo.busType == kMain)
			{
				result += busInfo.channelCount;
				mainBusBitset |= (uint64 (1) << i);

				mComponent->activateBus (kAudio, dir, i, true);
			}
			else if (busInfo.flags & BusInfo::kDefaultActive)
			{
				mComponent->activateBus (kAudio, dir, i, false);
			}
		}
	}
	return result;
}

//------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
void Vst2Wrapper::processMidiEvent (VstMidiEvent* midiEvent, int32 bus)
{
	Event toAdd = {bus, 0};
	uint8 status = midiEvent->midiData[0] & kStatusMask;
	uint8 channel = midiEvent->midiData[0] & kChannelMask;
	if (channel < 16)
	{
		//--- -----------------------------
		if (status == kNoteOn || status == kNoteOff)
		{
			toAdd.sampleOffset = midiEvent->deltaFrames;
			if (midiEvent->flags & kVstMidiEventIsRealtime)
				toAdd.flags |= Event::kIsLive;

			toAdd.ppqPosition = 0;

			if (status == kNoteOff) // note off
			{
				toAdd.type = Event::kNoteOffEvent;
				toAdd.noteOff.channel = channel;
				toAdd.noteOff.pitch = midiEvent->midiData[1];
				toAdd.noteOff.velocity = (float)midiEvent->noteOffVelocity * kMidiScaler;
				toAdd.noteOff.noteId = -1; // TODO ?
			}
			else if (status == kNoteOn) // note on
			{
				toAdd.type = Event::kNoteOnEvent;
				toAdd.noteOn.channel = channel;
				toAdd.noteOn.pitch = midiEvent->midiData[1];
				toAdd.noteOn.tuning = midiEvent->detune;
				toAdd.noteOn.velocity = (float)midiEvent->midiData[2] * kMidiScaler;
				toAdd.noteOn.length = midiEvent->noteLength;
				toAdd.noteOn.noteId = -1; // TODO ?
			}
			mInputEvents->addEvent (toAdd);
		}
		//--- -----------------------------
		else if (status == kPolyPressure)
		{
			toAdd.type = Vst::Event::kPolyPressureEvent;

			toAdd.sampleOffset = midiEvent->deltaFrames;
			if (midiEvent->flags & kVstMidiEventIsRealtime)
				toAdd.flags |= Event::kIsLive;

			toAdd.ppqPosition = 0;
			toAdd.polyPressure.channel = channel;
			toAdd.polyPressure.pitch = midiEvent->midiData[1] & kDataMask;
			toAdd.polyPressure.pressure = (midiEvent->midiData[2] & kDataMask) * kMidiScaler;
			toAdd.polyPressure.noteId = -1; // TODO ?

			mInputEvents->addEvent (toAdd);
		}
		//--- -----------------------------
		else if (status == kController) // controller
		{
			if (bus < kMaxMidiMappingBusses && mMidiCCMapping[bus][channel])
			{
				ParamID paramID = mMidiCCMapping[bus][channel][midiEvent->midiData[1]];
				if (paramID != kNoParamId)
				{
					ParamValue value = (double)midiEvent->midiData[2] * kMidiScaler;

					int32 index = 0;
					IParamValueQueue* queue = mInputChanges.addParameterData (paramID, index);
					if (queue)
					{
						queue->addPoint (midiEvent->deltaFrames, value, index);
					}
					mGuiTransfer.addChange (paramID, value, midiEvent->deltaFrames);
				}
			}
		}
		//--- -----------------------------
		else if (status == kPitchBendStatus)
		{
			if (bus < kMaxMidiMappingBusses && mMidiCCMapping[bus][channel])
			{
				ParamID paramID = mMidiCCMapping[bus][channel][Vst::kPitchBend];
				if (paramID != kNoParamId)
				{
					const double kPitchWheelScaler = 1. / (double)0x3FFF;

					const int32 ctrl = (midiEvent->midiData[1] & kDataMask) |
					                   (midiEvent->midiData[2] & kDataMask) << 7;
					ParamValue value = kPitchWheelScaler * (double)ctrl;

					int32 index = 0;
					IParamValueQueue* queue = mInputChanges.addParameterData (paramID, index);
					if (queue)
					{
						queue->addPoint (midiEvent->deltaFrames, value, index);
					}
					mGuiTransfer.addChange (paramID, value, midiEvent->deltaFrames);
				}
			}
		}
		//--- -----------------------------
		else if (status == kAfterTouchStatus)
		{
			if (bus < kMaxMidiMappingBusses && mMidiCCMapping[bus][channel])
			{
				ParamID paramID = mMidiCCMapping[bus][channel][Vst::kAfterTouch];
				if (paramID != kNoParamId)
				{
					ParamValue value =
					    (ParamValue) (midiEvent->midiData[1] & kDataMask) * kMidiScaler;

					int32 index = 0;
					IParamValueQueue* queue = mInputChanges.addParameterData (paramID, index);
					if (queue)
					{
						queue->addPoint (midiEvent->deltaFrames, value, index);
					}
					mGuiTransfer.addChange (paramID, value, midiEvent->deltaFrames);
				}
			}
		}
		//--- -----------------------------
		else if (status == kProgramChangeStatus)
		{
			if (mProgramChangeParameterIDs[channel] != kNoParamId &&
			    mProgramChangeParameterIdxs[channel] != -1)
			{
				ParameterInfo paramInfo = {0};
				if (mController->getParameterInfo (mProgramChangeParameterIdxs[channel],
				                                   paramInfo) == kResultTrue)
				{
					int32 program = midiEvent->midiData[1];
					if (paramInfo.stepCount > 0 && program <= paramInfo.stepCount)
					{
						ParamValue normalized =
						    (ParamValue)program / (ParamValue)paramInfo.stepCount;
						addParameterChange (mProgramChangeParameterIDs[channel], normalized,
						                    midiEvent->deltaFrames);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
VstInt32 Vst2Wrapper::processEvents (VstEvents* events)
{
	if (mInputEvents == 0)
		return 0;
	mInputEvents->clear ();

	for (int32 i = 0; i < events->numEvents; i++)
	{
		VstEvent* e = events->events[i];
		if (e->type == kVstMidiType)
		{
			VstMidiEvent* midiEvent = (VstMidiEvent*)e;
			processMidiEvent (midiEvent, 0);
		}
		//--- -----------------------------
		else if (e->type == kVstSysExType)
		{
			Event toAdd = {0};
			VstMidiSysexEvent& src = *(VstMidiSysexEvent*)e;
			toAdd.type = Event::kDataEvent;
			toAdd.sampleOffset = e->deltaFrames;
			toAdd.data.type = DataEvent::kMidiSysEx;
			toAdd.data.size = src.dumpBytes;
			toAdd.data.bytes = (uint8*)src.sysexDump;
			mInputEvents->addEvent (toAdd);
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
inline void Vst2Wrapper::processOutputEvents ()
{
	if (mVst2OutputEvents && mOutputEvents && mOutputEvents->getEventCount () > 0)
	{
		mVst2OutputEvents->flush ();

		Event e = {0};
		for (int32 i = 0, total = mOutputEvents->getEventCount (); i < total; i++)
		{
			if (mOutputEvents->getEvent (i, e) != kResultOk)
				break;

			//---SysExclusif----------------
			if (e.type == Event::kDataEvent && e.data.type == DataEvent::kMidiSysEx)
			{
				VstMidiSysexEvent sysexEvent = {0};
				sysexEvent.deltaFrames = e.sampleOffset;
				sysexEvent.dumpBytes = e.data.size;
				sysexEvent.sysexDump = (char*)e.data.bytes;

				if (!mVst2OutputEvents->add (sysexEvent))
					break;
			}
			else
			{
				VstMidiEvent midiEvent = {0};
				midiEvent.deltaFrames = e.sampleOffset;
				if (e.flags & Event::kIsLive)
					midiEvent.flags = kVstMidiEventIsRealtime;
				char* midiData = midiEvent.midiData;

				switch (e.type)
				{
					//--- ---------------------
					case Event::kNoteOnEvent:
						midiData[0] = (char)(kNoteOn | (e.noteOn.channel & kChannelMask));
						midiData[1] = (char)(e.noteOn.pitch & kDataMask);
						midiData[2] =
						    (char)((int32) (e.noteOn.velocity * 127.f + 0.4999999f) & kDataMask);
						if (midiData[2] == 0) // zero velocity => note off
							midiData[0] = (char)(kNoteOff | (e.noteOn.channel & kChannelMask));
						midiEvent.detune = (char)e.noteOn.tuning;
						midiEvent.noteLength = e.noteOn.length;
						break;

					//--- ---------------------
					case Event::kNoteOffEvent:
						midiData[0] = (char)(kNoteOff | (e.noteOff.channel & kChannelMask));
						midiData[1] = (char)(e.noteOff.pitch & kDataMask);
						midiData[2] = midiEvent.noteOffVelocity =
						    (char)((int32) (e.noteOff.velocity * 127.f + 0.4999999f) & kDataMask);
						break;

						break;
				}

				if (!mVst2OutputEvents->add (midiEvent))
					break;
			}
		}

		mOutputEvents->clear ();

		sendVstEventsToHost (*mVst2OutputEvents);
	}
}

//-----------------------------------------------------------------------------
inline void Vst2Wrapper::setupProcessTimeInfo ()
{
	VstTimeInfo* vst2timeInfo = getTimeInfo (0xffffffff);
	if (vst2timeInfo)
	{
		const uint32 portableFlags =
		    ProcessContext::kPlaying | ProcessContext::kCycleActive | ProcessContext::kRecording |
		    ProcessContext::kSystemTimeValid | ProcessContext::kProjectTimeMusicValid |
		    ProcessContext::kBarPositionValid | ProcessContext::kCycleValid |
		    ProcessContext::kTempoValid | ProcessContext::kTimeSigValid |
		    ProcessContext::kSmpteValid | ProcessContext::kClockValid;

		mProcessContext.state = ((uint32)vst2timeInfo->flags) & portableFlags;
		mProcessContext.sampleRate = vst2timeInfo->sampleRate;
		mProcessContext.projectTimeSamples = (TSamples)vst2timeInfo->samplePos;

		if (mProcessContext.state & ProcessContext::kSystemTimeValid)
			mProcessContext.systemTime = (TSamples)vst2timeInfo->nanoSeconds;
		else
			mProcessContext.systemTime = 0;

		if (mProcessContext.state & ProcessContext::kProjectTimeMusicValid)
			mProcessContext.projectTimeMusic = vst2timeInfo->ppqPos;
		else
			mProcessContext.projectTimeMusic = 0;

		if (mProcessContext.state & ProcessContext::kBarPositionValid)
			mProcessContext.barPositionMusic = vst2timeInfo->barStartPos;
		else
			mProcessContext.barPositionMusic = 0;

		if (mProcessContext.state & ProcessContext::kCycleValid)
		{
			mProcessContext.cycleStartMusic = vst2timeInfo->cycleStartPos;
			mProcessContext.cycleEndMusic = vst2timeInfo->cycleEndPos;
		}
		else
			mProcessContext.cycleStartMusic = mProcessContext.cycleEndMusic = 0.0;

		if (mProcessContext.state & ProcessContext::kTempoValid)
			mProcessContext.tempo = vst2timeInfo->tempo;
		else
			mProcessContext.tempo = 120.0;

		if (mProcessContext.state & ProcessContext::kTimeSigValid)
		{
			mProcessContext.timeSigNumerator = vst2timeInfo->timeSigNumerator;
			mProcessContext.timeSigDenominator = vst2timeInfo->timeSigDenominator;
		}
		else
			mProcessContext.timeSigNumerator = mProcessContext.timeSigDenominator = 4;

		mProcessContext.frameRate.flags = 0;
		if (mProcessContext.state & ProcessContext::kSmpteValid)
		{
			mProcessContext.smpteOffsetSubframes = vst2timeInfo->smpteOffset;
			switch (vst2timeInfo->smpteFrameRate)
			{
				case kVstSmpte24fps: ///< 24 fps
					mProcessContext.frameRate.framesPerSecond = 24;
					break;
				case kVstSmpte25fps: ///< 25 fps
					mProcessContext.frameRate.framesPerSecond = 25;
					break;
				case kVstSmpte2997fps: ///< 29.97 fps
					mProcessContext.frameRate.framesPerSecond = 30;
					mProcessContext.frameRate.flags = FrameRate::kPullDownRate;
					break;
				case kVstSmpte30fps: ///< 30 fps
					mProcessContext.frameRate.framesPerSecond = 30;
					break;
				case kVstSmpte2997dfps: ///< 29.97 drop
					mProcessContext.frameRate.framesPerSecond = 30;
					mProcessContext.frameRate.flags =
					    FrameRate::kPullDownRate | FrameRate::kDropRate;
					break;
				case kVstSmpte30dfps: ///< 30 drop
					mProcessContext.frameRate.framesPerSecond = 30;
					mProcessContext.frameRate.flags = FrameRate::kDropRate;
					break;
				case kVstSmpteFilm16mm: // not a smpte rate
				case kVstSmpteFilm35mm:
					mProcessContext.state &= ~ProcessContext::kSmpteValid;
					break;
				case kVstSmpte239fps: ///< 23.9 fps
					mProcessContext.frameRate.framesPerSecond = 24;
					mProcessContext.frameRate.flags = FrameRate::kPullDownRate;
					break;
				case kVstSmpte249fps: ///< 24.9 fps
					mProcessContext.frameRate.framesPerSecond = 25;
					mProcessContext.frameRate.flags = FrameRate::kPullDownRate;
					break;
				case kVstSmpte599fps: ///< 59.9 fps
					mProcessContext.frameRate.framesPerSecond = 60;
					mProcessContext.frameRate.flags = FrameRate::kPullDownRate;
					break;
				case kVstSmpte60fps: ///< 60 fps
					mProcessContext.frameRate.framesPerSecond = 60;
					break;
				default: mProcessContext.state &= ~ProcessContext::kSmpteValid; break;
			}
		}
		else
		{
			mProcessContext.smpteOffsetSubframes = 0;
			mProcessContext.frameRate.framesPerSecond = 0;
		}

		///< MIDI Clock Resolution (24 Per Quarter Note), can be negative (nearest)
		if (mProcessContext.state & ProcessContext::kClockValid)
			mProcessContext.samplesToNextClock = vst2timeInfo->samplesToNextClock;
		else
			mProcessContext.samplesToNextClock = 0;

		mProcessData.processContext = &mProcessContext;
	}
	else
		mProcessData.processContext = nullptr;
}

//-----------------------------------------------------------------------------
template <class T>
inline void Vst2Wrapper::setProcessingBuffers (T** inputs, T** outputs)
{
	// set processing buffers
	int32 sourceIndex = 0;
	for (int32 i = 0; i < mProcessData.numInputs; i++)
	{
		AudioBusBuffers& buffers = mProcessData.inputs[i];
		if (mMainAudioInputBuses & (uint64 (1) << i))
		{
			for (int32 j = 0; j < buffers.numChannels; j++)
			{
				buffers.channelBuffers32[j] = (Sample32*)inputs[sourceIndex++];
			}
		}
		else
			buffers.silenceFlags = HostProcessData::kAllChannelsSilent;
	}

	sourceIndex = 0;
	for (int32 i = 0; i < mProcessData.numOutputs; i++)
	{
		AudioBusBuffers& buffers = mProcessData.outputs[i];
		buffers.silenceFlags = 0;
		if (mMainAudioOutputBuses & (uint64 (1) << i))
		{
			for (int32 j = 0; j < buffers.numChannels; j++)
			{
				buffers.channelBuffers32[j] = (Sample32*)outputs[sourceIndex++];
			}
		}
	}
}

//-----------------------------------------------------------------------------
inline void Vst2Wrapper::setEventPPQPositions ()
{
	if (!mInputEvents)
		return;

	int32 eventCount = mInputEvents->getEventCount ();
	if (eventCount > 0 && (mProcessContext.state & ProcessContext::kTempoValid) &&
	    (mProcessContext.state & ProcessContext::kProjectTimeMusicValid))
	{
		TQuarterNotes projectTimeMusic = mProcessContext.projectTimeMusic;
		double secondsToQuarterNoteScaler = mProcessContext.tempo / 60.0;
		double multiplicator = secondsToQuarterNoteScaler / this->sampleRate;

		for (int32 i = 0; i < eventCount; i++)
		{
			Event* e = mInputEvents->getEventByIndex (i);
			if (e)
			{
				TQuarterNotes localTimeMusic = e->sampleOffset * multiplicator;
				e->ppqPosition = projectTimeMusic + localTimeMusic;
			}
		}
	}
}

//-----------------------------------------------------------------------------
inline void Vst2Wrapper::doProcess (VstInt32 sampleFrames)
{
	if (!mProcessor)
		return;

	mProcessData.numSamples = sampleFrames;

	if (processing == false)
		startProcess ();

	mProcessData.inputEvents = mInputEvents;
	mProcessData.outputEvents = mOutputEvents;

	setupProcessTimeInfo ();
	setEventPPQPositions ();

	mInputTransfer.transferChangesTo (mInputChanges);

	mProcessData.inputParameterChanges = &mInputChanges;
	mProcessData.outputParameterChanges = &mOutputChanges;
	mOutputChanges.clearQueue ();

	// VST 3 process call
	mProcessor->process (mProcessData);

	mOutputTransfer.transferChangesFrom (mOutputChanges);
	processOutputEvents ();

	// clear input parameters and events
	mInputChanges.clearQueue ();
	if (mInputEvents)
		mInputEvents->clear ();
}

//-----------------------------------------------------------------------------
void Vst2Wrapper::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	if (mProcessData.symbolicSampleSize != kSample32)
		return;

	setProcessingBuffers<float> (inputs, outputs);
	doProcess (sampleFrames);
}

//-----------------------------------------------------------------------------
void Vst2Wrapper::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
	if (mProcessData.symbolicSampleSize != kSample64)
		return;

	setProcessingBuffers<double> (inputs, outputs);
	doProcess (sampleFrames);
}

//-----------------------------------------------------------------------------
void Vst2Wrapper::onTimer (Timer*)
{
	if (!mController)
		return;

	ParamID id;
	ParamValue value;
	int32 sampleOffset;

	while (mOutputTransfer.getNextChange (id, value, sampleOffset))
	{
		mController->setParamNormalized (id, value);
	}
	while (mGuiTransfer.getNextChange (id, value, sampleOffset))
	{
		mController->setParamNormalized (id, value);
	}
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
AudioEffect* Vst2Wrapper::create (IPluginFactory* factory, const TUID vst3ComponentID,
                                  VstInt32 vst2ID, audioMasterCallback audioMaster)
{
	if (!factory)
		return 0;

	IAudioProcessor* processor = nullptr;
	FReleaser factoryReleaser (factory);

	factory->createInstance (vst3ComponentID, IAudioProcessor::iid, (void**)&processor);
	if (processor)
	{
		IEditController* controller = nullptr;
		if (processor->queryInterface (IEditController::iid, (void**)&controller) != kResultTrue)
		{
			FUnknownPtr<IComponent> component (processor);
			if (component)
			{
				FUID editorCID;
				if (component->getControllerClassId (editorCID) == kResultTrue)
				{
					factory->createInstance (editorCID, IEditController::iid, (void**)&controller);
				}
			}
		}

		Vst2Wrapper* wrapper =
		    new Vst2Wrapper (processor, controller, audioMaster, vst3ComponentID, vst2ID, factory);
		if (wrapper->init () == false)
		{
			delete wrapper;
			return 0;
		}

		FUnknownPtr<IPluginFactory2> factory2 (factory);
		if (factory2)
		{
			PFactoryInfo factoryInfo;
			if (factory2->getFactoryInfo (&factoryInfo) == kResultTrue)
				wrapper->setVendorName (factoryInfo.vendor);

			for (int32 i = 0; i < factory2->countClasses (); i++)
			{
				Steinberg::PClassInfo2 classInfo2;
				if (factory2->getClassInfo2 (i, &classInfo2) == Steinberg::kResultTrue)
				{
					if (memcmp (classInfo2.cid, vst3ComponentID, sizeof (TUID)) == 0)
					{
						wrapper->setSubCategories (classInfo2.subCategories);
						wrapper->setEffectName (classInfo2.name);
						wrapper->setEffectVersion (classInfo2.version);

						if (classInfo2.vendor[0] != 0)
							wrapper->setVendorName (classInfo2.vendor);

						break;
					}
				}
			}
		}

		return wrapper;
	}

	return 0;
}

// FUnknown
//-----------------------------------------------------------------------------
tresult PLUGIN_API Vst2Wrapper::queryInterface (const char* iid, void** obj)
{
	QUERY_INTERFACE (iid, obj, FUnknown::iid, Vst::IHostApplication)
	QUERY_INTERFACE (iid, obj, Vst::IHostApplication::iid, Vst::IHostApplication)
	QUERY_INTERFACE (iid, obj, Vst::IComponentHandler::iid, Vst::IComponentHandler)
	QUERY_INTERFACE (iid, obj, Vst::IUnitHandler::iid, Vst::IUnitHandler)

	// we are a VST 3 to VST 2 Wrapper
	QUERY_INTERFACE (iid, obj, Vst::IVst3ToVst2Wrapper::iid, Vst::IVst3ToVst2Wrapper)

	*obj = 0;
	return kNoInterface;
}

//-----------------------------------------------------------------------------
// IHostApplication
//-----------------------------------------------------------------------------
tresult PLUGIN_API Vst2Wrapper::createInstance (TUID cid, TUID iid, void** obj)
{
	FUID classID (cid);
	FUID interfaceID (iid);
	if (classID == IMessage::iid && interfaceID == IMessage::iid)
	{
		*obj = new HostMessage;
		return kResultTrue;
	}
	else if (classID == IAttributeList::iid && interfaceID == IAttributeList::iid)
	{
		*obj = new HostAttributeList;
		return kResultTrue;
	}
	*obj = 0;
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API Vst2Wrapper::getName (String128 name)
{
	char8 productString[128];
	if (getHostProductString (productString))
	{
		String str (productString);
		str.copyTo16 (name, 0, 127);

		return kResultTrue;
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
// IComponentHandler
//-----------------------------------------------------------------------------
tresult PLUGIN_API Vst2Wrapper::beginEdit (ParamID tag)
{
	std::map<ParamID, int32>::const_iterator iter = mParamIndexMap.find (tag);
	if (iter != mParamIndexMap.end ())
		AudioEffectX::beginEdit ((*iter).second);
	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API Vst2Wrapper::performEdit (ParamID tag, ParamValue valueNormalized)
{
	std::map<ParamID, int32>::const_iterator iter = mParamIndexMap.find (tag);
	if (iter != mParamIndexMap.end () && audioMaster)
		audioMaster (&cEffect, audioMasterAutomate, (*iter).second, 0, 0,
		             (float)valueNormalized); // value is in opt

	mInputTransfer.addChange (tag, valueNormalized, 0);

	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API Vst2Wrapper::endEdit (ParamID tag)
{
	std::map<ParamID, int32>::const_iterator iter = mParamIndexMap.find (tag);
	if (iter != mParamIndexMap.end ())
		AudioEffectX::endEdit ((*iter).second);
	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API Vst2Wrapper::restartComponent (int32 flags)
{
	tresult result = kResultFalse;

	//--- ----------------------
	if (flags & kIoChanged)
	{
		setupBuses ();
		ioChanged ();
		result = kResultTrue;
	}

	//--- ----------------------
	if ((flags & kParamValuesChanged) || (flags & kParamTitlesChanged))
	{
		updateDisplay ();
		result = kResultTrue;
	}

	//--- ----------------------
	if (flags & kLatencyChanged)
	{
		if (mProcessor)
			setInitialDelay (mProcessor->getLatencySamples ());

		ioChanged ();
		result = kResultTrue;
	}

	//--- ----------------------
	if (flags & kMidiCCAssignmentChanged)
	{
		initMidiCtrlerAssignment ();
		result = kResultTrue;
	}

	// kReloadComponent is Not supported in VST 2

	return result;
}

//-----------------------------------------------------------------------------
// IUnitHandler
//-----------------------------------------------------------------------------
tresult PLUGIN_API Vst2Wrapper::notifyUnitSelection (UnitID unitId)
{
	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API Vst2Wrapper::notifyProgramListChange (ProgramListID listId, int32 programIndex)
{
	// TODO -> redirect to hasMidiProgramsChanged somehow...
	return kResultTrue;
}

//-----------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg

extern bool InitModule ();

//-----------------------------------------------------------------------------
extern "C" {

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define VST_EXPORT __attribute__ ((visibility ("default")))
#elif WINDOWS
#define VST_EXPORT __declspec( dllexport )
#else
#define VST_EXPORT
#endif

//-----------------------------------------------------------------------------
/** Prototype of the export function main */
//-----------------------------------------------------------------------------
VST_EXPORT AEffect* VSTPluginMain (audioMasterCallback audioMaster)
{
	// Get VST Version of the Host
	if (!audioMaster (0, audioMasterVersion, 0, 0, 0, 0))
		return 0; // old version

	if (InitModule () == false)
		return 0;

	// Create the AudioEffect
	AudioEffect* effect = createEffectInstance (audioMaster);
	if (!effect)
		return 0;

	// Return the VST AEffect structure
	return effect->getAeffect ();
}

//-----------------------------------------------------------------------------
// support for old hosts not looking for VSTPluginMain
#if (TARGET_API_MAC_CARBON && __ppc__)
VST_EXPORT AEffect* main_macho (audioMasterCallback audioMaster)
{
	return VSTPluginMain (audioMaster);
}
#elif WIN32
VST_EXPORT AEffect* MAIN (audioMasterCallback audioMaster)
{
	return VSTPluginMain (audioMaster);
}
#elif BEOS
VST_EXPORT AEffect* main_plugin (audioMasterCallback audioMaster)
{
	return VSTPluginMain (audioMaster);
}
#endif

} // extern "C"
//-----------------------------------------------------------------------------

/// \endcond
