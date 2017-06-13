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

#ifndef RETRIGGER_SELECTOR_H
#define RETRIGGER_SELECTOR_H

#include "JuceHeader.h"
#include "synth_slider.h"

class RetriggerSelector : public SynthSlider {
  public:
    RetriggerSelector(String name);

    void mouseDown(const MouseEvent& e) override;
    void paint(Graphics& g) override;
    void resized() override;
    void selectType(int type);

  private:
    Path arrow_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RetriggerSelector)
};

#endif // RETRIGGER_SELECTOR_H
