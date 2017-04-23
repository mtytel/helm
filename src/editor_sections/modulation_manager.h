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

#pragma once
#ifndef MODULATION_MANAGER_H
#define MODULATION_MANAGER_H

#include "JuceHeader.h"

#include "helm_common.h"
#include "modulation_button.h"
#include "synth_section.h"
#include "synth_slider.h"
#include <set>

class ModulationHighlight;
class ModulationMeter;

class ModulationManager : public SynthSection, public Timer,
                          public ModulationButton::ModulationDisconnectListener,
                          public SynthSlider::SliderListener {
  public:
    ModulationManager (mopo::output_map modulation_sources,
                       std::map<std::string, ModulationButton*> modulation_buttons,
                       std::map<std::string, SynthSlider*> sliders,
                       mopo::output_map mono_modulations,
                       mopo::output_map poly_modulations);
    ~ModulationManager();

    void setModulationAmount(std::string source, std::string destination, mopo::mopo_float amount);
    void changeModulator(std::string new_modulator);
    void forgetModulator();
    std::string getCurrentModulator() { return current_modulator_; }
    void reset() override;

    void timerCallback() override;
    void updateModulationValues();
    void paint(Graphics& g) override;
    void resized() override;
    void buttonClicked(Button* clicked_button) override;
    void sliderValueChanged(Slider* moved_slider) override;
    void modulationDisconnected(mopo::ModulationConnection* connection, bool last) override;

    // SynthSlider::SliderListener
    void hoverStarted(const std::string& name) override;
    void hoverEnded(const std::string& name) override;
    void modulationsChanged(const std::string& name) override;

  private:
    void makeModulationsVisible(std::string destination, bool visible);
    void setSliderValues();

    ScopedPointer<Component> polyphonic_destinations_;
    ScopedPointer<Component> monophonic_destinations_;

    std::string current_modulator_;
    double last_value_;
    std::map<std::string, ModulationButton*> modulation_buttons_;

    std::map<std::string, Slider*> slider_lookup_;
    std::map<std::string, SynthSlider*> slider_model_lookup_;
    std::vector<Slider*> owned_sliders_;

    std::map<std::string, ModulationMeter*> meter_lookup_;
    std::map<std::string, ModulationHighlight*> overlay_lookup_;
    mopo::output_map modulation_sources_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationManager)
};

#endif // MODULATION_MANAGER_H
