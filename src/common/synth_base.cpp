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

#include "synth_base.h"

#include "load_save.h"
#include "startup.h"
#include "synth_gui_interface.h"
#include "utils.h"

#define OUTPUT_WINDOW_MIN_NOTE 16.0

SynthBase::SynthBase() {
  controls_ = engine_.getControls();

  keyboard_state_ = new MidiKeyboardState();
  midi_manager_ = new MidiManager(this, keyboard_state_, &save_info_, this);

  last_played_note_ = 0.0;
  last_num_pressed_ = 0;
  memset(output_memory_, 0, 2 * mopo::MEMORY_RESOLUTION * sizeof(float));
  memset(output_memory_write_, 0, 2 * mopo::MEMORY_RESOLUTION * sizeof(float));
  memory_reset_period_ = mopo::MEMORY_RESOLUTION;
  memory_input_offset_ = 0;
  memory_index_ = 0;

  Startup::doStartupChecks(midi_manager_);
}

void SynthBase::valueChanged(const std::string& name, mopo::mopo_float value) {
  value_change_queue_.enqueue(mopo::control_change(controls_[name], value));
}

void SynthBase::valueChangedInternal(const std::string& name, mopo::mopo_float value) {
  valueChanged(name, value);
  setValueNotifyHost(name, value);
}

void SynthBase::valueChangedThroughMidi(const std::string& name, mopo::mopo_float value) {
  controls_[name]->set(value);
  ValueChangedCallback* callback = new ValueChangedCallback(this, name, value);
  setValueNotifyHost(name, value);
  callback->post();
}

void SynthBase::patchChangedThroughMidi(File patch) {
  SynthGuiInterface* gui_interface = getGuiInterface();
  if (gui_interface) {
    gui_interface->updateFullGui();
    gui_interface->notifyFresh();
  }
}

void SynthBase::valueChangedExternal(const std::string& name, mopo::mopo_float value) {
  valueChanged(name, value);
  ValueChangedCallback* callback = new ValueChangedCallback(this, name, value);
  callback->post();
}

void SynthBase::changeModulationAmount(const std::string& source,
                                       const std::string& destination,
                                       mopo::mopo_float amount) {
  mopo::ModulationConnection* connection = getConnection(source, destination);
  if (connection == nullptr && amount != 0.0)
    connection = modulation_bank_.get(source, destination);

  if (connection)
    setModulationAmount(connection, amount);
}

mopo::ModulationConnection* SynthBase::getConnection(const std::string& source,
                                                     const std::string& destination) {
  for (mopo::ModulationConnection* connection : mod_connections_) {
    if (connection->source == source && connection->destination == destination)
      return connection;
  }
  return nullptr;
}

void SynthBase::setModulationAmount(mopo::ModulationConnection* connection,
                                    mopo::mopo_float amount) {
  if (amount == 0.0) {
    modulation_bank_.recycle(connection);
    mod_connections_.erase(connection);
  }
  else if (mod_connections_.count(connection) == 0)
    mod_connections_.insert(connection);
  modulation_change_queue_.enqueue(mopo::modulation_change(connection, amount));
}

void SynthBase::disconnectModulation(mopo::ModulationConnection* connection) {
  setModulationAmount(connection, 0.0);
}

void SynthBase::clearModulations() {
  while (mod_connections_.size())
    disconnectModulation(*mod_connections_.begin());
}

int SynthBase::getNumModulations(const std::string& destination) {
  int connections = 0;
  for (mopo::ModulationConnection* connection : mod_connections_) {
    if (connection->destination == destination)
      connections++;
  }
  return connections;
}

std::vector<mopo::ModulationConnection*>
SynthBase::getSourceConnections(const std::string& source) {
  std::vector<mopo::ModulationConnection*> connections;
  for (mopo::ModulationConnection* connection : mod_connections_) {
    if (connection->source == source)
      connections.push_back(connection);
  }
  return connections;
}

std::vector<mopo::ModulationConnection*>
SynthBase::getDestinationConnections(const std::string& destination) {
  std::vector<mopo::ModulationConnection*> connections;
  for (mopo::ModulationConnection* connection : mod_connections_) {
    if (connection->destination == destination)
      connections.push_back(connection);
  }
  return connections;
}

