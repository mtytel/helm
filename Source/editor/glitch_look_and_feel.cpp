#include "glitch_look_and_feel.h"

namespace {

  const float SLIDER_PADDING = 1.0;
  const float SLIDER_ROUNDING = 3.0;
} // namespace

void GlitchLookAndFeel::drawLinearSlider(Graphics& g,
                                         int x, int y,
                                         int width, int height,
                                         float sliderPos, float minSliderPos, float maxSliderPos,
                                         const Slider::SliderStyle, Slider& slider) {
  g.setColour(Colours::black);
  g.fillRoundedRectangle(x, y, width, height, SLIDER_ROUNDING);

  float percent_smaller = (width - 2 * SLIDER_PADDING) / width;
  g.setColour(Colours::grey);
  g.fillRoundedRectangle(x + SLIDER_PADDING, y + SLIDER_PADDING,
                         (sliderPos - x) * percent_smaller, height - 2 * SLIDER_PADDING,
                         SLIDER_ROUNDING);

}

void GlitchLookAndFeel::drawLinearSliderThumb(Graphics& g,
                                              int x, int y,
                                              int width, int height,
                                              float sliderPos,
                                              float minSliderPos, float maxSliderPos,
                                              const Slider::SliderStyle style, Slider& slider) {

}