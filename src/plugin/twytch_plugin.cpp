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

#include "twytch_plugin.h"
#include "twytch_common.h"
#include "twytch_editor.h"
#include "value_bridge.h"

#define PITCH_WHEEL_RESOLUTION 0x3fff
#define MAX_MEMORY_SAMPLES 1000000

TwytchPlugin::TwytchPlugin() {
  controls_ = synth_.getControls();
  for (auto control : controls_) {
    ValueBridge* bridge = new ValueBridge(control.first, control.second);
    bridge_lookup_[control.first] = bridge;
    addParameter(bridge);
  }

  output_memory_ = new mopo::Memory(MAX_MEMORY_SAMPLES);
  midi_manager_ = new MidiManager(&synth_, &getCallbackLock());
}

TwytchPlugin::~TwytchPlugin() { }

const String TwytchPlugin::getName() const {
  return JucePlugin_Name;
}

const String TwytchPlugin::getInputChannelName(int channel_index) const {
  return String(channel_index + 1);
}

const String TwytchPlugin::getOutputChannelName(int channel_index) const {
  return String(channel_index + 1);
}

bool TwytchPlugin::isInputChannelStereoPair(int index) const {
  return true;
}

bool TwytchPlugin::isOutputChannelStereoPair(int index) const {
  return true;
}

bool TwytchPlugin::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool TwytchPlugin::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool TwytchPlugin::silenceInProducesSilenceOut() const {
  return false;
}

double TwytchPlugin::getTailLengthSeconds() const {
  return 0.0;
}

int TwytchPlugin::getNumPrograms() {
  // Some hosts don't cope very well if you tell them there are 0 programs,
  // so this should be at least 1, even if you're not really implementing programs.
  return 1;
}

int TwytchPlugin::getCurrentProgram() {
  return 0;
}

void TwytchPlugin::setCurrentProgram(int index) {
}

const String TwytchPlugin::getProgramName(int index) {
  return String();
}

void TwytchPlugin::changeProgramName(int index, const String& new_name) {
}

void TwytchPlugin::prepareToPlay(double sample_rate, int buffer_size) {
  synth_.setSampleRate(sample_rate);
  synth_.setBufferSize(buffer_size);
}

void TwytchPlugin::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

void TwytchPlugin::processMidi(juce::MidiBuffer& midi_messages) {
  MidiBuffer::Iterator midi_iter(midi_messages);
  MidiMessage midi_message;
  int midi_sample_position = 0;
  while (midi_iter.getNextEvent(midi_message, midi_sample_position))
    midi_manager_->processMidiMessage(midi_message, midi_sample_position);
}

void TwytchPlugin::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midi_messages) {
  processMidi(midi_messages);

  int num_samples = buffer.getNumSamples();
  int num_channels = getNumOutputChannels();
  juce::AudioPlayHead::CurrentPositionInfo position_info;
  getPlayHead()->getCurrentPosition(position_info);
  synth_.setBpm(position_info.bpm);

  if (synth_.getBufferSize() != num_samples)
    synth_.setBufferSize(num_samples);
  synth_.process();

  const mopo::mopo_float* synth_output = synth_.output()->buffer;
  for (int channel = 0; channel < num_channels; ++channel) {
    float* channelData = buffer.getWritePointer(channel);

    for (int i = 0; i < num_samples; ++i)
      channelData[i] = synth_output[i];
  }

  for (int i = 0; i < num_samples; ++i)
    output_memory_->push(synth_output[i]);
}

bool TwytchPlugin::hasEditor() const {
  return true;
}

AudioProcessorEditor* TwytchPlugin::createEditor() {
  return new TwytchEditor(*this);
}

void TwytchPlugin::getStateInformation(MemoryBlock& dest_data) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void TwytchPlugin::setStateInformation(const void* data, int size_in_bytes) {
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new TwytchPlugin();
}
