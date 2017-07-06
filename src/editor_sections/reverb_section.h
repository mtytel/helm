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
#ifndef REVERB_SECTION_H
#define REVERB_SECTION_H

#include "JuceHeader.h"
#include "synth_button.h"
#include "synth_section.h"
#include "synth_slider.h"

class ReverbSection : public SynthSection {
  public:
    ReverbSection(String name);
    ~ReverbSection();

    void paintBackground(Graphics& g) override;
    void resized() override;

  private:
    ScopedPointer<SynthButton> on_;
    ScopedPointer<SynthSlider> feedback_;
    ScopedPointer<SynthSlider> damping_;
    ScopedPointer<SynthSlider> dry_wet_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbSection)
};

#endif // REVERB_SECTION_H
