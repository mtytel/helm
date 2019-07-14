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
#include "helm_editor.h"

#define KEYBOARD_MIDI_CHANNEL 1

HelmComputerKeyboard::HelmComputerKeyboard(mopo::HelmEngine* synth,
                                           MidiKeyboardState* keyboard_state,
                                           HelmEditor* editor, bool use_pipe) {
  this->use_pipe_ = use_pipe;
  synth_ = synth;
  keyboard_state_ = keyboard_state;
  computer_keyboard_offset_ = mopo::DEFAULT_KEYBOARD_OFFSET;
  layout_ = mopo::DEFAULT_KEYBOARD;
  up_key_ = mopo::DEFAULT_KEYBOARD_OCTAVE_UP;
  down_key_ = mopo::DEFAULT_KEYBOARD_OCTAVE_DOWN;
  editor_ = editor;
  autonext = false;
}

HelmComputerKeyboard::~HelmComputerKeyboard() {
}

void HelmComputerKeyboard::noteOn(int note, float vel) {
  this->keyboard_state_->noteOn(KEYBOARD_MIDI_CHANNEL, note, vel);
  if (this->use_pipe_)
      std::cout << "{\"event\":\"note-on\",\"key\":" << note << ",\"vel\":" << vel << "}" << std::endl;
}
void HelmComputerKeyboard::noteOff(int note) {
  this->keyboard_state_->noteOff(KEYBOARD_MIDI_CHANNEL, note, 1.0f);
  if (this->use_pipe_)
      std::cout << "{\"event\":\"note-off\",\"key\":" << note << "}" << std::endl;
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
      if (this->use_pipe_)
          std::cout << "{\"event\":\"note-on\",\"key\":" << note << "}" << std::endl;

      if (modifiers.isCtrlDown() || modifiers.isShiftDown())
        this->autonext = true;
      else if (modifiers.isAltDown())
        this->autonext = false;

      if (this->autonext)
        editor_->nextPatch();

    }
    else if (!KeyPress::isKeyCurrentlyDown(layout_[i]) &&
             keys_pressed_.count(layout_[i])) {
      keys_pressed_.erase(layout_[i]);
      keyboard_state_->noteOff(KEYBOARD_MIDI_CHANNEL, note, 1.0f);
      if (this->use_pipe_)
          std::cout << "{\"event\":\"note-off\",\"key\":" << note << "}" << std::endl;

    }
    consumed = true;
  }

  if (KeyPress::isKeyCurrentlyDown(down_key_)) {
    if (!keys_pressed_.count('\\')) {
      keys_pressed_.insert('\\');
      changeKeyboardOffset(computer_keyboard_offset_ - mopo::NOTES_PER_OCTAVE);
      consumed = true;
      if (this->use_pipe_)
          std::cout << "{\"event\":\"octave-up\"}" << std::endl;
    }
  }
  else
    keys_pressed_.erase('\\');

  if (KeyPress::isKeyCurrentlyDown(up_key_)) {
    if (!keys_pressed_.count('^')) {
      keys_pressed_.insert('^');
      changeKeyboardOffset(computer_keyboard_offset_ + mopo::NOTES_PER_OCTAVE);
      consumed = true;
      if (this->use_pipe_)
          std::cout << "{\"event\":\"octave-down\"}" << std::endl;
    }
  }
  else
    keys_pressed_.erase('^');

  if (KeyPress::isKeyCurrentlyDown(KeyPress::spaceKey)) {
    if (!keys_pressed_.count(' ')) {
      keys_pressed_.insert(' ');
      synth_->correctToTime(0.0);
      consumed = true;
    }
  }
  else
    keys_pressed_.erase(' ');

  // TODO events to python api
  if (KeyPress::isKeyCurrentlyDown(KeyPress::leftKey)) {
    if (!keys_pressed_.count('<')) {
      keys_pressed_.insert('<');
      editor_->prevPatch();
      consumed = true;
    }
  }
  else
    keys_pressed_.erase('<');

  if (KeyPress::isKeyCurrentlyDown(KeyPress::rightKey)) {
    if (!keys_pressed_.count('>')) {
      keys_pressed_.insert('>');
      editor_->nextPatch();
      consumed = true;
    }
  }
  else
    keys_pressed_.erase('>');

  
  return consumed;
}
