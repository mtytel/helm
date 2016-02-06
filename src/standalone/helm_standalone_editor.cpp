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
#include "load_save.h"
#include "mopo.h"
#include "startup.h"
#include "utils.h"

#define WIDTH 992
#define HEIGHT 734
#define MAX_OUTPUT_MEMORY 1048576
#define MAX_BUFFER_PROCESS 256

HelmStandaloneEditor::HelmStandaloneEditor() {
  setSynth(&synth_);
  setGuiState(&gui_state_);
  keyboard_state_ = new MidiKeyboardState();
  midi_manager_ = new MidiManager(&synth_, keyboard_state_, &gui_state_, &critical_section_, this);
  computer_keyboard_ = new HelmComputerKeyboard(&synth_, keyboard_state_, &critical_section_);
  output_memory_ = new mopo::Memory(MAX_OUTPUT_MEMORY);
  memory_offset_ = 0;

  Startup::doStartupChecks(midi_manager_, computer_keyboard_);
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
                           synth_.getPolyModulations(),
                           keyboard_state_);
  addAndMakeVisible(gui_);
  gui_->setOutputMemory(output_memory_.get());
  gui_->animate(LoadSave::shouldAnimateWidgets());
  setSize(WIDTH, HEIGHT);

  setWantsKeyboardFocus(true);
  grabKeyboardFocus();
  addKeyListener(computer_keyboard_);
  postMessage(new Message());
  setOpaque(true);
}

HelmStandaloneEditor::~HelmStandaloneEditor() {
  midi_manager_ = nullptr;
  computer_keyboard_ = nullptr;
  gui_ = nullptr;
  keyboard_state_ = nullptr;
  shutdownAudio();
}

void HelmStandaloneEditor::prepareToPlay(int buffer_size, double sample_rate) {
  synth_.setSampleRate(sample_rate);
  synth_.setBufferSize(std::min(buffer_size, MAX_BUFFER_PROCESS));
}

void HelmStandaloneEditor::getNextAudioBlock(const AudioSourceChannelInfo& buffer) {
  ScopedLock lock(critical_section_);
  int num_samples = buffer.buffer->getNumSamples();
  int synth_samples = std::min(num_samples, MAX_BUFFER_PROCESS);

  for (int b = 0; b < num_samples; b += synth_samples) {
    int current_samples = std::min<int>(synth_samples, num_samples - b);

    if (current_samples != synth_.getBufferSize())
      synth_.setBufferSize(current_samples);

    synth_.process();

    const mopo::mopo_float* synth_output_left = synth_.output(0)->buffer;
    const mopo::mopo_float* synth_output_right = synth_.output(1)->buffer;
    for (int channel = 0; channel < mopo::NUM_CHANNELS; ++channel) {
      float* channel_data = buffer.buffer->getWritePointer(channel, b);
      const mopo::mopo_float* synth_output = (channel % 2) ? synth_output_right : synth_output_left;

      #pragma clang loop vectorize(enable) interleave(enable)
      for (int i = 0; i < current_samples; ++i)
        channel_data[i] = synth_output[i];
    }

    int output_inc = synth_.getSampleRate() / mopo::MEMORY_SAMPLE_RATE;
    for (; memory_offset_ < current_samples; memory_offset_ += output_inc)
      output_memory_->push(synth_output_left[memory_offset_] + synth_output_right[memory_offset_]);
    memory_offset_ -= current_samples;
  }
}

void HelmStandaloneEditor::releaseResources() {
}

void HelmStandaloneEditor::paint(Graphics& g) {
}

void HelmStandaloneEditor::resized() {
  Rectangle<int> bounds = getBounds();
  double scale = std::min(double(bounds.getWidth()) / WIDTH, double(bounds.getHeight()) / HEIGHT);
  gui_->setTransform(AffineTransform::scale(scale));
  gui_->setBounds(Rectangle<int>(0, 0, bounds.getWidth() / scale, bounds.getHeight() / scale));
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
