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

#include "browser_look_and_feel.h"
#include "fonts.h"

void BrowserLookAndFeel::drawButtonBackground(Graphics& g, Button& button,
                                              const Colour &background_colour,
                                              bool hover, bool is_down) {
  g.setColour(button.findColour(TextButton::buttonColourId));
  g.fillAll();

  if (is_down)
    g.fillAll(Colour(0x11000000));
  else if (hover)
    g.fillAll(Colour(0x11ffffff));
}

void BrowserLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
                                          bool hover, bool is_down) {
  static const float text_percentage = 0.7f;
  static const float max_text_height = 16.0f;

  if (button.getToggleState())
    g.setColour(Colour(0xffffab00));
  else
    g.setColour(Colour(0xff313131));
  g.fillRect(button.getLocalBounds());

  if (button.getToggleState())
    g.setColour(Colours::white);
  else
    g.setColour(Colour(0xffbbbbbb));
  
  int height = button.getHeight();
  float text_height = std::min(max_text_height, height * text_percentage);
  g.setFont(Fonts::instance()->monospace().withPointHeight(text_height));
  g.drawText(button.getButtonText(), 0, 0,
             button.getWidth(), button.getHeight(), Justification::centred);

  g.setColour(Colour(0xff565656));
  g.drawRect(button.getLocalBounds());

  if (is_down) {
    g.setColour(Colour(0x11000000));
    g.fillRect(button.getLocalBounds());
  }
  else if (hover) {
    g.setColour(Colour(0x11ffffff));
    g.fillRect(button.getLocalBounds());
  }
}

Font BrowserLookAndFeel::getTextButtonFont(TextButton& button, int button_height) {
  static const float percent = 0.55f;
  int min = std::min(button_height, button.getWidth());
  return Fonts::instance()->monospace().withPointHeight(min * percent);
}
