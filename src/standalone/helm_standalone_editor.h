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

#ifndef HELM_STANDALONE_EDITOR_H
#define HELM_STANDALONE_EDITOR_H

#include "JuceHeader.h"

#include "full_interface.h"
#include "memory.h"
#include "midi_manager.h"
#include "helm_computer_keyboard.h"
#include "helm_engine.h"
#include "synth_gui_interface.h"

class HelmStandaloneEditor : public AudioAppComponent,
                             public MessageListener,
                             public SynthGuiInterface {
  public:
    HelmStandaloneEditor();
    ~HelmStandaloneEditor();

    // AudioAppComponent
    void prepareToPlay(int buffer_size, double sample_rate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& buffer) override;
    void releaseResources() override;
    void paint(Graphics& g) override;
    void resized() override;

    // MessageListener
    void handleMessage(const Message& message) override;

    // SynthGuiInterface
    const CriticalSection& getCriticalSection() override { return critical_section_; }
    MidiManager* getMidiManager() override { return midi_manager_.get(); }
    void updateFullGui() override;
    void updateGuiControl(const std::string& name, mopo::mopo_float value) override;
    AudioDeviceManager* getAudioDeviceManager() override { return &deviceManager; }

  private:
    mopo::HelmEngine synth_;
    CriticalSection critical_section_;

    ScopedPointer<mopo::Memory> output_memory_;
    ScopedPointer<FullInterface> gui_;
    ScopedPointer<MidiManager> midi_manager_;
    ScopedPointer<HelmComputerKeyboard> computer_keyboard_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HelmStandaloneEditor)
};

#endif  // HELM_STANDALONE_EDITOR_H
