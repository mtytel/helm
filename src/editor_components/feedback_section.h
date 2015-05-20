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
#ifndef FEEDBACK_SECTION_H
#define FEEDBACK_SECTION_H

#include "JuceHeader.h"
#include "synth_section.h"
#include "synth_slider.h"

class FeedbackSection : public SynthSection {
  public:
    FeedbackSection(String name);
    ~FeedbackSection();

    void paint(Graphics& g) override;
    void resized() override;

  private:
    ScopedPointer<SynthSlider> feedback_transpose_;
    ScopedPointer<SynthSlider> feedback_tune_;
    ScopedPointer<SynthSlider> feedback_amount_;
    ScopedPointer<SynthSlider> saturation_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FeedbackSection)
};

#endif // FEEDBACK_SECTION_H
