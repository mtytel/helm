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

#include "synth_gui_interface.h"


void SynthGuiInterface::armMidiLearn(std::string name, mopo::mopo_float min, mopo::mopo_float max) {
  control_armed_ = name;
  armed_range_ = std::pair<mopo::mopo_float, mopo::mopo_float>(min, max);
}

void SynthGuiInterface::cancelMidiLearn() {
  control_armed_ = "";
}

void SynthGuiInterface::clearMidiLearn(std::string name) {
  std::set<int> controls;
  for (auto midi_map : midi_learn_map_) {
    if (midi_map.second == name)
      controls.insert(midi_map.first);
  }
  for (int control : controls) {
    midi_learn_map_.erase(control);
    midi_learn_range_map_.erase(control);
  }
}

void SynthGuiInterface::midiInput(int control, mopo::mopo_float value) {
  if (control_armed_ == "") {
    if (midi_learn_map_.count(control)) {
      std::pair<mopo::mopo_float, mopo::mopo_float> range = midi_learn_range_map_[control];
      mopo::mopo_float percent = value / mopo::MIDI_SIZE;
      mopo::mopo_float translated = percent * (range.second - range.first) + range.first;
      valueChanged(midi_learn_map_[control], translated);
    }
  }
  else {
    midi_learn_map_[control] = control_armed_;
    midi_learn_range_map_[control] = armed_range_;
    mopo::mopo_float percent = value / mopo::MIDI_SIZE;
    mopo::mopo_float translated = percent * (armed_range_.second - armed_range_.first) +
    armed_range_.first;
    valueChanged(control_armed_, translated);
    control_armed_ = "";
  }
}