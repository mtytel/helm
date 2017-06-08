//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/vsttestsuite.cpp
// Created by  : Steinberg, 10/2005
// Description : VST Hosting Utilities
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

#include "vsttestsuite.h"
#include "public.sdk/source/vst/hosting/stringconvert.h"

#include "pluginterfaces/vst/ivstcomponent.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "pluginterfaces/vst/ivstprocesscontext.h"
#include "pluginterfaces/vst/ivstmessage.h"
#include "pluginterfaces/vst/ivstunits.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/vst/ivstnoteexpression.h"
#include "pluginterfaces/vst/vstpresetkeys.h"

#include <cstdio>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

namespace Steinberg {
namespace Vst {

#define NUM_ITERATIONS			20
#define DEFAULT_SAMPLE_RATE		44100
#define DEFAULT_BLOCK_SIZE		64
#define MAX_BLOCK_SIZE			8192
#define BUFFERS_ARE_EQUAL		0
#define NUM_AUDIO_BLOCKS_TO_PROCESS 3
#define CHANNEL_IS_SILENT		1

#define TOUGHTESTS				0

DEF_CLASS_IID (IPlugProvider)

FUnknown* gStandardPluginContext = nullptr;
void setStandardPluginContext (FUnknown* context) { gStandardPluginContext = context; };

//------------------------------------------------------------------------
template<class T> struct ArrayDeleter
{	
	ArrayDeleter (T* array): array (array) {}
	~ArrayDeleter () { if (array) delete[] array; }
	
	T* array;
};


//------------------------------------------------------------------------
// VstTestBase
//------------------------------------------------------------------------
VstTestBase::VstTestBase (IPlugProvider* plugProvider)
: plugProvider (plugProvider)
, vstPlug (nullptr)
, controller (nullptr)
{
	FUNKNOWN_CTOR 

	if (plugProvider)
		plugProvider->addRef ();
}

//------------------------------------------------------------------------
VstTestBase::VstTestBase ()
: plugProvider (nullptr)
, vstPlug (nullptr)
, controller (nullptr)
{
	FUNKNOWN_CTOR 
}

//------------------------------------------------------------------------
VstTestBase::~VstTestBase ()
{
	FUNKNOWN_DTOR

	if (plugProvider)
		plugProvider->release ();
}

//------------------------------------------------------------------------
IMPLEMENT_FUNKNOWN_METHODS (VstTestBase, ITest, ITest::iid)

//------------------------------------------------------------------------
bool VstTestBase::setup ()
{
	if (plugProvider)
	{
		vstPlug = plugProvider->getComponent ();
		controller = plugProvider->getController ();

		if (vstPlug)
		{
			vstPlug->activateBus (kAudio, kInput, 0, true);
			vstPlug->activateBus (kAudio, kOutput, 0, true);
	
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------
bool VstTestBase::teardown ()
{
	if (vstPlug)
	{
		if (vstPlug)
		{
			vstPlug->activateBus (kAudio, kInput, 0, false);
			vstPlug->activateBus (kAudio, kOutput, 0, false);
		}
		plugProvider->releasePlugIn (vstPlug, controller);
	}
	return true;
}

//------------------------------------------------------------------------
// Component Initialize / Terminate 
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// VstTestEnh
//------------------------------------------------------------------------
VstTestEnh::VstTestEnh (IPlugProvider* plugProvider, ProcessSampleSize sampl)
: VstTestBase (plugProvider)
, audioEffect (nullptr)
{
	// process setup defaults
	memset (&processSetup, 0, sizeof (ProcessSetup));

	processSetup.processMode = kRealtime;
	processSetup.symbolicSampleSize = sampl;
	processSetup.maxSamplesPerBlock = kMaxSamplesPerBlock;
	processSetup.sampleRate = kSampleRate;
}

//------------------------------------------------------------------------
VstTestEnh::~VstTestEnh ()
{
}

//------------------------------------------------------------------------
bool VstTestEnh::setup ()
{
	bool res = VstTestBase::setup ();

	if (vstPlug)
	{
		tresult check = vstPlug->queryInterface (IAudioProcessor::iid, (void**)&audioEffect);
		if (check != kResultTrue)
			return false;
	}

	return (res && audioEffect);
}

//------------------------------------------------------------------------
bool VstTestEnh::teardown ()
{
	if (audioEffect)
		audioEffect->release ();
	
	bool res = VstTestBase::teardown ();
	
	return res && audioEffect;
}

//------------------------------------------------------------------------
static void addMessage (ITestResult* testResult, const std::u16string& str)
{
	testResult->addMessage ((const tchar*)str.data ());
}

//------------------------------------------------------------------------
static void addMessage (ITestResult* testResult, const tchar* str)
{
	testResult->addMessage (str);
}

//------------------------------------------------------------------------
static void addErrorMessage (ITestResult* testResult, const tchar* str)
{
	testResult->addErrorMessage (str);
}

//------------------------------------------------------------------------
static void addErrorMessage (ITestResult* testResult, const std::u16string& str)
{
	testResult->addErrorMessage ((const tchar*)str.data ());
}

//------------------------------------------------------------------------
static void printTestHeader (VstTestBase* test, ITestResult* testResult)
{
	using VST3::StringConvert::convert;

	std::string str = "===";
	str += test->getName ();
	str += " ====================================";
	addMessage (testResult, convert (str));
}

//------------------------------------------------------------------------
static std::u16string printf (const char8* format, ...)
{
	using VST3::StringConvert::convert;

	char8 string[1024 * 4];
	
	va_list marker;
	va_start (marker, format);
	
	vsnprintf (string, kPrintfBufferSize, format, marker);
	return convert (string).data ();
}

//------------------------------------------------------------------------
// VstSuspendResumeTest
//------------------------------------------------------------------------
VstSuspendResumeTest::VstSuspendResumeTest (IPlugProvider* plugProvider, ProcessSampleSize sampl)
: VstTestEnh (plugProvider, sampl)
{}

//------------------------------------------------------------------------
bool PLUGIN_API VstSuspendResumeTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult)
		return false;

	printTestHeader (this, testResult);

	for (int32 i = 0; i < 3; ++i)
	{
		if (audioEffect)
		{
			if (audioEffect->canProcessSampleSize (kSample32) == kResultOk)
				processSetup.symbolicSampleSize = kSample32;
			else if (audioEffect->canProcessSampleSize (kSample64) == kResultOk)
				processSetup.symbolicSampleSize = kSample64;
			else
			{
				addErrorMessage (testResult, STR ("No appropriate symbolic sample size supported!"));
				return false;
			}
			
			if (audioEffect->setupProcessing (processSetup) != kResultOk)
			{
				addErrorMessage (testResult, STR ("Process setup failed!"));
				return false;
			}
			
		}
		tresult result = vstPlug->setActive (true);
		if (result != kResultOk)
			return false;

		result = vstPlug->setActive (false);
		if (result != kResultOk)
			return false;
	}
	return true;
}


//------------------------------------------------------------------------
//------------------------------------------------------------------------
// VstTerminateInitializeTest
//------------------------------------------------------------------------
VstTerminateInitializeTest::VstTerminateInitializeTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{}

//------------------------------------------------------------------------
bool VstTerminateInitializeTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult)
		return false;

	printTestHeader (this, testResult);

	bool result = true;
	if (vstPlug->terminate () != kResultTrue)
	{
		addErrorMessage (testResult, STR ("IPluginBase::terminate () failed."));
		result = false;
	}
	if (vstPlug->initialize (gStandardPluginContext) != kResultTrue)
	{
		addErrorMessage (testResult, STR ("IPluginBase::initialize (..) failed."));
		result = false;
	}
	return result;
}

//------------------------------------------------------------------------
// VstScanBussesTest
//------------------------------------------------------------------------
VstScanBussesTest::VstScanBussesTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{}

//------------------------------------------------------------------------
bool PLUGIN_API VstScanBussesTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult)
		return false;

	printTestHeader (this, testResult);

	int32 numBusses = 0;

	for (MediaType mediaType = kAudio; mediaType < kNumMediaTypes; mediaType++)
	{
		int32 numInputs = vstPlug->getBusCount (mediaType, kInput);
		int32 numOutputs = vstPlug->getBusCount (mediaType, kOutput);
		
		numBusses += (numInputs + numOutputs);
		
		if ((mediaType == (kNumMediaTypes - 1)) && (numBusses == 0))
		{
			addErrorMessage (testResult, STR ("This component does not export any buses!!!"));
			return false;
		}

		addMessage (testResult, printf ("=> %s Buses: [%d In(s) => %d Out(s)]", mediaType == kAudio ? "Audio" : "Event", numInputs, numOutputs));

		for (int32 i = 0; i < numInputs + numOutputs; ++i)
		{
			BusDirection busDirection = i < numInputs ? kInput : kOutput;
			int32 busIndex = busDirection == kInput ? i : i - numInputs;

			BusInfo busInfo = {0};
			if (vstPlug->getBusInfo (mediaType, busDirection, busIndex, busInfo) == kResultTrue)
			{
				auto busName = VST3::StringConvert::convert (busInfo.name);;

				if (busName.empty ())
				{
					addErrorMessage (testResult, printf ("Bus %d has no name!!!", busIndex));
					return false;
				}
				addMessage (testResult, printf ("     %s[%d]: \"%s\" (%s-%s) ",
										busDirection == kInput ? "In " : "Out",
										busIndex,
										busName.data(),
										busInfo.busType == kMain ? "Main" : "Aux",
										busInfo.kDefaultActive ? "Default Active" : "Default Inactive"));
			}
			else
				return false;
		}
	}
	return true;
}

//------------------------------------------------------------------------
// VstScanParametersTest
//------------------------------------------------------------------------
VstScanParametersTest::VstScanParametersTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{}

