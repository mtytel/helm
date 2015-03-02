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

#ifndef MODULATIONS_SLIDER_H
#define MODULATIONS_SLIDER_H

#include "JuceHeader.h"

class ModulationSlider : public Slider {
public:
    ModulationSlider(Slider* source);
    ~ModulationSlider();

    Slider* getDestinationSlider() { return destination_slider_; }

private:
    Slider* destination_slider_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationSlider)
};

#endif // MODULATIONS_SLIDER_H