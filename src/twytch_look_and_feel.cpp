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

void TwytchLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                                         float slider_pos, float min, float max,
                                         const Slider::SliderStyle style, Slider& slider) {
  g.fillAll(slider.findColour(Slider::textBoxOutlineColourId));
  g.setColour(slider.findColour(Slider::backgroundColourId));
  g.fillRect(x, y, width, height);
  g.setColour(slider.findColour(Slider::trackColourId));

  if (style == Slider::SliderStyle::LinearBar) {
    g.fillRect(float(x), float(y), slider_pos - x, float(height));
    g.setColour(slider.findColour(Slider::thumbColourId));
    g.fillRect(slider_pos - x, 1.0f * y, 2.0f, 1.0f * height);
  }

  else if (style == Slider::SliderStyle::LinearBarVertical) {
    g.fillRect(float(x), slider_pos, float(width), y + height - slider_pos);
    g.setColour(slider.findColour(Slider::thumbColourId));
    g.fillRect(1.0f * x, slider_pos - y, 1.0f * width, 2.0f);
  }
  else {
    LookAndFeel_V3::drawLinearSlider(g, x, y, width, height, slider_pos, min, max, style, slider);
  }
}

void TwytchLookAndFeel::drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
                                              float slider_pos, float min, float max,
                                              const Slider::SliderStyle style, Slider& slider) {
  LookAndFeel_V3::drawLinearSliderThumb(g, x, y, width, height,
                                        slider_pos, min, max, style, slider);
}

void TwytchLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                                         float slider_t, float start_angle, float end_angle,
                                         Slider& slider) {
  static const float stroke_width = 4.0f;
  static const PathStrokeType stroke_type =
      PathStrokeType(stroke_width, PathStrokeType::beveled, PathStrokeType::butt);

  float full_radius = std::min(width / 2.0f, height / 2.0f);
  float knob_radius = 0.65f * full_radius;
  float outer_radius = full_radius - stroke_width;
  float center_x = x + full_radius;
  float center_y = y + full_radius;
  float current_angle = start_angle + slider_t * (end_angle - start_angle);

  Path rail;
  rail.addCentredArc(center_x, center_y, outer_radius, outer_radius,
                     0.0f, start_angle, end_angle, true);

  g.setColour(slider.findColour(Slider::rotarySliderOutlineColourId));
  g.strokePath(rail, stroke_type);
  g.fillEllipse(full_radius - knob_radius, full_radius - knob_radius,
                2.0f * knob_radius, 2.0f * knob_radius);

  Path active_section;
  active_section.addCentredArc(center_x, center_y, outer_radius, outer_radius,
                               0.0f, start_angle, current_angle, true);

  g.setColour(slider.findColour(Slider::rotarySliderFillColourId));
  g.strokePath(active_section, stroke_type);

  float end_x = full_radius + knob_radius * sin(current_angle);
  float end_y = full_radius - knob_radius * cos(current_angle);
  g.drawLine(full_radius, full_radius, end_x, end_y, 2.0f);
  
  if (slider.getInterval() == 1) {
    g.setColour(slider.findColour(Slider::textBoxTextColourId));
    g.drawText(String(slider.getValue()), slider.getLocalBounds(),
               Justification::horizontallyCentred | Justification::verticallyCentred);
  }
}
