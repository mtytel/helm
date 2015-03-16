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

#include "twytch_slider.h"
#include "synth_gui_interface.h"

namespace {
    enum MenuIds {
        kCancel = 0,
        kArmMidiLearn,
        kClearMidiLearn,
        kDefaultValue
    };

} // namespace

void TwytchSlider::mouseDown(const MouseEvent& e) {
    Slider::mouseDown(e);

    if (e.mods.isPopupMenu()) {
        PopupMenu m;
        m.addItem(kArmMidiLearn, "Learn Controller Assignment");
        m.addItem(kClearMidiLearn, "Clear Controller Assignment");

        bool double_click_enabled = false;
        float default_value = getDoubleClickReturnValue(double_click_enabled);
        if (double_click_enabled)
            m.addItem(kDefaultValue, "Set to Default Value");

        int result = m.show();
        if (result == kArmMidiLearn) {
            SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
            if (parent)
                parent->armMidiLearn(getName().toStdString(), getMinimum(), getMaximum());
        }
        else if (result == kClearMidiLearn) {
            SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
            if (parent)
                parent->clearMidiLearn(getName().toStdString());
        }
        else if (result == kDefaultValue) {
            setValue(default_value);
        }
    }
}

void TwytchSlider::mouseUp(const MouseEvent& e) {
    Slider::mouseUp(e);
}
