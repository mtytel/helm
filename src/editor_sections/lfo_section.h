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
#ifndef LFO_SECTION_H
#define LFO_SECTION_H

#include "JuceHeader.h"
#include "synth_section.h"
#include "tempo_selector.h"
#include "wave_selector.h"
#include "wave_viewer.h"

class LfoSection : public SynthSection {
  public:
    LfoSection(String name, std::string value_preprend);
    ~LfoSection();

    void paintBackground(Graphics& g) override;
    void resized() override;

  private:
    ScopedPointer<WaveViewer> wave_viewer_;
    ScopedPointer<WaveSelector> wave_selector_;

    ScopedPointer<SynthSlider> amplitude_;
    ScopedPointer<SynthSlider> frequency_;
    ScopedPointer<SynthSlider> tempo_;
    ScopedPointer<TempoSelector> sync_;
    ScopedPointer<ModulationButton> modulation_button_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LfoSection)
};

#endif // LFO_SECTION_H
