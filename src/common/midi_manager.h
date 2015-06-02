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

#ifndef MIDI_MANAGER_H
#define MIDI_MANAGER_H

#include "JuceHeader.h"
#include "helm_engine.h"
#include <string>
#include <map>

class MidiManager : public MidiInputCallback {
  typedef std::pair<mopo::mopo_float, mopo::mopo_float> midi_range;
  public:
    class MidiManagerListener {
      public:
        virtual ~MidiManagerListener() { }
        virtual void valueChangedThroughMidi(std::string name, mopo::mopo_float value) = 0;
    };

    MidiManager(mopo::HelmEngine* synth, const CriticalSection* critical_section,
                MidiManagerListener* listener = nullptr) :
        synth_(synth), critical_section_(critical_section), listener_(listener),
        armed_range_(0.0, 1.0) { }
    virtual ~MidiManager() { }

    void armMidiLearn(std::string name, mopo::mopo_float min, mopo::mopo_float max);
    void cancelMidiLearn();
    void clearMidiLearn(std::string name);
    void midiInput(int control, mopo::mopo_float value);
    void processMidiMessage(const MidiMessage &midi_message, int sample_position = 0);
    bool isMidiMapped(std::string name);

    // MidiInputCallback
    void handleIncomingMidiMessage(MidiInput *source, const MidiMessage &midi_message) override;

  protected:
    mopo::HelmEngine* synth_;
    const CriticalSection* critical_section_;
    MidiManagerListener* listener_;
  
    std::string control_armed_;
    std::pair<mopo::mopo_float, mopo::mopo_float> armed_range_;
    std::map<int, std::map<std::string, midi_range>> midi_learn_map_;
};

#endif // MIDI_MANAGER_H
