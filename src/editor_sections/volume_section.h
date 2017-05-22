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
#ifndef VOLUME_SECTION_H
#define VOLUME_SECTION_H

#include "JuceHeader.h"
#include "synth_section.h"
#include "open_gl_peak_meter.h"

class VolumeSection : public SynthSection {
  public:
    VolumeSection(String name);
    ~VolumeSection();

    void resized() override;

  private:
    ScopedPointer<SynthSlider> volume_;
    ScopedPointer<OpenGLPeakMeter> peak_meter_left_;
    ScopedPointer<OpenGLPeakMeter> peak_meter_right_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VolumeSection)
};

#endif // VOLUME_SECTION_H
