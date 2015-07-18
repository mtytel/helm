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

class SynthGuiInterface : public MidiManager::MidiManagerListener {
  public:
    SynthGuiInterface() { }
    virtual ~SynthGuiInterface() { }

    void valueChangedThroughMidi(std::string name, mopo::mopo_float value) override;
    void valueChangedExternal(std::string name, mopo::mopo_float value);
    void valueChangedInternal(std::string name, mopo::mopo_float value);
    void valueChanged(std::string name, mopo::mopo_float value);
    void changeModulationAmount(std::string source, std::string destination,
                                mopo::mopo_float amount);
    void connectModulation(mopo::ModulationConnection* connection);
    void disconnectModulation(mopo::ModulationConnection* connection);
    std::vector<mopo::ModulationConnection*> getSourceConnections(std::string source);
    std::vector<mopo::ModulationConnection*> getDestinationConnections(std::string destination);

    int getNumActiveVoices();
    void lockSynth();
    void unlockSynth();
    mopo::Processor::Output* getModSource(std::string name);

    var saveToVar();
    void loadFromVar(var state);

    virtual void beginChangeGesture(std::string name) { }
    virtual void endChangeGesture(std::string name) { }
    virtual void setValueNotifyHost(std::string name, mopo::mopo_float value) { }
    virtual AudioDeviceManager* getAudioDeviceManager() { return nullptr; }

    void armMidiLearn(std::string name, mopo::mopo_float min, mopo::mopo_float max);
    void cancelMidiLearn();
    void clearMidiLearn(std::string name);
    bool isMidiMapped(std::string name);

  protected:
    virtual const CriticalSection& getCriticalSection() = 0;
    virtual MidiManager* getMidiManager() = 0;
    virtual void updateFullGui() = 0;
    virtual void updateGuiControl(std::string name, mopo::mopo_float value) = 0;
    mopo::ModulationConnection* getConnection(std::string source, std::string destination);

    void setSynth(mopo::HelmEngine* synth) {
      synth_ = synth;
      controls_ = synth->getControls();
    }

    mopo::HelmEngine* synth_;
    mopo::control_map controls_;
};

#endif // SYNTH_GUI_INTERFACE_H
