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
#ifndef FORMANT_RESPONSE_H
#define FORMANT_RESPONSE_H

#include "JuceHeader.h"
#include "helm_common.h"
#include "formant_manager.h"

class FormantResponse : public Component, SliderListener {
  public:
    FormantResponse(int resolution);
    ~FormantResponse();

    float getPercentForMidiNote(float midi_note);
    void resetResponsePath();
    void computeFilterCoefficients();
    void sliderValueChanged(Slider* moved_slider) override;

    void setResonanceSliders(std::vector<Slider*> sliders);
    void setCutoffSliders(std::vector<Slider*> sliders);
    void setGainSliders(std::vector<Slider*> sliders);

    void paint(Graphics& g) override;
    void paintBackground(Graphics& g);
    void resized() override;

    void mouseMove(const MouseEvent& e) override;

  private:
    Path filter_response_path_;
    int resolution_;

    mopo::FormantManager formant_filter_;

    std::vector<Slider*> cutoff_sliders_;
    std::vector<Slider*> resonance_sliders_;
    std::vector<Slider*> gain_sliders_;

    float midi_;
    float frequency_;
    float response_;
    float decibels_;

    Image background_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FormantResponse)
};

#endif // FORMANT_RESPONSE_H