//------------------------------------------------------------------------
bool PLUGIN_API VstScanParametersTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;
	
	printTestHeader (this, testResult);
	
	if (!controller)
	{
		addMessage (testResult, STR ("No Edit Controller supplied!"));
		return true;
	}
		
	int32 numParameters = controller->getParameterCount ();
	if (numParameters <= 0)
	{
		addMessage (testResult, STR ("This component does not export any parameters!"));
		return true;
	}

	addMessage (testResult, printf ("This component exports %d parameter(s)", numParameters));

	FUnknownPtr<IUnitInfo> iUnitInfo2 (controller);
	if (!iUnitInfo2 && numParameters > 20)
	{
		addMessage (testResult, STR ("Note: it could be better to use UnitInfo in order to sort Parameters (>20)."));
	}

	// used for ID check
	int32* paramIds = new int32[numParameters];

	bool foundBypass = false;
	for (int32 i = 0; i < numParameters; ++i)
	{
		ParameterInfo paramInfo = {0};
	
		tresult result = controller->getParameterInfo (i, paramInfo);
		if (result != kResultOk)
		{
			addErrorMessage (testResult, printf ("Param %03d: is missing!!!", i));
			return false;
		}

		int32 paramId = paramInfo.id;
		paramIds[i] = paramId;
		if (paramId < 0)
		{
			addErrorMessage (testResult, printf ("Param %03d: Invalid Id!!!", i));
			return false;			
		}

		// check if ID is already used by another parameter
		for (int32 idIndex = 0; idIndex < i; idIndex++)
		{
			if (paramIds[idIndex] == paramIds[i])
			{
				addErrorMessage (testResult, printf ("Param %03d: ID already used (by %03d)!!!", i, idIndex));
				return false;
			}
		}

		const tchar* paramType = kEmptyString;
		if (paramInfo.stepCount < 0)
		{
			addErrorMessage (testResult, printf ("Param %03d: invalid stepcount!!!", i));
			return false;
		}
		if (paramInfo.stepCount == 0)
			paramType = STR ("Float");
		else if (paramInfo.stepCount == 1)
			paramType = STR ("Toggle");
		else
			paramType = STR ("Discrete");

		
		auto paramTitle = VST3::StringConvert::convert (paramInfo.title);
		auto paramUnits = VST3::StringConvert::convert (paramInfo.units);

		if (paramTitle.empty ())
		{
			addErrorMessage (testResult, printf ("Param %03d: has no title!!!", i));
			return false;
		}

		if (paramInfo.defaultNormalizedValue != -1.f && (paramInfo.defaultNormalizedValue < 0. || paramInfo.defaultNormalizedValue > 1.))
		{
			addErrorMessage (testResult, printf ("Param %03d: defaultValue is not normalized!!!", i));
			return false;
		}

		int32 unitId = paramInfo.unitId;
		if (unitId < -1)
		{
			addErrorMessage (testResult, printf ("Param %03d: No appropriate unit ID!!!", i));
			return false;			
		}
		if (unitId >= -1)
		{
			FUnknownPtr<IUnitInfo> iUnitInfo (controller);
			if (!iUnitInfo && unitId != 0)
			{
				addErrorMessage (testResult, printf ("IUnitInfo interface is missing, but ParameterInfo::unitID is not %03d (kRootUnitId).", kRootUnitId));
				return false;
			}
			else if (iUnitInfo)
			{
				bool found = false;
				int32 uc = iUnitInfo->getUnitCount ();
				for (int32 ui = 0; ui < uc; ++ui)
				{
					UnitInfo uinfo = {0};
					if (iUnitInfo->getUnitInfo (ui, uinfo) != kResultTrue)
					{
						addErrorMessage (testResult, STR ("IUnitInfo::getUnitInfo (..) failed."));
						return false;
					}
					if (uinfo.id == unitId)
						found = true;
				}
				if (!found && unitId != kRootUnitId)
				{
					addErrorMessage (testResult, STR ("Parameter has a UnitID, which isn't defined in IUnitInfo."));
					return false;
				}
			}
		}
		if (((paramInfo.flags & ParameterInfo::kCanAutomate) != 0) && ((paramInfo.flags & ParameterInfo::kIsReadOnly) != 0))
		{
			addErrorMessage (testResult, STR ("Parameter must not be kCanAutomate and kReadOnly at the same time."));
			return false;
		}

		if ((paramInfo.flags & ParameterInfo::kIsBypass) != 0)
		{
			if (!foundBypass)
				foundBypass = true;
			else
			{
				addErrorMessage (testResult, STR ("There can only be one bypass (kIsBypass)."));
				return false;
			}
		}

		addMessage (testResult, printf ("   Param %03d (ID = %d): [title=\"%s\"] [unit=\"%s\"] [type = %s, default = %lf, unit = %d]",
										i, paramId, paramTitle.data (), paramUnits.data (),
										paramType, paramInfo.defaultNormalizedValue, unitId)
								);
	}

	if (foundBypass == false)
	{
		addMessage (testResult, STR ("Warning: No bypass parameter found. Is this intended ?"));
	}

	if (paramIds)
		delete [] paramIds;

	return true;
}

//------------------------------------------------------------------------
// VstMidiMappingTest
//------------------------------------------------------------------------
VstMidiMappingTest::VstMidiMappingTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{}

//------------------------------------------------------------------------
bool PLUGIN_API VstMidiMappingTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;
	
	printTestHeader (this, testResult);
	
	if (!controller)
	{
		addMessage (testResult, STR ("No Edit Controller supplied!"));
		return true;
	}

	FUnknownPtr<IMidiMapping> midiMapping (controller);
	if (!midiMapping)
	{
		addMessage (testResult, STR ("No MIDI Mapping interface supplied!"));
		return true;
	}

	int32 numParameters = controller->getParameterCount ();
	int32 eventBusCount = vstPlug->getBusCount (kEvent, kInput);
	bool interruptProcess = false;

	for (int32 bus = 0; bus < eventBusCount + 1; bus++)
	{
		if (interruptProcess)
			break;

		BusInfo info;
		if (vstPlug->getBusInfo (kEvent, kInput, bus, info) == kResultTrue)
		{
			if (bus >= eventBusCount)
			{
				addMessage (testResult, STR ("getBusInfo supplied for an unknown event bus"));
				break;
			}
		}
		else
			break;

		for (int16 channel = 0; channel < info.channelCount; channel++)
		{
			if (interruptProcess)
				break;

			int32 foundCount = 0;
			for (CtrlNumber cc = 0; cc < kCountCtrlNumber + 1; cc++)
			{
				ParamID tag;
				if (midiMapping->getMidiControllerAssignment (bus, channel, cc, tag) == kResultTrue)
				{
					if (bus >= eventBusCount)
					{
						addMessage (testResult, STR ("MIDI Mapping supplied for an unknown event bus"));
						interruptProcess = true;
						break;
					}
					if (cc >= kCountCtrlNumber)
					{
						addMessage (testResult, STR ("MIDI Mapping supplied for an wrong ControllerNumbers value (bigger than the max)"));
						break;
					}

					bool foundParameter = false;
					for (int32 i = 0; i < numParameters; ++i)
					{
						ParameterInfo info;
						if (controller->getParameterInfo (i, info) == kResultTrue)
						{
							if (info.id == tag)
							{
								foundParameter = true;
								break;
							}
						}
					}
					if (!foundParameter)
					{
						addErrorMessage (testResult, printf ("Unknown ParamID [%d] returned for MIDI Mapping", tag));
						return false;
					}
					foundCount++;
				}
				else
				{
					if (bus >= eventBusCount)
						interruptProcess = true;
				}
			}
			if (foundCount == 0 && (bus < eventBusCount))
			{
				addMessage (testResult, printf ("MIDI Mapping getMidiControllerAssignment (%d, %d) : no assignment available!", bus, channel));
			}
		}
	}

	return true;
}

//------------------------------------------------------------------------
// VstNoteExpressionTest
//------------------------------------------------------------------------
VstNoteExpressionTest::VstNoteExpressionTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{}

//------------------------------------------------------------------------
bool PLUGIN_API VstNoteExpressionTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;

	printTestHeader (this, testResult);

	if (!controller)
	{
		addMessage (testResult, STR ("No Edit Controller supplied!"));
		return true;
	}

	FUnknownPtr<INoteExpressionController> noteExpression (controller);
	if (!noteExpression)
	{
		addMessage (testResult, STR ("No Note Expression interface supplied!"));
		return true;
	}

	int32 eventBusCount = vstPlug->getBusCount (kEvent, kInput);

	for (int32 bus = 0; bus < eventBusCount; bus++)
	{
		BusInfo busInfo;
		vstPlug->getBusInfo (kEvent, kInput, bus, busInfo);
		
		for (int16 channel = 0; channel < busInfo.channelCount; channel++)
		{
			int32 count = noteExpression->getNoteExpressionCount (bus, channel);
			if (count > 0)
			{
				addMessage (testResult, printf ("Note Expression count bus[%d], channel[%d]: %d", bus, channel, count));
			}
			
			for (int32 i = 0; i < count; ++i)
			{
				NoteExpressionTypeInfo info;
				if (noteExpression->getNoteExpressionInfo (bus, channel, i, info) == kResultTrue)
				{
					addMessage (testResult, printf ("Note Expression TypeID: %d [%s]", info.typeId, VST3::StringConvert::convert (info.title).data ()));
					NoteExpressionTypeID id = info.typeId;
					NoteExpressionValue valueNormalized = info.valueDesc.defaultValue;
					String128 string;
					if (noteExpression->getNoteExpressionStringByValue (bus, channel, id, valueNormalized, string) != kResultTrue)
					{
						addMessage (testResult, printf ("Note Expression getNoteExpressionStringByValue (%d, %d, %d) return kResultFalse!", bus, channel, id));
					}

					if (noteExpression->getNoteExpressionValueByString (bus, channel, id, string, valueNormalized) != kResultTrue)
					{
						addMessage (testResult, printf ("Note Expression getNoteExpressionValueByString (%d, %d, %d) return kResultFalse!", bus, channel, id));
					}
				}
				else
				{
					addErrorMessage (testResult, printf ("Note Expression getNoteExpressionInfo (%d, %d, %d) return kResultFalse!", bus, channel, i));
					return false;
				}
			}
		}
	}

	return true;
}



//------------------------------------------------------------------------
// VstKeyswitchTest
//------------------------------------------------------------------------
VstKeyswitchTest::VstKeyswitchTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{}

