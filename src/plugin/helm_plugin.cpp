/* Copyright 2013-2016 Matt Tytel
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
#include "load_save.h"

#define PITCH_WHEEL_RESOLUTION 0x3fff
#define MAX_BUFFER_PROCESS 256
#define SET_PROGRAM_WAIT_MILLISECONDS 500

HelmPlugin::HelmPlugin() {
  set_state_time_ = 0;

  current_program_ = 0;
  num_programs_ = LoadSave::getNumPatches();
  if (num_programs_ <= 0)
    num_programs_ = 1;

  for (auto control : controls_) {
    ValueBridge* bridge = new ValueBridge(control.first, control.second);
    bridge->setListener(this);
    bridge_lookup_[control.first] = bridge;
    addParameter(bridge);
  }
}

HelmPlugin::~HelmPlugin() {
  midi_manager_ = nullptr;
  keyboard_state_ = nullptr;
}

SynthGuiInterface* HelmPlugin::getGuiInterface() {
  AudioProcessorEditor* editor = getActiveEditor();
  if (editor)
    return dynamic_cast<SynthGuiInterface*>(editor);
  return nullptr;
}

void HelmPlugin::beginChangeGesture(const std::string& name) {
  bridge_lookup_[name]->beginChangeGesture();
}

void HelmPlugin::endChangeGesture(const std::string& name) {
  bridge_lookup_[name]->endChangeGesture();
}

void HelmPlugin::setValueNotifyHost(const std::string& name, mopo::mopo_float value) {
  mopo::mopo_float plugin_value =  bridge_lookup_[name]->convertToPluginValue(value);
  bridge_lookup_[name]->setValueNotifyingHost(plugin_value);
}

const CriticalSection& HelmPlugin::getCriticalSection() {
  return getCallbackLock();
}

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
  return num_programs_;
}

int HelmPlugin::getCurrentProgram() {
  return current_program_;
}

void HelmPlugin::setCurrentProgram(int index) {
  // Hack for some DAWs that set program on load for VSTs.
  if (Time::getMillisecondCounter() - set_state_time_ < SET_PROGRAM_WAIT_MILLISECONDS)
    return;

  current_program_ = index;
  LoadSave::loadPatch(-1, -1, index, &engine_, save_info_);
  AudioProcessorEditor* editor = getActiveEditor();
  if (editor) {
    HelmEditor* t_editor = dynamic_cast<HelmEditor*>(editor);
    t_editor->updateFullGui();
  }
}

const String HelmPlugin::getProgramName(int index) {
  return LoadSave::getPatchFile(-1, -1, index).getFileNameWithoutExtension();
}

void HelmPlugin::changeProgramName(int index, const String& new_name) {
  File patch = LoadSave::getPatchFile(-1, -1, index);
  File parent = patch.getParentDirectory();
  File new_patch_location = parent.getChildFile(new_name + "." + mopo::PATCH_EXTENSION);
  patch.moveFileTo(new_patch_location);
}

void HelmPlugin::prepareToPlay(double sample_rate, int buffer_size) {
  engine_.setSampleRate(sample_rate);
  engine_.setBufferSize(std::min<int>(buffer_size, MAX_BUFFER_PROCESS));
  midi_manager_->setSampleRate(sample_rate);
}

void HelmPlugin::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

void HelmPlugin::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midi_messages) {
  int total_samples = buffer.getNumSamples();
  int num_channels = getTotalNumOutputChannels();
  AudioPlayHead::CurrentPositionInfo position_info;
  getPlayHead()->getCurrentPosition(position_info);
  engine_.setBpm(position_info.bpm);

  if (position_info.isPlaying || position_info.isLooping || position_info.isRecording)
    engine_.correctToTime(position_info.timeInSamples);

  processControlChanges();
  processKeyboardEvents(total_samples);

  for (int sample_offset = 0; sample_offset < total_samples;) {
    int num_samples = std::min<int>(total_samples - sample_offset, MAX_BUFFER_PROCESS);

    processMidi(midi_messages, sample_offset, sample_offset + num_samples);
    processAudio(&buffer, num_channels, num_samples, sample_offset);

    sample_offset += num_samples;
  }

  midi_manager_->replaceKeyboardMessages(midi_messages, total_samples);
}

bool HelmPlugin::hasEditor() const {
  return true;
}

AudioProcessorEditor* HelmPlugin::createEditor() {
  return new HelmEditor(*this);
}

void HelmPlugin::parameterChanged(std::string name, mopo::mopo_float value) {
  valueChangedExternal(name, value);
}

void HelmPlugin::getStateInformation(MemoryBlock& dest_data) {
  var state = LoadSave::stateToVar(&engine_, save_info_, getCallbackLock());
  String data_string = JSON::toString(state);
  MemoryOutputStream stream;
  stream.writeString(data_string);
  dest_data.append(stream.getData(), stream.getDataSize());
}

void HelmPlugin::setStateInformation(const void* data, int size_in_bytes) {
  set_state_time_ = Time::getMillisecondCounter();

  MemoryInputStream stream(data, size_in_bytes, false);
  String data_string = stream.readEntireStreamAsString();
  var state;
  if (JSON::parse(data_string, state).wasOk())
    LoadSave::varToState(&engine_, save_info_, state);
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new HelmPlugin();
}
