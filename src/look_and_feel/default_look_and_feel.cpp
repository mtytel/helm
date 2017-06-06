/* Copyright 2013-2017 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "default_look_and_feel.h"
#include "colors.h"
#include "fonts.h"
#include "synth_slider.h"
#include "utils.h"

#define POWER_ARC_ANGLE 2.5

DefaultLookAndFeel::DefaultLookAndFeel() {
  setColour(PopupMenu::backgroundColourId, Colour(0xff333333));
  setColour(PopupMenu::textColourId, Colour(0xffcccccc));
  setColour(PopupMenu::headerTextColourId, Colour(0xff333333));
  setColour(PopupMenu::highlightedBackgroundColourId, Colour(0xff111111));
  setColour(PopupMenu::highlightedTextColourId, Colour(0xffcccccc));
  setColour(BubbleComponent::backgroundColourId, Colour(0xff222222));
  setColour(TooltipWindow::textColourId, Colour(0xffdddddd));
}
                                                
void DefaultLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                                          float slider_pos, float min, float max,
                                          const Slider::SliderStyle style, Slider& slider) {
  static const DropShadow thumb_shadow(Colour(0x88000000), 3, Point<int>(-1, 0));

  bool bipolar = false;
  bool flip_coloring = false;
  bool active = true;
  SynthSlider* s_slider = dynamic_cast<SynthSlider*>(&slider);
  if (s_slider) {
    bipolar = s_slider->isBipolar();
    flip_coloring = s_slider->isFlippedColor();
    active = s_slider->isActive();
  }

  Colour slider_color(0xff888888);
  Colour lighten_color(0x55ffffff);
  Colour thumb_color(0xffffffff);

  if (!active) {
    slider_color = Colour(0xff424242);
    thumb_color = Colour(0xff888888);
    lighten_color = Colour(0x22ffffff);
  }

  float pos = slider_pos - 1.0f;
  if (style == Slider::SliderStyle::LinearBar) {
    g.setColour(Colour(0x22000000));
    float w = slider.getWidth();
    float h = slider.getHeight();
    g.fillRect(0.0f, 0.0f, w, h);

    g.setColour(Colour(0xff2a2a2a));
    fillSplitHorizontalRect(g, 0.0f, w, h, Colours::transparentBlack);

    g.setColour(slider_color);
    if (bipolar)
      fillSplitHorizontalRect(g, w / 2.0f, pos, h, lighten_color);
    else if (flip_coloring)
      fillSplitHorizontalRect(g, pos, w - pos, h, lighten_color);
    else
      fillSplitHorizontalRect(g, 0.0f, pos, h, lighten_color);

    thumb_shadow.drawForRectangle(g, Rectangle<int>(pos + 0.5f, 0, 2, h));
    g.setColour(thumb_color);
    g.fillRect(pos, 0.0f, 2.0f, h);
  }
  else if (style == Slider::SliderStyle::LinearBarVertical) {
    g.setColour(Colour(0x22000000));
    float w = slider.getWidth();
    float h = slider.getHeight();
    g.fillRect(0.0f, 0.0f, w, h);

    g.setColour(Colour(0xff2a2a2a));
    fillSplitVerticalRect(g, 0.0f, h, w, Colours::transparentBlack);

    g.setColour(slider_color);
    if (bipolar)
      fillSplitVerticalRect(g, h / 2.0f, pos, w, lighten_color);
    else if (flip_coloring)
      fillSplitVerticalRect(g, h + 1, pos, w, lighten_color);
    else
      fillSplitVerticalRect(g, 0, pos, w, lighten_color);

    thumb_shadow.drawForRectangle(g, Rectangle<int>(0, pos + 0.5f, w, 2));
    g.setColour(thumb_color);
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
  static const float stroke_percent = 0.1f;
  
  float full_radius = std::min(width / 2.0f, height / 2.0f);
  float stroke_width = 2.0f * full_radius * stroke_percent;
  float knob_radius = 0.63f * full_radius;
  float small_outer_radius = knob_radius + stroke_width / 6.0f;
  PathStrokeType outer_stroke =
      PathStrokeType(stroke_width, PathStrokeType::beveled, PathStrokeType::butt);

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
    g.setFont(Fonts::instance()->proportional_regular().withPointHeight(0.2f * height));
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
  rail.addCentredArc(full_radius, full_radius, small_outer_radius, small_outer_radius,
                     0.0f, start_angle, end_angle, true);

  if (active)
    g.setColour(Colour(0xff4a4a4a));
  else
    g.setColour(Colour(0xff333333));

  g.strokePath(rail, outer_stroke);

  if (bipolar) {
    active_section.addCentredArc(full_radius, full_radius, small_outer_radius, small_outer_radius,
                                 0.0f, 0.0f, current_angle - 2.0f * mopo::PI, true);
  }
  else {
    active_section.addCentredArc(full_radius, full_radius, small_outer_radius, small_outer_radius,
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

  g.fillEllipse(full_radius - knob_radius,
                full_radius - knob_radius,
                2.0f * knob_radius,
                2.0f * knob_radius);

  if (active)
    g.setColour(Colour(0xff666666));
  else
    g.setColour(Colour(0xff555555));

  g.drawEllipse(full_radius - knob_radius + stroke_width / 4.0f + 0.5f,
                full_radius - knob_radius + stroke_width / 4.0f + 0.5f,
                2.0f * knob_radius - stroke_width / 2.0f - 1.0f,
                2.0f * knob_radius - stroke_width / 2.0f - 1.0f, 1.5f);

  g.setColour(Colour(0xff999999));
  g.drawLine(full_radius, full_radius, end_x, end_y, 1.0f);
}

void DefaultLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
                                          bool hover, bool is_down) {
  static const DropShadow shadow(Colour(0x88000000), 1.0f, Point<int>(0, 0));
  static float stroke_percent = 0.1f;
  float ratio = button.getWidth() / 20.0f;
  float padding = ratio * 3.0f;
  float hover_padding = ratio;

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

  if (is_down) {
    g.setColour(Colour(0x11000000));
    g.fillEllipse(hover_padding, hover_padding,
                  button.getWidth() - 2 * hover_padding, button.getHeight() - 2 * hover_padding);
  }
  else if (hover) {
    g.setColour(Colour(0x11ffffff));
    g.fillEllipse(hover_padding, hover_padding,
                  button.getWidth() - 2 * hover_padding, button.getHeight() - 2 * hover_padding);  }
}

void DefaultLookAndFeel::drawButtonBackground(Graphics& g, Button& button,
                                              const Colour &backgroundColour,
                                              bool hover,
                                              bool is_down) {
  if (button.isEnabled())
    g.fillAll(Colour(0xff323232));
  else
    g.fillAll(Colour(0xff484848));

  g.setColour(Colour(0xff505050));
  g.drawRect(button.getLocalBounds());

  if (is_down)
    g.fillAll(Colour(0x11000000));
  else if (hover)
    g.fillAll(Colour(0x11ffffff));
}

void DefaultLookAndFeel::drawButtonText(Graphics& g, TextButton& button,
                                        bool hover, bool is_down) {
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(14.0f));
  if (button.isEnabled())
    g.setColour(Colour(0xffaaaaaa));
  else
    g.setColour(Colour(0xff666666));

  g.drawFittedText(button.getName(), button.getLocalBounds(), Justification::centred, 1);
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

void DefaultLookAndFeel::fillSplitHorizontalRect(Graphics& g, float x1, float x2, float height,
                                                 Colour fill_color) {
  float h = (height - SynthSlider::linear_rail_width) / 2.0f;
  float x = std::min(x1, x2);
  float width = fabsf(x1 - x2);

  g.saveState();
  g.setColour(fill_color);
  g.fillRect(x, 0.0f, width, height);
  g.restoreState();

  g.fillRect(x, 0.0f, width, h);
  g.fillRect(x, h + SynthSlider::linear_rail_width, width, h);
}

void DefaultLookAndFeel::fillSplitVerticalRect(Graphics& g, float y1, float y2, float width,
                                               Colour fill_color) {
  float w = (width - SynthSlider::linear_rail_width) / 2.0f;
  float y = std::min(y1, y2);
  float height = fabsf(y1 - y2);

  g.saveState();
  g.setColour(fill_color);
  g.fillRect(0.0f, y, width, height);
  g.restoreState();

  g.fillRect(0.0f, y, w, height);
  float x2 = w + SynthSlider::linear_rail_width;
  g.fillRect(x2, y, width - x2, height);
}

int DefaultLookAndFeel::getSliderPopupPlacement(Slider& slider) {
  SynthSlider* s_slider = dynamic_cast<SynthSlider*>(&slider);
  if (s_slider)
    return s_slider->getPopupPlacement();

  return LookAndFeel_V3::getSliderPopupPlacement(slider);
}

Font DefaultLookAndFeel::getPopupMenuFont() {
  return Fonts::instance()->proportional_regular().withPointHeight(14.0f);
}

Font DefaultLookAndFeel::getSliderPopupFont(Slider& slider) {
  return Fonts::instance()->proportional_regular().withPointHeight(14.0f);
}
