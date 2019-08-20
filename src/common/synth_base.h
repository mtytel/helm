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

#ifndef SYNTH_BASE_H
#define SYNTH_BASE_H

#include "JuceHeader.h"
#include "concurrentqueue.h"

#include "helm_common.h"
#include "helm_engine.h"
#include "memory.h"
#include "midi_manager.h"
#include <string>

class SynthGuiInterface;

class SynthBase : public MidiManager::Listener {
  public:
    SynthBase();
    virtual ~SynthBase() { }

    void valueChanged(const std::string& name, mopo::mopo_float value);
    void valueChangedThroughMidi(const std::string& name, mopo::mopo_float value) override;
    void patchChangedThroughMidi(File patch) override;
    void valueChangedExternal(const std::string& name, mopo::mopo_float value);
    void valueChangedInternal(const std::string& name, mopo::mopo_float value);
    void changeModulationAmount(const std::string& source, const std::string& destination,
                               mopo::mopo_float amount);
    void setModulationAmount(mopo::ModulationConnection* connection, mopo::mopo_float amount);
    void disconnectModulation(mopo::ModulationConnection* connection);
    void clearModulations();
    int getNumModulations(const std::string& destination);
    std::set<mopo::ModulationConnection*> getModulationConnections() { return mod_connections_; }
    std::vector<mopo::ModulationConnection*> getSourceConnections(const std::string& source);
    std::vector<mopo::ModulationConnection*> getDestinationConnections(
        const std::string& destination);
  
    mopo::Output* getModSource(const std::string& name);

    void loadInitPatch();
    bool loadFromFile(File patch);
    bool exportToFile();
    bool saveToFile(File patch);
    bool saveToActiveFile();
    File getActiveFile() { return active_file_; }

    virtual void beginChangeGesture(const std::string& name) { }
    virtual void endChangeGesture(const std::string& name) { }
    virtual void setValueNotifyHost(const std::string& name, mopo::mopo_float value) { }

    void armMidiLearn(const std::string& name);
    void cancelMidiLearn();
    void clearMidiLearn(const std::string& name);
    bool isMidiMapped(const std::string& name);

    void setAuthor(String author);
    void setPatchName(String patch_name);
    void setFolderName(String folder_name);
    String getAuthor();
    String getPatchName();
    String getFolderName();

    mopo::control_map& getControls() { return controls_; }
    mopo::HelmEngine* getEngine() { return &engine_; }
    MidiKeyboardState* getKeyboardState() { return keyboard_state_; }
    const float* getOutputMemory() { return output_memory_; }
    mopo::ModulationConnectionBank& getModulationBank() { return modulation_bank_; }

    struct ValueChangedCallback : public CallbackMessage {
      ValueChangedCallback(SynthBase* listener, std::string name, mopo::mopo_float val) :
          listener(listener), control_name(name), value(val) { }

      void messageCallback() override;

      SynthBase* listener;
      std::string control_name;
      mopo::mopo_float value;
    };

  protected:
    virtual const CriticalSection& getCriticalSection() = 0;
    virtual SynthGuiInterface* getGuiInterface() = 0;
    var saveToVar(String author);
    void loadFromVar(var state);
    mopo::ModulationConnection* getConnection(const std::string& source,
                                              const std::string& destination);

    inline bool getNextControlChange(mopo::control_change& change) {
      return value_change_queue_.try_dequeue(change);
    }

    inline bool getNextModulationChange(mopo::modulation_change& change) {
      return modulation_change_queue_.try_dequeue(change);
    }

    void processAudio(AudioSampleBuffer* buffer, int channels, int samples, int offset);
    void processMidi(MidiBuffer& buffer, int start_sample = 0, int end_sample = 0);
    void processKeyboardEvents(MidiBuffer& buffer, int num_samples);
    void processControlChanges();
    void processModulationChanges();
    void updateMemoryOutput(int samples, const mopo::mopo_float* left,
                                         const mopo::mopo_float* right);

    mopo::ModulationConnectionBank modulation_bank_;
    mopo::HelmEngine engine_;
    ScopedPointer<MidiManager> midi_manager_;
    ScopedPointer<MidiKeyboardState> keyboard_state_;

    File active_file_;
    float output_memory_[2 * mopo::MEMORY_RESOLUTION];
    float output_memory_write_[2 * mopo::MEMORY_RESOLUTION];
    mopo::mopo_float last_played_note_;
    int last_num_pressed_;
    mopo::mopo_float memory_reset_period_;
    mopo::mopo_float memory_input_offset_;
    int memory_index_;

    std::map<std::string, String> save_info_;
    mopo::control_map controls_;
    std::set<mopo::ModulationConnection*> mod_connections_;
    moodycamel::ConcurrentQueue<mopo::control_change> value_change_queue_;
    moodycamel::ConcurrentQueue<mopo::modulation_change> modulation_change_queue_;
};

#endif // SYNTH_BASE_H
