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

#include "midi_manager.h"
#include "load_save.h"

#define PITCH_WHEEL_RESOLUTION 0x3fff
#define MOD_WHEEL_RESOLUTION 127
#define BANK_SELECT_NUMBER 0
#define FOLDER_SELECT_NUMBER 32
#define MOD_WHEEL_CONTROL_NUMBER 1

MidiManager::MidiManager(mopo::HelmEngine* synth, MidiKeyboardState* keyboard_state,
                         std::map<std::string, String>* gui_state,
                         const CriticalSection* critical_section, Listener* listener) :
    synth_(synth), keyboard_state_(keyboard_state), gui_state_(gui_state),
    critical_section_(critical_section), listener_(listener),
    armed_range_(0.0, 1.0) {
  keyboard_state_->addListener(this);
}

MidiManager::~MidiManager() {
  keyboard_state_->removeListener(this);
}

void MidiManager::handleNoteOn(MidiKeyboardState* source,
                               int midiChannel, int midiNoteNumber, float velocity) {
  ScopedLock lock(*critical_section_);
  synth_->noteOn(midiNoteNumber, velocity, 0, midiChannel - 1);
}

void MidiManager::handleNoteOff(MidiKeyboardState* source,
                                int midiChannel, int midiNoteNumber, float velocity) {
  ScopedLock lock(*critical_section_);
  synth_->noteOff(midiNoteNumber);
}

void MidiManager::armMidiLearn(std::string name, mopo::mopo_float min, mopo::mopo_float max) {
  current_bank_ = -1;
  current_folder_ = -1;
  current_patch_ = -1;
  control_armed_ = name;
  armed_range_ = std::pair<mopo::mopo_float, mopo::mopo_float>(min, max);
}

void MidiManager::cancelMidiLearn() {
  control_armed_ = "";
}

void MidiManager::clearMidiLearn(const std::string& name) {
  for (auto controls : midi_learn_map_) {
    if (controls.second.count(name)) {
      midi_learn_map_[controls.first].erase(name);
      LoadSave::saveMidiMapConfig(this);
    }
  }
}

void MidiManager::midiInput(int midi_id, mopo::mopo_float value) {
  if (control_armed_ != "") {
    midi_learn_map_[midi_id][control_armed_] = armed_range_;
    control_armed_ = "";
    LoadSave::saveMidiMapConfig(this);
  }

  if (midi_learn_map_.count(midi_id)) {
    for (auto control : midi_learn_map_[midi_id]) {
      midi_range range = control.second;
      mopo::mopo_float percent = value / mopo::MIDI_SIZE;
      mopo::mopo_float translated = percent * (range.second - range.first) + range.first;

      CallbackMessage* callback = new MidiValueChangeCallback(listener_, control.first, translated);
      callback->post();
    }
  }
}

bool MidiManager::isMidiMapped(const std::string& name) const {
  for (auto controls : midi_learn_map_) {
    if (controls.second.count(name))
      return true;
  }
  return false;
}

void MidiManager::processMidiMessage(const juce::MidiMessage &midi_message, int sample_position) {
  if (midi_message.isProgramChange()) {
    current_patch_ = midi_message.getProgramChangeNumber();
    File patch = LoadSave::loadPatch(current_bank_, current_folder_, current_patch_,
                                     synth_, *gui_state_, *critical_section_);
    MidiPatchLoadCallback* callback = new MidiPatchLoadCallback(listener_, patch);
    callback->post();
    return;
  }

  keyboard_state_->processNextMidiEvent(midi_message);

  ScopedLock lock(*critical_section_);
  if (midi_message.isSustainPedalOn())
    synth_->sustainOn();
  else if (midi_message.isSustainPedalOff())
    synth_->sustainOff();
  else if (midi_message.isAftertouch()) {
    mopo::mopo_float note = midi_message.getNoteNumber();
    mopo::mopo_float value = (1.0 * midi_message.getAfterTouchValue()) / mopo::MIDI_SIZE;
    synth_->setAftertouch(note, value);
  }
  else if (midi_message.isPitchWheel()) {
    double percent = (1.0 * midi_message.getPitchWheelValue()) / PITCH_WHEEL_RESOLUTION;
    double value = 2 * percent - 1.0;
    synth_->setPitchWheel(value, midi_message.getChannel());
  }
  else if (midi_message.isController()) {
    int controller_number = midi_message.getControllerNumber();
    if (controller_number == MOD_WHEEL_CONTROL_NUMBER) {
      double percent = (1.0 * midi_message.getControllerValue()) / MOD_WHEEL_RESOLUTION;
      synth_->setModWheel(percent, midi_message.getChannel());
    }
    else if (controller_number == BANK_SELECT_NUMBER)
      current_bank_ = midi_message.getControllerValue();
    else if (controller_number == FOLDER_SELECT_NUMBER)
      current_folder_ = midi_message.getControllerValue();
    midiInput(midi_message.getControllerNumber(), midi_message.getControllerValue());
  }
}

void MidiManager::handleIncomingMidiMessage(MidiInput *source,
                                            const MidiMessage &midi_message) {
  MidiMessageCallback* midi_callback = new MidiMessageCallback(this, midi_message);
  midi_callback->post();
}
