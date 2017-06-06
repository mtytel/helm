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

#ifndef DEFAULT_LOOK_AND_FEEL_H
#define DEFAULT_LOOK_AND_FEEL_H

#include "JuceHeader.h"

class DefaultLookAndFeel : public juce::LookAndFeel_V3 {
  public:
    void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                          float slider_pos, float min, float max,
                          const Slider::SliderStyle style, Slider& slider) override;

    void drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
                               float slider_pos, float min, float max,
                               const Slider::SliderStyle style, Slider& slider) override;

    virtual void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                                  float slider_t, float start_angle, float end_angle,
                                  Slider& slider) override;

    virtual void drawToggleButton(Graphics& g, ToggleButton& button,
                                  bool hover, bool is_down) override;

    void drawButtonBackground(Graphics& g, Button& button,
                              const Colour &backgroundColour,
                              bool hover,
                              bool is_down) override;

    void drawButtonText(Graphics& g, TextButton& button,
                        bool hover, bool is_down) override;

    void fillHorizontalRect(Graphics& g, float x1, float x2, float height);
    void fillVerticalRect(Graphics& g, float y1, float y2, float width);
    void fillSplitHorizontalRect(Graphics& g, float x1, float x2, float height, Colour fill_color);
    void fillSplitVerticalRect(Graphics& g, float y1, float y2, float width, Colour fill_color);

    int getSliderPopupPlacement(Slider& slider) override;

    Font getPopupMenuFont() override;
    Font getSliderPopupFont(Slider& slider) override;

    static DefaultLookAndFeel* instance() {
      static DefaultLookAndFeel instance;
      return &instance;
    }

  protected:
    DefaultLookAndFeel();
};

#endif // DEFAULT_LOOK_AND_FEEL_H
