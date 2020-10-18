/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

/*
  Copyright © 2019 IOhannes m zmölnig

  Provide additional structs & enum-aliases, to allow compilation with JUCE-5.4.1

  The names of the struct-members and enum-aliases have been derived by comparing
  juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp of
  both JUCE-5.3.2 and JUCE-5.4.1
 */

#define JUCE_VSTINTERFACE_H_INCLUDED

using namespace juce;

#if JUCE_MSVC
 #define VSTINTERFACECALL __cdecl
 #pragma pack(push)
 #pragma pack(8)
#elif JUCE_MAC || JUCE_IOS
 #define VSTINTERFACECALL
 #if JUCE_64BIT
  #pragma options align=power
 #else
  #pragma options align=mac68k
 #endif
#else
 #define VSTINTERFACECALL
 #pragma pack(push, 8)
#endif

#define VSTCALLBACK VSTINTERFACECALL

const int32 juceVstInterfaceVersion = 2400;
#define kVstVersion 2400
const int32 juceVstInterfaceIdentifier = 0x56737450;    // The "magic" identifier in the SDK is 'VstP'.

//==============================================================================
/** Structure used for VSTs

    @tags{Audio}
*/
struct VstEffectInterface
{
    int32 interfaceIdentifier;
    pointer_sized_int (VSTINTERFACECALL* dispatchFunction)          (VstEffectInterface*, int32 op, int32 index, pointer_sized_int value, void* ptr, float opt);
    void              (VSTINTERFACECALL* processAudioFunction)      (VstEffectInterface*, float** inputs, float** outputs, int32 numSamples);
    void              (VSTINTERFACECALL* setParameterValueFunction) (VstEffectInterface*, int32 parameterIndex, float value);
    float             (VSTINTERFACECALL* getParameterValueFunction) (VstEffectInterface*, int32 parameterIndex);
    int32 numPrograms;
    int32 numParameters;
    int32 numInputChannels;
    int32 numOutputChannels;
    int32 flags;
    pointer_sized_int hostSpace1;
    pointer_sized_int hostSpace2;
    int32 latency;
    int32 deprecated1;
    int32 deprecated2;
    float deprecated3;
    void* effectPointer;
    void* userPointer;
    int32 plugInIdentifier;
    int32 plugInVersion;
    void (VSTINTERFACECALL* processAudioInplaceFunction)       (VstEffectInterface*, float**  inputs, float**  outputs, int32 numSamples);
    void (VSTINTERFACECALL* processDoubleAudioInplaceFunction) (VstEffectInterface*, double** inputs, double** outputs, int32 numSamples);
    char emptySpace[56];
};

struct AEffect;
typedef pointer_sized_int (VSTINTERFACECALL*AEffectDispatcherProc)(AEffect*, int32 op, int32 index, pointer_sized_int value, void* ptr, float opt);
typedef void  (VSTINTERFACECALL*AEffectProcessProc)(AEffect*, float** inputs, float** outputs, int32 numSamples);
typedef void  (VSTINTERFACECALL*AEffectProcessDoubleProc)(AEffect*, double** inputs, double** outputs, int32 numSamples);
typedef float (VSTINTERFACECALL*AEffectGetParameterProc)(AEffect*, int32 parameterIndex);
typedef void  (VSTINTERFACECALL*AEffectSetParameterProc)(AEffect*, int32 parameterIndex, float value);

struct AEffect
{
    int32 magic;
  AEffectDispatcherProc dispatcher;
  AEffectProcessProc process;
  AEffectSetParameterProc setParameter;
  AEffectGetParameterProc getParameter;
    int32 numPrograms;
    int32 numParams;
    int32 numInputs;
    int32 numOutputs;
    int32 flags;
    pointer_sized_int hostSpace1;
    pointer_sized_int hostSpace2;
    int32 initialDelay;
    int32 deprecated1;
    int32 deprecated2;
    float deprecated3;
    void* object;
    void* userPointer;
    int32 uniqueID;
    int32 version;
  AEffectProcessProc processReplacing;
  AEffectProcessDoubleProc processDoubleReplacing;
    char emptySpace[56];
};


typedef pointer_sized_int (VSTINTERFACECALL* VstHostCallback) (VstEffectInterface*, int32 op, int32 index, pointer_sized_int value, void* ptr, float opt);
typedef pointer_sized_int (VSTINTERFACECALL* audioMasterCallback) (AEffect*, int32 op, int32 index, pointer_sized_int value, void* ptr, float opt);

