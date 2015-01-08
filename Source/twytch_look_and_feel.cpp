#include "twytch_look_and_feel.h"

namespace {

  const float SLIDER_PADDING = 1.0;
  const float SLIDER_ROUNDING = 3.0;
} // namespace

void TwytchLookAndFeel::drawLinearSlider(Graphics& g,
                                         int x, int y,
                                         int width, int height,
                                         float sliderPos, float minSliderPos, float maxSliderPos,
                                         const Slider::SliderStyle style, Slider& slider) {
  g.setColour(Colours::black);
  g.fillRoundedRectangle(x, y, width, height, SLIDER_ROUNDING);

  g.setColour(Colours::grey);
  if (style == Slider::SliderStyle::LinearHorizontal) {
    float percent_smaller = (width - 2 * SLIDER_PADDING) / width;
    g.fillRoundedRectangle(x + SLIDER_PADDING, y + SLIDER_PADDING,
                           (sliderPos - x) * percent_smaller, height - 2 * SLIDER_PADDING,
                           SLIDER_ROUNDING);
  }

  else if (style == Slider::SliderStyle::LinearVertical) {
    float percent_smaller = (height - 2 * SLIDER_PADDING) / height;
    g.fillRoundedRectangle(x + SLIDER_PADDING, y + SLIDER_PADDING,
                           width - 2 * SLIDER_PADDING, percent_smaller * (sliderPos - y),
                           SLIDER_ROUNDING);
  }
}

void TwytchLookAndFeel::drawLinearSliderThumb(Graphics& g,
                                              int x, int y,
                                              int width, int height,
                                              float sliderPos,
                                              float minSliderPos, float maxSliderPos,
                                              const Slider::SliderStyle style, Slider& slider) {

}