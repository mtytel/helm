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

#if JUCE_WINDOWS || DOXYGEN

//==============================================================================
/**
    Audio format which uses the Windows Media codecs (Windows only).

    @tags{Audio}
*/
class WindowsMediaAudioFormat  : public AudioFormat
{
public:
    //==============================================================================
    WindowsMediaAudioFormat();
    ~WindowsMediaAudioFormat();

    //==============================================================================
    Array<int> getPossibleSampleRates() override;
    Array<int> getPossibleBitDepths() override;
    bool canDoStereo() override;
    bool canDoMono() override;
    bool isCompressed() override;

    //==============================================================================
    AudioFormatReader* createReaderFor (InputStream*, bool deleteStreamIfOpeningFails) override;

    AudioFormatWriter* createWriterFor (OutputStream*, double sampleRateToUse,
                                        unsigned int numberOfChannels, int bitsPerSample,
                                        const StringPairArray& metadataValues, int qualityOptionIndex) override;
    using AudioFormat::createWriterFor;
};

#endif

}
