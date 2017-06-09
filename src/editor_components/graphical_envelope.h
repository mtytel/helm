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
#ifndef GRAPHICAL_ENVELOPE_H
#define GRAPHICAL_ENVELOPE_H

#include "JuceHeader.h"
#include "helm_common.h"
#include "synth_slider.h"

class GraphicalEnvelope : public Component, SynthSlider::SliderListener {
  public:
    GraphicalEnvelope();
    ~GraphicalEnvelope();

    void resetEnvelopeLine();
    void guiChanged(SynthSlider* slider) override;

    void setAttackSlider(SynthSlider* attack_slider);
    void setDecaySlider(SynthSlider* decay_slider);
    void setSustainSlider(SynthSlider* sustain_slider);
    void setReleaseSlider(SynthSlider* release_slider);

    void paint(Graphics& g) override;
    void resized() override;
    void mouseMove(const MouseEvent& e) override;
    void mouseExit(const MouseEvent& e) override;
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;

  private:
    float getAttackX();
    float getDecayX();
    float getSustainY();
    float getReleaseX();

    void setAttackX(double x);
    void setDecayX(double x);
    void setSustainY(double y);
    void setReleaseX(double x);

    bool attack_hover_;
    bool decay_hover_;
    bool sustain_hover_;
    bool release_hover_;
    bool mouse_down_;
    Path envelope_line_;

    SynthSlider* attack_slider_;
    SynthSlider* decay_slider_;
    SynthSlider* sustain_slider_;
    SynthSlider* release_slider_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicalEnvelope)
};

#endif // GRAPHICAL_ENVELOPE_H
