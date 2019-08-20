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

#ifndef MIDI_MANAGER_H
#define MIDI_MANAGER_H

#include "JuceHeader.h"
#include "common.h"
#include "helm_common.h"
#include <string>
#include <map>

class SynthBase;

namespace mopo {
  class HelmEngine;
} // namespace mopo

class MidiManager : public MidiInputCallback {
  public:
    typedef std::map<int, std::map<std::string, const mopo::ValueDetails*>> midi_map;

    class Listener {
      public:
        virtual ~Listener() { }
        virtual void valueChangedThroughMidi(const std::string& name,
                                             mopo::mopo_float value) = 0;
        virtual void patchChangedThroughMidi(File patch) = 0;
    };

    MidiManager(SynthBase* synth, MidiKeyboardState* keyboard_state,
                std::map<std::string, String>* gui_state, Listener* listener = nullptr);
    virtual ~MidiManager();

    void armMidiLearn(std::string name);
    void cancelMidiLearn();
    void clearMidiLearn(const std::string& name);
    void midiInput(int control, mopo::mopo_float value);
    void processMidiMessage(const MidiMessage &midi_message, int sample_position = 0);
    bool isMidiMapped(const std::string& name) const;

    void setSampleRate(double sample_rate);
    void removeNextBlockOfMessages(MidiBuffer& buffer, int num_samples);
    void replaceKeyboardMessages(MidiBuffer& buffer, int num_samples);

    midi_map getMidiLearnMap() { return midi_learn_map_; }
    void setMidiLearnMap(midi_map midi_learn_map) { midi_learn_map_ = midi_learn_map; }

    // MidiInputCallback
    void handleIncomingMidiMessage(MidiInput *source, const MidiMessage &midi_message) override;

    struct PatchLoadedCallback : public CallbackMessage {
      PatchLoadedCallback(Listener* lis, File pat) : listener(lis), patch(pat) { }

      void messageCallback() override {
        if (listener)
          listener->patchChangedThroughMidi(patch);
      }

      Listener* listener;
      File patch;
    };

  protected:
    SynthBase* synth_;
    mopo::HelmEngine* engine_;
    MidiKeyboardState* keyboard_state_;
    MidiMessageCollector midi_collector_;
    std::map<std::string, String>* gui_state_;
    Listener* listener_;
    int current_bank_;
    int current_folder_;
    int current_patch_;

    const mopo::ValueDetails* armed_value_;
    midi_map midi_learn_map_;
};

#endif // MIDI_MANAGER_H
