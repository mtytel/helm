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

#include "helm_computer_keyboard.h"
#include "helm_common.h"

#define KEYBOARD_MIDI_CHANNEL 1

HelmComputerKeyboard::HelmComputerKeyboard(mopo::HelmEngine* synth,
                                           MidiKeyboardState* keyboard_state) {
  synth_ = synth;
  keyboard_state_ = keyboard_state;
  computer_keyboard_offset_ = mopo::DEFAULT_KEYBOARD_OFFSET;
  layout_ = mopo::DEFAULT_KEYBOARD;
  up_key_ = mopo::DEFAULT_KEYBOARD_OCTAVE_UP;
  down_key_ = mopo::DEFAULT_KEYBOARD_OCTAVE_DOWN;
}

HelmComputerKeyboard::~HelmComputerKeyboard() {
}

void HelmComputerKeyboard::changeKeyboardOffset(int new_offset) {
  for (int i = 0; i < layout_.length(); ++i) {
    int note = computer_keyboard_offset_ + i;
    keyboard_state_->noteOff(KEYBOARD_MIDI_CHANNEL, note, 1.0f);
    keys_pressed_.erase(layout_[i]);
  }

  int max = (mopo::MIDI_SIZE / mopo::NOTES_PER_OCTAVE - 1) * mopo::NOTES_PER_OCTAVE;
  computer_keyboard_offset_ = mopo::utils::iclamp(new_offset, 0, max);
}

bool HelmComputerKeyboard::keyPressed(const KeyPress &key, Component *origin) {
  return false;
}

bool HelmComputerKeyboard::keyStateChanged(bool isKeyDown, Component *origin) {
  bool consumed = false;
  for (int i = 0; i < layout_.length(); ++i) {
    int note = computer_keyboard_offset_ + i;

    ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
    if (KeyPress::isKeyCurrentlyDown(layout_[i]) &&
        !keys_pressed_.count(layout_[i]) && isKeyDown && !modifiers.isCommandDown()) {
      keys_pressed_.insert(layout_[i]);
      keyboard_state_->noteOn(KEYBOARD_MIDI_CHANNEL, note, 1.0f);
    }
    else if (!KeyPress::isKeyCurrentlyDown(layout_[i]) &&
             keys_pressed_.count(layout_[i])) {
      keys_pressed_.erase(layout_[i]);
      keyboard_state_->noteOff(KEYBOARD_MIDI_CHANNEL, note, 1.0f);
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
