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

#pragma once
#ifndef MIDI_KEYBOARD_H
#define MIDI_KEYBOARD_H

#include "JuceHeader.h"

class MidiKeyboard : public MidiKeyboardComponent {
  public:
    MidiKeyboard(MidiKeyboardState& state, Orientation orientation);

    virtual void drawBlackNote(int midiNoteNumber, Graphics& g,
                               Rectangle<float> area,
                               bool isDown, bool isOver,
                               Colour noteFillColour) override;

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiKeyboard)
};

#endif // MODULATION_BUTTON_H
