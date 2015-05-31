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
#include "modulation_button.h"
#include "twytch_common.h"
#include <map>

#define MODULATION_BUTTON_WIDTH 32

class SynthSlider;

class SynthSection : public Component, public SliderListener, public ButtonListener {
  public:
    SynthSection(String name) : Component(name) { }

    // Drawing.
    virtual void resized() override;
    virtual void paint(Graphics& g) override;
    virtual void paintBackground();
    void paintKnobShadows();
    void drawTextForComponent(Graphics& g, String text, Component* component);

    // Widget Listeners.
    virtual void sliderValueChanged(Slider* moved_slider) override;
    virtual void buttonClicked(Button* clicked_button) override;

    std::map<std::string, SynthSlider*> getAllSliders() { return all_sliders_; }
    std::map<std::string, Button*> getAllButtons() { return all_buttons_; }
    std::map<std::string, ModulationButton*> getAllModulationButtons() {
      return all_modulation_buttons_;
    }

    virtual void setActive(bool active = true);
    void setAllValues(mopo::control_map& controls);
    void setValue(std::string name, mopo::mopo_float value,
                  NotificationType notification = sendNotificationAsync);

  protected:
    void addButton(Button* button, bool show = true);
    void addModulationButton(ModulationButton* button, bool show = true);
    void addSlider(SynthSlider* slider, bool show = true);
    void addSubSection(SynthSection* section, bool show = true);

    std::map<std::string, SynthSection*> sub_sections_;

    std::map<std::string, SynthSlider*> slider_lookup_;
    std::map<std::string, Button*> button_lookup_;
    std::map<std::string, ModulationButton*> modulation_buttons_;

    std::map<std::string, SynthSlider*> all_sliders_;
    std::map<std::string, Button*> all_buttons_;
    std::map<std::string, ModulationButton*> all_modulation_buttons_;

    Image background_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthSection)
};

#endif // SYNTH_SECTION_H