enum VstEffectInterfaceFlags
{
    vstEffectFlagHasEditor          = 1,
    vstEffectFlagInplaceAudio       = 16,
    vstEffectFlagDataInChunks       = 32,
    vstEffectFlagIsSynth            = 256,
    vstEffectFlagInplaceDoubleAudio = 4096

    , effFlagsHasEditor = vstEffectFlagHasEditor
    , effFlagsNoSoundInStop = 0 // FIXXME
    , effFlagsProgramChunks = vstEffectFlagDataInChunks

    , effFlagsCanReplacing = vstEffectFlagInplaceAudio
    , effFlagsIsSynth = vstEffectFlagIsSynth
    , effFlagsCanDoubleReplacing = vstEffectFlagInplaceDoubleAudio
};

//==============================================================================
enum VstHostToPlugInOpcodes
{
    plugInOpcodeOpen,
    plugInOpcodeClose,
    plugInOpcodeSetCurrentProgram,
    plugInOpcodeGetCurrentProgram,
    plugInOpcodeSetCurrentProgramName,
    plugInOpcodeGetCurrentProgramName,
    plugInOpcodeGetParameterLabel,
    plugInOpcodeGetParameterText,
    plugInOpcodeGetParameterName,
    plugInOpcodeSetSampleRate = plugInOpcodeGetParameterName + 2,
    plugInOpcodeSetBlockSize,
    plugInOpcodeResumeSuspend,
    plugInOpcodeGetEditorBounds,
    plugInOpcodeOpenEditor,
    plugInOpcodeCloseEditor,
    plugInOpcodeDrawEditor,
    plugInOpcodeGetMouse,
    plugInOpcodeEditorIdle = plugInOpcodeGetMouse + 2,
    plugInOpcodeeffEditorTop,
    plugInOpcodeSleepEditor,
    plugInOpcodeIdentify,
    plugInOpcodeGetData,
    plugInOpcodeSetData,
    plugInOpcodePreAudioProcessingEvents,
    plugInOpcodeIsParameterAutomatable,
    plugInOpcodeParameterValueForText,
    plugInOpcodeGetProgramName = plugInOpcodeParameterValueForText + 2,
    plugInOpcodeConnectInput = plugInOpcodeGetProgramName + 2,
    plugInOpcodeConnectOutput,
    plugInOpcodeGetInputPinProperties,
    plugInOpcodeGetOutputPinProperties,
    plugInOpcodeGetPlugInCategory,
    plugInOpcodeSetSpeakerConfiguration = plugInOpcodeGetPlugInCategory + 7,
    plugInOpcodeSetBypass = plugInOpcodeSetSpeakerConfiguration + 2,
    plugInOpcodeGetPlugInName,
    plugInOpcodeGetManufacturerName = plugInOpcodeGetPlugInName + 2,
    plugInOpcodeGetManufacturerProductName,
    plugInOpcodeGetManufacturerVersion,
    plugInOpcodeManufacturerSpecific,
    plugInOpcodeCanPlugInDo,
    plugInOpcodeGetTailSize,
    plugInOpcodeIdle,
    plugInOpcodeKeyboardFocusRequired = plugInOpcodeIdle + 4,
    plugInOpcodeGetVstInterfaceVersion,
    plugInOpcodeGetCurrentMidiProgram = plugInOpcodeGetVstInterfaceVersion + 5,
    plugInOpcodeGetSpeakerArrangement = plugInOpcodeGetCurrentMidiProgram + 6,
    plugInOpcodeNextPlugInUniqueID,
    plugInOpcodeStartProcess,
    plugInOpcodeStopProcess,
    plugInOpcodeSetNumberOfSamplesToProcess,
    plugInOpcodeSetSampleFloatType = plugInOpcodeSetNumberOfSamplesToProcess + 4,
    pluginOpcodeGetNumMidiInputChannels,
    pluginOpcodeGetNumMidiOutputChannels,
    plugInOpcodeMaximum = pluginOpcodeGetNumMidiOutputChannels

