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

#pragma once
#ifndef FULL_INTERFACE_H
#define FULL_INTERFACE_H

#include "JuceHeader.h"

#include "about_section.h"
#include "arp_section.h"
#include "global_tool_tip.h"
#include "modulation_manager.h"
#include "oscilloscope.h"
#include "patch_browser.h"
#include "patch_selector.h"
#include "synthesis_interface.h"
#include "synth_section.h"
#include "update_check_section.h"

class FullInterface : public SynthSection {
  public:
    FullInterface(mopo::control_map controls, mopo::output_map modulation_sources,
                  mopo::output_map mono_modulations, mopo::output_map poly_modulations);
    ~FullInterface();

    void setOutputMemory(const mopo::Memory* output_memory);

    void createModulationSliders(mopo::output_map modulation_sources,
                                 mopo::output_map mono_modulations,
                                 mopo::output_map poly_modulations);

    void setToolTipText(String parameter, String value);

    void paintBackground(Graphics& g) override;
    void resized();
    void buttonClicked(Button* clicked_button) override;

  private:
    std::map<std::string, SynthSlider*> slider_lookup_;
    std::map<std::string, Button*> button_lookup_;
    ScopedPointer<ModulationManager> modulation_manager_;
    TooltipWindow tooltip_;
    ScopedPointer<SynthSlider> arp_tempo_;

    ScopedPointer<AboutSection> about_section_;
    ScopedPointer<UpdateCheckSection> update_check_section_;
    ScopedPointer<Component> standalone_settings_section_;
    ScopedPointer<ImageButton> logo_button_;
    ScopedPointer<ArpSection> arp_section_;
    ScopedPointer<SynthesisInterface> synthesis_interface_;
    ScopedPointer<SynthSlider> arp_frequency_;
    ScopedPointer<SynthSlider> arp_gate_;
    ScopedPointer<SynthSlider> arp_octaves_;
    ScopedPointer<SynthSlider> arp_pattern_;
    ScopedPointer<Oscilloscope> oscilloscope_;
    ScopedPointer<ToggleButton> arp_on_;
    ScopedPointer<SynthSlider> beats_per_minute_;
    ScopedPointer<GlobalToolTip> global_tool_tip_;
    ScopedPointer<TempoSelector> arp_sync_;
    ScopedPointer<PatchSelector> patch_selector_;
    ScopedPointer<PatchBrowser> patch_browser_;
    ScopedPointer<SaveSection> save_section_;
    ScopedPointer<DeleteSection> delete_section_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FullInterface)
};

#endif // FULL_INTERFACE_H
