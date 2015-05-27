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
#include "synth_gui_interface.h"
#include "text_look_and_feel.h"

void ModulationLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                                             float slider_pos, float min, float max,
                                             const Slider::SliderStyle style, Slider& slider) {
  ModulationSlider* mod_slider = dynamic_cast<ModulationSlider*>(&slider);
  if (!mod_slider)
    return;
  Slider* destination = mod_slider->getDestinationSlider();
  float destination_percentage = destination->valueToProportionOfLength(destination->getValue());
  float destination_range = destination->getMaximum() - destination->getMinimum();
  float mod_percentage = slider.getValue() / destination_range;

  if (mod_percentage == 0.0) {
    g.setColour(Colour(0x11b9f6ca));
    g.fillRect(0, 0, slider.getWidth(), slider.getHeight());
    g.setColour(Colour(0xffb9f6ca));
    g.drawRect(0, 0, slider.getWidth(), slider.getHeight(), 1);
  }
  else {
    g.setColour(Colour(0x1100e676));
    g.fillRect(0, 0, slider.getWidth(), slider.getHeight());
    g.setColour(Colour(0xff00e676));
    g.drawRect(0.0f, 0.0f, float(slider.getWidth()), float(slider.getHeight()), 2.5f);
  }

  g.setColour(Colour(0x5500e676));

  if (style == Slider::SliderStyle::LinearBar) {
    float destination_position = width * destination_percentage;
    float mod_diff = width * mod_percentage;
    float from = std::min<float>(destination_position + mod_diff, destination_position);
    float to = std::max<float>(destination_position + mod_diff, destination_position);

    g.fillRect(x + from, float(y), to - from, float(height));
    g.setColour(Colour(0xffffffff));
    g.fillRect(x + destination_position + mod_diff, 1.0f * y, 2.0f, 1.0f * height);
  }
  else if (style == Slider::SliderStyle::LinearBarVertical) {
    float destination_position = height * (1.0f - destination_percentage);
    float mod_diff = height * mod_percentage;
    float from = std::min<float>(destination_position - mod_diff, destination_position);
    float to = std::max<float>(destination_position - mod_diff, destination_position);

    g.fillRect(float(x), y + from, float(width), to - from);
    g.setColour(Colour(0xffffffff));
    g.fillRect(float(x), y + destination_position - mod_diff, 1.0f * width, 2.0f);
  }
}

void ModulationLookAndFeel::drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
                                                  float slider_pos, float min, float max,
                                                  const Slider::SliderStyle style, Slider& slider) {
}

void ModulationLookAndFeel::drawTextModulation(Graphics& g, Slider& slider, float percent) {
  if (percent == 0.0f) {
    g.setColour(Colour(0x11b9f6ca));
    g.fillRect(0, 0, slider.getWidth(), slider.getHeight());
    g.setColour(Colour(0xffb9f6ca));
    g.drawRect(0, 0, slider.getWidth(), slider.getHeight(), 1);
  }
  else {
    g.setColour(Colour(0x1100e676));
    g.fillRect(0, 0, slider.getWidth(), slider.getHeight());
    g.setColour(Colour(0xff00e676));
    g.drawRect(0.0f, 0.0f, float(slider.getWidth()), float(slider.getHeight()), 2.5f);
  }

  g.setColour(Colour(0x5500e676));

  if (percent < 0.0f) {
    g.fillRect(2.5f, 2.5f, slider.getWidth() - 5.0f, -percent * (slider.getHeight() - 5.0f));
  }
  else if (percent > 0.0f) {
    float height = percent * (slider.getHeight() - 5.0f);
    g.fillRect(2.5f, slider.getHeight() - height - 2.5f, slider.getWidth() - 5.0f, height);
  }
}

void ModulationLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                                             float slider_t, float start_angle, float end_angle,
                                             Slider& slider) {
  ModulationSlider* mod_slider = dynamic_cast<ModulationSlider*>(&slider);
  if (!mod_slider)
    return;

  SynthSlider* destination = mod_slider->getDestinationSlider();
  if (&destination->getLookAndFeel() == TextLookAndFeel::instance()) {
    float destination_range = destination->getMaximum() - destination->getMinimum();
    float mod_percentage = slider.getValue() / destination_range;
    drawTextModulation(g, slider, mod_percentage);
    return;
  }

  float destination_percentage = destination->valueToProportionOfLength(destination->getValue());
  float destination_range = destination->getMaximum() - destination->getMinimum();
  float destination_angle = start_angle + destination_percentage * (end_angle - start_angle);
  float mod_diff = slider.getValue() * (end_angle - start_angle) / destination_range;

  float draw_radius = std::min(width / 2.0f, height / 2.0f);
  float knob_radius = 0.65f * draw_radius;
  PathStrokeType stroke_type =
      PathStrokeType(knob_radius, PathStrokeType::beveled, PathStrokeType::butt);

  if (mod_diff == 0.0) {
    g.setColour(Colour(0x33b9f6ca));
    g.fillEllipse(width / 2.0f - knob_radius, height / 2.0f - knob_radius,
                  2.0 * knob_radius, 2.0 * knob_radius);
    g.setColour(Colour(0xff00c853));
    g.drawEllipse(width / 2.0f - knob_radius + 0.5f, height / 2.0f - knob_radius + 0.5f,
                  2.0f * knob_radius - 1.0f, 2.0f * knob_radius - 1.0f, 1.0f);
  }
  else {
    g.setColour(Colour(0xaa00e676));
    g.fillEllipse(width / 2.0f - knob_radius, height / 2.0f - knob_radius,
                  2.0 * knob_radius, 2.0 * knob_radius);
    g.setColour(Colour(0xff00c853));
    g.drawEllipse(width / 2.0f - knob_radius + 1.5f, height / 2.0f - knob_radius + 1.5f,
                  2.0f * knob_radius - 3.0f, 2.0f * knob_radius - 3.0f, 3.0f);
  }


  Path active_section;
  float center_x = x + draw_radius;
  float center_y = y + draw_radius;
  if (destination_angle > mopo::PI)
    destination_angle -= 2.0 * mopo::PI;
  active_section.addCentredArc(center_x, center_y, knob_radius / 2.0, knob_radius / 2.0,
                               destination_angle, mod_diff, 0, true);

  g.setColour(Colour(0xff69f0ae));
  g.strokePath(active_section, stroke_type);

  float end_x = draw_radius + 0.9f * knob_radius * sin(destination_angle + mod_diff);
  float end_y = draw_radius - 0.9f * knob_radius * cos(destination_angle + mod_diff);
  g.setColour(Colour(0xffffffff));
  g.drawLine(draw_radius, draw_radius, end_x, end_y, 1.0f);
}

void ModulationLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
                                             bool isMouseOverButton, bool isButtonDown) {
  static const PathStrokeType stroke(3.0f, PathStrokeType::beveled, PathStrokeType::rounded);
  static const DropShadow shadow(Colour(0xff000000), 1, Point<int>(0, 0));

  Colour background = Colour(0xff303030);
  Colour icon_source_color = Colour(0xff565656);
  Colour icon_dest_color = Colour(0xff565656);
  if (button.getToggleState())
    icon_source_color = Colour(0xff4fc3f7);

  SynthGuiInterface* parent = button.findParentComponentOfClass<SynthGuiInterface>();
  if (parent && parent->getSourceConnections(button.getName().toStdString()).size())
    icon_dest_color = Colour(0xff00c853);

  g.setColour(background);
  g.fillAll();

  float width = button.getWidth();
  float height = button.getHeight();

  Path modulation_source;
  modulation_source.addEllipse(width * 0.55f, height * 0.3f, width * 0.4f, height * 0.4f);
  Path modulation_dest;
  modulation_dest.addEllipse(width * 0.05f, height * 0.3f, width * 0.4f, height * 0.4f);

  // Shadows.
  shadow.drawForPath(g, modulation_source);
  shadow.drawForPath(g, modulation_dest);

  // Modulation destination port.
  g.setColour(icon_dest_color);
  g.fillPath(modulation_dest);

  // Hole in modulation destination
  g.setColour(Colour(0xff111111));
  g.fillEllipse(width * 0.13f, height * 0.38f, width * 0.24f, height * 0.24f);
  g.fillRect(width * 0.25f, height * 0.4f, width * 0.19f, height * 0.2f);

  // Ball and wire for modulation source.
  g.setColour(icon_source_color);
  g.fillPath(modulation_source);
  g.fillRoundedRectangle(width * 0.17f, height * 0.43f,
                         width * 0.5f, height * 0.14f, 2.0f);

  if (isButtonDown) {
    g.setColour(Colour(0x11000000));
    g.fillAll();
  }
  else if (isMouseOverButton) {
    g.setColour(Colour(0x11ffffff));
    g.fillAll();
  }
}