    , effOpen = plugInOpcodeOpen
    , effClose = plugInOpcodeClose
    , effSetProgram = plugInOpcodeSetCurrentProgram
    , effGetProgram = plugInOpcodeGetCurrentProgram
    , effSetProgramName = plugInOpcodeSetCurrentProgramName
    , effGetProgramName = plugInOpcodeGetCurrentProgramName
    , effGetParamLabel = plugInOpcodeGetParameterLabel
    , effGetParamDisplay = plugInOpcodeGetParameterText
    , effGetParamName = plugInOpcodeGetParameterName
    , effSetSampleRate = plugInOpcodeSetSampleRate
    , effSetBlockSize = plugInOpcodeSetBlockSize
    , effMainsChanged = plugInOpcodeResumeSuspend
    , effEditGetRect = plugInOpcodeGetEditorBounds
    , effEditOpen = plugInOpcodeOpenEditor
    , effEditClose = plugInOpcodeCloseEditor
    , effIdentify = plugInOpcodeIdentify
    , effGetChunk = plugInOpcodeGetData
    , effSetChunk = plugInOpcodeSetData
    , effProcessEvents = plugInOpcodePreAudioProcessingEvents
    , effCanBeAutomated = plugInOpcodeIsParameterAutomatable
    , effString2Parameter = plugInOpcodeParameterValueForText
    , effGetProgramNameIndexed = plugInOpcodeGetProgramName
    , effGetInputProperties = plugInOpcodeGetInputPinProperties
    , effGetOutputProperties = plugInOpcodeGetOutputPinProperties
    , effGetPlugCategory = plugInOpcodeGetPlugInCategory
    , effSetSpeakerArrangement = plugInOpcodeSetSpeakerConfiguration
    , effSetBypass = plugInOpcodeSetBypass
    , effGetEffectName = plugInOpcodeGetPlugInName
    , effGetProductString = plugInOpcodeGetManufacturerProductName
    , effGetVendorString = plugInOpcodeGetManufacturerName
    , effGetVendorVersion = plugInOpcodeGetManufacturerVersion
    , effVendorSpecific = plugInOpcodeManufacturerSpecific
    , effCanDo = plugInOpcodeCanPlugInDo
    , effGetTailSize = plugInOpcodeGetTailSize
    , effKeysRequired = plugInOpcodeKeyboardFocusRequired
    , effGetVstVersion = plugInOpcodeGetVstInterfaceVersion
    , effGetCurrentMidiProgram = plugInOpcodeGetCurrentMidiProgram
    , effGetSpeakerArrangement = plugInOpcodeGetSpeakerArrangement
    , effSetTotalSampleToProcess = plugInOpcodeSetNumberOfSamplesToProcess
    , effSetProcessPrecision = plugInOpcodeSetSampleFloatType
    , effGetNumMidiInputChannels = pluginOpcodeGetNumMidiInputChannels
    , effGetNumMidiOutputChannels = pluginOpcodeGetNumMidiOutputChannels

    , effConnectInput = plugInOpcodeConnectInput
    , effConnectOutput = plugInOpcodeConnectOutput
    , effEditIdle = plugInOpcodeEditorIdle
    , effIdle = plugInOpcodeIdle
    , effShellGetNextPlugin = plugInOpcodeNextPlugInUniqueID
    , effStartProcess = plugInOpcodeStartProcess
    , effStopProcess = plugInOpcodeStopProcess
};


enum VstPlugInToHostOpcodes
{
    hostOpcodeParameterChanged,
    hostOpcodeVstVersion,
    hostOpcodeCurrentId,
    hostOpcodeIdle,
    hostOpcodePinConnected,
    hostOpcodePlugInWantsMidi = hostOpcodePinConnected + 2,
    hostOpcodeGetTimingInfo,
    hostOpcodePreAudioProcessingEvents,
    hostOpcodeSetTime,
    hostOpcodeTempoAt,
    hostOpcodeGetNumberOfAutomatableParameters,
    hostOpcodeGetParameterInterval,
    hostOpcodeIOModified,
    hostOpcodeNeedsIdle,
    hostOpcodeWindowSize,
    hostOpcodeGetSampleRate,
    hostOpcodeGetBlockSize,
    hostOpcodeGetInputLatency,
    hostOpcodeGetOutputLatency,
    hostOpcodeGetPreviousPlugIn,
    hostOpcodeGetNextPlugIn,
    hostOpcodeWillReplace,
    hostOpcodeGetCurrentAudioProcessingLevel,
    hostOpcodeGetAutomationState,
    hostOpcodeOfflineStart,
    hostOpcodeOfflineReadSource,
    hostOpcodeOfflineWrite,
    hostOpcodeOfflineGetCurrentPass,
    hostOpcodeOfflineGetCurrentMetaPass,
    hostOpcodeSetOutputSampleRate,
    hostOpcodeGetOutputSpeakerConfiguration,
    hostOpcodeGetManufacturerName,
    hostOpcodeGetProductName,
    hostOpcodeGetManufacturerVersion,
    hostOpcodeManufacturerSpecific,
    hostOpcodeSetIcon,
    hostOpcodeCanHostDo,
    hostOpcodeGetLanguage,
    hostOpcodeOpenEditorWindow,
    hostOpcodeCloseEditorWindow,
    hostOpcodeGetDirectory,
    hostOpcodeUpdateView,
    hostOpcodeParameterChangeGestureBegin,
    hostOpcodeParameterChangeGestureEnd