mopo::Output* SynthBase::getModSource(const std::string& name) {
  ScopedLock lock(getCriticalSection());
  return engine_.getModulationSource(name);
}

var SynthBase::saveToVar(String author) {
  save_info_["author"] = author;
  return LoadSave::stateToVar(this, save_info_, getCriticalSection());
}

void SynthBase::loadInitPatch() {
  getCriticalSection().enter();
  LoadSave::initSynth(this, save_info_);
  getCriticalSection().exit();
}

void SynthBase::loadFromVar(juce::var state) {
  getCriticalSection().enter();
  LoadSave::varToState(this, save_info_, state);
  getCriticalSection().exit();
}

bool SynthBase::loadFromFile(File patch) {
  var parsed_json_state;
  if (patch.exists() && JSON::parse(patch.loadFileAsString(), parsed_json_state).wasOk()) {
    active_file_ = patch;
    File parent = patch.getParentDirectory();
    loadFromVar(parsed_json_state);
    setFolderName(parent.getFileNameWithoutExtension());
    setPatchName(patch.getFileNameWithoutExtension());

    SynthGuiInterface* gui_interface = getGuiInterface();
    if (gui_interface) {
      gui_interface->updateFullGui();
      gui_interface->notifyFresh();
    }

    return true;
  }
  return false;
}

bool SynthBase::exportToFile() {
  File active_file = getActiveFile();
  FileChooser save_box("Export Patch", File(), String("*.") + mopo::PATCH_EXTENSION);
  if (!save_box.browseForFileToSave(true))
    return false;

  saveToFile(save_box.getResult());
  return true;
}

bool SynthBase::saveToFile(File patch) {
  if (patch.getFileExtension() != String(mopo::PATCH_EXTENSION))
    patch = patch.withFileExtension(String(mopo::PATCH_EXTENSION));

  File parent = patch.getParentDirectory();
  setFolderName(parent.getFileNameWithoutExtension());
  setPatchName(patch.getFileNameWithoutExtension());

  SynthGuiInterface* gui_interface = getGuiInterface();
  if (gui_interface) {
    gui_interface->updateFullGui();
    gui_interface->notifyFresh();
  }

  if (patch.replaceWithText(JSON::toString(saveToVar(save_info_["author"])))) {
    active_file_ = patch;
    return true;
  }
  return false;
}

bool SynthBase::saveToActiveFile() {
  if (!active_file_.exists() || !active_file_.hasWriteAccess())
    return false;

  return saveToFile(active_file_);
}

void SynthBase::processAudio(AudioSampleBuffer* buffer, int channels, int samples, int offset) {
  mopo::utils::enableDenormalFlushing(true);

  if (engine_.getBufferSize() != samples)
    engine_.setBufferSize(samples);

  engine_.process();

  const mopo::mopo_float* engine_output_left = engine_.output(0)->buffer;
  const mopo::mopo_float* engine_output_right = engine_.output(1)->buffer;
  for (int channel = 0; channel < channels; ++channel) {
    float* channelData = buffer->getWritePointer(channel, offset);
    const mopo::mopo_float* synth_output = (channel % 2) ? engine_output_right : engine_output_left;

    VECTORIZE_LOOP
    for (int i = 0; i < samples; ++i) {
      channelData[i] = synth_output[i];
      MOPO_ASSERT(std::isfinite(synth_output[i]));
    }
  }

  updateMemoryOutput(samples, engine_output_left, engine_output_right);
}

void SynthBase::processMidi(MidiBuffer& midi_messages, int start_sample, int end_sample) {
  MidiBuffer::Iterator midi_iter(midi_messages);
  MidiMessage midi_message;
  int midi_sample = 0;
  bool process_all = end_sample == 0;
  while (midi_iter.getNextEvent(midi_message, midi_sample)) {
    if (process_all || (midi_sample >= start_sample && midi_sample < end_sample))
      midi_manager_->processMidiMessage(midi_message, midi_sample - start_sample);
  }
}

void SynthBase::processKeyboardEvents(MidiBuffer& buffer, int num_samples) {
  MidiBuffer keyboard_messages;
  midi_manager_->replaceKeyboardMessages(keyboard_messages, num_samples);
  midi_manager_->replaceKeyboardMessages(buffer, num_samples);

  processMidi(keyboard_messages);
}

