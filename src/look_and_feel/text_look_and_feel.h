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

#ifndef TEXT_LOOK_AND_FEEL_H
#define TEXT_LOOK_AND_FEEL_H

#include "JuceHeader.h"
#include "default_look_and_feel.h"

class TextLookAndFeel : public DefaultLookAndFeel {
  public:
    TextLookAndFeel();
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                          float slider_t, float start_angle, float end_angle,
                          Slider& slider) override;

    void drawToggleButton(Graphics& g, ToggleButton& button,
                          bool hover, bool is_down) override;

    void drawTickBox(Graphics &, Component &, float x, float y, float w, float h, bool ticked,
                     bool enabled, bool mouse_over, bool button_down) override;

    static TextLookAndFeel* instance() {
      static TextLookAndFeel instance;
      return &instance;
    }
};

#endif // TEXT_LOOK_AND_FEEL_H
