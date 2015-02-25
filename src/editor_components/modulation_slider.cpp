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

#include "modulation_slider.h"
#include "mopo.h"

ModulationSlider::ModulationSlider(Slider* destination) {
    // MOPO_ASSERT(destination->getSkewFactor() == 1.0);
    destination_slider_ = destination;

    float destination_range = destination->getMaximum() - destination->getMinimum();
    setName(destination->getName());
    setRange(-destination_range, destination_range);
    setDoubleClickReturnValue(true, 0.0f);
    setSliderStyle(destination->getSliderStyle());
    setVelocityBasedMode(true);
    setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
}

ModulationSlider::~ModulationSlider() {
}