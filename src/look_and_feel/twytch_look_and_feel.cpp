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
#include "utils.h"

#define POWER_ARC_ANGLE 2.5

void TwytchLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                                         float slider_pos, float min, float max,
                                         const Slider::SliderStyle style, Slider& slider) {
  static const DropShadow thumb_shadow(Colour(0x88000000), 3, Point<int>(-1, 0));

  float pos = slider_pos - 1.0f;
  if (style == Slider::SliderStyle::LinearBar) {
    float h = slider.getHeight();
    thumb_shadow.drawForRectangle(g, Rectangle<int>(pos + 0.5f, 0, 2, h));
    g.setColour(slider.findColour(Slider::thumbColourId));
    g.fillRect(pos, 0.0f, 2.0f, h);
  }
  else if (style == Slider::SliderStyle::LinearBarVertical) {
    float w = slider.getWidth();
    thumb_shadow.drawForRectangle(g, Rectangle<int>(0, pos + 0.5f, w, 2));
    g.setColour(slider.findColour(Slider::thumbColourId));
    g.fillRect(0.0f, pos, w, 2.0f);
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
  float full_radius = std::min(width / 2.0f, height / 2.0f);
  float knob_radius = 0.65f * full_radius;
  float current_angle = start_angle + slider_t * (end_angle - start_angle);
  float end_x = full_radius + 0.9f * knob_radius * sin(current_angle);
  float end_y = full_radius - 0.9f * knob_radius * cos(current_angle);

  g.setColour(slider.findColour(Slider::rotarySliderOutlineColourId));
  g.fillEllipse(full_radius - knob_radius, full_radius - knob_radius,
                2.0f * knob_radius, 2.0f * knob_radius);

  g.setColour(Colour(0xff666666));

  g.drawEllipse(full_radius - knob_radius + 1.0f, full_radius - knob_radius + 1.0f,
                2.0f * knob_radius - 2.0f, 2.0f * knob_radius - 2.0f, 2.0f);

  g.setColour(slider.findColour(Slider::rotarySliderFillColourId));
  g.drawLine(full_radius, full_radius, end_x, end_y, 1.0f);
  
  if (slider.getInterval() == 1) {
    g.setColour(slider.findColour(Slider::textBoxTextColourId));
    g.drawText(String(slider.getValue()), slider.getLocalBounds(),
               Justification::horizontallyCentred | Justification::verticallyCentred);
  }
}


void TwytchLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
                                         bool isMouseOverButton, bool isButtonDown) {
  static float stroke_percent = 0.1;

  float full_radius = std::min(button.getWidth(), button.getHeight()) / 2.0;
  float stroke_width = 2.0f * full_radius * stroke_percent;
  PathStrokeType stroke_type(stroke_width, PathStrokeType::beveled, PathStrokeType::rounded);
  float outer_radius = full_radius - stroke_width;
  Path outer;
  outer.addCentredArc(full_radius, full_radius, outer_radius, outer_radius,
                      mopo::PI, -POWER_ARC_ANGLE, POWER_ARC_ANGLE, true);
  if (button.getToggleState())
    g.setColour(Colours::white);
  else
    g.setColour(Colours::grey);
  
  g.strokePath(outer, stroke_type);
  g.fillRoundedRectangle(full_radius - 1.0f, 0.0f, 2.0f, full_radius, 1.0f);
}