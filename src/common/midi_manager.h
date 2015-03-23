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

#ifndef MIDI_MANAGER_H
#define MIDI_MANAGER_H

#include "JuceHeader.h"
#include "twytch_engine.h"
#include <string>
#include <map>

class MidiManager : public MidiInputCallback {
  public:
    MidiManager(mopo::TwytchEngine* synth, const CriticalSection* critical_section) :
        synth_(synth), critical_section_(critical_section), armed_range_(0.0, 1.0) { }
    virtual ~MidiManager() { }

    void armMidiLearn(std::string name, mopo::mopo_float min, mopo::mopo_float max);
    void cancelMidiLearn();
    void clearMidiLearn(std::string name);
    void midiInput(int control, mopo::mopo_float value);
    void processMidiMessage(const MidiMessage &midi_message, int sample_position = 0);

    // MidiInputCallback
    void handleIncomingMidiMessage(MidiInput *source, const MidiMessage &midi_message) override;

  protected:
    mopo::TwytchEngine* synth_;
    const CriticalSection* critical_section_;
  
    std::string control_armed_;
    std::pair<mopo::mopo_float, mopo::mopo_float> armed_range_;
    std::map<int, std::string> midi_learn_map_;
    std::map<int, std::pair<mopo::mopo_float, mopo::mopo_float>> midi_learn_range_map_;
};

#endif // MIDI_MANAGER_H
