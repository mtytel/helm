/* Copyright 2013-2015 Matt Tytel
 *
 * twytch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * twytch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with twytch.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "twytch_standalone_editor.h"

#include "mopo.h"
#include "utils.h"

#define NUM_CHANNELS 2

#define WIDTH 754
#define HEIGHT 740
#define MAX_OUTPUT_MEMORY 1048576

TwytchStandaloneEditor::TwytchStandaloneEditor() {
  setSynth(&synth_);
  midi_manager_ = new MidiManager(&synth_, &critical_section_);
  computer_keyboard_ = new TwytchComputerKeyboard(&synth_, &critical_section_);
  output_memory_ = new mopo::Memory(MAX_OUTPUT_MEMORY);
  setAudioChannels(0, NUM_CHANNELS);

  int midi_index = MidiInput::getDefaultDeviceIndex();
  midi_input_ = MidiInput::openDevice(midi_index, midi_manager_);
  if (midi_input_.get())
    midi_input_->start();

  setLookAndFeel(TwytchLookAndFeel::instance());

  gui_ = new FullInterface(synth_.getControls(),
                           synth_.getModulationSources(),
                           synth_.getMonoModulations(),
                           synth_.getPolyModulations());
  gui_->setOutputMemory(output_memory_.get());
  gui_->setModulationConnections(synth_.getModulationConnections());
  addAndMakeVisible(gui_);
  setSize(WIDTH, HEIGHT);

  grabKeyboardFocus();
  setWantsKeyboardFocus(true);
  addKeyListener(computer_keyboard_);
  postMessage(new Message());
  setOpaque(true);
}

TwytchStandaloneEditor::~TwytchStandaloneEditor() {
  shutdownAudio();
}

void TwytchStandaloneEditor::prepareToPlay(int buffer_size, double sample_rate) {
  synth_.setSampleRate(sample_rate);
  synth_.setBufferSize(std::min(buffer_size, mopo::MAX_BUFFER_SIZE));
}

void TwytchStandaloneEditor::getNextAudioBlock(const AudioSourceChannelInfo& buffer) {
  ScopedLock lock(critical_section_);
  int num_samples = buffer.buffer->getNumSamples();
  int synth_samples = std::min(num_samples, mopo::MAX_BUFFER_SIZE);

  MOPO_ASSERT(num_samples % synth_samples == 0);

  for (int b = 0; b < num_samples; b += synth_samples) {
    synth_.process();

    const mopo::mopo_float* synth_output = synth_.output()->buffer;
    for (int channel = 0; channel < NUM_CHANNELS; ++channel) {
      float* channelData = buffer.buffer->getWritePointer(channel);

      for (int i = 0; i < synth_samples; ++i)
        channelData[b + i] = synth_output[i];
    }

    for (int i = 0; i < synth_samples; ++i)
      output_memory_->push(synth_output[i]);
  }
}

void TwytchStandaloneEditor::releaseResources() {
}

void TwytchStandaloneEditor::paint(Graphics& g) {
}

void TwytchStandaloneEditor::resized() {
  gui_->setBounds(0, 0, getWidth(), getHeight());
}

void TwytchStandaloneEditor::updateFullGui() {
  gui_->setAllValues(controls_);
  gui_->setModulationConnections(synth_.getModulationConnections());
}

void TwytchStandaloneEditor::handleMessage(const Message& message) {
#ifdef JUCE_MAC
  Thread::sleep(100);
  if(!hasKeyboardFocus(false)) {
    postMessage(new Message());
    grabKeyboardFocus();
  }
#endif
}
