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

#include "twytch_tempo_selector.h"
#include "synth_gui_interface.h"

namespace {
    enum MenuIds {
        kCancel = 0,
        kHertz,
        kTempo,
        kTempoDotted,
        kTempoTriplet
    };

} // namespace

TwytchTempoSelector::TwytchTempoSelector(String name) : Component(name) { }

void TwytchTempoSelector::mouseDown(const MouseEvent& e) {
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    if (parent == 0)
        return;

    PopupMenu m;
    m.addItem(kHertz, "Hertz");
    m.addItem(kTempo, "Tempo");
    m.addItem(kTempoDotted, "Tempo Dotted");
    m.addItem(kTempoTriplet, "Tempo Triplet");

    int value = m.show() - 1;
    if (value >= 0)
        parent->valueChanged(getName().toStdString(), value);
}

void TwytchTempoSelector::paint(Graphics& g) {
    g.setColour(Colours::black);
    g.fillAll();
}