//------------------------------------------------------------------------
bool PLUGIN_API VstKeyswitchTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;

	printTestHeader (this, testResult);

	if (!controller)
	{
		addMessage (testResult, STR ("No Edit Controller supplied!"));
		return true;
	}

	FUnknownPtr<IKeyswitchController> keyswitch (controller);
	if (!keyswitch)
	{
		addMessage (testResult, STR ("No Keyswitch interface supplied!"));
		return true;
	}

	int32 eventBusCount = vstPlug->getBusCount (kEvent, kInput);

	for (int32 bus = 0; bus < eventBusCount; bus++)
	{
		BusInfo busInfo;
		vstPlug->getBusInfo (kEvent, kInput, bus, busInfo);

		for (int16 channel = 0; channel < busInfo.channelCount; channel++)
		{
			int32 count = keyswitch->getKeyswitchCount (bus, channel);
			
			if (count > 0)
			{
				addMessage (testResult, printf ("Keyswitch support bus[%d], channel[%d]: %d", bus, channel, count));
			}

			for (int32 i = 0; i < count; ++i)
			{
				KeyswitchInfo info;
				if (keyswitch->getKeyswitchInfo (bus, channel, i, info) == kResultTrue)
				{
				}
				else
				{
					addErrorMessage (testResult, printf ("Keyswitch getKeyswitchInfo (%d, %d, %d) return kResultFalse!", bus, channel, i));
					return false;
				}
			}
		}
	}

	return true;
}

//------------------------------------------------------------------------
// VstEditorClassesTest
//------------------------------------------------------------------------
VstEditorClassesTest::VstEditorClassesTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{}

//------------------------------------------------------------------------
bool PLUGIN_API VstEditorClassesTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult)
		return false;

	printTestHeader (this, testResult);

	// no controller is allowed...
	if (FUnknownPtr<IEditController> (vstPlug).getInterface ())
	{
		addMessage (testResult, STR ("Processor and edit controller united."));
		return true;	
	}

	FUID controllerClassUID;
	if (vstPlug->getControllerClassId (controllerClassUID) != kResultOk)
	{
		addMessage (testResult, STR ("This component does not export an edit controller class ID!!!"));
		return true;
	}
	if (controllerClassUID.isValid () == false)
	{
		addErrorMessage (testResult, STR ("The edit controller class has no valid UID!!!"));
		return false;
	}

	addMessage (testResult, STR ("This component has an edit controller class"));

	char8 cidString[50];

	controllerClassUID.toRegistryString (cidString);
	addMessage (testResult, printf ("   Controller CID: %s", cidString));

	return true;
}


//------------------------------------------------------------------------
// VstUnitInfoTest
//------------------------------------------------------------------------
VstUnitInfoTest::VstUnitInfoTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool VstUnitInfoTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult)
		return false;

	printTestHeader (this, testResult);

	FUnknownPtr<IUnitInfo> iUnitInfo (controller);
	if (iUnitInfo)
	{
		int32 unitCount = iUnitInfo->getUnitCount ();
		if (unitCount <= 0)
		{
			addMessage (testResult, STR ("No units found, while controller implements IUnitInfo !!!"));
		}
		else
		{
			addMessage (testResult, printf ("This component has %d unit(s).", unitCount));
		}
		
		int32* unitIds = new int32[unitCount];
		ArrayDeleter<int32> unitIdDeleter (unitIds);

		for (int32 unitIndex = 0; unitIndex < unitCount; unitIndex++)
		{
			UnitInfo unitInfo = {0};

			if (iUnitInfo->getUnitInfo (unitIndex, unitInfo) == kResultOk)
			{
				int32 unitId = unitInfo.id;
				unitIds[unitIndex] = unitId;
				if (unitId < 0)
				{
					addErrorMessage (testResult, printf ("Unit %03d: Invalid ID!", unitIndex));
					return false;
				}

				// check if ID is already used by another unit
				for (int32 idIndex = 0; idIndex < unitIndex; idIndex++)
				{
					if (unitIds[idIndex] == unitIds[unitIndex])
					{
						addErrorMessage (testResult, printf ("Unit %03d: ID already used!!!", unitIndex));
						return false;
					}
				}

				auto unitName = VST3::StringConvert::convert (unitInfo.name);
				if (unitName.empty ())
				{
					addErrorMessage (testResult, printf ("Unit %03d: No name!", unitIndex));
					return false;				
				}

				int32 parentUnitId = unitInfo.parentUnitId;
				if (parentUnitId < -1)
				{
					addErrorMessage (testResult, printf ("Unit %03d: Invalid parent ID!", unitIndex));
					return false;	
				}
				else if (parentUnitId == unitId)
				{
					addErrorMessage (testResult, printf ("Unit %03d: Parent ID is equal to Unit ID!", unitIndex));
					return false;	
				}

				int32 unitProgramListId = unitInfo.programListId;
				if (unitProgramListId < -1)
				{
					addErrorMessage (testResult, printf ("Unit %03d: Invalid programlist ID!", unitIndex));
					return false;	
				}

				addMessage (testResult, printf ("   Unit%03d (ID = %d): \"%s\" (parent ID = %d, programlist ID = %d)",
										unitIndex, unitId, unitName.data (), parentUnitId, unitProgramListId));

				// test select Unit
				if (iUnitInfo->selectUnit (unitIndex) == kResultTrue)
				{
					UnitID newSelected = iUnitInfo->getSelectedUnit ();
					if (newSelected != unitIndex)
					{
						addMessage (testResult, printf ("The host has selected Unit ID = %d but getSelectedUnit returns ID = %d!!!",
												unitIndex, newSelected));
					}
				}
			}
			else
			{
				addMessage (testResult, printf ("Unit%03d: No unit info!", unitIndex));
			}
		}	
	}
	else
	{
		addMessage (testResult, STR ("This component has no units."));
	}

	return true;
}


//------------------------------------------------------------------------
// VstUnitStructureTest
//------------------------------------------------------------------------
VstUnitStructureTest::VstUnitStructureTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool VstUnitStructureTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult)
		return false;

	printTestHeader (this, testResult);

	FUnknownPtr<IUnitInfo> iUnitInfo (controller);
	if (iUnitInfo)
	{
		int32 unitCount = iUnitInfo->getUnitCount ();
		if (unitCount <= 0)
		{
			addMessage (testResult, STR ("No units found, while controller implements IUnitInfo !!!"));
		}

		UnitInfo unitInfo = {0};
		UnitInfo tmpInfo = {0};
		bool rootFound = false;
		for (int32 unitIndex = 0; unitIndex < unitCount; unitIndex++)
		{
			if (iUnitInfo->getUnitInfo (unitIndex, unitInfo) == kResultOk)
			{
				// check parent Id
				if (unitInfo.parentUnitId != kNoParentUnitId) //-1: connected to root
				{
					bool noParent = true;
					for (int32 i = 0; i < unitCount; ++i)
					{
						if (iUnitInfo->getUnitInfo (i, tmpInfo) == kResultOk)
						{
							if (unitInfo.parentUnitId == tmpInfo.id)
							{
								noParent = false;
								break;
							}
						}
					}
					if (noParent && unitInfo.parentUnitId != kRootUnitId)
					{
						addErrorMessage (testResult, printf ("Unit %03d: Parent does not exist!!", unitInfo.id));
						return false;
					}
				}
				else if (!rootFound)
				{
					// root Unit have always the rootID
					if (unitInfo.id != kRootUnitId)
					{
						// we should have a root unit id
						addErrorMessage (testResult, printf ("Unit %03d: Should be the Root Unit => id should be %03d!!", unitInfo.id, kRootUnitId));
						return false;
					}
					rootFound = true;
				}
				else
				{
					addErrorMessage (testResult, printf ("Unit %03d: Has no parent, but there is a root already.", unitInfo.id));
					return false;
				}
			}
			else
			{
				addErrorMessage (testResult, printf ("Unit %03d: No unit info.", unitInfo.id));
				return false;
			}
		}
		addMessage (testResult, STR ("All units have valid parent IDs."));
	}
	else
	{
		addMessage (testResult, STR ("This component does not support IUnitInfo!"));
	}
	return true;
}


