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

// The following checks should cause a compile error if you've forgotten to
// define all your plugin settings properly..

#if ! (JucePlugin_Build_VST || JucePlugin_Build_VST3 \
        || JucePlugin_Build_AU || JucePlugin_Build_AUv3 \
        ||JucePlugin_Build_RTAS || JucePlugin_Build_AAX \
        || JucePlugin_Build_Standalone || JucePlugin_Build_LV2 \
        || JucePlugin_Build_Unity)
 #error "You need to enable at least one plugin format!"
#endif

#ifdef JUCE_CHECKSETTINGMACROS_H
 #error "This header should never be included twice! Otherwise something is wrong."
#endif
#define JUCE_CHECKSETTINGMACROS_H

#ifndef JucePlugin_IsSynth
 #error "You need to define the JucePlugin_IsSynth value!"
#endif

#ifndef JucePlugin_ManufacturerCode
 #error "You need to define the JucePlugin_ManufacturerCode value!"
#endif

#ifndef JucePlugin_PluginCode
 #error "You need to define the JucePlugin_PluginCode value!"
#endif

#ifndef JucePlugin_ProducesMidiOutput
 #error "You need to define the JucePlugin_ProducesMidiOutput value!"
#endif

#ifndef JucePlugin_WantsMidiInput
 #error "You need to define the JucePlugin_WantsMidiInput value!"
#endif

#ifdef JucePlugin_Latency
 #error "JucePlugin_Latency is now deprecated - instead, call the AudioProcessor::setLatencySamples() method if your plugin has a non-zero delay"
#endif

#ifndef JucePlugin_EditorRequiresKeyboardFocus
 #error "You need to define the JucePlugin_EditorRequiresKeyboardFocus value!"
#endif

//==============================================================================
#if _WIN64 || (__LP64__ && (defined (__APPLE_CPP__) || defined (__APPLE_CC__)))
 #undef JucePlugin_Build_RTAS
 #define JucePlugin_Build_RTAS 0
#endif

#if ! (defined (_MSC_VER) || defined (__APPLE_CPP__) || defined (__APPLE_CC__))
 #undef JucePlugin_Build_VST3
 #define JucePlugin_Build_VST3 0
#endif

//==============================================================================
#if JucePlugin_Build_LV2 && ! defined (JucePlugin_LV2URI)
 #error "You need to define the JucePlugin_LV2URI value!"
#endif

#if JucePlugin_Build_AAX && ! defined (JucePlugin_AAXIdentifier)
 #error "You need to define the JucePlugin_AAXIdentifier value!"
#endif

#if defined (__ppc__)
 #undef JucePlugin_Build_AAX
 #define JucePlugin_Build_AAX 0
#endif
