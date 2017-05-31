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
#ifndef ENVELOPE_SECTION_H
#define ENVELOPE_SECTION_H

#include "JuceHeader.h"
#include "open_gl_envelope.h"
#include "synth_section.h"

class EnvelopeSection : public SynthSection {
  public:
    EnvelopeSection(String name, std::string value_preprend);
    ~EnvelopeSection();

    void paintBackground(Graphics& g) override;
    void resized() override;
    void reset() override;

  private:
    ScopedPointer<OpenGLEnvelope> envelope_;
    ScopedPointer<SynthSlider> attack_;
    ScopedPointer<SynthSlider> decay_;
    ScopedPointer<SynthSlider> sustain_;
    ScopedPointer<SynthSlider> release_;
    ScopedPointer<ModulationButton> modulation_button_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeSection)
};

#endif // ENVELOPE_SECTION_H
