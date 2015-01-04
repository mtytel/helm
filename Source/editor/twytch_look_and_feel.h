#ifndef GLITCH_LOOK_AND_FEEL_H
#define GLITCH_LOOK_AND_FEEL_H

#include "../JuceLibraryCode/JuceHeader.h"

class TwytchLookAndFeel : public juce::LookAndFeel_V3 {
  public:

    void drawLinearSlider(Graphics& g,
                          int x, int y,
                          int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          const Slider::SliderStyle, Slider& slider) override;

    void drawLinearSliderThumb(Graphics& g,
                               int x, int y,
                               int width, int height,
                               float sliderPos, float minSliderPos, float maxSliderPos,
                               const Slider::SliderStyle style, Slider& slider) override;
  
  private:
};


#endif // GLITCH_LOOK_AND_FEEL_H