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

#include "modulation_slider.h"
#include "mopo.h"

ModulationSlider::ModulationSlider(SynthSlider* destination) : SynthSlider(destination->getName()) {
  destination_slider_ = destination;

  float destination_range = destination->getMaximum() - destination->getMinimum();
  setName(destination->getName());
  setRange(-destination_range, destination_range);
  setDoubleClickReturnValue(true, 0.0f);
  setSliderStyle(destination->getSliderStyle());
  setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
  setStringLookup(destination->getStringLookup());
  setPostMultiply(destination->getPostMultiply());
  setUnits(destination->getUnits());
  setScalingType(destination->getScalingType());

  if (destination->isRotary())
    setMouseDragSensitivity(2.0f * getMouseDragSensitivity());
  else
    setVelocityBasedMode(true);
  setOpaque(false);
}

ModulationSlider::~ModulationSlider() {
}

void ModulationSlider::mouseDown(const juce::MouseEvent &e) {
  // We'll ignore right clicks.
  if (!e.mods.isPopupMenu())
    SynthSlider::mouseDown(e);
}
