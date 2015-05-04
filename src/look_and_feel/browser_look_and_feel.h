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

#ifndef BROWSER_LOOK_AND_FEEL_H
#define BROWSER_LOOK_AND_FEEL_H

#include "JuceHeader.h"

class BrowswerLookAndFeel : public juce::LookAndFeel_V3 {
  public:
    void drawToggleButton(Graphics& g, ToggleButton& button,
                          bool isMouseOverButton, bool isButtonDown) override;

    static BrowswerLookAndFeel* instance() {
      static BrowswerLookAndFeel instance;
      return &instance;
    }
};

#endif // BROWSER_LOOK_AND_FEEL_H
