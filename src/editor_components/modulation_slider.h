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

#ifndef MODULATIONS_SLIDER_H
#define MODULATIONS_SLIDER_H

#include "JuceHeader.h"
#include "synth_slider.h"

class ModulationSlider : public SynthSlider, public Slider::Listener {
  public:
    ModulationSlider(SynthSlider* source);
    ~ModulationSlider();

    virtual void mouseDown(const MouseEvent& e) override;
    virtual void mouseUp(const MouseEvent& e) override;
    void sliderValueChanged(Slider* moved_slider) override;

    SynthSlider* getDestinationSlider() { return destination_slider_; }

  private:
    SynthSlider* destination_slider_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationSlider)
};

#endif // MODULATIONS_SLIDER_H
