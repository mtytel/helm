#ifndef TWYTCH_LOOK_AND_FEEL_H
#define TWYTCH_LOOK_AND_FEEL_H

#include "../JuceLibraryCode/JuceHeader.h"

class TwytchLookAndFeel : public juce::LookAndFeel_V3 {
  public:

    enum ColorId {
      kBackground,
      kStepSeqBackground,
      kStepSeqSlider,
      kStepSeqHover,
    };

    void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                          float slider_pos, float min, float max,
                          const Slider::SliderStyle style, Slider& slider) override;

    void drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
                               float slider_pos, float min, float max,
                               const Slider::SliderStyle style, Slider& slider) override;

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                          float slider_t, float start_angle, float end_angle,
                          Slider& slider);
};

#endif // TWYTCH_LOOK_AND_FEEL_H