    , audioMasterProcessEvents = hostOpcodePreAudioProcessingEvents
    , audioMasterWantMidi = hostOpcodePlugInWantsMidi
    , audioMasterVendorSpecific = hostOpcodeManufacturerSpecific
    , audioMasterAutomate = hostOpcodeParameterChanged
    , audioMasterBeginEdit = hostOpcodeParameterChangeGestureBegin
    , audioMasterEndEdit = hostOpcodeParameterChangeGestureEnd
    , audioMasterUpdateDisplay = hostOpcodeUpdateView
    , audioMasterIOChanged = hostOpcodeIOModified
    , audioMasterCanDo = hostOpcodeCanHostDo
    , audioMasterGetCurrentProcessLevel = hostOpcodeGetCurrentAudioProcessingLevel
    , audioMasterGetTime = hostOpcodeGetTimingInfo
    , audioMasterSizeWindow = hostOpcodeWindowSize
    , audioMasterVersion = hostOpcodeVstVersion

    , audioMasterCloseWindow = hostOpcodeCloseEditorWindow
    , audioMasterCurrentId = hostOpcodeCurrentId
    , audioMasterGetAutomationState = hostOpcodeGetAutomationState
    , audioMasterGetBlockSize = hostOpcodeGetBlockSize
    , audioMasterGetDirectory = hostOpcodeGetDirectory
    , audioMasterGetInputLatency = hostOpcodeGetInputLatency
    , audioMasterGetLanguage = hostOpcodeGetLanguage
    , audioMasterGetNextPlug = hostOpcodeGetNextPlugIn
    , audioMasterGetNumAutomatableParameters = hostOpcodeGetNumberOfAutomatableParameters
    , audioMasterGetOutputLatency = hostOpcodeGetOutputLatency
    , audioMasterGetOutputSpeakerArrangement = hostOpcodeGetOutputSpeakerConfiguration
    , audioMasterGetParameterQuantization = hostOpcodeGetParameterInterval
    , audioMasterGetPreviousPlug = hostOpcodeGetPreviousPlugIn
    , audioMasterGetProductString = hostOpcodeGetProductName
    , audioMasterGetSampleRate = hostOpcodeGetSampleRate
    , audioMasterGetVendorString = hostOpcodeGetManufacturerName
    , audioMasterGetVendorVersion = hostOpcodeGetManufacturerVersion
    , audioMasterIdle = hostOpcodeIdle
    , audioMasterNeedIdle = hostOpcodeNeedsIdle
    , audioMasterOfflineGetCurrentMetaPass = hostOpcodeOfflineGetCurrentMetaPass
    , audioMasterOfflineGetCurrentPass = hostOpcodeOfflineGetCurrentPass
    , audioMasterOfflineRead = hostOpcodeOfflineReadSource
    , audioMasterOfflineStart = hostOpcodeOfflineStart
    , audioMasterOfflineWrite = hostOpcodeOfflineWrite
    , audioMasterOpenWindow = hostOpcodeOpenEditorWindow
    , audioMasterPinConnected = hostOpcodePinConnected
    , audioMasterSetIcon = hostOpcodeSetIcon
    , audioMasterSetOutputSampleRate = hostOpcodeSetOutputSampleRate
    , audioMasterSetTime = hostOpcodeSetTime
    , audioMasterTempoAt = hostOpcodeTempoAt
    , audioMasterWillReplaceOrAccumulate = hostOpcodeWillReplace

};

//==============================================================================
enum VstProcessingSampleType
{
    vstProcessingSampleTypeFloat,
    vstProcessingSampleTypeDouble

    , kVstProcessPrecision32 = vstProcessingSampleTypeFloat
    , kVstProcessPrecision64 = vstProcessingSampleTypeDouble
};

//==============================================================================
// These names must be identical to the Steinberg SDK so JUCE users can set
// exactly what they want.
typedef enum VstPlugInCategory
{
    kPlugCategUnknown,
    kPlugCategEffect,
    kPlugCategSynth,
    kPlugCategAnalysis,
    kPlugCategMastering,
    kPlugCategSpacializer,
    kPlugCategRoomFx,
    kPlugSurroundFx,
    kPlugCategRestoration,
    kPlugCategOfflineProcess,
    kPlugCategShell,
    kPlugCategGenerator
} VstPlugCategory;

