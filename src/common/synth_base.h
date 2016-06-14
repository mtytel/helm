/* Copyright 2013-2016 Matt Tytel
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
    void connectModulation(mopo::ModulationConnection* connection);
    void disconnectModulation(mopo::ModulationConnection* connection);
    std::vector<mopo::ModulationConnection*> getSourceConnections(const std::string& source);
    std::vector<mopo::ModulationConnection*> getDestinationConnections(const std::string& destination);
  
    mopo::Processor::Output* getModSource(const std::string& name);

    var saveToVar(String author);
    void loadFromVar(var state);
    void loadFromFile(File patch_file);

    virtual void beginChangeGesture(const std::string& name) { }
    virtual void endChangeGesture(const std::string& name) { }
    virtual void setValueNotifyHost(const std::string& name, mopo::mopo_float value) { }

    void armMidiLearn(const std::string& name, mopo::mopo_float min, mopo::mopo_float max);
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
    const mopo::Memory* getOutputMemory() { return output_memory_; }

  protected:
    virtual const CriticalSection& getCriticalSection() = 0;
    virtual SynthGuiInterface* getGuiInterface() = 0;
    mopo::ModulationConnection* getConnection(const std::string& source,
                                              const std::string& destination);

    inline bool getNextControlChange(mopo::control_change& change) {
      return value_change_queue_.try_dequeue(change);
    }

    void processAudio(AudioSampleBuffer* buffer, int channels, int samples, int offset);
    void processMidi(MidiBuffer&, int start_sample, int end_sample);
    void processKeyboardEvents(int num_samples);
    void processControlChanges();

    mopo::HelmEngine engine_;
    ScopedPointer<MidiManager> midi_manager_;
    ScopedPointer<MidiKeyboardState> keyboard_state_;
    mopo::Memory* output_memory_;

    std::map<std::string, String> save_info_;
    mopo::control_map controls_;
    moodycamel::ConcurrentQueue<mopo::control_change> value_change_queue_;
};

#endif // SYNTH_BASE_H
