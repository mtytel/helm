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

#include "modulation_look_and_feel.h"
#include "modulation_slider.h"
#include "mopo.h"

void ModulationLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                                             float slider_pos, float min, float max,
                                             const Slider::SliderStyle style, Slider& slider) {
  ModulationSlider* mod_slider = dynamic_cast<ModulationSlider*>(&slider);
  if (!mod_slider)
    return;
  Slider* source = mod_slider->getSourceSlider();
  float source_percentage = source->valueToProportionOfLength(source->getValue());

  g.fillAll(Colour(0x22ffaa00));
  g.setColour(slider.findColour(Slider::backgroundColourId));
  g.fillRect(x, y, width, height);
  g.setColour(Colour(0x55ffaa00));

  if (style == Slider::SliderStyle::LinearBar) {
    float source_position = width * source_percentage;
    float mod_diff = width * slider.getValue();
    float from = std::min<float>(source_position + mod_diff, source_position);
    float to = std::max<float>(source_position + mod_diff, source_position);

    g.fillRect(x + from, float(y), to - from, float(height));
    g.setColour(Colour(0x88ffaa00));
    g.fillRect(x + source_position + mod_diff, 1.0f * y, 2.0f, 1.0f * height);
  }
  else if (style == Slider::SliderStyle::LinearBarVertical) {
    float source_position = height * source_percentage;
    float mod_diff = height * slider.getValue();
    float from = std::min<float>(source_position + mod_diff, source_position);
    float to = std::max<float>(source_position + mod_diff, source_position);

    g.fillRect(float(x), y + from, float(width), to - from);
    g.setColour(Colour(0x88ffddaa));
    g.fillRect(1.0f * x, y + source_position + mod_diff, 1.0f * width, 2.0f);
  }
}

void ModulationLookAndFeel::drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
                                                  float slider_pos, float min, float max,
                                                  const Slider::SliderStyle style, Slider& slider) {
}

void ModulationLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                                             float slider_t, float start_angle, float end_angle,
                                             Slider& slider) {
  ModulationSlider* mod_slider = dynamic_cast<ModulationSlider*>(&slider);
  if (!mod_slider)
    return;

  Slider* source = mod_slider->getSourceSlider();
  float source_percentage = source->valueToProportionOfLength(source->getValue());
  float source_angle = start_angle + source_percentage * (end_angle - start_angle);
  float mod_diff = slider.getValue() * (end_angle - start_angle);

  float draw_radius = std::min(width / 2.0f, height / 2.0f);
  float knob_radius = 0.65f * draw_radius;
  PathStrokeType stroke_type =
      PathStrokeType(knob_radius, PathStrokeType::beveled, PathStrokeType::butt);

  g.setColour(Colour(0x22ffaa00));
  g.fillEllipse(width / 2.0f - knob_radius, height / 2.0f - knob_radius,
                2.0 * knob_radius, 2.0 * knob_radius);

  Path active_section;
  float center_x = x + draw_radius;
  float center_y = y + draw_radius;
  if (source_angle > mopo::PI)
    source_angle -= 2.0 * mopo::PI;
  active_section.addCentredArc(center_x, center_y, knob_radius / 2.0, knob_radius / 2.0,
                               source_angle, mod_diff, 0, true);

  g.setColour(Colour(0x55ffaa00));
  g.strokePath(active_section, stroke_type);

  float end_x = draw_radius + knob_radius * sin(source_angle + mod_diff);
  float end_y = draw_radius - knob_radius * cos(source_angle + mod_diff);
  g.setColour(Colour(0x88ffddaa));
  g.drawLine(draw_radius, draw_radius, end_x, end_y);
}
