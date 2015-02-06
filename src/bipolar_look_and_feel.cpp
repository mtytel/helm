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

#include "bipolar_look_and_feel.h"
#include "mopo.h"

namespace {
  const float SLIDER_ROUNDING = 1.0;
} // namespace

void BipolarLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                                          float slider_pos, float min, float max,
                                          const Slider::SliderStyle style, Slider& slider) {
  g.fillAll(slider.findColour(Slider::textBoxOutlineColourId));
  g.setColour(slider.findColour(Slider::backgroundColourId));
  g.fillRect(x, y, width, height);
  g.setColour(slider.findColour(Slider::trackColourId));

  if (style == Slider::SliderStyle::LinearBar) {
    float from = std::min<float>(width / 2.0, slider_pos - x);
    float to = std::max<float>(width / 2.0, slider_pos - x);
    g.fillRoundedRectangle(x + from, y, to - from, height, SLIDER_ROUNDING);
  }

  else if (style == Slider::SliderStyle::LinearBarVertical) {
    float from = std::min<float>(height / 2.0, slider_pos - y);
    float to = std::max<float>(height / 2.0, slider_pos - y);
    g.fillRoundedRectangle(x, y + from, width, to - from, SLIDER_ROUNDING);
  }
}

void BipolarLookAndFeel::drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
                                               float slider_pos, float min, float max,
                                               const Slider::SliderStyle style, Slider& slider) {
}

void BipolarLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                                          float slider_t, float start_angle, float end_angle,
                                          Slider& slider) {
  static const float stroke_width = 8.0f;
  static const PathStrokeType stroke_type =
      PathStrokeType(stroke_width, PathStrokeType::beveled, PathStrokeType::butt);

  float current_angle = start_angle + slider_t * (end_angle - start_angle);

  Path background;
  float center_x = x + width / 2.0f;
  float center_y = y + height / 2.0f;
  background.addCentredArc(center_x, center_y, width / 2.0f, height / 2.0f,
                           0.0f, start_angle, end_angle, true);
  g.setColour(slider.findColour(Slider::backgroundColourId));
  g.fillPath(background);

  float slider_radius = width / 2.0f - stroke_width;

  Path rail;
  rail.addCentredArc(center_x, center_y, slider_radius, slider_radius,
                     0.0f, start_angle, end_angle, true);

  g.setColour(slider.findColour(Slider::rotarySliderOutlineColourId));
  g.strokePath(rail, stroke_type);

  Path active_section;
  if (current_angle > mopo::PI)
    current_angle -= 2.0 * mopo::PI;
  active_section.addCentredArc(center_x, center_y, slider_radius, slider_radius,
                               0.0f, current_angle, 0, true);

  g.setColour(slider.findColour(Slider::rotarySliderFillColourId));
  g.strokePath(active_section, stroke_type);

  if (slider.getInterval() == 1) {
    g.setColour(slider.findColour(Slider::textBoxTextColourId));
    g.drawText(String(slider.getValue()), slider.getLocalBounds(),
               Justification::horizontallyCentred | Justification::verticallyCentred);
  }
}