//------------------------------------------------------------------------
// VstProgramInfoTest
//------------------------------------------------------------------------
VstProgramInfoTest::VstProgramInfoTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool VstProgramInfoTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult)
		return false;

	printTestHeader (this, testResult);

	FUnknownPtr<IUnitInfo> iUnitInfo (controller);
	if (iUnitInfo)
	{
		int32 programListCount = iUnitInfo->getProgramListCount ();
		if (programListCount == 0)
		{
			addMessage (testResult, STR ("This component does not export any programs."));
			return true;
		}
		else if (programListCount < 0)
		{
			addErrorMessage (testResult, STR ("IUnitInfo::getProgramListCount () returned a negative number."));
			return false;
		}

		// used to check double IDs
		int32* programListIds = new int32 [programListCount];
		ArrayDeleter<int32> idDeleter (programListIds);

		for (int32 programListIndex = 0; programListIndex < programListCount; programListIndex++)
		{
			// get programm list info 
			ProgramListInfo programListInfo;
			if (iUnitInfo->getProgramListInfo (programListIndex, programListInfo) == kResultOk)
			{
				int32 programListId = programListInfo.id;
				programListIds [programListIndex] = programListId;
				if (programListId < 0)
				{
					addErrorMessage (testResult, printf ("Programlist%03d: Invalid ID!!!", programListIndex));
					return false;
				}

				// check if ID is already used by another parameter
				for (int32 idIndex = 0; idIndex < programListIndex; idIndex++)
				{
					if (programListIds[idIndex] == programListIds[programListIndex])
					{
						addErrorMessage (testResult, printf ("Programlist%03d: ID already used!!!", programListIndex));
						return false;
					}
				}

				auto programListName = VST3::StringConvert::convert (programListInfo.name);
				if (programListName.empty ())
				{
					addErrorMessage (testResult, printf ("Programlist%03d (ID = %d): No name!!!", programListIndex, programListId));
					return false;
				}

				int32 programCount = programListInfo.programCount;
				if (programCount <= 0)
				{
					addMessage (testResult, printf ("Programlist%03d (ID = %d): \"%s\" No programs!!! (programCount is null!)", programListIndex, programListId, VST3::StringConvert::convert (programListName).data ()));
					//return false;
				}
				
				addMessage (testResult, printf ("Programlist%03d (ID = %d):  \"%s\" (%d programs).", programListIndex, programListId, programListName.data (), programCount));

				for (int32 programIndex = 0; programIndex < programCount; programIndex++)
				{	
					TChar programName[256];
					if (iUnitInfo->getProgramName (programListId, programIndex, programName) == kResultOk)
					{
						if (programName[0] == 0)
						{
							addErrorMessage (testResult, printf ("Programlist%03d-Program%03d: has no name!!!", programListIndex, programIndex));
							return false;
						}

						auto programNameUTF8 = VST3::StringConvert::convert (programName);
						auto msg = printf ("Programlist%03d-Program%03d: \"%s\"", programListIndex, programIndex, programNameUTF8.data ());
						
						String128 programInfo {};
						if (iUnitInfo->getProgramInfo (programListId, programIndex, PresetAttributes::kInstrument, programInfo) == kResultOk)
						{
							auto programInfoUTF8 = VST3::StringConvert::convert (programInfo);
							msg += VST3::StringConvert::convert (" (instrument = \"");
							msg += (const char16_t*)programInfo;
							msg += VST3::StringConvert::convert ("\")");
						}
					
						addMessage (testResult, msg.data ());

						if (iUnitInfo->hasProgramPitchNames (programListId, programIndex) == kResultOk)
						{
							addMessage (testResult, printf (" => \"%s\": supports PitchNames", programNameUTF8.data ()));

							String128 pitchName = {0};
							for (int16 midiPitch = 0; midiPitch < 128; midiPitch++)
							{
								if (iUnitInfo->getProgramPitchName (programListId, programIndex, midiPitch, pitchName) == kResultOk)
								{
									msg = printf ("   => MIDI Pitch %d => \"", midiPitch);
									msg += (const char16_t*)pitchName;
									msg += VST3::StringConvert::convert ("\"");
									addMessage (testResult, msg.data ());
								}
							}
						}
					}
				}
			}
		}
	}
	else
		addMessage (testResult, STR ("This component does not export any programs."));

	return true;
}

//------------------------------------------------------------------------
// VstValidStateTransitionTest
//------------------------------------------------------------------------
VstValidStateTransitionTest::VstValidStateTransitionTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{}

//------------------------------------------------------------------------
bool PLUGIN_API VstValidStateTransitionTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult)
		return false;

	printTestHeader (this, testResult);

	for (int32 i = 0; i < 3; ++i)
	{
		tresult result = vstPlug->setActive (true);
		if (result != kResultTrue)
			return false;

		result = vstPlug->setActive (false);
		if (result != kResultTrue)
			return false;

		result = vstPlug->terminate ();
		if (result != kResultTrue)
			return false;

		result = vstPlug->initialize (gStandardPluginContext);
		if (result != kResultTrue)
			return false;
	}
	return true;
}

//------------------------------------------------------------------------
// VstInvalidStateTransitionTest
//------------------------------------------------------------------------
VstInvalidStateTransitionTest::VstInvalidStateTransitionTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{}

//------------------------------------------------------------------------
bool PLUGIN_API VstInvalidStateTransitionTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult)
		return false;

	printTestHeader (this, testResult);

	// created
	tresult result = vstPlug->initialize (gStandardPluginContext);
	if (result == kResultFalse)
			return false;

	// initialized
	result = vstPlug->setActive (false);
	if (result == kResultOk)
			return false;

	result = vstPlug->setActive (true);
	if (result == kResultFalse)
			return false;

	// allocated
	result = vstPlug->initialize (gStandardPluginContext);
	if (result == kResultOk)
			return false;

	result = vstPlug->setActive (false);
	if (result == kResultFalse)
			return false;

	// deallocated (initialized)
	result = vstPlug->initialize (gStandardPluginContext);
	if (result == kResultOk)
		return false;

	result = vstPlug->terminate ();
	if (result == kResultFalse)
			return false;

	// terminated (created)
	result = vstPlug->setActive (false);
	if (result == kResultOk)
			return false;

	result = vstPlug->terminate ();
	if (result == kResultOk)
			return false;

	return true;
}

//------------------------------------------------------------------------
// VstRepeatIdenticalStateTransitionTest
//------------------------------------------------------------------------
VstRepeatIdenticalStateTransitionTest::VstRepeatIdenticalStateTransitionTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{}

//------------------------------------------------------------------------
bool VstRepeatIdenticalStateTransitionTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult)
		return false;

	printTestHeader (this, testResult);

	tresult result = vstPlug->initialize (gStandardPluginContext);
	if (result != kResultFalse)
		return false;

	result = vstPlug->setActive (true);
	if (result != kResultOk)
		return false;

	result = vstPlug->setActive (true);
	if (result != kResultFalse)
		return false;

	result = vstPlug->setActive (false);
	if (result != kResultOk)
		return false;

	result = vstPlug->setActive (false);
	if (result == kResultOk)
		return false;

	result = vstPlug->terminate ();
	if (result != kResultOk)
		return false;

	result = vstPlug->terminate ();
	if (result == kResultOk)
		return false;

	result = vstPlug->initialize (gStandardPluginContext);
	if (result != kResultOk)
		return false;

	return true;
}

//------------------------------------------------------------------------
// VstBusConsistencyTest
//------------------------------------------------------------------------
VstBusConsistencyTest::VstBusConsistencyTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API VstBusConsistencyTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult)
		return false;

	printTestHeader (this, testResult);

	bool failed = false;
	int32 numFalseDescQueries = 0;

	for (MediaType mediaType = kAudio; mediaType < kNumMediaTypes; mediaType++)
	{
		for (BusDirection dir = kInput; dir <= kOutput; dir++)
		{
			int32 numBusses = vstPlug->getBusCount (mediaType, dir);	
			if (numBusses > 0)
			{
				BusInfo* busArray = new BusInfo[numBusses];
				if (busArray)
				{
					// get all bus descriptions and save them in an array
					int32 busIndex;
					for (busIndex = 0; busIndex < numBusses; busIndex++)
					{
						memset (&busArray[busIndex], 0, sizeof (BusInfo));
						vstPlug->getBusInfo (mediaType, dir, busIndex, busArray[busIndex]);
					}	
					
					// test by getting descriptions randomly and comparing with saved ones
					int32 randIndex = 0;
					BusInfo info = {0};

					for (busIndex = 0; busIndex <= numBusses * NUM_ITERATIONS; busIndex++)
					{
						randIndex = rand () % (numBusses);

						memset (&info, 0, sizeof (BusInfo));
						
						/*tresult result =*/ vstPlug->getBusInfo (mediaType, dir, randIndex, info);
						if (memcmp ((void*)&busArray[randIndex], (void*)&info, sizeof (BusInfo)) != BUFFERS_ARE_EQUAL)
						{
							failed |= true;
							numFalseDescQueries++;
						}
					}
					delete [] busArray;
				}
			}
		}
	}

	if (numFalseDescQueries > 0)
	{
		addErrorMessage (testResult, printf ("The component returned %i inconsistent buses! (getBusInfo () returns sometime different info for the same bus!", numFalseDescQueries));
	}

	return failed == false;
}

//------------------------------------------------------------------------
// VstBusInvalidIndexTest
//------------------------------------------------------------------------
VstBusInvalidIndexTest::VstBusInvalidIndexTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API VstBusInvalidIndexTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult)
		return false;

	printTestHeader (this, testResult);

	bool failed = false;
	int32 numInvalidDesc = 0;

	for (MediaType mediaType = kAudio; mediaType < kNumMediaTypes; mediaType++)
	{
		int32 numBusses = vstPlug->getBusCount (mediaType, kInput) + vstPlug->getBusCount (mediaType, kOutput);
		for (BusDirection dir = kInput; dir <= kOutput; dir++)
		{
			BusInfo descBefore = {0};
			BusInfo descAfter = {0};
	
			int32 randIndex = 0;

			// todo: rand with negative numbers
			for (int32 i = 0; i <= numBusses * NUM_ITERATIONS; ++i)
			{
				randIndex = rand ();
				if (0 > randIndex || randIndex > numBusses)
				{
					/*tresult result =*/ vstPlug->getBusInfo (mediaType, dir, randIndex, descAfter);
					
					if (memcmp ((void*)&descBefore, (void*)&descAfter, sizeof (BusInfo)) != 0)
					{
						failed |= true;
						numInvalidDesc++;
					}
				}
			}
		}
	}

	if (numInvalidDesc > 0)
	{
		addErrorMessage (testResult, printf ("The component returned %i buses queried with an invalid index!", numInvalidDesc));
	}

	return failed == false;
}

//------------------------------------------------------------------------
// VstProcessTest
//------------------------------------------------------------------------
VstProcessTest::VstProcessTest (IPlugProvider* plugProvider, ProcessSampleSize sampl)
: VstTestEnh (plugProvider, sampl)
{
	processData.numSamples = DEFAULT_BLOCK_SIZE;
	processData.symbolicSampleSize = sampl;

	processSetup.processMode = kRealtime;
	processSetup.symbolicSampleSize = sampl;
	processSetup.maxSamplesPerBlock = MAX_BLOCK_SIZE;
	processSetup.sampleRate = DEFAULT_SAMPLE_RATE;
}

//------------------------------------------------------------------------
bool PLUGIN_API VstProcessTest::setup ()
{
	if (!VstTestEnh::setup ())
		return false;
	if (!vstPlug || !audioEffect)
		return false;
	if (processSetup.symbolicSampleSize != processData.symbolicSampleSize)
		return false;
	if (audioEffect->canProcessSampleSize (processSetup.symbolicSampleSize) != kResultOk)
		return true;	// this fails in run (..)

	prepareProcessing ();

	if (vstPlug->setActive (true) != kResultTrue)
		return false;
	return true;
}

