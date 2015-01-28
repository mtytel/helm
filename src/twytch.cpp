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

#include "twytch.h"
#include "twytch_editor.h"

#define PITCH_WHEEL_RESOLUTION 0x3fff

Twytch::Twytch() { }

Twytch::~Twytch() { }

const String Twytch::getName() const {
  return JucePlugin_Name;
}

int Twytch::getNumParameters() {
  return 0;
}

float Twytch::getParameter(int index) {
  return 0.0f;
}

void Twytch::setParameter(int index, float new_value) {
}

const String Twytch::getParameterName(int index) {
  return String();
}

const String Twytch::getParameterText(int index) {
  return String();
}

const String Twytch::getInputChannelName(int channel_index) const {
  return String (channel_index + 1);
}

const String Twytch::getOutputChannelName(int channel_index) const {
  return String (channel_index + 1);
}

bool Twytch::isInputChannelStereoPair(int index) const {
  return true;
}

bool Twytch::isOutputChannelStereoPair(int index) const {
  return true;
}

bool Twytch::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool Twytch::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool Twytch::silenceInProducesSilenceOut() const {
  return false;
}

double Twytch::getTailLengthSeconds() const {
  return 0.0;
}

int Twytch::getNumPrograms() {
  // Some hosts don't cope very well if you tell them there are 0 programs,
  // so this should be at least 1, even if you're not really implementing programs.
  return 1;
}

int Twytch::getCurrentProgram() {
  return 0;
}

void Twytch::setCurrentProgram(int index) {
}

const String Twytch::getProgramName(int index) {
  return String();
}

void Twytch::changeProgramName(int index, const String& new_name) {
}

void Twytch::prepareToPlay(double sample_rate, int buffer_size) {
  synth_.setSampleRate(sample_rate);
  synth_.setBufferSize(buffer_size);
}

void Twytch::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

void Twytch::processMidi(juce::MidiBuffer& midi_messages) {
  MidiBuffer::Iterator midi_iter(midi_messages);
  MidiMessage midi_message;
  int midi_sample_position = 0;
  while (midi_iter.getNextEvent(midi_message, midi_sample_position)) {
    if (midi_message.isNoteOn()) {
      float velocity = (1.0 * midi_message.getVelocity()) / mopo::MIDI_SIZE;
      synth_.noteOn(midi_message.getNoteNumber(), velocity, midi_sample_position);
    }
    else if (midi_message.isNoteOff())
      synth_.noteOff(midi_message.getNoteNumber(), midi_sample_position);
    else if (midi_message.isSustainPedalOn())
      synth_.sustainOn();
    else if (midi_message.isSustainPedalOff())
      synth_.sustainOff();
    else if (midi_message.isPitchWheel()) {
      double percent = (1.0 * midi_message.getPitchWheelValue()) / PITCH_WHEEL_RESOLUTION;
      double value = 2 * percent - 1.0;
      synth_.setPitchWheel(value);
    }
    else if (midi_message.isAllNotesOff())
      synth_.allNotesOff();
  }
}

void Twytch::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midi_messages) {
  processMidi(midi_messages);

  int num_samples = buffer.getNumSamples();
  int num_channels = getNumOutputChannels();

  if (synth_.getBufferSize() != num_samples)
    synth_.setBufferSize(num_samples);
  synth_.process();

  const mopo::mopo_float* synth_output = synth_.output()->buffer;
  for (int channel = 0; channel < num_channels; ++channel) {
    float* channelData = buffer.getWritePointer(channel);

    for (int i = 0; i < num_samples; ++i) {
      channelData[i] = synth_output[i];
      output_memory_.push(synth_output[i]);
    }
  }
}

bool Twytch::hasEditor() const {
  return true;
}

AudioProcessorEditor* Twytch::createEditor() {
  return new TwytchEditor(*this);
}

void Twytch::getStateInformation(MemoryBlock& dest_data) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void Twytch::setStateInformation(const void* data, int size_in_bytes) {
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new Twytch();
}