//==============================================================================
/** Structure used for VSTs

    @tags{Audio}
*/
struct VstEditorBounds
{
    int16 upper;
    int16 leftmost;
    int16 lower;
    int16 rightmost;
};
struct ERect {
    int16 top;
    int16 left;
    int16 bottom;
    int16 right;
};

//==============================================================================
enum VstMaxStringLengths
{
    vstMaxNameLength                     = 64,
    vstMaxParameterOrPinLabelLength      = 64,
    vstMaxParameterOrPinShortLabelLength = 8,
    vstMaxCategoryLength                 = 24,
    vstMaxManufacturerStringLength       = 64,
    vstMaxPlugInNameStringLength         = 64

    , kVstMaxLabelLen = vstMaxParameterOrPinLabelLength
    , kVstMaxShortLabelLen = vstMaxParameterOrPinShortLabelLength
    , kVstMaxProductStrLen = vstMaxPlugInNameStringLength
    , kVstMaxVendorStrLen = vstMaxManufacturerStringLength
};

//==============================================================================
/** Structure used for VSTs

    @tags{Audio}
*/
struct VstPinInfo
{
    char text[vstMaxParameterOrPinLabelLength];
    int32 flags;
    int32 configurationType;
    char shortText[vstMaxParameterOrPinShortLabelLength];
    char unused[48];
};
struct VstPinProperties
{
    char label[vstMaxParameterOrPinLabelLength];
    int32 flags;
  int32 arrangementType;
    char shortLabel[vstMaxParameterOrPinShortLabelLength];
    char unused[48];
};

enum VstPinInfoFlags
{
    vstPinInfoFlagIsActive = 1,
    vstPinInfoFlagIsStereo = 2,
    vstPinInfoFlagValid    = 4

    , kVstPinIsActive = vstPinInfoFlagIsActive
    , kVstPinUseSpeaker = vstPinInfoFlagValid
    , kVstPinIsStereo = vstPinInfoFlagIsStereo
};

//==============================================================================
/** Structure used for VSTs

    @tags{Audio}
*/
struct VstEvent
{
    int32 type;
  int32 byteSize; // size;
  int32 deltaFrames; //? sampleOffset;
    int32 flags;
    char content[16];
};

enum VstEventTypes
{
    vstMidiEventType  = 1,
    vstSysExEventType = 6,
    kVstMidiType = vstMidiEventType,
    kVstSysExType = vstSysExEventType
};

/** Structure used for VSTs

    @tags{Audio}
*/
struct VstEvents
{
    int32 numEvents;
    pointer_sized_int future;
    VstEvent* events[2];
};

/** Structure used for VSTs

    @tags{Audio}
*/
struct VstMidiEvent
{
    int32 type;
  int32 byteSize; // size;
  int32 deltaFrames; //? sampleOffset;
    int32 flags;
  int32 noteLength; // noteSampleLength;
  int32 noteOffset; // noteSampleOffset;
    char midiData[4];
  char detune; // tuning;
  char noteOffVelocity; // noteVelocityOff;
    char future1;
    char future2;
};

enum VstMidiEventFlags
{
    vstMidiEventIsRealtime = 1
};

/** Structure used for VSTs

    @tags{Audio}
*/
struct VstSysExEvent
{
    int32 type;
  int32 byteSize; // size;
  int32 deltaFrames; //? offsetSamples;
    int32 flags;
  int32 dumpBytes; // sysExDumpSize;
  pointer_sized_int resvd1; // future1;
  char* sysexDump; // sysExDump;
  pointer_sized_int resvd2; // future2;
};
typedef VstSysExEvent VstMidiSysexEvent;

//==============================================================================
/** Structure used for VSTs

    @tags{Audio}
*/
struct VstTimingInformation
{
    double samplePosition;
    double sampleRate;
    double systemTimeNanoseconds;
    double musicalPosition;
    double tempoBPM;
    double lastBarPosition;
    double loopStartPosition;
    double loopEndPosition;
    int32 timeSignatureNumerator;
    int32 timeSignatureDenominator;
    int32 smpteOffset;
    int32 smpteRate;
    int32 samplesToNearestClock;
    int32 flags;
};
struct VstTimeInfo
{
  double samplePos;
    double sampleRate;
    double systemTimeNanoseconds;
  double ppqPos;
  double tempo;
    double barStartPos;
  double cycleStartPos;
  double cycleEndPos;
  int32 timeSigNumerator;
  int32 timeSigDenominator;
    int32 smpteOffset;
  int32 smpteFrameRate;
    int32 samplesToNearestClock;
    int32 flags;
};

