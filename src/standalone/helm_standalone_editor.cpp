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

#include "helm_standalone_editor.h"

#include "default_look_and_feel.h"
#include "helm_common.h"
#include "mopo.h"
#include "startup.h"
#include "utils.h"

#define WIDTH 996
#define HEIGHT 670
#define MAX_OUTPUT_MEMORY 1048576

HelmStandaloneEditor::HelmStandaloneEditor() {
  setSynth(&synth_);
  midi_manager_ = new MidiManager(&synth_, &critical_section_, this);
  computer_keyboard_ = new HelmComputerKeyboard(&synth_, &critical_section_);
  output_memory_ = new mopo::Memory(MAX_OUTPUT_MEMORY);
  Startup::doStartupChecks();
  setAudioChannels(0, mopo::NUM_CHANNELS);
  AudioDeviceManager::AudioDeviceSetup setup;
  deviceManager.getAudioDeviceSetup(setup);
  setup.sampleRate = mopo::DEFAULT_SAMPLE_RATE;
  deviceManager.initialise(0, mopo::NUM_CHANNELS, nullptr, true, "", &setup);

  if (deviceManager.getCurrentAudioDevice() == nullptr) {
    const OwnedArray<AudioIODeviceType>& device_types = deviceManager.getAvailableDeviceTypes();

    for (AudioIODeviceType* device_type : device_types) {
      deviceManager.setCurrentAudioDeviceType(device_type->getTypeName(), true);
      if (deviceManager.getCurrentAudioDevice())
        break;
    }
  }

  const StringArray all_midi_ins(MidiInput::getDevices());

  for (int i = 0; i < all_midi_ins.size(); ++i)
    deviceManager.setMidiInputEnabled(all_midi_ins[i], true);

  deviceManager.addMidiInputCallback("", midi_manager_);

  setLookAndFeel(DefaultLookAndFeel::instance());

  gui_ = new FullInterface(synth_.getControls(),
                           synth_.getModulationSources(),
                           synth_.getMonoModulations(),
                           synth_.getPolyModulations());
  gui_->setOutputMemory(output_memory_.get());
  addAndMakeVisible(gui_);
  setSize(WIDTH, HEIGHT);

  grabKeyboardFocus();
  setWantsKeyboardFocus(true);
  addKeyListener(computer_keyboard_);
  postMessage(new Message());
  setOpaque(true);
}

HelmStandaloneEditor::~HelmStandaloneEditor() {
  shutdownAudio();
}

void HelmStandaloneEditor::prepareToPlay(int buffer_size, double sample_rate) {
  synth_.setSampleRate(sample_rate);
  synth_.setBufferSize(std::min(buffer_size, mopo::MAX_BUFFER_SIZE));
}

void HelmStandaloneEditor::getNextAudioBlock(const AudioSourceChannelInfo& buffer) {
  ScopedLock lock(critical_section_);
  int num_samples = buffer.buffer->getNumSamples();
  int synth_samples = std::min(num_samples, mopo::MAX_BUFFER_SIZE);

  MOPO_ASSERT(num_samples % synth_samples == 0);

  for (int b = 0; b < num_samples; b += synth_samples) {
    synth_.process();

    const mopo::mopo_float* synth_output_left = synth_.output(0)->buffer;
    const mopo::mopo_float* synth_output_right = synth_.output(1)->buffer;
    for (int channel = 0; channel < mopo::NUM_CHANNELS; ++channel) {
      float* channelData = buffer.buffer->getWritePointer(channel);
      const mopo::mopo_float* synth_output = (channel % 2) ? synth_output_right : synth_output_left;

      for (int i = 0; i < synth_samples; ++i)
        channelData[b + i] = synth_output[i];
    }

    for (int i = 0; i < synth_samples; ++i)
      output_memory_->push(synth_output_left[i] + synth_output_right[i]);
  }
}

void HelmStandaloneEditor::releaseResources() {
}

void HelmStandaloneEditor::paint(Graphics& g) {
}

void HelmStandaloneEditor::resized() {
  gui_->setBounds(0, 0, getWidth(), getHeight());
}

void HelmStandaloneEditor::updateFullGui() {
  gui_->setAllValues(controls_);
}

void HelmStandaloneEditor::updateGuiControl(const std::string& name, mopo::mopo_float value) {
  gui_->setValue(name, value, NotificationType::dontSendNotification);
}

void HelmStandaloneEditor::handleMessage(const Message& message) {
#ifdef JUCE_MAC
  Thread::sleep(100);
  if(!hasKeyboardFocus(false)) {
    postMessage(new Message());
    grabKeyboardFocus();
  }
#endif
}