//------------------------------------------------------------------------
bool PLUGIN_API VstProcessTest::run (ITestResult* testResult)
{
	if (!testResult || !audioEffect)
		return false;
	if (processSetup.symbolicSampleSize != processData.symbolicSampleSize)
		return false;
	if (!canProcessSampleSize (testResult))
		return true;

	audioEffect->setProcessing (true);

	for (int32 i = 0; i < NUM_AUDIO_BLOCKS_TO_PROCESS; ++i)
	{
		if (!preProcess (testResult))
			return false;
		tresult result = audioEffect->process (processData);
		if (result != kResultOk)
		{
			addErrorMessage (testResult, STR ("IAudioProcessor::process (..) failed."));
			
			audioEffect->setProcessing (false);
			return false;
		}
		if (!postProcess (testResult))
		{
			audioEffect->setProcessing (false);
			return false;
		}
	}

	audioEffect->setProcessing (false);
	return true;
}

//------------------------------------------------------------------------
bool VstProcessTest::preProcess (ITestResult* testResult)
{
	return true;
}

//------------------------------------------------------------------------
bool VstProcessTest::postProcess (ITestResult* testResult)
{
	return true;
}

//------------------------------------------------------------------------
bool VstProcessTest::canProcessSampleSize (ITestResult* testResult)
{
	if (!testResult || !audioEffect)
		return false;
	if (processSetup.symbolicSampleSize != processData.symbolicSampleSize)
		return false;
	if (audioEffect->canProcessSampleSize (processSetup.symbolicSampleSize) != kResultOk)
	{
		if (processSetup.symbolicSampleSize == kSample32)
			addMessage (testResult, STR ("32bit Audio Processing not supported."));
		else
			addMessage (testResult, STR ("64bit Audio Processing not supported."));
		return false;
	}
	return true;
}

//------------------------------------------------------------------------
bool PLUGIN_API VstProcessTest::teardown ()
{
	unprepareProcessing ();
	if (!vstPlug || (vstPlug->setActive (false) != kResultOk))
		return false;
	return VstTestEnh::teardown ();
}

//------------------------------------------------------------------------
bool VstProcessTest::prepareProcessing ()
{
	if (!vstPlug || !audioEffect)
		return false;

	if (audioEffect->setupProcessing (processSetup) == kResultOk)
	{
		processData.prepare (*vstPlug, 0, processSetup.symbolicSampleSize);

		for (BusDirection dir = kInput; dir <= kOutput; dir++)
		{
			int32 numBusses = vstPlug->getBusCount (kAudio, dir);
			AudioBusBuffers* audioBuffers = dir == kInput ? processData.inputs : processData.outputs; //new AudioBusBuffers [numBusses];
			if (!setupBuffers (numBusses, audioBuffers, dir))
				return false;

			if (dir == kInput)
			{
				processData.numInputs = numBusses;
				processData.inputs = audioBuffers;
			}
			else
			{
				processData.numOutputs = numBusses;
				processData.outputs = audioBuffers;
			}
		}
		return true;
	}
	return false;
}

//------------------------------------------------------------------------
bool VstProcessTest::setupBuffers (int32 numBusses, AudioBusBuffers* audioBuffers, BusDirection dir)
{
	if (((numBusses > 0) && !audioBuffers) || !vstPlug)
		return false;
	for (int32 busIndex = 0; busIndex < numBusses; busIndex++) // buses
	{
		BusInfo busInfo;
		if (vstPlug->getBusInfo (kAudio, dir, busIndex, busInfo) == kResultTrue)
		{
			if (!setupBuffers (audioBuffers[busIndex]))
				return false;

			if ((busInfo.flags & BusInfo::kDefaultActive) != 0)
			{
				for (int32 channelIndex = 0; channelIndex < busInfo.channelCount; channelIndex++) // channels per bus
					audioBuffers[busIndex].silenceFlags |= (uint64)CHANNEL_IS_SILENT << channelIndex;
			}
		}
		else
			return false;
	}
	return true;
}