enum VstTimingInformationFlags
{
    vstTimingInfoFlagTransportChanged          = 1,
    vstTimingInfoFlagCurrentlyPlaying          = 2,
    vstTimingInfoFlagLoopActive                = 4,
    vstTimingInfoFlagCurrentlyRecording        = 8,
    vstTimingInfoFlagAutomationWriteModeActive = 64,
    vstTimingInfoFlagAutomationReadModeActive  = 128,
    vstTimingInfoFlagNanosecondsValid          = 256,
    vstTimingInfoFlagMusicalPositionValid      = 512,
    vstTimingInfoFlagTempoValid                = 1024,
    vstTimingInfoFlagLastBarPositionValid      = 2048,
    vstTimingInfoFlagLoopPositionValid         = 4096,
    vstTimingInfoFlagTimeSignatureValid        = 8192,
    vstTimingInfoFlagSmpteValid                = 16384,
    vstTimingInfoFlagNearestClockValid         = 32768

    , kVstTransportPlaying = vstTimingInfoFlagCurrentlyPlaying
    , kVstTransportCycleActive = vstTimingInfoFlagLoopActive
    , kVstTransportRecording = vstTimingInfoFlagCurrentlyRecording
    , kVstPpqPosValid = vstTimingInfoFlagMusicalPositionValid
    , kVstTempoValid = vstTimingInfoFlagTempoValid
    , kVstBarsValid = vstTimingInfoFlagLastBarPositionValid
    , kVstCyclePosValid = vstTimingInfoFlagLoopPositionValid
    , kVstTimeSigValid = vstTimingInfoFlagTimeSignatureValid
    , kVstSmpteValid = vstTimingInfoFlagSmpteValid
    , kVstClockValid = vstTimingInfoFlagNearestClockValid

    , kVstAutomationReading = vstTimingInfoFlagAutomationReadModeActive
    , kVstAutomationWriting = vstTimingInfoFlagAutomationWriteModeActive
    , kVstNanosValid = vstTimingInfoFlagNanosecondsValid
    , kVstTransportChanged = vstTimingInfoFlagTransportChanged
};

//==============================================================================
enum VstSmpteRates
{
    vstSmpteRateFps24,
    vstSmpteRateFps25,
    vstSmpteRateFps2997,
    vstSmpteRateFps30,
    vstSmpteRateFps2997drop,
    vstSmpteRateFps30drop,

    vstSmpteRate16mmFilm,
    vstSmpteRate35mmFilm,

    vstSmpteRateFps239 = vstSmpteRate35mmFilm + 3,
    vstSmpteRateFps249,
    vstSmpteRateFps599,
    vstSmpteRateFps60

    , kVstSmpte239fps = vstSmpteRateFps239
    , kVstSmpte24fps = vstSmpteRateFps24
    , kVstSmpte25fps = vstSmpteRateFps25
    , kVstSmpte2997fps = vstSmpteRateFps2997
    , kVstSmpte30fps = vstSmpteRateFps30
    , kVstSmpte2997dfps = vstSmpteRateFps2997drop
    , kVstSmpte30dfps = vstSmpteRateFps30drop
    , kVstSmpteFilm16mm = vstSmpteRate16mmFilm
    , kVstSmpteFilm35mm = vstSmpteRate35mmFilm
    , kVstSmpte249fps = vstSmpteRateFps249
    , kVstSmpte599fps = vstSmpteRateFps599
    , kVstSmpte60fps = vstSmpteRateFps60
};

//==============================================================================
/** Structure used for VSTs

    @tags{Audio}
*/
struct VstIndividualSpeakerInfo
{
    float azimuthalAngle;
    float elevationAngle;
    float radius;
    float reserved;
    char label[vstMaxNameLength];
    int32 type;
    char unused[28];
};
typedef VstIndividualSpeakerInfo VstSpeakerProperties;

