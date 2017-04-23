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
#ifndef OSCILLATOR_SECTION_H
#define OSCILLATOR_SECTION_H

#include "JuceHeader.h"
#include "synth_section.h"
#include "wave_selector.h"
#include "wave_viewer.h"

class OscillatorSection : public SynthSection {
  public:
    OscillatorSection(String name);
    ~OscillatorSection();

    void paintBackground(Graphics& g) override;
    void resized() override;
    void reset() override;

  private:
    ScopedPointer<WaveViewer> wave_viewer_1_;
    ScopedPointer<WaveViewer> wave_viewer_2_;
    ScopedPointer<SynthSlider> wave_selector_1_;
    ScopedPointer<SynthSlider> wave_selector_2_;
    ScopedPointer<SynthSlider> transpose_1_;
    ScopedPointer<SynthSlider> transpose_2_;
    ScopedPointer<SynthSlider> tune_1_;
    ScopedPointer<SynthSlider> tune_2_;
    ScopedPointer<SynthSlider> unison_voices_1_;
    ScopedPointer<SynthSlider> unison_voices_2_;
    ScopedPointer<SynthSlider> unison_detune_1_;
    ScopedPointer<SynthSlider> unison_detune_2_;
    ScopedPointer<ToggleButton> unison_harmonize_1_;
    ScopedPointer<ToggleButton> unison_harmonize_2_;
    ScopedPointer<SynthSlider> cross_modulation_;

    Path top_left_cross_path_;
    Path top_right_cross_path_;
    Path bottom_left_cross_path_;
    Path bottom_right_cross_path_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorSection)
};

#endif // OSCILLATOR_SECTION_H
