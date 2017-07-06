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
#ifndef FILTER_SECTION_H
#define FILTER_SECTION_H

#include "JuceHeader.h"
#include "filter_selector.h"
#include "filter_response.h"
#include "synth_button.h"
#include "synth_section.h"

class FilterSection : public SynthSection {
  public:
    FilterSection(String name);
    ~FilterSection();

    void paintBackground(Graphics& g) override;
    void resized() override;
    void reset() override;
    void sliderValueChanged(Slider* changed_slider) override;

    void resizeLowPass(float x, float y, float width, float height);
    void resizeHighPass(float x, float y, float width, float height);

    void setActive(bool active) override;
    void resetResponse();

  private:
    ScopedPointer<SynthButton> filter_on_;
    ScopedPointer<FilterSelector> filter_shelf_;
    ScopedPointer<SynthSlider> cutoff_;
    ScopedPointer<SynthSlider> resonance_;
    ScopedPointer<SynthSlider> blend_;
    ScopedPointer<FilterResponse> filter_response_;
    ScopedPointer<SynthSlider> fil_env_depth_;
    ScopedPointer<SynthSlider> keytrack_;
    ScopedPointer<SynthSlider> filter_style_;
    ScopedPointer<SynthSlider> drive_;

    Path low_pass_;
    Path high_pass_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterSection)
};

#endif // FILTER_SECTION_H