enum VstIndividualSpeakerType
{
    vstIndividualSpeakerTypeUndefined = 0x7fffffff,
    vstIndividualSpeakerTypeMono = 0,
    vstIndividualSpeakerTypeLeft,
    vstIndividualSpeakerTypeRight,
    vstIndividualSpeakerTypeCentre,
    vstIndividualSpeakerTypeLFE,
    vstIndividualSpeakerTypeLeftSurround,
    vstIndividualSpeakerTypeRightSurround,
    vstIndividualSpeakerTypeLeftCentre,
    vstIndividualSpeakerTypeRightCentre,
    vstIndividualSpeakerTypeSurround,
    vstIndividualSpeakerTypeCentreSurround = vstIndividualSpeakerTypeSurround,
    vstIndividualSpeakerTypeLeftRearSurround,
    vstIndividualSpeakerTypeRightRearSurround,
    vstIndividualSpeakerTypeTopMiddle,
    vstIndividualSpeakerTypeTopFrontLeft,
    vstIndividualSpeakerTypeTopFrontCentre,
    vstIndividualSpeakerTypeTopFrontRight,
    vstIndividualSpeakerTypeTopRearLeft,
    vstIndividualSpeakerTypeTopRearCentre,
    vstIndividualSpeakerTypeTopRearRight,
    vstIndividualSpeakerTypeLFE2

    , kSpeakerL = vstIndividualSpeakerTypeLeft
    , kSpeakerR = vstIndividualSpeakerTypeRight
    , kSpeakerC = vstIndividualSpeakerTypeCentre
    , kSpeakerLfe = vstIndividualSpeakerTypeLFE
    , kSpeakerLs = vstIndividualSpeakerTypeLeftSurround
    , kSpeakerRs = vstIndividualSpeakerTypeRightSurround
    , kSpeakerLc = vstIndividualSpeakerTypeLeftCentre
    , kSpeakerRc = vstIndividualSpeakerTypeRightCentre
    , kSpeakerS = vstIndividualSpeakerTypeSurround
    , kSpeakerSl = vstIndividualSpeakerTypeLeftRearSurround
    , kSpeakerSr = vstIndividualSpeakerTypeRightRearSurround
    , kSpeakerTm = vstIndividualSpeakerTypeTopMiddle
    , kSpeakerTfl = vstIndividualSpeakerTypeTopFrontLeft
    , kSpeakerTfc = vstIndividualSpeakerTypeTopFrontCentre
    , kSpeakerTfr = vstIndividualSpeakerTypeTopFrontRight
    , kSpeakerTrl = vstIndividualSpeakerTypeTopRearLeft
    , kSpeakerTrc = vstIndividualSpeakerTypeTopRearCentre
    , kSpeakerTrr = vstIndividualSpeakerTypeTopRearRight
    , kSpeakerLfe2 = vstIndividualSpeakerTypeLFE2
};

/** Structure used for VSTs

    @tags{Audio}
*/
struct VstSpeakerConfiguration
{
    int32 type;
  int32 numChannels; // numberOfChannels;
    VstIndividualSpeakerInfo speakers[8];
};
typedef VstSpeakerConfiguration VstSpeakerArrangement;

enum VstSpeakerConfigurationType
{
    vstSpeakerConfigTypeUser  = -2,
    vstSpeakerConfigTypeEmpty = -1,
    vstSpeakerConfigTypeMono  = 0,
    vstSpeakerConfigTypeLR,
    vstSpeakerConfigTypeLsRs,
    vstSpeakerConfigTypeLcRc,
    vstSpeakerConfigTypeSlSr,
    vstSpeakerConfigTypeCLfe,
    vstSpeakerConfigTypeLRC,
    vstSpeakerConfigTypeLRS,
    vstSpeakerConfigTypeLRCLfe,
    vstSpeakerConfigTypeLRLfeS,
    vstSpeakerConfigTypeLRCS,
    vstSpeakerConfigTypeLRLsRs,
    vstSpeakerConfigTypeLRCLfeS,
    vstSpeakerConfigTypeLRLfeLsRs,
    vstSpeakerConfigTypeLRCLsRs,
    vstSpeakerConfigTypeLRCLfeLsRs,
    vstSpeakerConfigTypeLRCLsRsCs,
    vstSpeakerConfigTypeLRLsRsSlSr,
    vstSpeakerConfigTypeLRCLfeLsRsCs,
    vstSpeakerConfigTypeLRLfeLsRsSlSr,
    vstSpeakerConfigTypeLRCLsRsLcRc,
    vstSpeakerConfigTypeLRCLsRsSlSr,
    vstSpeakerConfigTypeLRCLfeLsRsLcRc,
    vstSpeakerConfigTypeLRCLfeLsRsSlSr,
    vstSpeakerConfigTypeLRCLsRsLcRcCs,
    vstSpeakerConfigTypeLRCLsRsCsSlSr,
    vstSpeakerConfigTypeLRCLfeLsRsLcRcCs,
    vstSpeakerConfigTypeLRCLfeLsRsCsSlSr,
    vstSpeakerConfigTypeLRCLfeLsRsTflTfcTfrTrlTrrLfe2

