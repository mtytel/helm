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
#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include "JuceHeader.h"
#include "memory.h"

class Oscilloscope : public Component, public Timer {
  public:
    Oscilloscope();
    ~Oscilloscope();

    void timerCallback() override;
    void paint(Graphics& g) override;
    void paintBackground(Graphics& g);
    void resized() override;

    void resetWavePath();
    void setOutputMemory(const float* memory) { output_memory_ = memory; }
    void showRealtimeFeedback(bool show_feedback = true);

  private:
    const float* output_memory_;
    Path wave_path_;
    Image background_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscilloscope)
};

#endif // OSCILLOSCOPE_H
