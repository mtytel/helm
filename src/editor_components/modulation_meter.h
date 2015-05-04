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

#ifndef MODULATION_METER_H
#define MODULATION_METER_H

#include "JuceHeader.h"
#include "processor.h"
#include "synth_slider.h"

class ModulationMeter : public Component {
public:
    ModulationMeter(const mopo::Processor::Output* mono_total,
                    const mopo::Processor::Output* poly_total,
                    const SynthSlider* slider);
    ~ModulationMeter();

    void paint(Graphics& g) override;
    void resized() override;

    void update(int num_voices);

private:
    void drawSlider(Graphics& g);

    void drawKnob(Graphics& g);
    void fillHorizontalRect(Graphics& g, float x1, float x2, float height);
    void fillVerticalRect(Graphics& g, float y1, float y2, float width);

    const mopo::Processor::Output* mono_total_;
    const mopo::Processor::Output* poly_total_;
    const SynthSlider* destination_;

    double current_knob_percent_;
    double current_mod_percent_;

    PathStrokeType knob_stroke_;
    float full_radius_;
    float outer_radius_;

    float knob_percent_;
    float mod_percent_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationMeter)
};

#endif // MODULATION_METER_H
