/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "audio_viewer.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#define FRAMES_PER_SECOND 32
#define MAX_RESOLUTION 128
#define GRID_CELL_WIDTH 10
//[/MiscUserDefs]

//==============================================================================
AudioViewer::AudioViewer (int num_samples)
{

    //[UserPreSize]
    output_memory_ = nullptr;
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    samples_to_show_ = num_samples;
    for (int i = 0; i < MAX_RESOLUTION; ++i)
      peak_values_.push_back(0.0f);
    last_offset_ = 0;
    setFramesPerSecond(FRAMES_PER_SECOND);
    //[/Constructor]
}

AudioViewer::~AudioViewer()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AudioViewer::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff33064f));

    //[UserPaint] Add your own custom painting code here..
    g.setColour(Colour(0xff43165f));
    for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
        g.drawLine(x, 0, x, getHeight());
    for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
        g.drawLine(0, y, getWidth(), y);

    g.setGradientFill(ColourGradient(Colour(0x33cb587a), 0.0f, getHeight() / 2.0f,
                                     Colour(0xffeca769), 0.0f, getHeight() / 4.0f, false));
    g.fillPath(wave_top_);
    g.setGradientFill(ColourGradient(Colour(0x33cb587a), 0.0f, getHeight() / 2.0f,
                                     Colour(0xffeca769), 0.0f, 3.0f * getHeight() / 4.0f, false));
    g.fillPath(wave_bottom_);
    g.setColour(Colour(0xffaaaaaa));
    g.strokePath(wave_top_, PathStrokeType(1.0f, PathStrokeType::beveled, PathStrokeType::rounded));
    g.strokePath(wave_bottom_, PathStrokeType(1.0f, PathStrokeType::beveled, PathStrokeType::rounded));
    //[/UserPaint]
}

void AudioViewer::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    resetWavePath();
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void AudioViewer::resetWavePath() {
    static const float padding = 5.0f;

    if (output_memory_ == nullptr)
        return;

    wave_bottom_.clear();
    wave_top_.clear();

    float draw_width = getWidth() - 2.0f * padding;
    float draw_height = getHeight() - 2.0f * padding;

    wave_top_.startNewSubPath(padding, getHeight() / 2.0f);
    wave_bottom_.startNewSubPath(padding, getHeight() / 2.0f);
    int inc = samples_to_show_ / MAX_RESOLUTION;
    int size = output_memory_->getSize();
    int new_samples = (output_memory_->getOffset() + size - last_offset_) % size;

    const mopo::mopo_float* memory = output_memory_->getBuffer();
    int new_points = new_samples / inc;

    for (int i = 0; i < new_points; ++i) {
        float max = 0.0f;
        for (int s = 0; s < inc; ++s) {
            last_offset_ = (last_offset_ + 1) % size;
            max = std::max<float>(max, std::fabs(memory[last_offset_]));
        }
        peak_values_.pop_front();
        peak_values_.push_back(max);
    }

    std::list<float>::iterator iter = peak_values_.begin();
    for (int i = 0; iter != peak_values_.end(); ++i, ++iter) {
        float t = (1.0f * i) / MAX_RESOLUTION;
        float val = *iter;
        wave_top_.lineTo(padding + t * draw_width, padding + draw_height * ((1.0f - val) / 2.0f));
        wave_bottom_.lineTo(padding + t * draw_width, padding + draw_height * ((1.0f + val) / 2.0f));
    }

    wave_top_.lineTo(getWidth() - padding, getHeight() / 2.0f);
    wave_bottom_.lineTo(getWidth() - padding, getHeight() / 2.0f);
}

void AudioViewer::update() {
    resetWavePath();
    repaint();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AudioViewer" componentName=""
                 parentClasses="public AnimatedAppComponent" constructorParams="int num_samples"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff33064f"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
