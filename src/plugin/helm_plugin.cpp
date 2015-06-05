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

#include "helm_plugin.h"
#include "helm_common.h"
#include "helm_editor.h"
#include "value_bridge.h"

#define PITCH_WHEEL_RESOLUTION 0x3fff
#define MAX_MEMORY_SAMPLES 1000000

HelmPlugin::HelmPlugin() {
  controls_ = synth_.getControls();
  for (auto control : controls_) {
    ValueBridge* bridge = new ValueBridge(control.first, control.second);
    bridge->setListener(this);
    bridge_lookup_[control.first] = bridge;
    addParameter(bridge);
  }

  output_memory_ = new mopo::Memory(MAX_MEMORY_SAMPLES);
  midi_manager_ = new MidiManager(&synth_, &getCallbackLock());
}

HelmPlugin::~HelmPlugin() { }

const String HelmPlugin::getName() const {
  return JucePlugin_Name;
}

const String HelmPlugin::getInputChannelName(int channel_index) const {
  return String(channel_index + 1);
}

const String HelmPlugin::getOutputChannelName(int channel_index) const {
  return String(channel_index + 1);
}

bool HelmPlugin::isInputChannelStereoPair(int index) const {
  return true;
}

bool HelmPlugin::isOutputChannelStereoPair(int index) const {
  return true;
}

bool HelmPlugin::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool HelmPlugin::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool HelmPlugin::silenceInProducesSilenceOut() const {
  return false;
}

double HelmPlugin::getTailLengthSeconds() const {
  return 0.0;
}

int HelmPlugin::getNumPrograms() {
  // Some hosts don't cope very well if you tell them there are 0 programs,
  // so this should be at least 1, even if you're not really implementing programs.
  return 1;
}

int HelmPlugin::getCurrentProgram() {
  return 0;
}

void HelmPlugin::setCurrentProgram(int index) {
}

const String HelmPlugin::getProgramName(int index) {
  return String();
}

void HelmPlugin::changeProgramName(int index, const String& new_name) {
}

void HelmPlugin::prepareToPlay(double sample_rate, int buffer_size) {
  synth_.setSampleRate(sample_rate);
  synth_.setBufferSize(buffer_size);
}

void HelmPlugin::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

void HelmPlugin::beginChangeGesture(std::string name) {
  bridge_lookup_[name]->beginChangeGesture();
}

void HelmPlugin::endChangeGesture(std::string name) {
  bridge_lookup_[name]->endChangeGesture();
}

void HelmPlugin::processMidi(juce::MidiBuffer& midi_messages) {
  MidiBuffer::Iterator midi_iter(midi_messages);
  MidiMessage midi_message;
  int midi_sample_position = 0;
  while (midi_iter.getNextEvent(midi_message, midi_sample_position))
    midi_manager_->processMidiMessage(midi_message, midi_sample_position);
}

void HelmPlugin::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midi_messages) {
  processMidi(midi_messages);

  int num_samples = buffer.getNumSamples();
  int num_channels = getNumOutputChannels();
  AudioPlayHead::CurrentPositionInfo position_info;
  getPlayHead()->getCurrentPosition(position_info);
  synth_.setBpm(position_info.bpm);

  if (synth_.getBufferSize() != num_samples)
    synth_.setBufferSize(num_samples);
  synth_.process();

  const mopo::mopo_float* synth_output_left = synth_.output(0)->buffer;
  const mopo::mopo_float* synth_output_right = synth_.output(1)->buffer;
  for (int channel = 0; channel < num_channels; ++channel) {
    float* channelData = buffer.getWritePointer(channel);
    const mopo::mopo_float* synth_output = (channel % 2) ? synth_output_right : synth_output_left;

    for (int i = 0; i < num_samples; ++i)
      channelData[i] = synth_output[i];
  }

  for (int i = 0; i < num_samples; ++i)
    output_memory_->push(synth_output_left[i] + synth_output_right[i]);
}

bool HelmPlugin::hasEditor() const {
  return true;
}

AudioProcessorEditor* HelmPlugin::createEditor() {
  return new HelmEditor(*this);
}

void HelmPlugin::parameterChanged(std::string name, mopo::mopo_float value) {
  AudioProcessorEditor* editor = getActiveEditor();
  HelmEditor* t_editor = dynamic_cast<HelmEditor*>(editor);
  if (t_editor)
    t_editor->valueChangedExternal(name, value);
}

void HelmPlugin::setValueNotifyHost(std::string name, mopo::mopo_float value) {
  mopo::mopo_float plugin_value =  bridge_lookup_[name]->convertToPluginValue(value);
  bridge_lookup_[name]->setValueNotifyingHost(plugin_value);
}

void HelmPlugin::getStateInformation(MemoryBlock& dest_data) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void HelmPlugin::setStateInformation(const void* data, int size_in_bytes) {
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new HelmPlugin();
}
