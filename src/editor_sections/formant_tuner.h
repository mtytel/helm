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
#ifndef FORMANT_TUNER_H
#define FORMANT_TUNER_H

#include "JuceHeader.h"
#include "formant_response.h"
#include "synth_section.h"

class FormantTuner : public SynthSection {
  public:
    FormantTuner(String name);
    ~FormantTuner();

    void paintBackground(Graphics& g) override;
    void resized() override;

    static const int kNumFormants = 4;

  private:
    ScopedPointer<SynthSlider> cutoffs_[kNumFormants];
    ScopedPointer<SynthSlider> resonances_[kNumFormants];
    ScopedPointer<SynthSlider> gains_[kNumFormants];

    ScopedPointer<FormantResponse> formant_response_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FormantTuner)
};

#endif // FORMANT_TUNER_H