//------------------------------------------------------------------------
bool VstProcessTest::setupBuffers (AudioBusBuffers& audioBuffers)
{
	if (processSetup.symbolicSampleSize != processData.symbolicSampleSize)
		return false;
	audioBuffers.silenceFlags = 0;
	for (int32 channelIndex = 0; channelIndex < audioBuffers.numChannels; channelIndex++)
	{
		if (processSetup.symbolicSampleSize == kSample32)
		{
			if (audioBuffers.channelBuffers32)
			{
				audioBuffers.channelBuffers32[channelIndex] = new Sample32[processSetup.maxSamplesPerBlock];
				if (audioBuffers.channelBuffers32[channelIndex])
					memset (audioBuffers.channelBuffers32[channelIndex], 0, processSetup.maxSamplesPerBlock * sizeof (Sample32));
				else
					return false;
			}
			else
				return false;
		}
		else if (processSetup.symbolicSampleSize == kSample64)
		{
			if (audioBuffers.channelBuffers64)
			{
				audioBuffers.channelBuffers64[channelIndex] = new Sample64[processSetup.maxSamplesPerBlock];
				if (audioBuffers.channelBuffers64[channelIndex])
					memset (audioBuffers.channelBuffers64[channelIndex], 0, processSetup.maxSamplesPerBlock * sizeof (Sample64));
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	return true;
}

//------------------------------------------------------------------------
bool VstProcessTest::unprepareProcessing ()
{
	bool ret = true;
	ret &= freeBuffers (processData.numInputs, processData.inputs);
	ret &= freeBuffers (processData.numOutputs, processData.outputs);
	processData.unprepare ();
	return ret;
}

//------------------------------------------------------------------------
bool VstProcessTest::freeBuffers (int32 numBuses, AudioBusBuffers* buses)
{
	if (processSetup.symbolicSampleSize != processData.symbolicSampleSize)
		return false;
	for (int32 busIndex = 0; busIndex < numBuses; busIndex++)
	{
		for (int32 channelIndex = 0; channelIndex < buses[busIndex].numChannels; channelIndex++)
		{
			if (processSetup.symbolicSampleSize == kSample32)
				delete [] buses[busIndex].channelBuffers32[channelIndex];
			else if (processSetup.symbolicSampleSize == kSample64)
				delete [] buses[busIndex].channelBuffers64[channelIndex];
			else
				return false;
		}
	}
	return true;
}


//------------------------------------------------------------------------
// VstSpeakerArrangementTest
//------------------------------------------------------------------------
VstSpeakerArrangementTest::VstSpeakerArrangementTest (IPlugProvider* plugProvider, ProcessSampleSize sampl, 
													  SpeakerArrangement inSpArr, SpeakerArrangement outSpArr)
: VstProcessTest (plugProvider, sampl)
, inSpArr (inSpArr)
, outSpArr (outSpArr)
{
}

//------------------------------------------------------------------------
const char* VstSpeakerArrangementTest::getSpeakerArrangementName (SpeakerArrangement spArr)
{
	const char* saName = nullptr;
	switch (spArr)
	{
		case SpeakerArr::kMono:				saName = "Mono"; break;
		case SpeakerArr::kStereo:			saName = "Stereo"; break;
		case SpeakerArr::kStereoSurround:	saName = "StereoSurround"; break;
		case SpeakerArr::kStereoCenter:		saName = "StereoCenter"; break;
		case SpeakerArr::kStereoSide:		saName = "StereoSide"; break;
		case SpeakerArr::kStereoCLfe:		saName = "StereoCLfe"; break;
		case SpeakerArr::k30Cine:			saName = "30Cine"; break;
		case SpeakerArr::k30Music:			saName = "30Music"; break;
		case SpeakerArr::k31Cine:			saName = "31Cine"; break;
		case SpeakerArr::k31Music:			saName = "31Music"; break;
		case SpeakerArr::k40Cine:			saName = "40Cine"; break;
		case SpeakerArr::k40Music:			saName = "40Music"; break;
		case SpeakerArr::k41Cine:			saName = "41Cine"; break;
		case SpeakerArr::k41Music:			saName = "41Music"; break;
		case SpeakerArr::k50:				saName = "50"; break;
		case SpeakerArr::k51:				saName = "51"; break;
		case SpeakerArr::k60Cine:			saName = "60Cine"; break;
		case SpeakerArr::k60Music:			saName = "60Music"; break;
		case SpeakerArr::k61Cine:			saName = "61Cine"; break;
		case SpeakerArr::k61Music:			saName = "61Music"; break;
		case SpeakerArr::k70Cine:			saName = "70Cine"; break;
		case SpeakerArr::k70Music:			saName = "70Music"; break;
		case SpeakerArr::k71Cine:			saName = "71Cine"; break;
		case SpeakerArr::k71Music:			saName = "71Music"; break;
		case SpeakerArr::k80Cine:			saName = "80Cine"; break;
		case SpeakerArr::k80Music:			saName = "80Music"; break;
		case SpeakerArr::k81Cine:			saName = "81Cine"; break;
		case SpeakerArr::k81Music:			saName = "81Music"; break;
		case SpeakerArr::k102:				saName = "102"; break;
		case SpeakerArr::k122:			    saName = "122"; break;
		case SpeakerArr::k80Cube:		    saName = "80Cube"; break;
		case SpeakerArr::kBFormat:			saName = "BFormat"; break;
		case SpeakerArr::k90:		    saName = "9.0"; break;
		case SpeakerArr::k91:		    saName = "9.1"; break;
		case SpeakerArr::k100:		    saName = "10.0"; break;
		case SpeakerArr::k101:		    saName = "10.1"; break;
		case SpeakerArr::k110:		    saName = "11.0"; break;
		case SpeakerArr::k111:		    saName = "11.1"; break;
		case SpeakerArr::k130:		    saName = "13.0"; break;
		case SpeakerArr::k131:		    saName = "13.1"; break;
		case SpeakerArr::kEmpty:			saName = "Empty"; break;
		default:							saName = "Unknown"; break;
	}
	return saName;
}

//------------------------------------------------------------------------
const char* VstSpeakerArrangementTest::getName () const
{
	const auto inSaName = getSpeakerArrangementName (inSpArr);
	const auto outSaName = getSpeakerArrangementName (outSpArr);
	if (inSaName && outSaName)
	{
		static std::string str;
		str = "In: ";
		str += inSaName;
		str += ": ";
		str += std::to_string (SpeakerArr::getChannelCount (inSpArr));
		str += " Channels, Out: ";
		str += outSaName;
		str += ": ";
		str += std::to_string(SpeakerArr::getChannelCount (outSpArr));
		str += " Channels";
		return str.data ();
	}
	return "error";
}

//------------------------------------------------------------------------
bool VstSpeakerArrangementTest::prepareProcessing ()
{
	if (!vstPlug || !audioEffect)
		return false;
	
	bool ret = true;
	int32 is = vstPlug->getBusCount (kAudio, kInput);
	SpeakerArrangement* inSpArrs = new SpeakerArrangement[is];
	for (int32 i = 0; i < is; ++i)
		inSpArrs[i] = inSpArr;

	int32 os = vstPlug->getBusCount (kAudio, kOutput);
	SpeakerArrangement* outSpArrs = new SpeakerArrangement[os];
	for (int32 o = 0; o < os; o++)
		outSpArrs[o] = outSpArr;
	
	if (audioEffect->setBusArrangements (inSpArrs, is, outSpArrs, os) != kResultTrue)
		ret = false;
	
	ret &= VstProcessTest::prepareProcessing ();
	
	delete [] inSpArrs;
	delete [] outSpArrs;
	
	return ret;
}

//------------------------------------------------------------------------
bool VstSpeakerArrangementTest::run (ITestResult* testResult)
{
	if (!testResult || !audioEffect || !vstPlug)
		return false;
	
	printTestHeader (this, testResult);

	SpeakerArrangement spArr = SpeakerArr::kEmpty;
	SpeakerArrangement compareSpArr = SpeakerArr::kEmpty;
	BusDirections bd = kInput;
	BusInfo busInfo = {0};
	int32 count = 0;
	do
	{
		count++;
		int32 numBusses = 0;
		if (bd == kInput)
		{
			numBusses = processData.numInputs;
			compareSpArr = inSpArr;
		}
		else
		{
			numBusses = processData.numOutputs;
			compareSpArr = outSpArr;
		}
		for (int32 i = 0; i < numBusses; ++i)
		{
			if (audioEffect->getBusArrangement (bd, i, spArr) != kResultTrue)
			{
				addErrorMessage (testResult, STR ("IAudioProcessor::getBusArrangement (..) failed."));
				return false;
			}
			if (spArr != compareSpArr)
			{
				addMessage (testResult, printf ("    %s %sSpeakerArrangement is not supported. Plug-in suggests: %s.",
												getSpeakerArrangementName (compareSpArr),
												bd == kInput ? "Input-" : "Output-",
												getSpeakerArrangementName (spArr)));
			}
			if (vstPlug->getBusInfo (kAudio, bd, i, busInfo) != kResultTrue)
			{
				addErrorMessage (testResult, STR ("IComponent::getBusInfo (..) failed."));
				return false;
			}
			if (spArr == compareSpArr && SpeakerArr::getChannelCount (spArr) != busInfo.channelCount)
			{
				addErrorMessage (testResult, STR ("SpeakerArrangement mismatch (BusInfo::channelCount inconsistency)."));
				return false;
			}
		}
		bd = kOutput;
	} while (count < 2);
	
	bool ret = true;
	
	// not a Pb ret &= verifySA (processData.numInputs, processData.inputs, inSpArr, testResult);
	// not a Pb ret &= verifySA (processData.numOutputs, processData.outputs, outSpArr, testResult);
	ret &= VstProcessTest::run (testResult);
	
	return ret;
}

//------------------------------------------------------------------------
bool VstSpeakerArrangementTest::verifySA (int32 numBusses, AudioBusBuffers* buses, SpeakerArrangement spArr, ITestResult* testResult)
{
	if (!testResult || !buses)
		return false;
	for (int32 i = 0; i < numBusses; ++i)
	{
		if (buses[i].numChannels != SpeakerArr::getChannelCount (spArr))
		{
			addErrorMessage (testResult, STR ("ChannelCount is not matching SpeakerArrangement."));
			return false;
		}
	}
	return true;
}

IMPLEMENT_FUNKNOWN_METHODS (ParamChanges, IParamValueQueue, IParamValueQueue::iid)

//------------------------------------------------------------------------
// VstAutomationTest
//------------------------------------------------------------------------
IMPLEMENT_FUNKNOWN_METHODS (VstAutomationTest, IParameterChanges, IParameterChanges::iid)

//------------------------------------------------------------------------
VstAutomationTest::VstAutomationTest (IPlugProvider* plugProvider, ProcessSampleSize sampl,
									  int32 everyNSamples, int32 numParams, bool sampleAccuracy)
: VstProcessTest (plugProvider, sampl)
, bypassId (-1)
, paramChanges (nullptr)
, countParamChanges (0)
, everyNSamples (everyNSamples)
, numParams (numParams)
, sampleAccuracy (sampleAccuracy)
, onceExecuted (false)
{
	FUNKNOWN_CTOR
}

//------------------------------------------------------------------------
VstAutomationTest::~VstAutomationTest ()
{
	FUNKNOWN_DTOR
}

//------------------------------------------------------------------------
const char* VstAutomationTest::getName () const
{
	static std::string text;
	const char* accTxt = "Sample";
	if (!sampleAccuracy)
		accTxt = "Block";
	text = "Accuracy: ";
	text += accTxt;
	if (numParams < 1)
		text += ", All Parameters";
	else
	{
		text += ", ";
		text += std::to_string (numParams);
		text += " Parameters";
	}
	text += ", Change every";
	text += std::to_string (everyNSamples);
	text += " Samples";
	return text.data ();
}

//------------------------------------------------------------------------
bool VstAutomationTest::setup ()
{
	onceExecuted = false;
	if (!VstProcessTest::setup ())
		return false;

	if (!controller)
		return false;
	if ((numParams < 1) || (numParams > controller->getParameterCount ()))
		numParams = controller->getParameterCount ();
	if (audioEffect && (numParams > 0))
	{
		paramChanges = new ParamChanges [numParams];
		ParameterInfo inf = {0};
		for (int32 i = 0; i < numParams; ++i)
		{
			tresult r = controller->getParameterInfo (i, inf);
			if (r != kResultTrue)
				return false;
			if ((inf.flags & inf.kCanAutomate) != 0)
				paramChanges[i].init (inf.id, processSetup.maxSamplesPerBlock);
		}

		for (int32 i = 0; i < controller->getParameterCount (); ++i)
		{
			tresult r = controller->getParameterInfo (i, inf);
			if (r != kResultTrue)
				return false;
			if ((inf.flags & inf.kIsBypass) != 0)
			{
				bypassId = inf.id;
				break;
			}
		}
		return true;
	}
	return numParams == 0;
}

//------------------------------------------------------------------------
bool VstAutomationTest::run (ITestResult* testResult)
{
	printTestHeader (this, testResult);

	if (!testResult)
		return false;
	if (numParams == 0)
		addMessage (testResult, STR ("No Parameters present."));
	bool ret = VstProcessTest::run (testResult);
	return ret;
}

//------------------------------------------------------------------------
bool VstAutomationTest::teardown ()
{
	if (paramChanges)
	{
		delete [] paramChanges;
		paramChanges = 0;
	}
	return VstProcessTest::teardown ();
}

//------------------------------------------------------------------------
bool VstAutomationTest::preProcess (ITestResult* testResult)
{
	if (!testResult)
		return false;
	if (!paramChanges)
		return numParams == 0;
	bool check = true;
	for (int32 i = 0; i < numParams; ++i)
	{
		paramChanges[i].resetPoints ();
		int32 point = 0;
		for (int32 pos = 0; pos < processData.numSamples; pos++)
		{
			bool add = (rand () % everyNSamples) == 0;
			if (!onceExecuted)
			{
				if (pos == 0)
				{
					add = true;
					if (!sampleAccuracy)
						onceExecuted = true;
				}
				else if ((pos == 1) && sampleAccuracy)
				{
					add = true;
					onceExecuted = true;
				}
			}
			if (add)
				check &= paramChanges[i].setPoint (point++, pos, ((float)(rand () % 1000000000)) / 1000000000.0);
		}
		if (check)
			processData.inputParameterChanges = this;
	}
	return check;
}

//------------------------------------------------------------------------
bool VstAutomationTest::postProcess (ITestResult* testResult)
{
	if (!testResult)
		return false;
	if (!paramChanges)
		return numParams == 0;
	
	for (int32 i = 0; i < numParams; ++i)
	{
		if ((paramChanges[i].getPointCount () > 0) && !paramChanges[i].havePointsBeenRead (!sampleAccuracy))
		{
			if (sampleAccuracy)
				addMessage (testResult, STR ("   Not all points have been read via IParameterChanges"));
			else
				addMessage (testResult, STR ("   No point at all has been read via IParameterChanges"));

			return true;// should not be a problem
		}
	}
	return true;
}

//------------------------------------------------------------------------
int32 VstAutomationTest::getParameterCount ()
{
	if (paramChanges)
		return numParams;
	return 0;
}

//------------------------------------------------------------------------
IParamValueQueue* VstAutomationTest::getParameterData (int32 index)
{
	if (paramChanges && (index >= 0) && (index < getParameterCount ()))
		return &paramChanges[index];
	return 0;
}

//------------------------------------------------------------------------
IParamValueQueue* VstAutomationTest::addParameterData (const ParamID& id, int32& index)
{
	return 0;
}

//------------------------------------------------------------------------
// VstFlushParamTest
//------------------------------------------------------------------------
VstFlushParamTest::VstFlushParamTest (IPlugProvider* plugProvider, ProcessSampleSize sampl)
: VstAutomationTest (plugProvider, sampl, 100, 1, false)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API VstFlushParamTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult || !audioEffect)
		return false;
	if (!canProcessSampleSize (testResult))
		return true;

	printTestHeader (this, testResult);

	unprepareProcessing ();

	processData.numSamples = 0;
	processData.numInputs = 0;
	processData.numOutputs = 0;
	processData.inputs = nullptr;
	processData.outputs = nullptr;

	audioEffect->setProcessing (true);
	
	preProcess (testResult);
	
	tresult result = audioEffect->process (processData);
	if (result != kResultOk)
	{
		addErrorMessage (testResult, STR ("The component failed to process without audio buffers!"));

		audioEffect->setProcessing (false);
		return false;
	}
	
	postProcess (testResult);
		
	audioEffect->setProcessing (false);
	return true;
}


//------------------------------------------------------------------------
// VstSilenceFlagsTest
//------------------------------------------------------------------------
VstSilenceFlagsTest::VstSilenceFlagsTest (IPlugProvider* plugProvider, ProcessSampleSize sampl)
: VstProcessTest (plugProvider, sampl)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API VstSilenceFlagsTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult || !audioEffect)
		return false;
	if (!canProcessSampleSize (testResult))
		return true;

	printTestHeader (this, testResult);

	if (processData.inputs != 0)
	{
		audioEffect->setProcessing (true);
	
		for (int32 inputsIndex = 0; inputsIndex < processData.numInputs; inputsIndex++)
		{
			int32 numSilenceFlagsCombinations = (1 << processData.inputs[inputsIndex].numChannels) - 1;
			for (int32 flagCombination = 0; flagCombination <= numSilenceFlagsCombinations; flagCombination++)
			{
				processData.inputs[inputsIndex].silenceFlags = flagCombination;
				tresult result = audioEffect->process (processData);
				if (result != kResultOk)
				{
					addErrorMessage (testResult, printf ("The component failed to process bus %i with silence flag combination %x!", inputsIndex, flagCombination));
					audioEffect->setProcessing (false);
					return false;
				}
			}
		}
	}
	else if (processData.numInputs > 0)
	{
		addErrorMessage (testResult, STR ("ProcessData::inputs are 0 but ProcessData::numInputs are nonzero."));
		return false;
	}
	
	audioEffect->setProcessing (false);
	return true;
}

