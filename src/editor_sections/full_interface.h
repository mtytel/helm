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
#ifndef FULL_INTERFACE_H
#define FULL_INTERFACE_H

#include "JuceHeader.h"

#include "synth_section.h"
#include "modulation_manager.h"
#include "synthesis_interface.h"
#include "oscilloscope.h"
#include "global_tool_tip.h"
#include "patch_browser.h"

class FullInterface : public SynthSection {
  public:
    FullInterface(mopo::control_map controls, mopo::output_map modulation_sources,
                  mopo::output_map mono_modulations, mopo::output_map poly_modulations);
    ~FullInterface();

    void setValue(std::string name, mopo::mopo_float value,
                  NotificationType notification = sendNotificationAsync);
    void setOutputMemory(const mopo::Memory* output_memory);

    void createModulationSliders(mopo::output_map modulation_sources,
                                 mopo::output_map mono_modulations,
                                 mopo::output_map poly_modulations);

    void setToolTipText(String parameter, String value);

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);

  private:
    std::map<std::string, SynthSlider*> slider_lookup_;
    std::map<std::string, Button*> button_lookup_;
    ScopedPointer<ModulationManager> modulation_manager_;
    TooltipWindow tooltip_;
    ScopedPointer<SynthSlider> arp_tempo_;

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
    ScopedPointer<PatchBrowser> patch_browser_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FullInterface)
};

#endif // FULL_INTERFACE_H
