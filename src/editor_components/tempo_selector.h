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

#ifndef TEMPO_SELECTOR_H
#define TEMPO_SELECTOR_H

#include "JuceHeader.h"
#include "synth_slider.h"

class TempoSelector : public SynthSlider {
  public:
    TempoSelector(String name);

    void mouseDown(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void paint(Graphics& g) override;
    void valueChanged() override;
    void resized() override;

    void setFreeSlider(Slider* slider);
    void setTempoSlider(Slider* slider);

  private:
    Slider* free_slider_;
    Slider* tempo_slider_;

    Path arrow_;
    Path clock_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TempoSelector)
};

#endif // TEMPO_SELECTOR_H
