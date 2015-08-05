/* Copyright 2013-2015 Matt Tytel
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

#include "helm_computer_keyboard.h"

#define DEFAULT_KEYBOARD_OFFSET 48
#define KEYBOARD "awsedftgzhujkolpöä"

HelmComputerKeyboard::HelmComputerKeyboard(mopo::HelmEngine* synth,
                                           const CriticalSection* critical_section) {
  synth_ = synth;
  critical_section_ = critical_section;
  computer_keyboard_offset_ = DEFAULT_KEYBOARD_OFFSET;
}

HelmComputerKeyboard::~HelmComputerKeyboard() {
}

void HelmComputerKeyboard::changeKeyboardOffset(int new_offset) {
  ScopedLock lock(*critical_section_);
  for (int i = 0; i < strlen(KEYBOARD); ++i) {
    int note = computer_keyboard_offset_ + i;
    synth_->noteOff(note);
    keys_pressed_.erase(KEYBOARD[i]);
  }

  computer_keyboard_offset_ = CLAMP(new_offset, 0, mopo::MIDI_SIZE - mopo::NOTES_PER_OCTAVE);
}

bool HelmComputerKeyboard::keyPressed(const KeyPress &key, Component *origin) {
  return false;
}

bool HelmComputerKeyboard::keyStateChanged(bool isKeyDown, Component *originatingComponent) {
  bool consumed = false;
  ScopedLock lock(*critical_section_);
  for (int i = 0; i < strlen(KEYBOARD); ++i) {
    int note = computer_keyboard_offset_ + i;

    if (KeyPress::isKeyCurrentlyDown(KEYBOARD[i]) && !keys_pressed_.count(KEYBOARD[i])) {
      keys_pressed_.insert(KEYBOARD[i]);
      synth_->noteOn(note);
    }
    else if (!KeyPress::isKeyCurrentlyDown(KEYBOARD[i]) && keys_pressed_.count(KEYBOARD[i])) {
      keys_pressed_.erase(KEYBOARD[i]);
      synth_->noteOff(note);
    }
    consumed = true;
  }

  if (KeyPress::isKeyCurrentlyDown('y')) {
    if (!keys_pressed_.count('y')) {
      keys_pressed_.insert('y');
      changeKeyboardOffset(computer_keyboard_offset_ - mopo::NOTES_PER_OCTAVE);
      consumed = true;
    }
  }
  else
    keys_pressed_.erase('y');

  if (KeyPress::isKeyCurrentlyDown('x')) {
    if (!keys_pressed_.count('x')) {
      keys_pressed_.insert('x');
      changeKeyboardOffset(computer_keyboard_offset_ + mopo::NOTES_PER_OCTAVE);
      consumed = true;
    }
  }
  else
    keys_pressed_.erase('x');

  if (KeyPress::isKeyCurrentlyDown(KeyPress::spaceKey)) {
    if (!keys_pressed_.count(' ')) {
      keys_pressed_.insert(' ');
      synth_->correctToTime(0.0);
      consumed = true;
    }
  }
  else
    keys_pressed_.erase(' ');

  return consumed;
}
