/*

    IMPORTANT! This file is auto-generated each time you save your
    project - if you alter its contents, your changes may be overwritten!

*/

#pragma once

//==============================================================================
// Audio plugin settings..

#ifndef  JucePlugin_Build_VST
 #define JucePlugin_Build_VST              1
#endif
#ifndef  JucePlugin_Build_VST3
 #define JucePlugin_Build_VST3             1
#endif
#ifndef  JucePlugin_Build_AU
 #define JucePlugin_Build_AU               1
#endif
#ifndef  JucePlugin_Build_AUv3
 #define JucePlugin_Build_AUv3             0
#endif
#ifndef  JucePlugin_Build_RTAS
 #define JucePlugin_Build_RTAS             0
#endif
#ifndef  JucePlugin_Build_AAX
 #define JucePlugin_Build_AAX              0
#endif
#ifndef  JucePlugin_Build_Standalone
 #define JucePlugin_Build_Standalone       0
#endif
#ifndef  JucePlugin_Build_Unity
 #define JucePlugin_Build_Unity            0
#endif
#ifndef  JucePlugin_Enable_IAA
 #define JucePlugin_Enable_IAA             0
#endif
#ifndef  JucePlugin_Name
 #define JucePlugin_Name                   "Helm"
#endif
#ifndef  JucePlugin_Desc
 #define JucePlugin_Desc                   ""
#endif
#ifndef  JucePlugin_Manufacturer
 #define JucePlugin_Manufacturer           "Matt Tytel"
#endif
#ifndef  JucePlugin_ManufacturerWebsite
 #define JucePlugin_ManufacturerWebsite    "tytel.org"
#endif
#ifndef  JucePlugin_ManufacturerEmail
 #define JucePlugin_ManufacturerEmail      "matthewtytel@gmail.com"
#endif
#ifndef  JucePlugin_ManufacturerCode
 #define JucePlugin_ManufacturerCode       0x54797465
#endif
#ifndef  JucePlugin_PluginCode
 #define JucePlugin_PluginCode             0x48656c6d
#endif
#ifndef  JucePlugin_IsSynth
 #define JucePlugin_IsSynth                1
#endif
#ifndef  JucePlugin_WantsMidiInput
 #define JucePlugin_WantsMidiInput         1
#endif
#ifndef  JucePlugin_ProducesMidiOutput
 #define JucePlugin_ProducesMidiOutput     0
#endif
#ifndef  JucePlugin_IsMidiEffect
 #define JucePlugin_IsMidiEffect           0
#endif
#ifndef  JucePlugin_EditorRequiresKeyboardFocus
 #define JucePlugin_EditorRequiresKeyboardFocus  1
#endif
#ifndef  JucePlugin_Version
 #define JucePlugin_Version                0.9.0
#endif
#ifndef  JucePlugin_VersionCode
 #define JucePlugin_VersionCode            0x900
#endif
#ifndef  JucePlugin_VersionString
 #define JucePlugin_VersionString          "0.9.0"
#endif
#ifndef  JucePlugin_VSTUniqueID
 #define JucePlugin_VSTUniqueID            JucePlugin_PluginCode
#endif
#ifndef  JucePlugin_VSTCategory
 #define JucePlugin_VSTCategory            kPlugCategSynth
#endif
#ifndef  JucePlugin_Vst3Category
 #define JucePlugin_Vst3Category           "Instrument|Synth"
#endif
#ifndef  JucePlugin_AUMainType
 #define JucePlugin_AUMainType             'aumu'
#endif
#ifndef  JucePlugin_AUSubType
 #define JucePlugin_AUSubType              JucePlugin_PluginCode
#endif
#ifndef  JucePlugin_AUExportPrefix
 #define JucePlugin_AUExportPrefix         helm
#endif
#ifndef  JucePlugin_AUExportPrefixQuoted
 #define JucePlugin_AUExportPrefixQuoted   "helm"
#endif
#ifndef  JucePlugin_AUManufacturerCode
 #define JucePlugin_AUManufacturerCode     JucePlugin_ManufacturerCode
#endif
#ifndef  JucePlugin_CFBundleIdentifier
 #define JucePlugin_CFBundleIdentifier     org.tytel.helmplugin
#endif
#ifndef  JucePlugin_RTASCategory
 #define JucePlugin_RTASCategory           0
#endif
#ifndef  JucePlugin_RTASManufacturerCode
 #define JucePlugin_RTASManufacturerCode   JucePlugin_ManufacturerCode
#endif
#ifndef  JucePlugin_RTASProductId
 #define JucePlugin_RTASProductId          JucePlugin_PluginCode
#endif
#ifndef  JucePlugin_RTASDisableBypass
 #define JucePlugin_RTASDisableBypass      0
#endif
#ifndef  JucePlugin_RTASDisableMultiMono
 #define JucePlugin_RTASDisableMultiMono   0
#endif
#ifndef  JucePlugin_AAXIdentifier
 #define JucePlugin_AAXIdentifier          org.tytel.helm
#endif
#ifndef  JucePlugin_AAXManufacturerCode
 #define JucePlugin_AAXManufacturerCode    JucePlugin_ManufacturerCode
#endif
#ifndef  JucePlugin_AAXProductId
 #define JucePlugin_AAXProductId           JucePlugin_PluginCode
#endif
#ifndef  JucePlugin_AAXCategory
 #define JucePlugin_AAXCategory            2048
#endif
#ifndef  JucePlugin_AAXDisableBypass
 #define JucePlugin_AAXDisableBypass       0
#endif
#ifndef  JucePlugin_AAXDisableMultiMono
 #define JucePlugin_AAXDisableMultiMono    0
#endif
#ifndef  JucePlugin_IAAType
 #define JucePlugin_IAAType                0x61757269
#endif
#ifndef  JucePlugin_IAASubType
 #define JucePlugin_IAASubType             JucePlugin_PluginCode
#endif
#ifndef  JucePlugin_IAAName
 #define JucePlugin_IAAName                "Matt Tytel: Helm"
#endif
#ifndef  JucePlugin_VSTNumMidiInputs
 #define JucePlugin_VSTNumMidiInputs       16
#endif
#ifndef  JucePlugin_VSTNumMidiOutputs
 #define JucePlugin_VSTNumMidiOutputs      16
#endif
#ifndef  JucePlugin_MaxNumInputChannels
 #define JucePlugin_MaxNumInputChannels    0
#endif
#ifndef  JucePlugin_MaxNumOutputChannels
 #define JucePlugin_MaxNumOutputChannels   2
#endif
#ifndef  JucePlugin_PreferredChannelConfigurations
 #define JucePlugin_PreferredChannelConfigurations  {0, 1}, {0, 2}
#endif
