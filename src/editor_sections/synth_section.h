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

#pragma once
#ifndef SYNTH_SECTION_H
#define SYNTH_SECTION_H

#include "JuceHeader.h"
#include "twytch_common.h"
#include <map>

class SynthSlider;

class SynthSection : public Component, public SliderListener, public ButtonListener {
  public:
    SynthSection(String name) : Component(name) { }
    virtual void paint(Graphics& g) override;
    virtual void sliderValueChanged(Slider* moved_slider) override;
    virtual void buttonClicked(Button* clicked_button) override;
    void setAllValues(mopo::control_map& controls);
    void drawTextForSlider(Graphics& g, String text, SynthSlider* slider);
    std::map<std::string, SynthSlider*> getAllSliders();

  protected:
    void addButton(Button* button, bool show = true);
    void addSlider(SynthSlider* slider, bool show = true);
    void addSubSection(SynthSection* section, bool show = true);

    std::map<std::string, SynthSlider*> slider_lookup_;
    std::map<std::string, Button*> button_lookup_;
    std::map<std::string, SynthSection*> sub_sections_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthSection)
};

#endif // SYNTH_SECTION_H
