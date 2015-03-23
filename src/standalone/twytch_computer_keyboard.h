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

#ifndef TWYTCH_COMPUTER_KEYBOARD_H
#define TWYTCH_COMPUTER_KEYBOARD_H

#include "JuceHeader.h"

#include "twytch_engine.h"

class TwytchComputerKeyboard : public KeyListener {
  public:
    TwytchComputerKeyboard(mopo::TwytchEngine* synth, const CriticalSection* critical_section);
    ~TwytchComputerKeyboard();

    void changeKeyboardOffset(int new_offset);

    // KeyListener
    bool keyPressed(const KeyPress &key, Component *origin) override;
    bool keyStateChanged(bool isKeyDown, Component *originatingComponent) override;

  private:
    mopo::TwytchEngine* synth_;
    const CriticalSection* critical_section_;
    std::set<char> keys_pressed_;
    int computer_keyboard_offset_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TwytchComputerKeyboard)
};

#endif  // TWYTCH_COMPUTER_KEYBOARD_H
