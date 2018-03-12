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

#include "midi_manager.h"
#include "helm_engine.h"
#include "load_save.h"
#include "synth_base.h"

#define PITCH_WHEEL_RESOLUTION 0x3fff
#define MOD_WHEEL_RESOLUTION 127
#define BANK_SELECT_NUMBER 0
#define FOLDER_SELECT_NUMBER 32
#define MOD_WHEEL_CONTROL_NUMBER 1

MidiManager::MidiManager(SynthBase* synth, MidiKeyboardState* keyboard_state,
                         std::map<std::string, String>* gui_state, Listener* listener) :
    synth_(synth), keyboard_state_(keyboard_state), gui_state_(gui_state),
    listener_(listener), armed_range_(0.0, 1.0) {
  engine_ = synth_->getEngine();
}

MidiManager::~MidiManager() {
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
  for (auto& controls : midi_learn_map_) {
    if (controls.second.count(name)) {
      midi_learn_map_[controls.first].erase(name);
      LoadSave::saveMidiMapConfig(this);
    }
  }
}

void MidiManager::midiInput(int midi_id, mopo::mopo_float value) {
  if (control_armed_.length()) {
    midi_learn_map_[midi_id][control_armed_] = armed_range_;
    control_armed_ = "";

    // TODO: Probably shouldn't write this config on the audio thread.
    LoadSave::saveMidiMapConfig(this);
  }

  if (midi_learn_map_.count(midi_id)) {
    for (auto& control : midi_learn_map_[midi_id]) {
      midi_range range = control.second;
      mopo::mopo_float percent = value / (mopo::MIDI_SIZE - 1);
      mopo::mopo_float translated = percent * (range.second - range.first) + range.first;
      listener_->valueChangedThroughMidi(control.first, translated);
    }
  }
}

bool MidiManager::isMidiMapped(const std::string& name) const {
  for (auto& controls : midi_learn_map_) {
    if (controls.second.count(name))
      return true;
  }
  return false;
}

void MidiManager::setSampleRate(double sample_rate) {
  midi_collector_.reset(sample_rate);
}

void MidiManager::removeNextBlockOfMessages(MidiBuffer& buffer, int num_samples) {
  midi_collector_.removeNextBlockOfMessages(buffer, num_samples);
}

void MidiManager::processMidiMessage(const MidiMessage& midi_message, int sample_position) {              
  if (midi_message.isProgramChange()) {
    current_patch_ = midi_message.getProgramChangeNumber();
    File patch = LoadSave::loadPatch(current_bank_, current_folder_, current_patch_,
                                     synth_, *gui_state_);
    PatchLoadedCallback* callback = new PatchLoadedCallback(listener_, patch);
    callback->post();
    return;
  }

  if (midi_message.isNoteOn()) {
    engine_->noteOn(midi_message.getNoteNumber(),
                    midi_message.getVelocity() / (mopo::MIDI_SIZE - 1.0),
                    0, midi_message.getChannel() - 1);
  }
  else if (midi_message.isNoteOff())
    engine_->noteOff(midi_message.getNoteNumber());
  else if (midi_message.isAllNotesOff())
    engine_->allNotesOff();
  else if (midi_message.isSustainPedalOn())
    engine_->sustainOn();
  else if (midi_message.isSustainPedalOff())
    engine_->sustainOff();
  // "aftertouch" is pressure per note
  else if (midi_message.isAftertouch()) {
    mopo::mopo_float note = midi_message.getNoteNumber();
    mopo::mopo_float value = (1.0 * midi_message.getAfterTouchValue()) / mopo::MIDI_SIZE;
    engine_->setAftertouch(note, value);
  }
  // "pressure" is aftertouch for a whole channel (e.g. keyboard send only one value even if several keys are pressed)
  // TODO: create a separate modifier
  else if (midi_message.isChannelPressure()) {
    mopo::mopo_float note = midi_message.getNoteNumber();
    mopo::mopo_float value = (1.0 * midi_message.getChannelPressureValue()) / (mopo::MIDI_SIZE - 1.0);
    // channel - 1 as with NoteOn above
    engine_->setPressure(value, midi_message.getChannel() - 1, sample_position);
  }
  else if (midi_message.isPitchWheel()) {
    double percent = (1.0 * midi_message.getPitchWheelValue()) / PITCH_WHEEL_RESOLUTION;
    double value = 2 * percent - 1.0;
    engine_->setPitchWheel(value, midi_message.getChannel());
  }
  else if (midi_message.isController()) {
    int controller_number = midi_message.getControllerNumber();
    if (controller_number == MOD_WHEEL_CONTROL_NUMBER) {
      double percent = (1.0 * midi_message.getControllerValue()) / MOD_WHEEL_RESOLUTION;
      engine_->setModWheel(percent, midi_message.getChannel());
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
  midi_collector_.addMessageToQueue(midi_message);
}

void MidiManager::replaceKeyboardMessages(MidiBuffer& buffer, int num_samples) {
  keyboard_state_->processNextMidiBuffer(buffer, 0, num_samples, true);
}

