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
#ifndef FORMANT_SECTION_H
#define FORMANT_SECTION_H

#include "JuceHeader.h"
#include "synth_button.h"
#include "synth_section.h"
#include "xy_pad.h"

class FormantSection : public SynthSection {
  public:
    FormantSection(String name);
    ~FormantSection();

    void paintBackground(Graphics& g) override;
    void resized() override;
    void setActive(bool active = true) override;

  private:
    ScopedPointer<SynthButton> on_;
    ScopedPointer<SynthSlider> x_;
    ScopedPointer<SynthSlider> y_;
    ScopedPointer<XYPad> xy_pad_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FormantSection)
};

#endif // FORMANT_SECTION_H
