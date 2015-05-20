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
#ifndef FILTER_RESPONSE_H
#define FILTER_RESPONSE_H

#include "JuceHeader.h"
#include "twytch_common.h"
#include "filter.h"

class FilterResponse : public Component, SliderListener {
  public:
    FilterResponse (int resolution);
    ~FilterResponse();

    float getPercentForMidiNote(float midi_note);
    void resetResponsePath();
    void computeFilterCoefficients();
    void setFilterSettingsFromPosition(Point<int> position);
    void sliderValueChanged(Slider* moved_slider) override;

    void setResonanceSlider(Slider* slider);
    void setCutoffSlider(Slider* slider);
    void setFilterTypeSlider(Slider* slider);

    void paint (Graphics& g);
    void resized();
    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);

  private:
    Path filter_response_path_;
    int resolution_;

    mopo::Filter filter_;

    Slider* filter_type_slider_;
    Slider* cutoff_slider_;
    Slider* resonance_slider_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterResponse)
};

#endif // FILTER_RESPONSE_H
