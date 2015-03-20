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

#ifndef SYNTH_GUI_INTERFACE_H
#define SYNTH_GUI_INTERFACE_H

#include "twytch_common.h"
#include "utils.h"
#include "value.h"
#include <string>
#include <map>

class SynthGuiInterface {
  public:
    SynthGuiInterface() : armed_range_(1.0, 1.0) { }
    virtual ~SynthGuiInterface() { }

    virtual void valueChanged(std::string name, mopo::mopo_float value) = 0;
    virtual void connectModulation(mopo::ModulationConnection* connection) = 0;
    virtual void disconnectModulation(mopo::ModulationConnection* connection) = 0;
    virtual int getNumActiveVoices() = 0;
    virtual void enterCriticalSection() = 0;
    virtual void exitCriticalSection() = 0;
    virtual mopo::Processor::Output* getModSource(std::string name) = 0;

    virtual void armMidiLearn(std::string name, mopo::mopo_float min, mopo::mopo_float max) {
      control_armed_ = name;
      armed_range_ = std::pair<mopo::mopo_float, mopo::mopo_float>(min, max);
    }

    virtual void cancelMidiLearn() {
      control_armed_ = "";
    }

    virtual void clearMidiLearn(std::string name) {
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

    virtual void midiInput(int control, mopo::mopo_float value) {
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

  protected:
    std::string control_armed_;
    std::pair<mopo::mopo_float, mopo::mopo_float> armed_range_;
    std::map<int, std::string> midi_learn_map_;
    std::map<int, std::pair<mopo::mopo_float, mopo::mopo_float>> midi_learn_range_map_;
};

#endif // SYNTH_GUI_INTERFACE_H
