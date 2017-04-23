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

#ifndef MODULATION_HIGHLIGHT_H
#define MODULATION_HIGHLIGHT_H

#include "JuceHeader.h"
#include "processor.h"
#include "synth_slider.h"

class ModulationHighlight : public Component {
  public:
    ModulationHighlight();
    virtual ~ModulationHighlight();

    void paint(Graphics& g) override;
    void resized() override;

    void updateValue();
    void updateDrawing();

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationHighlight)
};

#endif // MODULATION_HIGHLIGHT_H
