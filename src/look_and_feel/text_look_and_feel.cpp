/* Copyright 2013-2015 Matt Tytel
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

#include "text_look_and_feel.h"

void TextLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                                       float slider_t, float start_angle, float end_angle,
                                       Slider& slider) {
  static Font slider_font(Typeface::createSystemTypefaceFor(BinaryData::DroidSansMono_ttf,
                                                            BinaryData::DroidSansMono_ttfSize));
  static const float text_percentage = 0.7f;
  g.fillAll(Colour(0xff424242));

  g.setColour(Colour(0xff565656));
  g.drawRect(slider.getLocalBounds());

  g.setColour(Colours::white);
  g.setFont(slider_font.withPointHeight(height * text_percentage));
  g.drawText(slider.getTextFromValue(slider.getValue()),
             x, y, width, height, Justification::centred, false);
}

void TextLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
                                       bool isMouseOverButton, bool isButtonDown) {
  if (button.getToggleState())
    g.setColour(Colour(0xffffc400));
  else
    g.setColour(Colour(0xff313131));
  g.fillRect(button.getLocalBounds());

  g.setColour(Colour(0xff565656));
  g.drawRect(button.getLocalBounds());

  if (isButtonDown) {
    g.setColour(Colour(0x11000000));
    g.fillRect(button.getLocalBounds());
  }
  else if (isMouseOverButton) {
    g.setColour(Colour(0x11ffffff));
    g.fillRect(button.getLocalBounds());
  }
}
