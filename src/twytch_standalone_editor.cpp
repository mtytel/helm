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

#define PITCH_WHEEL_RESOLUTION 0x3fff
#define NUM_CHANNELS 2

#define WIDTH 1000
#define HEIGHT 800
#define DEFAULT_KEYBOARD_OFFSET 48
#define KEYBOARD "awsedftgyhujkolp;'"

TwytchStandaloneEditor::TwytchStandaloneEditor() {
  setAudioChannels(0, NUM_CHANNELS);
  postMessage(new Message());

  computer_keyboard_offset_ = DEFAULT_KEYBOARD_OFFSET;

  int midi_index = MidiInput::getDefaultDeviceIndex();
  midi_input_.reset(MidiInput::openDevice(midi_index, this));
  if (midi_input_.get())
    midi_input_->start();

  controls_ = synth_.getControls();
  setLookAndFeel(&look_and_feel_);

  gui_ = new FullInterface(controls_, synth_.getModulationDestinations());
  gui_->setOutputMemory(&output_memory_);
  gui_->setModulationConnections(synth_.getModulationConnections());
  addAndMakeVisible(gui_);
  setSize(WIDTH, HEIGHT);

  grabKeyboardFocus();
  setWantsKeyboardFocus(true);
  addKeyListener(this);
}

TwytchStandaloneEditor::~TwytchStandaloneEditor() {
  shutdownAudio();
}

void TwytchStandaloneEditor::changeKeyboardOffset(int new_offset) {
  for (int i = 0; i < strlen(KEYBOARD); ++i) {
    int note = computer_keyboard_offset_ + i;
    synth_.noteOff(note);
    keys_pressed_.erase(KEYBOARD[i]);
  }

  computer_keyboard_offset_ = CLAMP(new_offset, 0, mopo::MIDI_SIZE - mopo::NOTES_PER_OCTAVE);
}

void TwytchStandaloneEditor::prepareToPlay(int buffer_size, double sample_rate) {
  synth_.setSampleRate(sample_rate);
  synth_.setBufferSize(buffer_size);
}

void TwytchStandaloneEditor::getNextAudioBlock(const AudioSourceChannelInfo& buffer) {
  ScopedLock lock(critical_section_);
  synth_.process();
  int num_samples = buffer.buffer->getNumSamples();

  const mopo::mopo_float* synth_output = synth_.output()->buffer;
  for (int channel = 0; channel < NUM_CHANNELS; ++channel) {
    float* channelData = buffer.buffer->getWritePointer(channel);

    for (int i = 0; i < num_samples; ++i)
      channelData[i] = synth_output[i];
  }

  for (int i = 0; i < num_samples; ++i)
    output_memory_.push(synth_output[i]);
}

void TwytchStandaloneEditor::releaseResources() {
}

void TwytchStandaloneEditor::paint(Graphics& g) {
  g.fillAll(Colours::black);
}

void TwytchStandaloneEditor::resized() {
  gui_->setBounds(0, 0, getWidth(), getHeight());
}

bool TwytchStandaloneEditor::keyPressed(const KeyPress &key, Component *origin) {
  return false;
}

bool TwytchStandaloneEditor::keyStateChanged(bool isKeyDown, Component *originatingComponent) {
  bool consumed = false;
  ScopedLock lock(critical_section_);
  for (int i = 0; i < strlen(KEYBOARD); ++i) {
    int note = computer_keyboard_offset_ + i;

    if (KeyPress::isKeyCurrentlyDown(KEYBOARD[i]) && !keys_pressed_.count(KEYBOARD[i])) {
      keys_pressed_.insert(KEYBOARD[i]);
      synth_.noteOn(note);
    }
    else if (!KeyPress::isKeyCurrentlyDown(KEYBOARD[i]) && keys_pressed_.count(KEYBOARD[i])) {
      keys_pressed_.erase(KEYBOARD[i]);
      synth_.noteOff(note);
    }
    consumed = true;
  }

  if (KeyPress::isKeyCurrentlyDown('z')) {
    if (!keys_pressed_.count('z')) {
      keys_pressed_.insert('z');
      changeKeyboardOffset(computer_keyboard_offset_ - mopo::NOTES_PER_OCTAVE);
    }
  }
  else
    keys_pressed_.erase('z');

  if (KeyPress::isKeyCurrentlyDown('x')) {
    if (!keys_pressed_.count('x')) {
      keys_pressed_.insert('x');
      changeKeyboardOffset(computer_keyboard_offset_ + mopo::NOTES_PER_OCTAVE);
    }
  }
  else
    keys_pressed_.erase('x');

  return consumed;
}

