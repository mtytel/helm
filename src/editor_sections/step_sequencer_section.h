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
#ifndef STEP_SEQUENCER_SECTION_H
#define STEP_SEQUENCER_SECTION_H

#include "JuceHeader.h"
#include "retrigger_selector.h"
#include "synth_section.h"
#include "tempo_selector.h"
#include "graphical_step_sequencer.h"

class StepSequencerSection : public SynthSection {
  public:
    StepSequencerSection(String name);
    ~StepSequencerSection();

    void paintBackground(Graphics& g) override;
    void resized() override;
    void reset() override;
    void animate(bool animate = true) override;

  private:
    void createStepSequencerSliders();

    std::vector<Slider*> sequencer_sliders_;
    ScopedPointer<GraphicalStepSequencer> step_sequencer_;
    ScopedPointer<RetriggerSelector> retrigger_;
    ScopedPointer<SynthSlider> num_steps_;
    ScopedPointer<SynthSlider> frequency_;
    ScopedPointer<SynthSlider> tempo_;
    ScopedPointer<TempoSelector> sync_;
    ScopedPointer<SynthSlider> smoothing_;
    ScopedPointer<ModulationButton> modulation_button_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StepSequencerSection)
};

#endif // STEP_SEQUENCER_SECTION_H
