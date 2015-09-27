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

#ifndef SYNTH_GUI_INTERFACE_H
#define SYNTH_GUI_INTERFACE_H

#include "JuceHeader.h"
#include "midi_manager.h"
#include "helm_common.h"
#include "helm_engine.h"
#include <string>

class SynthGuiInterface : public MidiManager::Listener {
  public:
    SynthGuiInterface() : synth_(nullptr), gui_state_(nullptr) { }
    virtual ~SynthGuiInterface() { }

    void valueChangedThroughMidi(const std::string& name, mopo::mopo_float value) override;
    void patchChangedThroughMidi(File patch) override;
    void valueChangedExternal(const std::string& name, mopo::mopo_float value);
    void valueChangedInternal(const std::string& name, mopo::mopo_float value);
    void valueChanged(const std::string& name, mopo::mopo_float value);
    void changeModulationAmount(const std::string& source, const std::string& destination,
                                mopo::mopo_float amount);
    void connectModulation(mopo::ModulationConnection* connection);
    void disconnectModulation(mopo::ModulationConnection* connection);
    std::vector<mopo::ModulationConnection*> getSourceConnections(const std::string& source);
    std::vector<mopo::ModulationConnection*> getDestinationConnections(const std::string& destination);

    int getNumActiveVoices();
    void lockSynth();
    void unlockSynth();
    mopo::Processor::Output* getModSource(const std::string& name);

    var saveToVar(String author);
    void loadFromVar(var state);
    void loadFromFile(File patch_file);

    virtual void beginChangeGesture(const std::string& name) { }
    virtual void endChangeGesture(const std::string& name) { }
    virtual void setValueNotifyHost(const std::string& name, mopo::mopo_float value) { }
    virtual AudioDeviceManager* getAudioDeviceManager() { return nullptr; }

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

  protected:
    virtual const CriticalSection& getCriticalSection() = 0;
    virtual MidiManager* getMidiManager() = 0;
    virtual void updateFullGui() = 0;
    virtual void updateGuiControl(const std::string& name, mopo::mopo_float value) = 0;
    mopo::ModulationConnection* getConnection(const std::string& source,
                                              const std::string& destination);

    void setSynth(mopo::HelmEngine* synth) {
      synth_ = synth;
      controls_ = synth->getControls();
    }

    void setGuiState(std::map<std::string, String>* gui_state) {
      gui_state_ = gui_state;
    }

    mopo::HelmEngine* synth_;
    std::map<std::string, String>* gui_state_;
    mopo::control_map controls_;
};

#endif // SYNTH_GUI_INTERFACE_H
