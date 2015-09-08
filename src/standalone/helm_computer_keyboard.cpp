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
#include "helm_common.h"

HelmComputerKeyboard::HelmComputerKeyboard(mopo::HelmEngine* synth,
                                           const CriticalSection* critical_section) {
  synth_ = synth;
  critical_section_ = critical_section;
  computer_keyboard_offset_ = mopo::DEFAULT_KEYBOARD_OFFSET;
  layout_ = mopo::DEFAULT_KEYBOARD;
  up_key_ = mopo::DEFAULT_KEYBOARD_OCTAVE_UP;
  down_key_ = mopo::DEFAULT_KEYBOARD_OCTAVE_DOWN;
}

HelmComputerKeyboard::~HelmComputerKeyboard() {
}

void HelmComputerKeyboard::changeKeyboardOffset(int new_offset) {
  ScopedLock lock(*critical_section_);
  for (int i = 0; i < layout_.length(); ++i) {
    int note = computer_keyboard_offset_ + i;
    synth_->noteOff(note);
    keys_pressed_.erase(layout_[i]);
  }

  computer_keyboard_offset_ = CLAMP(new_offset, 0, mopo::MIDI_SIZE - mopo::NOTES_PER_OCTAVE);
}

bool HelmComputerKeyboard::keyPressed(const KeyPress &key, Component *origin) {
  return false;
}

bool HelmComputerKeyboard::keyStateChanged(bool isKeyDown, Component *origin) {
  bool consumed = false;
  ScopedLock lock(*critical_section_);
  for (int i = 0; i < layout_.length(); ++i) {
    int note = computer_keyboard_offset_ + i;

    if (KeyPress::isKeyCurrentlyDown(layout_[i]) &&
        !keys_pressed_.count(layout_[i]) && isKeyDown) {
      keys_pressed_.insert(layout_[i]);
      synth_->noteOn(note);
    }
    else if (!KeyPress::isKeyCurrentlyDown(layout_[i]) &&
             keys_pressed_.count(layout_[i])) {
      keys_pressed_.erase(layout_[i]);
      synth_->noteOff(note);
    }
    consumed = true;
  }

  if (KeyPress::isKeyCurrentlyDown(down_key_)) {
    if (!keys_pressed_.count(down_key_)) {
      keys_pressed_.insert(down_key_);
      changeKeyboardOffset(computer_keyboard_offset_ - mopo::NOTES_PER_OCTAVE);
      consumed = true;
    }
  }
  else
    keys_pressed_.erase(down_key_);

  if (KeyPress::isKeyCurrentlyDown(up_key_)) {
    if (!keys_pressed_.count(up_key_)) {
      keys_pressed_.insert(up_key_);
      changeKeyboardOffset(computer_keyboard_offset_ + mopo::NOTES_PER_OCTAVE);
      consumed = true;
    }
  }
  else
    keys_pressed_.erase(up_key_);

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
