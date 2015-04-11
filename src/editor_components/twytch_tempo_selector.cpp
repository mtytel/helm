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

TwytchTempoSelector::TwytchTempoSelector(String name) : TwytchSlider(name),
                                                        free_slider_(0), tempo_slider_(0) { }

void TwytchTempoSelector::mouseDown(const MouseEvent& e) {
    PopupMenu m;
    m.addItem(kHertz, "Hertz");
    m.addItem(kTempo, "Tempo");
    m.addItem(kTempoDotted, "Tempo Dotted");
    m.addItem(kTempoTriplet, "Tempo Triplet");

    int result = m.show();
    setValue(result - 1);
}

void TwytchTempoSelector::valueChanged() {
    bool free_slider = getValue() == (kHertz - 1);
    
    if (free_slider_)
        free_slider_->setVisible(free_slider);
    if (tempo_slider_)
        tempo_slider_->setVisible(!free_slider);
}

void TwytchTempoSelector::paint(Graphics& g) {
    g.setColour(Colours::black);
    g.fillAll();
}