void SynthBase::processControlChanges() {
  mopo::control_change change;
  while (getNextControlChange(change))
    change.first->set(change.second);
}

void SynthBase::processModulationChanges() {
  mopo::modulation_change change;
  while (getNextModulationChange(change)) {
    mopo::ModulationConnection* connection = change.first;
    mopo::mopo_float amount = change.second;
    connection->amount.set(amount);

    bool active = engine_.isModulationActive(connection);
    if (active && amount == 0.0)
      engine_.disconnectModulation(connection);
    else if (!active && amount)
      engine_.connectModulation(connection);
  }
}

void SynthBase::updateMemoryOutput(int samples, const mopo::mopo_float* left,
                                                const mopo::mopo_float* right) {
  mopo::mopo_float last_played = std::max(engine_.getLastActiveNote(), OUTPUT_WINDOW_MIN_NOTE);
  int num_pressed = engine_.getPressedNotes().size();
  int output_inc = std::max<int>(1, engine_.getSampleRate() / mopo::MEMORY_SAMPLE_RATE);

  if (last_played && (last_played_note_ != last_played || num_pressed > last_num_pressed_)) {
    last_played_note_ = last_played;
    
    mopo::mopo_float frequency = mopo::utils::midiNoteToFrequency(last_played_note_);
    mopo::mopo_float period = engine_.getSampleRate() / frequency;
    int window_length = output_inc * mopo::MEMORY_RESOLUTION;

    memory_reset_period_ = period;
    while (memory_reset_period_ < window_length)
      memory_reset_period_ += memory_reset_period_;

    memory_reset_period_ = std::min(memory_reset_period_, 2.0 * window_length);
    memory_index_ = 0;
    mopo::utils::copyBufferf(output_memory_, output_memory_write_, 2 * mopo::MEMORY_RESOLUTION);
  }
  last_num_pressed_ = num_pressed;

  for (; memory_input_offset_ < samples; memory_input_offset_ += output_inc) {
    int input_index = mopo::utils::iclamp(memory_input_offset_, 0, samples);
    memory_index_ = mopo::utils::iclamp(memory_index_, 0, 2 * mopo::MEMORY_RESOLUTION - 1);
    MOPO_ASSERT(input_index >= 0);
    MOPO_ASSERT(input_index < samples);
    MOPO_ASSERT(memory_index_ >= 0);
    MOPO_ASSERT(memory_index_ < 2 * mopo::MEMORY_RESOLUTION);
    output_memory_write_[memory_index_++] = (left[input_index] + right[input_index]) / 2.0;

    if (memory_index_ * output_inc >= memory_reset_period_) {
      memory_input_offset_ += memory_reset_period_ - memory_index_ * output_inc;
      memory_index_ = 0;
      mopo::utils::copyBufferf(output_memory_, output_memory_write_, 2 * mopo::MEMORY_RESOLUTION);
    }
  }

  memory_input_offset_ -= samples;
}

void SynthBase::armMidiLearn(const std::string& name) {
  midi_manager_->armMidiLearn(name);
}

void SynthBase::cancelMidiLearn() {
  midi_manager_->cancelMidiLearn();
}

void SynthBase::clearMidiLearn(const std::string& name) {
  midi_manager_->clearMidiLearn(name);
}

bool SynthBase::isMidiMapped(const std::string& name) {
  return midi_manager_->isMidiMapped(name);
}

void SynthBase::setAuthor(String author) {
  save_info_["author"] = author;
}

void SynthBase::setPatchName(String patch_name) {
  save_info_["patch_name"] = patch_name;
}

void SynthBase::setFolderName(String folder_name) {
  save_info_["folder_name"] = folder_name;
}

String SynthBase::getAuthor() {
  return save_info_["author"];
}

String SynthBase::getPatchName() {
  return save_info_["patch_name"];
}

String SynthBase::getFolderName() {
  return save_info_["folder_name"];
}

void SynthBase::ValueChangedCallback::messageCallback() {
  if (listener) {
    SynthGuiInterface* gui_interface = listener->getGuiInterface();
    if (gui_interface) {
      gui_interface->updateGuiControl(control_name, value);
      gui_interface->notifyChange();
    }
  }
}
