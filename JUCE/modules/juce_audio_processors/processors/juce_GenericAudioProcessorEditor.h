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

namespace juce
{

//==============================================================================
/**
    A type of UI component that displays the parameters of an AudioProcessor as
    a simple list of sliders, combo boxes and switches.

    This can be used for showing an editor for a processor that doesn't supply
    its own custom editor.

    @see AudioProcessor

    @tags{Audio}
*/
class JUCE_API  GenericAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    //==============================================================================
    GenericAudioProcessorEditor (AudioProcessor&);
    ~GenericAudioProcessorEditor() override;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    // This constructor has been changed to take a reference instead of a pointer
    JUCE_DEPRECATED_WITH_BODY (GenericAudioProcessorEditor (AudioProcessor* p), : GenericAudioProcessorEditor (*p) {})

private:
    //==============================================================================
    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenericAudioProcessorEditor)
};

} // namespace juce
