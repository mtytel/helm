/* Copyright 2013-2016 Matt Tytel
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
#include "synth_section.h"

class FilterSection : public SynthSection {
  public:
    FilterSection(String name);
    ~FilterSection();

    void paintBackground(Graphics& g) override;
    void resized() override;
    void reset() override;

  private:
    ScopedPointer<FilterSelector> filter_type_;
    ScopedPointer<SynthSlider> cutoff_;
    ScopedPointer<SynthSlider> resonance_;
    ScopedPointer<FilterResponse> filter_response_;
    ScopedPointer<SynthSlider> fil_env_depth_;
    ScopedPointer<SynthSlider> keytrack_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterSection)
};

#endif // FILTER_SECTION_H