//------------------------------------------------------------------------
// VstSilenceProcessingTest
//------------------------------------------------------------------------
VstSilenceProcessingTest::VstSilenceProcessingTest (IPlugProvider* plugProvider, ProcessSampleSize sampl)
: VstProcessTest (plugProvider, sampl)
{
}

//------------------------------------------------------------------------
bool VstSilenceProcessingTest::isBufferSilent (void* buffer, int32 numSamples, ProcessSampleSize sampl)
{
	if (sampl == kSample32)
	{
		const float kSilenceThreshold = 0.000132184039f;

		float* floatBuffer = (float*)buffer;
		while (numSamples--)
		{
			if (fabsf (*floatBuffer) > kSilenceThreshold)
				return false;
			floatBuffer++;
		}
	}
	else if (sampl == kSample64)
	{
		const double kSilenceThreshold = 0.000132184039;

		double* floatBuffer = (double*)buffer;
		while (numSamples--)
		{
			if (fabs (*floatBuffer) > kSilenceThreshold)
				return false;
			floatBuffer++;
		}
	}
	return true;
}

//------------------------------------------------------------------------
bool PLUGIN_API VstSilenceProcessingTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult || !audioEffect)
		return false;
	if (!canProcessSampleSize (testResult))
		return true;

	printTestHeader (this, testResult);

	if (processData.inputs != 0)
	{
		audioEffect->setProcessing (true);

		for (int32 busIndex = 0; busIndex < processData.numInputs; busIndex++)
		{
			processData.inputs[busIndex].silenceFlags = 0;
			for (int32 channelIndex = 0; channelIndex < processData.inputs[busIndex].numChannels; channelIndex++)
			{
				processData.inputs[busIndex].silenceFlags |= (uint64)1 << (uint64)channelIndex;
				if (processData.symbolicSampleSize == kSample32)
					memset (processData.inputs[busIndex].channelBuffers32[channelIndex], 0, sizeof (float) * processData.numSamples);
				else if (processData.symbolicSampleSize == kSample64)
					memset (processData.inputs[busIndex].channelBuffers32[channelIndex], 0, sizeof (double) * processData.numSamples);
			}
		}
		
		for (int32 busIndex = 0; busIndex < processData.numOutputs; busIndex++)
		{
			if (processData.numInputs > busIndex)
				processData.outputs[busIndex].silenceFlags = processData.inputs[busIndex].silenceFlags;
			else
			{
				processData.outputs[busIndex].silenceFlags = 0;
				for (int32 channelIndex = 0; channelIndex < processData.inputs[busIndex].numChannels; channelIndex++)
					processData.outputs[busIndex].silenceFlags |= (uint64)1 << (uint64)channelIndex;
			}
		}

		tresult result = audioEffect->process (processData);
		if (result != kResultOk)
		{
			addErrorMessage (testResult, printf ("%s", "The component failed to process!"));

			audioEffect->setProcessing (false);
			return false;
		}
		
		for (int32 busIndex = 0; busIndex < processData.numOutputs; busIndex++)
		{
			for (int32 channelIndex = 0; channelIndex < processData.outputs[busIndex].numChannels; channelIndex++)
			{
				bool channelShouldBeSilent = (processData.outputs[busIndex].silenceFlags & (uint64)1 << (uint64)channelIndex) != 0;
				bool channelIsSilent = isBufferSilent (processData.outputs[busIndex].channelBuffers32[channelIndex], processData.numSamples, processData.symbolicSampleSize);
				if (channelShouldBeSilent != channelIsSilent)
				{
					constexpr auto silentText = STR ("The component reported a wrong silent flag for its output buffer! : output is silent but silenceFlags not set !");
					constexpr auto nonSilentText = STR ("The component reported a wrong silent flag for its output buffer! : silenceFlags is set to silence but output is not silent");
					addMessage (testResult, channelIsSilent ? silentText : nonSilentText);
					break;
				}
			}
		}
		
	}
	else if (processData.numInputs > 0)
	{
		addErrorMessage (testResult, STR ("ProcessData::inputs are 0 but ProcessData::numInputs are nonzero."));
		return false;
	}
	
	audioEffect->setProcessing (false);
	return true;
}

//------------------------------------------------------------------------
// VstVariableBlockSizeTest
//------------------------------------------------------------------------
VstVariableBlockSizeTest::VstVariableBlockSizeTest (IPlugProvider* plugProvider,
													ProcessSampleSize sampl)
: VstProcessTest (plugProvider, sampl)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API VstVariableBlockSizeTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult || !audioEffect)
		return false;
	if (!canProcessSampleSize (testResult))
		return true;

	printTestHeader (this, testResult);

	audioEffect->setProcessing (true);

	for (int32 i = 0; i <= NUM_ITERATIONS; ++i)
	{
		int32 sampleFrames = rand () % processSetup.maxSamplesPerBlock;
		processData.numSamples = sampleFrames;
		if (i == 0)
			processData.numSamples = 0;
	#if TOUGHTESTS
		else if (i == 1)
			processData.numSamples = -50000;
		else if (i == 2)
			processData.numSamples = processSetup.maxSamplesPerBlock * 2;
	#endif
		tresult result = audioEffect->process (processData);
		if ((result != kResultOk) 
		#if TOUGHTESTS
			&& (i > 1)
		#else
			&& (i > 0)
		#endif
			)
		{
			addErrorMessage (testResult, printf ("The component failed to process an audioblock of size %i", sampleFrames));
			audioEffect->setProcessing (false);
			return false;
		}
	}
	
	audioEffect->setProcessing (false);
	return true;
}


//------------------------------------------------------------------------
// VstProcessFormatTest
//------------------------------------------------------------------------
VstProcessFormatTest::VstProcessFormatTest (IPlugProvider* plugProvider, ProcessSampleSize sampl)
: VstProcessTest (plugProvider, sampl)
{}

//------------------------------------------------------------------------
bool PLUGIN_API VstProcessFormatTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult || !audioEffect)
		return false;
	if (!canProcessSampleSize (testResult))
		return true;

	printTestHeader (this, testResult);

	int32 numFails = 0;
	const int32 numRates = 11;
	SampleRate sampleRateFormats [numRates] = {22050., 32000., 44100., 48000., 88200., 96000., 192000.,
		1234.5678, 12345.678, 123456.78, 1234567.8};
  	
	tresult result = vstPlug->setActive (false);
	if (result != kResultOk)
	{
		addErrorMessage (testResult, STR ("IComponent::setActive (false) failed."));
		return false;
	}

	addMessage (testResult, STR ("***Tested Sample Rates***"));

	for (int32 i = 0; i < numRates; ++i)
	{
		processSetup.sampleRate = sampleRateFormats [i];
		result = audioEffect->setupProcessing (processSetup);
		if (result == kResultOk)
		{
			result = vstPlug->setActive (true);
			if (result != kResultOk)
			{
				addErrorMessage (testResult, STR ("IComponent::setActive (true) failed."));
				return false;
			}
		
			audioEffect->setProcessing (true);
			result = audioEffect->process (processData);
			audioEffect->setProcessing (false);

			if (result == kResultOk)
			{
				addMessage (testResult, printf (" %10.10G Hz - processed successfully!", sampleRateFormats [i]));
			}
			else
			{
				numFails++;
				addErrorMessage (testResult, printf (" %10.10G Hz - failed to process!", sampleRateFormats [i]));
			}

			result = vstPlug->setActive (false);
			if (result != kResultOk)
			{
				addErrorMessage (testResult, STR ("IComponent::setActive (false) failed."));
				return false;
			}
		}
		else if (sampleRateFormats[i] > 0.)
		{
			addErrorMessage (testResult, printf ("IAudioProcessor::setupProcessing (..) failed for samplerate %.3f Hz! ", sampleRateFormats [i]));
			//return false;
		}
	}

	result = vstPlug->setActive (true);
	if (result != kResultOk)
		return false;

	return true;
}

//------------------------------------------------------------------------
// VstBusActivationTest
//------------------------------------------------------------------------
VstBusActivationTest::VstBusActivationTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{}

