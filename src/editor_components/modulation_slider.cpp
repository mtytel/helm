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

ModulationSlider::ModulationSlider(Slider* source) {
    source_slider_ = source;
    setName(source->getName());
    setRange(-1.0, 1.0);
    setDoubleClickReturnValue(true, 0.0f);
    setSliderStyle(source->getSliderStyle());
    setVelocityBasedMode(true);
    setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
}

ModulationSlider::~ModulationSlider() {
}