void TwytchStandaloneEditor::handleIncomingMidiMessage(MidiInput *source,
                                                       const MidiMessage &midi_message) {
  ScopedLock lock(critical_section_);
  if (midi_message.isNoteOn()) {
    float velocity = (1.0 * midi_message.getVelocity()) / mopo::MIDI_SIZE;
    synth_.noteOn(midi_message.getNoteNumber(), velocity);
  }
  else if (midi_message.isNoteOff())
    synth_.noteOff(midi_message.getNoteNumber());
  else if (midi_message.isSustainPedalOn())
    synth_.sustainOn();
  else if (midi_message.isSustainPedalOff())
    synth_.sustainOff();
  else if (midi_message.isAllNotesOff())
    synth_.allNotesOff();
  else if (midi_message.isAftertouch()) {
    mopo::mopo_float note = midi_message.getNoteNumber();
    mopo::mopo_float value = (1.0 * midi_message.getAfterTouchValue()) / mopo::MIDI_SIZE;
    synth_.setAftertouch(note, value);
  }
  else if (midi_message.isPitchWheel()) {
    double percent = (1.0 * midi_message.getPitchWheelValue()) / PITCH_WHEEL_RESOLUTION;
    double value = 2 * percent - 1.0;
    synth_.setPitchWheel(value);
    int tmp = midi_message.getNoteNumber();
    std::cout<< tmp << std::endl;
  }
}

void TwytchStandaloneEditor::valueChanged(std::string name, mopo::mopo_float value) {
  if (controls_.count(name))
    controls_[name]->set(value);
}

void TwytchStandaloneEditor::connectModulation(mopo::ModulationConnection* connection) {
  ScopedLock lock(critical_section_);
  synth_.connectModulation(connection);
}

void TwytchStandaloneEditor::disconnectModulation(mopo::ModulationConnection* connection) {
  ScopedLock lock(critical_section_);
  synth_.disconnectModulation(connection);
}

const mopo::Processor::Output* TwytchStandaloneEditor::getModulationSourceOutput(std::string name) {
  return synth_.getModulationSourceOutput(name);
}

var TwytchStandaloneEditor::stateToVar() {
  mopo::control_map controls = synth_.getControls();
  DynamicObject* state_object = new DynamicObject();

  for (auto control : controls)
    state_object->setProperty(String(control.first), control.second->value());

  std::set<mopo::ModulationConnection*> modulations = synth_.getModulationConnections();
  Array<var> modulation_states;
  for (mopo::ModulationConnection* connection: modulations) {
    DynamicObject* mod_object = new DynamicObject();
    mod_object->setProperty("source", connection->source.c_str());
    mod_object->setProperty("destination", connection->destination.c_str());
    mod_object->setProperty("amount", connection->amount.value());
    modulation_states.add(mod_object);
  }

  state_object->setProperty("modulations", modulation_states);
  return state_object;
}

void TwytchStandaloneEditor::varToState(var state) {
  mopo::control_map controls = synth_.getControls();
  DynamicObject* object_state = state.getDynamicObject();

  ScopedLock lock(critical_section_);
  NamedValueSet properties = object_state->getProperties();
  int size = properties.size();
  for (int i = 0; i < size; ++i) {
    Identifier id = properties.getName(i);
    if (id.isValid()) {
      std::string name = id.toString().toStdString();
      if (controls.count(name)) {
        mopo::mopo_float value = properties.getValueAt(i);
        controls[name]->set(value);
      }
    }
  }

  synth_.clearModulations();
  Array<var>* modulations = object_state->getProperty("modulations").getArray();
  var* modulation = modulations->begin();
  for (; modulation != modulations->end(); ++modulation) {
    DynamicObject* mod = modulation->getDynamicObject();
    std::string source = mod->getProperty("source").toString().toStdString();
    std::string destination = mod->getProperty("destination").toString().toStdString();
    mopo::ModulationConnection* connection = new mopo::ModulationConnection(source, destination);
    connection->amount.set(mod->getProperty("amount"));
    synth_.connectModulation(connection);
  }

  gui_->setAllValues(controls_);
  gui_->setModulationConnections(synth_.getModulationConnections());
}

void TwytchStandaloneEditor::handleMessage(const Message& message) {
#ifdef JUCE_MAC
  if(!hasKeyboardFocus(false)) {
    postMessage(new Message());
    grabKeyboardFocus();
  }
#endif
}
