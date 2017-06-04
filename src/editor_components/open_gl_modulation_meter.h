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

#ifndef OPEN_GL_MODULATION_METER_H
#define OPEN_GL_MODULATION_METER_H

#include "JuceHeader.h"
#include "open_gl_component.h"
#include "processor.h"
#include "synth_slider.h"

class OpenGLModulationMeter : public Component {
  public:
    OpenGLModulationMeter(const mopo::Output* mono_total,
                          const mopo::Output* poly_total,
                          const SynthSlider* slider,
                          float* vertices);
    virtual ~OpenGLModulationMeter();

    void paint(Graphics& g) override;
    void resized() override;
    void setVisible(bool should_be_visible) override;

    void updateDrawing();

    bool isModulated() { return modulated_; }
    void setModulated(bool modulated) { modulated_ = modulated; }

  private:
    void setVertices();
    void collapseVertices();

    const mopo::Output* mono_total_;
    const mopo::Output* poly_total_;
    const SynthSlider* destination_;
    float* vertices_;

    double current_value_;
    double knob_percent_;
    double mod_percent_;

    float full_radius_;
    float outer_radius_;
    bool modulated_;
    bool rotary_;

    float left_, right_, top_, bottom_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGLModulationMeter)
};

#endif // OPEN_GL_MODULATION_METER_H
