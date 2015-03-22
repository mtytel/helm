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

    void armMidiLearn(std::string name, mopo::mopo_float min, mopo::mopo_float max);
    void cancelMidiLearn();
    void clearMidiLearn(std::string name);
    void midiInput(int control, mopo::mopo_float value);

  protected:
    std::string control_armed_;
    std::pair<mopo::mopo_float, mopo::mopo_float> armed_range_;
    std::map<int, std::string> midi_learn_map_;
    std::map<int, std::pair<mopo::mopo_float, mopo::mopo_float>> midi_learn_range_map_;
};

#endif // SYNTH_GUI_INTERFACE_H
