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

#ifndef MODULATION_LOOK_AND_FEEL_H
#define MODULATION_LOOK_AND_FEEL_H

#include "JuceHeader.h"

class SynthSlider;

class ModulationLookAndFeel : public LookAndFeel_V3 {
  public:
    void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                          float slider_pos, float min, float max,
                          const Slider::SliderStyle style, Slider& slider) override;

    void drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
                               float slider_pos, float min, float max,
                               const Slider::SliderStyle style, Slider& slider) override;

    void drawTextModulation(Graphics& g, Slider& slider, float percent);

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                          float slider_t, float start_angle, float end_angle,
                          Slider& slider) override;

    void drawToggleButton(Graphics& g, ToggleButton& button,
                          bool hover, bool is_down) override;

    int getSliderPopupPlacement(Slider& slider) override;

    Font getPopupMenuFont() override;
    Font getSliderPopupFont(Slider& slider) override;

    static ModulationLookAndFeel* instance() {
      static ModulationLookAndFeel instance;
      return &instance;
    }

  protected:
    ModulationLookAndFeel();
};

#endif // MODULATION_LOOK_AND_FEEL_H