    , kSpeakerArrMono = vstSpeakerConfigTypeMono
    , kSpeakerArrStereo = vstSpeakerConfigTypeLR
    , kSpeakerArrStereoCLfe = vstSpeakerConfigTypeCLfe
    , kSpeakerArrStereoCenter = vstSpeakerConfigTypeLcRc
    , kSpeakerArrStereoSide = vstSpeakerConfigTypeSlSr
    , kSpeakerArrStereoSurround = vstSpeakerConfigTypeLsRs
    , kSpeakerArr30Cine = vstSpeakerConfigTypeLRC
    , kSpeakerArr31Cine = vstSpeakerConfigTypeLRCLfe
    , kSpeakerArr40Cine = vstSpeakerConfigTypeLRCS
    , kSpeakerArr41Cine = vstSpeakerConfigTypeLRCLfeS
    , kSpeakerArr60Cine = vstSpeakerConfigTypeLRCLsRsCs
    , kSpeakerArr61Cine = vstSpeakerConfigTypeLRCLfeLsRsCs
    , kSpeakerArr70Cine = vstSpeakerConfigTypeLRCLsRsLcRc // create7point0SDDS
    , kSpeakerArr71Cine = vstSpeakerConfigTypeLRCLfeLsRsLcRc
    , kSpeakerArr80Cine = vstSpeakerConfigTypeLRCLsRsLcRcCs
    , kSpeakerArr81Cine = vstSpeakerConfigTypeLRCLfeLsRsLcRcCs
    , kSpeakerArr30Music = vstSpeakerConfigTypeLRS
    , kSpeakerArr31Music = vstSpeakerConfigTypeLRLfeS
    , kSpeakerArr40Music = vstSpeakerConfigTypeLRLsRs
    , kSpeakerArr41Music = vstSpeakerConfigTypeLRLfeLsRs
    , kSpeakerArr60Music = vstSpeakerConfigTypeLRLsRsSlSr
    , kSpeakerArr61Music = vstSpeakerConfigTypeLRLfeLsRsSlSr
    , kSpeakerArr70Music = vstSpeakerConfigTypeLRCLsRsSlSr // create7point0
    , kSpeakerArr71Music = vstSpeakerConfigTypeLRCLfeLsRsSlSr
    , kSpeakerArr80Music = vstSpeakerConfigTypeLRCLsRsCsSlSr
    , kSpeakerArr81Music = vstSpeakerConfigTypeLRCLfeLsRsCsSlSr
    , kSpeakerArr50 = vstSpeakerConfigTypeLRCLsRs
    , kSpeakerArr51 = vstSpeakerConfigTypeLRCLfeLsRs
    , kSpeakerArr102 = vstSpeakerConfigTypeLRCLfeLsRsTflTfcTfrTrlTrrLfe2
    , kSpeakerArrEmpty = vstSpeakerConfigTypeEmpty
    , kSpeakerArrUserDefined = vstSpeakerConfigTypeUser
};

#if JUCE_BIG_ENDIAN
 #define JUCE_MULTICHAR_CONSTANT(a, b, c, d) (a | (((uint32) b) << 8) | (((uint32) c) << 16) | (((uint32) d) << 24))
#else
 #define JUCE_MULTICHAR_CONSTANT(a, b, c, d) (d | (((uint32) c) << 8) | (((uint32) b) << 16) | (((uint32) a) << 24))
#endif

enum PresonusExtensionConstants
{
    presonusVendorID                = JUCE_MULTICHAR_CONSTANT ('P', 'r', 'e', 'S'),
    presonusSetContentScaleFactor   = JUCE_MULTICHAR_CONSTANT ('A', 'e', 'C', 's')
};

//==============================================================================
/** Structure used for VSTs

    @tags{Audio}
*/
struct vst2FxBank
{
    int32 magic1;
    int32 size;
    int32 magic2;
    int32 version1;
    int32 fxID;
    int32 version2;
    int32 elements;
    int32 current;
    char shouldBeZero[124];
    int32 chunkSize;
    char chunk[1];
};

#if JUCE_MSVC
 #pragma pack(pop)
#elif JUCE_MAC || JUCE_IOS
 #pragma options align=reset
#else
 #pragma pack(pop)
#endif
