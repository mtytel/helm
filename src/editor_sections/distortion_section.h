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
#ifndef DISTORTION_SECTION_H
#define DISTORTION_SECTION_H

#include "JuceHeader.h"
#include "synth_section.h"
#include "synth_button.h"
#include "synth_slider.h"
#include "text_selector.h"

class DistortionSection : public SynthSection {
  public:
    DistortionSection(String name);
    ~DistortionSection();

    void paintBackground(Graphics& g) override;
    void resized() override;

  private:
    ScopedPointer<SynthButton> on_;
    ScopedPointer<TextSelector> type_;
    ScopedPointer<SynthSlider> drive_;
    ScopedPointer<SynthSlider> mix_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionSection)
};

#endif // DISTORTION_SECTION_H