//------------------------------------------------------------------------
bool PLUGIN_API VstBusActivationTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult)
		return false;
	printTestHeader (this, testResult);

	int32 numTotalBusses = 0;
	int32 numFailedActivations = 0;

	for (MediaType type = kAudio; type < kNumMediaTypes; type++)
	{
		int32 numInputs = vstPlug->getBusCount (type, kInput);
		int32 numOutputs = vstPlug->getBusCount (type, kOutput);

		numTotalBusses += (numInputs + numOutputs);

		for (int32 i = 0; i < numInputs + numOutputs; ++i)
		{
			BusDirection busDirection = i < numInputs ? kInput : kOutput;
			int32 busIndex = busDirection == kInput ? i : i - numInputs;
			
			BusInfo busInfo = {0};
			if (vstPlug->getBusInfo (type, busDirection, busIndex, busInfo) != kResultTrue)
			{
				addErrorMessage (testResult, STR ("IComponent::getBusInfo (..) failed."));
				return false;
			}

			addMessage (testResult, printf ("   Bus Activation: %s %s Bus (%d) (%s)",
									busDirection == kInput ? "Input" : "Output",
									type == kAudio ? "Audio" : "Event", busIndex,
									busInfo.busType == kMain ? "kMain" : "kAux"));

			if ((busInfo.flags & BusInfo::kDefaultActive) == false)
			{
				if (vstPlug->activateBus (type, busDirection, busIndex, true) != kResultOk)
					numFailedActivations++;
				if (vstPlug->activateBus (type, busDirection, busIndex, false) != kResultOk)
					numFailedActivations++;
			}
			else if ((busInfo.flags & BusInfo::kDefaultActive) == true)
			{
				if (vstPlug->activateBus (type, busDirection, busIndex, false) != kResultOk)
					numFailedActivations++;
				if (vstPlug->activateBus (type, busDirection, busIndex, true) != kResultOk)
					numFailedActivations++;
			}
		}
	}

	if (numFailedActivations > 0)
		addErrorMessage (testResult, STR ("Bus activation failed."));

	return (numFailedActivations == 0);
}


//------------------------------------------------------------------------
// VstCheckAudioBusArrangementTest
//------------------------------------------------------------------------
VstCheckAudioBusArrangementTest::VstCheckAudioBusArrangementTest (IPlugProvider* plugProvider)
: VstTestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool VstCheckAudioBusArrangementTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult)
		return false;

	printTestHeader (this, testResult);

	int32 numInputs = vstPlug->getBusCount (kAudio, kInput);
	int32 numOutputs = vstPlug->getBusCount (kAudio, kOutput);
	int32 arrangementMismatchs = 0;
	
	FUnknownPtr <IAudioProcessor> audioEffect (vstPlug);
	if (audioEffect)
	{
		for (int32 i = 0; i < numInputs + numOutputs; ++i)
		{
			BusDirection dir = i < numInputs ? kInput : kOutput;
			int32 busIndex = dir == kInput ? i : i - numInputs;
			
			addMessage (testResult, printf ("   Check %s Audio Bus Arrangement (%d)", dir == kInput ? "Input" : "Output", busIndex));

			BusInfo busInfo = {0};
			if (vstPlug->getBusInfo (kAudio, dir, busIndex, busInfo) == kResultTrue)
			{
				SpeakerArrangement arrangement;
				if (audioEffect->getBusArrangement (dir, busIndex, arrangement) == kResultTrue)
				{
					if (busInfo.channelCount != SpeakerArr::getChannelCount (arrangement))
					{
						arrangementMismatchs++;
						addErrorMessage (testResult, STR ("channelCount is inconsistent!"));
					}
				}
				else
				{
					addErrorMessage (testResult, STR ("IAudioProcessor::getBusArrangement (..) failed!"));
					return false;
				}
			}
			else
			{
				addErrorMessage (testResult, STR ("IComponent::getBusInfo (..) failed!"));
				return false;
			}
		}
	}
	return (arrangementMismatchs == 0);
}

//------------------------------------------------------------------------
// VstProcessTailTest
//------------------------------------------------------------------------
VstProcessTailTest::VstProcessTailTest (IPlugProvider* plugProvider, ProcessSampleSize sampl)
: VstProcessTest (plugProvider, sampl)
, mTailSamples (0)
, mInTail (0)
, dataPtrFloat (nullptr)
, dataPtrDouble (nullptr)
, mInSilenceInput (false)
, mDontTest (false)
{
	FUNKNOWN_CTOR
}

//------------------------------------------------------------------------
VstProcessTailTest::~VstProcessTailTest ()
{
	if (dataPtrFloat)
	{
		delete []dataPtrFloat;
		dataPtrFloat = nullptr;
	}
	if (dataPtrDouble)
	{
		delete []dataPtrDouble;
		dataPtrDouble = nullptr;
	}
}

//------------------------------------------------------------------------
bool PLUGIN_API VstProcessTailTest::setup ()
{
	bool result = VstProcessTest::setup ();
	if (result)
	{
		mTailSamples = audioEffect->getTailSamples ();

		std::string subCat (plugProvider->getSubCategories ());
		if (subCat.find ("Generator") != std::string::npos || subCat.find ("Instrument") != std::string::npos)
		{
			mDontTest = true;
		}
	}

	return result;
}

//------------------------------------------------------------------------
bool VstProcessTailTest::preProcess (ITestResult* testResult)
{
	if (!mInSilenceInput)
	{
		if (processSetup.symbolicSampleSize == kSample32)
		{
			if (!dataPtrFloat)
				dataPtrFloat = new float[processData.numSamples];
			float* ptr = dataPtrFloat;
			for (int32 i = 0; i < processData.numSamples ; ++i)
				ptr[i] = (float)(2 * rand () / 32767.0 - 1);
		}
		else
		{
			if (!dataPtrDouble)
				dataPtrDouble = new double[processData.numSamples];
			double* ptr = (double*)dataPtrDouble;
			for (int32 i = 0; i < processData.numSamples ; ++i)
				ptr[i] = (double)(2 * rand () / 32767.0 - 1);
		}
		for (int32 i = 0; i < processData.numOutputs; ++i)
		{
			for (int32 c = 0; c < processData.outputs->numChannels; ++c)
			{
				if (processSetup.symbolicSampleSize == kSample32)
					memset (processData.outputs->channelBuffers32[c], 0, processData.numSamples * sizeof (float));
				else
					memset (processData.outputs->channelBuffers64[c], 0, processData.numSamples * sizeof (double));
			}
		}
		for (int32 i = 0; i < processData.numInputs ; ++i)
		{
			for (int32 c = 0; c < processData.inputs->numChannels; ++c)
			{
				if (processSetup.symbolicSampleSize == kSample32)
					memcpy (processData.inputs->channelBuffers32[c], dataPtrFloat, processData.numSamples * sizeof (float));
				else
					memcpy (processData.inputs->channelBuffers64[c], dataPtrDouble, processData.numSamples * sizeof (double));
			}
		}
	}
	else
	{
		// process with silent buffers
		for (int32 i = 0; i < processData.numOutputs; ++i)
		{
			for (int32 c = 0; c < processData.outputs->numChannels; ++c)
			{
				if (processSetup.symbolicSampleSize == kSample32)
					memset (processData.outputs->channelBuffers32[c], 0, processData.numSamples * sizeof (float));
				else
					memset (processData.outputs->channelBuffers64[c], 0, processData.numSamples * sizeof (double));
			}
		}
		for (int32 i = 0; i < processData.numInputs; ++i)
		{
			for (int32 c = 0; c < processData.inputs->numChannels; ++c)
			{
				if (processSetup.symbolicSampleSize == kSample32)
					memset (processData.inputs->channelBuffers32[c], 0, processData.numSamples * sizeof (float));
				else
					memset (processData.inputs->channelBuffers64[c], 0, processData.numSamples * sizeof (double));
			}
		}
	}

	return true;
}

//------------------------------------------------------------------------
bool VstProcessTailTest::postProcess (ITestResult* testResult)
{
	if (mInSilenceInput)
	{
		// should be silence
		if (mTailSamples < mInTail + processData.numSamples)
		{
			int32 start = mTailSamples > mInTail ? mTailSamples - mInTail : 0;
			int32 end = processData.numSamples;
			
			for (int32 i = 0; i < processData.numOutputs; ++i)
			{
				for (int32 c = 0; c < processData.outputs->numChannels; ++c)
				{
					if (processSetup.symbolicSampleSize == kSample32)
					{
						for (int32 s = start; s < end; ++s)
						{
							if (fabsf (processData.outputs->channelBuffers32[c][s]) >= 1e-7)
							{
								addErrorMessage (testResult, printf ("IAudioProcessor::process (..) generates non silent output for silent input for tail above %d samples.", mTailSamples));
								return false;
							}
						}
					}
					else
					{
						for (int32 s = start; s < end; ++s)
						{
							if (fabs (processData.outputs->channelBuffers64[c][s]) >= 1e-7)
							{
								addErrorMessage (testResult, printf ("IAudioProcessor::process (..) generates non silent output for silent input for tail above %d samples.", mTailSamples));
								return false;
							}
						}
					}
					
				}	
			}
		}
		mInTail += processData.numSamples;
	}
	return true;
}

//------------------------------------------------------------------------
bool PLUGIN_API VstProcessTailTest::run (ITestResult* testResult)
{
	if (!testResult || !audioEffect)
		return false;
	if (processSetup.symbolicSampleSize != processData.symbolicSampleSize)
		return false;
	if (!canProcessSampleSize (testResult))
		return true;

	if (mDontTest)
		return true;

	addMessage (testResult, printf ("===%s == Tail=%d ======================", getName (), mTailSamples));

	audioEffect->setProcessing (true);
	
	// process with signal (noise) and silence
	for (int32 i = 0; i < 20 * NUM_AUDIO_BLOCKS_TO_PROCESS; ++i)
	{
		mInSilenceInput = i > 10;
	
		if (!preProcess (testResult))
			return false;
		tresult result = audioEffect->process (processData);
		if (result != kResultOk)
		{
			addErrorMessage (testResult, STR ("IAudioProcessor::process (..) failed."));

			audioEffect->setProcessing (false);
			return false;
		}
		if (!postProcess (testResult))
		{
			audioEffect->setProcessing (false);
			return false;
		}
	}

	audioEffect->setProcessing (false);
	return true;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
