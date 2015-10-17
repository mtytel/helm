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

#ifndef HELM_PLUGIN_H
#define HELM_PLUGIN_H

#include "JuceHeader.h"
#include "memory.h"
#include "midi_manager.h"
#include "helm_engine.h"
#include "value_bridge.h"

class ValueBridge;

class HelmPlugin : public AudioProcessor, public ValueBridge::Listener, MidiManager::Listener {
  public:
    HelmPlugin();
    virtual ~HelmPlugin();

    void prepareToPlay(double sample_rate, int buffer_size) override;
    void releaseResources() override;

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    const String getInputChannelName(int channel_index) const override;
    const String getOutputChannelName(int channel_index) const override;
    bool isInputChannelStereoPair(int index) const override;
    bool isOutputChannelStereoPair(int index) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& new_name) override;

    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int size_in_bytes) override;

    void valueChangedThroughMidi(const std::string& name, mopo::mopo_float value) override;
    void patchChangedThroughMidi(File patch) override;

    void beginChangeGesture(std::string name);
    void endChangeGesture(std::string name);
    void setValueNotifyHost(std::string name, mopo::mopo_float value);
    void processMidi(MidiBuffer&, int start_sample, int end_sample);
    mopo::HelmEngine* getSynth() { return &synth_; }
    std::map<std::string, String>* getGuiState() { return &gui_state_; }
    const mopo::Memory* getOutputMemory() { return output_memory_; }
    MidiManager* getMidiManager() { return midi_manager_; }

    // ValueBridge::Listener
    void parameterChanged(std::string name, mopo::mopo_float value);

  private:
    mopo::HelmEngine synth_;
    mopo::control_map controls_;
    std::map<std::string, String> gui_state_;

    mopo::Memory* output_memory_;
    ScopedPointer<MidiManager> midi_manager_;

    int num_programs_;
    int current_program_;

    std::map<std::string, ValueBridge*> bridge_lookup_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HelmPlugin)
};

#endif // HELM_PLUGIN_H
