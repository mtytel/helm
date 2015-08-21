/* Copyright 2013-2015 Matt Tytel
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
#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include "JuceHeader.h"
#include "memory.h"

class Oscilloscope : public AnimatedAppComponent {
  public:
    Oscilloscope(int num_samples);
    ~Oscilloscope();

    void paint(Graphics& g) override;
    void resized() override;
    void update() override;

    void resetWavePath();
    void setOutputMemory(const mopo::Memory* memory) { output_memory_ = memory; }

  private:
    const mopo::Memory* output_memory_;
    int samples_to_show_;
    Path wave_path_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscilloscope)
};

#endif // OSCILLOSCOPE_H
