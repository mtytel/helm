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

#ifndef TEXT_SLIDER_H
#define TEXT_SLIDER_H

#include "JuceHeader.h"
#include "synth_slider.h"

class TextSlider : public SynthSlider {
  public:
    TextSlider(String name);

    void paint(Graphics& g) override;
    void resized() override;

    void mouseEvent(const MouseEvent& e);
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;

    void setShortStringLookup(const std::string* lookup) { short_lookup_ = lookup; }

  private:
    const std::string* short_lookup_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextSlider)
};

#endif // TEXT_SLIDER_H
