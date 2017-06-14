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

#ifndef HELM_EDITOR_H
#define HELM_EDITOR_H

#include "JuceHeader.h"

#include "full_interface.h"
#include "memory.h"
#include "midi_manager.h"
#include "helm_computer_keyboard.h"
#include "helm_engine.h"
#include "synth_base.h"
#include "synth_gui_interface.h"

class HelmEditor : public AudioAppComponent,
                   public SynthBase,
                   public SynthGuiInterface {
  public:
    HelmEditor(bool use_gui = true);
    ~HelmEditor();

    // AudioAppComponent
    void prepareToPlay(int buffer_size, double sample_rate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& buffer) override;
    void releaseResources() override;
    void paint(Graphics& g) override;
    void resized() override;

    // SynthBase
    const CriticalSection& getCriticalSection() override { return critical_section_; }
    SynthGuiInterface* getGuiInterface() override { return this; }

    // SynthGuiInterface
    AudioDeviceManager* getAudioDeviceManager() override { return &deviceManager; }

    void animate(bool animate);

  private:
    ScopedPointer<HelmComputerKeyboard> computer_keyboard_;
    CriticalSection critical_section_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HelmEditor)
};

#endif  // HELM_EDITOR_H
