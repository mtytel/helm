/* Copyright 2013-2015 Matt Tytel
 *
 * twytch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * twytch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with twytch.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "twytch_look_and_feel.h"

namespace {

  const float SLIDER_PADDING = 1.0;
  const float SLIDER_ROUNDING = 3.0;
} // namespace

void TwytchLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                                         float slider_pos, float min, float max,
                                         const Slider::SliderStyle style, Slider& slider) {
  g.setColour(Colours::black);
  g.fillRect(x, y, width, height);
  
  g.setColour(Colours::grey);
  if (style == Slider::SliderStyle::LinearHorizontal) {
    float percent_smaller = (width - 2 * SLIDER_PADDING) / width;
    g.fillRoundedRectangle(x + SLIDER_PADDING, y + SLIDER_PADDING,
                           (slider_pos - x) * percent_smaller, height - 2 * SLIDER_PADDING,
                           SLIDER_ROUNDING);
  }

  else if (style == Slider::SliderStyle::LinearVertical) {
    float percent_smaller = (height - 2 * SLIDER_PADDING) / height;
    g.fillRoundedRectangle(x + SLIDER_PADDING, y + SLIDER_PADDING,
                           width - 2 * SLIDER_PADDING, percent_smaller * (slider_pos - y),
                           SLIDER_ROUNDING);
  }
}

void TwytchLookAndFeel::drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
                                              float slider_pos, float min, float max,
                                              const Slider::SliderStyle style, Slider& slider) {

}

void TwytchLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                                         float slider_t, float start_angle, float end_angle,
                                         Slider& slider) {
  static const float stroke_width = 8.0f;
  static const PathStrokeType stroke_type =
      PathStrokeType(stroke_width, PathStrokeType::beveled, PathStrokeType::butt);

  Path background;
  float center_x = x + width / 2.0f;
  float center_y = y + height / 2.0f;
  background.addCentredArc(center_x, center_y, width / 2.0f, height / 2.0f,
                           0.0f, start_angle, end_angle, true);
  g.setColour(Colours::darkgrey);
  g.fillPath(background);

  float slider_radius = width / 2.0f - stroke_width;

  Path rail;
  rail.addCentredArc(center_x, center_y, slider_radius, slider_radius,
                     0.0f, start_angle, end_angle, true);

  g.setColour(Colours::black);
  g.strokePath(rail, stroke_type);

  float current_angle = start_angle + slider_t * (end_angle - start_angle);
  Path active_section;
  active_section.addCentredArc(center_x, center_y, slider_radius, slider_radius,
                               0.0f, start_angle, current_angle, true);

  g.setColour(Colour(0xffcccccc));
  g.strokePath(active_section, stroke_type);

  if (slider.getInterval() == 1) {
    g.drawText(String(slider.getValue()), slider.getLocalBounds(),
               Justification::horizontallyCentred | Justification::verticallyCentred);
  }
}