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

#include "default_look_and_feel.h"
#include "synth_slider.h"
#include "utils.h"

#define POWER_ARC_ANGLE 2.5

void DefaultLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                                          float slider_pos, float min, float max,
                                          const Slider::SliderStyle style, Slider& slider) {
  static const DropShadow thumb_shadow(Colour(0x88000000), 3, Point<int>(-1, 0));

  bool bipolar = false;
  bool active = true;
  SynthSlider* s_slider = dynamic_cast<SynthSlider*>(&slider);
  if (s_slider) {
    bipolar = s_slider->isBipolar();
    active = s_slider->isActive();
  }

  float pos = slider_pos - 1.0f;
  if (style == Slider::SliderStyle::LinearBar) {
    float h = slider.getHeight();

    g.setColour(Colour(0xff888888));
    if (bipolar)
      fillHorizontalRect(g, width / 2.0f, pos, h);
    else
      fillHorizontalRect(g, 0.0f, pos, h);

    thumb_shadow.drawForRectangle(g, Rectangle<int>(pos + 0.5f, 0, 2, h));
    g.setColour(slider.findColour(Slider::thumbColourId));
    g.fillRect(pos, 0.0f, 2.0f, h);
  }
  else if (style == Slider::SliderStyle::LinearBarVertical) {
    float w = slider.getWidth();

    g.setColour(Colour(0xff888888));
    if (bipolar)
      fillVerticalRect(g, height / 2.0f, pos, w);
    else
      fillVerticalRect(g, 0, pos, w);

    thumb_shadow.drawForRectangle(g, Rectangle<int>(0, pos + 0.5f, w, 2));
    g.setColour(slider.findColour(Slider::thumbColourId));
    g.fillRect(0.0f, pos, w, 2.0f);
  }
}

void DefaultLookAndFeel::drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
                                               float slider_pos, float min, float max,
                                               const Slider::SliderStyle style, Slider& slider) {
  LookAndFeel_V3::drawLinearSliderThumb(g, x, y, width, height,
                                        slider_pos, min, max, style, slider);
}

void DefaultLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                                          float slider_t, float start_angle, float end_angle,
                                          Slider& slider) {
  static const float stroke_percent = 0.12f;
  static Font roboto_regular(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                               BinaryData::RobotoRegular_ttfSize));

  float full_radius = std::min(width / 2.0f, height / 2.0f);
  float stroke_width = 2.0f * full_radius * stroke_percent;
  float outer_radius = full_radius - stroke_width;
  PathStrokeType outer_stroke =
      PathStrokeType(stroke_width, PathStrokeType::beveled, PathStrokeType::butt);

  float knob_radius = 0.65f * full_radius;
  float current_angle = start_angle + slider_t * (end_angle - start_angle);
  float end_x = full_radius + 0.8f * knob_radius * sin(current_angle);
  float end_y = full_radius - 0.8f * knob_radius * cos(current_angle);

  if (slider.getInterval() == 1) {
    static const float TEXT_W_PERCENT = 0.35f;
    Rectangle<float> text_bounds(1.0f + width * (1.0f - TEXT_W_PERCENT) / 2.0f,
                                 0.5f * height, width * TEXT_W_PERCENT, 0.5f * height);

    g.setColour(Colour(0xff464646));
    g.fillRoundedRectangle(text_bounds, 2.0f);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_regular.withPointHeight(0.2f * height));
    g.drawFittedText(String(slider.getValue()), text_bounds.getSmallestIntegerContainer(),
                     Justification::horizontallyCentred | Justification::bottom, 1);
  }

  Path active_section;
  bool bipolar = false;
  bool active = true;
  SynthSlider* s_slider = dynamic_cast<SynthSlider*>(&slider);
  if (s_slider) {
    bipolar = s_slider->isBipolar();
    active = s_slider->isActive();
  }

  Path rail;
  rail.addCentredArc(full_radius, full_radius, outer_radius, outer_radius,
                     0.0f, start_angle, end_angle, true);

  if (active)
    g.setColour(Colour(0xff4a4a4a));
  else
    g.setColour(Colour(0xff333333));

  g.strokePath(rail, outer_stroke);

  if (bipolar) {
    active_section.addCentredArc(full_radius, full_radius, outer_radius, outer_radius,
                                 0.0f, 0.0f, current_angle - 2.0f * mopo::PI, true);
  }
  else {
    active_section.addCentredArc(full_radius, full_radius, outer_radius, outer_radius,
                                 0.0f, start_angle, current_angle, true);
  }

  if (active)
    g.setColour(Colour(0xffffab00));
  else
    g.setColour(Colour(0xff555555));

  g.strokePath(active_section, outer_stroke);

  if (active)
    g.setColour(Colour(0xff000000));
  else
    g.setColour(Colour(0xff444444));

  g.fillEllipse(full_radius - knob_radius + stroke_width / 2.0f,
                full_radius - knob_radius + stroke_width / 2.0f,
                2.0f * knob_radius - stroke_width,
                2.0f * knob_radius - stroke_width);

  if (active)
    g.setColour(Colour(0xff666666));
  else
    g.setColour(Colour(0xff555555));

  g.drawEllipse(full_radius - knob_radius + 1.0f, full_radius - knob_radius + 1.0f,
                2.0f * knob_radius - 2.0f, 2.0f * knob_radius - 2.0f, 2.0f);

  g.setColour(Colour(0xff999999));
  g.drawLine(full_radius, full_radius, end_x, end_y, 1.0f);
}

void DefaultLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
                                          bool isMouseOverButton, bool isButtonDown) {
  static const DropShadow shadow(Colour(0x88000000), 1.0f, Point<int>(0, 0));
  static float stroke_percent = 0.1;
  static float padding = 3.0f;
  static float hover_padding = 1.0f;

  float full_radius = std::min(button.getWidth(), button.getHeight()) / 2.0;
  float stroke_width = 2.0f * full_radius * stroke_percent;
  PathStrokeType stroke_type(stroke_width, PathStrokeType::beveled, PathStrokeType::rounded);
  float outer_radius = full_radius - stroke_width - padding;
  Path outer;
  outer.addCentredArc(full_radius, full_radius, outer_radius, outer_radius,
                      mopo::PI, -POWER_ARC_ANGLE, POWER_ARC_ANGLE, true);

  Path shadow_path;
  stroke_type.createStrokedPath(shadow_path, outer);
  shadow.drawForPath(g, shadow_path);
  Rectangle<int> bar_shadow_rect(full_radius - 1.0f, padding, 2.0f, full_radius - padding);
  shadow.drawForRectangle(g, bar_shadow_rect);

  if (button.getToggleState())
    g.setColour(Colours::white);
  else
    g.setColour(Colours::grey);

  g.strokePath(outer, stroke_type);
  g.fillRoundedRectangle(full_radius - 1.0f, padding, 2.0f, full_radius - padding, 1.0f);

  if (isButtonDown) {
    g.setColour(Colour(0x11000000));
    g.fillEllipse(hover_padding, hover_padding,
                  button.getWidth() - 2 * hover_padding, button.getHeight() - 2 * hover_padding);
  }
  else if (isMouseOverButton) {
    g.setColour(Colour(0x11ffffff));
    g.fillEllipse(hover_padding, hover_padding,
                  button.getWidth() - 2 * hover_padding, button.getHeight() - 2 * hover_padding);  }
}

void DefaultLookAndFeel::fillHorizontalRect(Graphics& g, float x1, float x2, float height) {
  float x = std::min(x1, x2);
  float width = fabsf(x1 - x2);
  g.fillRect(x, 0.0f, width, height);
}

void DefaultLookAndFeel::fillVerticalRect(Graphics& g, float y1, float y2, float width) {
  float y = std::min(y1, y2);
  float height = fabsf(y1 - y2);
  g.fillRect(0.0f, y, width, height);
}
