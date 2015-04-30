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

#include "modulation_button.h"

namespace {
    enum MenuIds {
        kCancel = 0,
        kDisconnect,
    };

} // namespace

ModulationButton::ModulationButton(String name) :
        ToggleButton(name) {
    setBufferedToImage(true);
}

void ModulationButton::mouseDown(const MouseEvent& e) {
    if (e.mods.isPopupMenu()) {
        PopupMenu m;
        m.addItem(kDisconnect, "Disconnect");
        // m.addItem(kClearMidiLearn, "Clear Controller Assignment");

        int result = m.show();
        if (result == kDisconnect) {

        }
    }
    else
        ToggleButton::mouseDown(e);
}

void ModulationButton::mouseUp(const MouseEvent& e) {
    if (!e.mods.isPopupMenu())
        ToggleButton::mouseUp(e);
}