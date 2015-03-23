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

#include "midi_manager.h"

#define PITCH_WHEEL_RESOLUTION 0x3fff

void MidiManager::armMidiLearn(std::string name, mopo::mopo_float min, mopo::mopo_float max) {
  control_armed_ = name;
  armed_range_ = std::pair<mopo::mopo_float, mopo::mopo_float>(min, max);
}

void MidiManager::cancelMidiLearn() {
  control_armed_ = "";
}

void MidiManager::clearMidiLearn(std::string name) {
  for (auto controls : midi_learn_map_) {
    if (controls.second.count(name))
      controls.second.erase(name);
  }
}

void MidiManager::midiInput(int midi_id, mopo::mopo_float value) {
  ScopedLock lock(*critical_section_);
  if (control_armed_ == "") {
    if (midi_learn_map_.count(midi_id)) {
      mopo::control_map controls = synth_->getControls();

      for (auto control : midi_learn_map_[midi_id]) {
        midi_range range = control.second;
        mopo::mopo_float percent = value / mopo::MIDI_SIZE;
        mopo::mopo_float translated = percent * (range.second - range.first) + range.first;
        controls[control.first]->set(translated);
      }

    }
  }
  else {
    midi_learn_map_[midi_id][control_armed_] = armed_range_;
    mopo::mopo_float percent = value / mopo::MIDI_SIZE;
    mopo::mopo_float translated = percent * (armed_range_.second - armed_range_.first) +
    armed_range_.first;
    synth_->getControls()[control_armed_]->set(translated);
    control_armed_ = "";
  }
}

void MidiManager::processMidiMessage(const juce::MidiMessage &midi_message, int sample_position) {
  ScopedLock lock(*critical_section_);
  if (midi_message.isNoteOn()) {
    float velocity = (1.0 * midi_message.getVelocity()) / mopo::MIDI_SIZE;
    synth_->noteOn(midi_message.getNoteNumber(), velocity, sample_position);
  }
  else if (midi_message.isNoteOff())
    synth_->noteOff(midi_message.getNoteNumber(), sample_position);
  else if (midi_message.isSustainPedalOn())
    synth_->sustainOn();
  else if (midi_message.isSustainPedalOff())
    synth_->sustainOff();
  else if (midi_message.isAllNotesOff())
    synth_->allNotesOff();
  else if (midi_message.isAftertouch()) {
    mopo::mopo_float note = midi_message.getNoteNumber();
    mopo::mopo_float value = (1.0 * midi_message.getAfterTouchValue()) / mopo::MIDI_SIZE;
    synth_->setAftertouch(note, value);
  }
  else if (midi_message.isPitchWheel()) {
    double percent = (1.0 * midi_message.getPitchWheelValue()) / PITCH_WHEEL_RESOLUTION;
    double value = 2 * percent - 1.0;
    synth_->setPitchWheel(value);
  }
  else if (midi_message.isController()) {
    midiInput(midi_message.getControllerNumber(), midi_message.getControllerValue());
  }
}

void MidiManager::handleIncomingMidiMessage(MidiInput *source,
                                            const MidiMessage &midi_message) {
  processMidiMessage(midi_message);
}