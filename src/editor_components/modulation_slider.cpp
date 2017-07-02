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

#include "default_look_and_feel.h"
#include "mopo.h"

namespace {
  enum MenuIds {
    kCancel = 0,
    kClearModulation,
  };

  static void sliderPopupCallback(int result, ModulationSlider* slider) {
    if (slider != nullptr && result == kClearModulation)
      slider->setValue(0.0);
  }
} // namespace


ModulationSlider::ModulationSlider(SynthSlider* destination) : SynthSlider(destination->getName()) {
  destination_slider_ = destination;

  float destination_range = destination->getMaximum() - destination->getMinimum();
  setName(destination->getName());
  if (details_.steps)
    setRange(-destination_range, destination_range,
             destination_range / (details_.steps - 1));
  else
    setRange(-destination_range, destination_range);
  setDoubleClickReturnValue(true, 0.0f);
  setSliderStyle(destination->getSliderStyle());
  setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
  setPostMultiply(destination->getPostMultiply());
  setUnits(destination->getUnits());
  setScalingType(destination->getScalingType());
  setPopupPlacement(destination->getPopupPlacement(), destination->getPopupBuffer());

  destination->addListener(this);

  if (destination->isRotary())
    setMouseDragSensitivity(2.0f * getMouseDragSensitivity());
  else
    setVelocityBasedMode(true);
  setOpaque(false);
}

ModulationSlider::~ModulationSlider() {
}

void ModulationSlider::sliderValueChanged(Slider* moved_slider) {
  if (isVisible())
    repaint();
}

void ModulationSlider::mouseDown(const juce::MouseEvent &e) {
  if (e.mods.isPopupMenu()) {
    if (getValue() != 0.0) {
      PopupMenu m;
      m.setLookAndFeel(DefaultLookAndFeel::instance());
      m.addItem(kClearModulation, "Clear Modulation");
      m.showMenuAsync(PopupMenu::Options(),
                      ModalCallbackFunction::forComponent(sliderPopupCallback, this));
    }
  }
  else
    SynthSlider::mouseDown(e);
}

void ModulationSlider::mouseUp(const juce::MouseEvent &e) {
  if (!e.mods.isPopupMenu())
    SynthSlider::mouseUp(e);
}
