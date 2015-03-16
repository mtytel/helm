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

#include "twytch_engine.h"

#include "twytch_voice_handler.h"

#include <fenv.h>

#define MAX_DELAY_SAMPLES 1000000

namespace mopo {

  TwytchEngine::TwytchEngine() : was_playing_arp_(false) {
    fesetenv(FE_DFL_DISABLE_SSE_DENORMS_ENV);

    // Voice Handler.
    Value* polyphony = new Value(1);
    voice_handler_ = new TwytchVoiceHandler();
    addSubmodule(voice_handler_);
    voice_handler_->setPolyphony(32);
    voice_handler_->plug(polyphony, VoiceHandler::kPolyphony);
    controls_["polyphony"] = polyphony;

    // Monophonic LFO 1.
    Processor* lfo_1_waveform = createMonoModControl("mono_lfo_1_waveform", Wave::kSin, true);
    Processor* lfo_1_frequency = createMonoModControl("mono_lfo_1_frequency", 0.0,
                                                      false, false, kExponential);
    Oscillator* lfo_1 = new Oscillator();
    lfo_1->plug(lfo_1_waveform, Oscillator::kWaveform);
    lfo_1->plug(lfo_1_frequency, Oscillator::kFrequency);

    addProcessor(lfo_1);
    mod_sources_["mono_lfo_1"] = lfo_1->output();

    // Monophonic LFO 1.
    Processor* lfo_2_waveform = createMonoModControl("mono_lfo_2_waveform", Wave::kSin, true);
    Processor* lfo_2_frequency = createMonoModControl("mono_lfo_2_frequency", 0.0,
                                                      false, false, kExponential);
    Oscillator* lfo_2 = new Oscillator();
    lfo_2->plug(lfo_2_waveform, Oscillator::kWaveform);
    lfo_2->plug(lfo_2_frequency, Oscillator::kFrequency);

    addProcessor(lfo_2);
    mod_sources_["mono_lfo_2"] = lfo_2->output();

    // Step Sequencer.
    Processor* num_steps = createMonoModControl("num_steps", 16, true);
    Processor* step_frequency = createMonoModControl("step_frequency", 3.0,
                                                     false, false, kExponential);
    StepGenerator* step_sequencer = new StepGenerator(MAX_STEPS);
    step_sequencer->plug(num_steps, StepGenerator::kNumSteps);
    step_sequencer->plug(step_frequency, StepGenerator::kFrequency);

    addProcessor(step_sequencer);

    for (int i = 0; i < MAX_STEPS; ++i) {
      std::string num = std::to_string(i);
      if (num.length() == 1)
        num = "0" + num;
      Value* step = new Value(0.0);
      controls_[std::string("step_seq_") + num] = step;
      step_sequencer->plug(step, StepGenerator::kSteps + i);
    }
    
    mod_sources_["step_sequencer"] = step_sequencer->output();

    // Arpeggiator.
    Processor* arp_frequency = createMonoModControl("arp_frequency", 2.0,
                                                    true, false, kExponential);
    Value* arp_octaves = new Value(1);
    Value* arp_pattern = new Value(0);
    Processor* arp_gate = createMonoModControl("arp_gate", 0.5, true);
    arp_on_ = new Value(0);
    arpeggiator_ = new Arpeggiator(voice_handler_);
    arpeggiator_->plug(arp_frequency, Arpeggiator::kFrequency);
    arpeggiator_->plug(arp_octaves, Arpeggiator::kOctaves);
    arpeggiator_->plug(arp_pattern, Arpeggiator::kPattern);
    arpeggiator_->plug(arp_gate, Arpeggiator::kGate);

    controls_["arp_octaves"] = arp_octaves;
    controls_["arp_pattern"] = arp_pattern;
    controls_["arp_on"] = arp_on_;

    addProcessor(arpeggiator_);
    addProcessor(voice_handler_);

    // Delay effect.
    Processor* delay_time = createMonoModControl("delay_time", -3.0, false, true, kExponential);
    Processor* delay_feedback = createMonoModControl("delay_feedback", -0.3, false, true);
    Processor* delay_wet = createMonoModControl("delay_dry_wet", 0.3, false, true);

    Delay* delay = new Delay(MAX_DELAY_SAMPLES);
    delay->plug(voice_handler_, Delay::kAudio);
    delay->plug(delay_time, Delay::kDelayTime);
    delay->plug(delay_feedback, Delay::kFeedback);
    delay->plug(delay_wet, Delay::kWet);

    addProcessor(delay);

    // Volume.
    Processor* volume = createMonoModControl("volume", 0.6, false, true, kQuadratic);
    Multiply* scaled_audio = new Multiply();
    scaled_audio->plug(delay, 0);
    scaled_audio->plug(volume, 1);
    Clamp* clamp = new Clamp();
    clamp->plug(scaled_audio);

    addProcessor(clamp);
    addProcessor(volume);
    addProcessor(scaled_audio);
    registerOutput(clamp->output());
  }

  void TwytchEngine::connectModulation(ModulationConnection* connection) {
    Processor::Output* source = getModulationSource(connection->source);
    Processor* destination = getModulationDestination(connection->destination,
                                                      source->owner->isPolyphonic());
    MOPO_ASSERT(source != nullptr);
    MOPO_ASSERT(destination != nullptr);

    connection->modulation_scale.plug(source, 0);
    connection->modulation_scale.plug(&connection->amount, 1);
    connection->modulation_scale.setControlRate(source->owner->isControlRate());
    destination->plugNext(&connection->modulation_scale);

    source->owner->router()->addProcessor(&connection->modulation_scale);
    mod_connections_.insert(connection);
  }

  void TwytchEngine::disconnectModulation(ModulationConnection* connection) {
    Processor::Output* source = getModulationSource(connection->source);
    Processor* destination = getModulationDestination(connection->destination,
                                                      source->owner->isPolyphonic());
    destination->unplug(&connection->modulation_scale);

    source->owner->router()->removeProcessor(&connection->modulation_scale);
    mod_connections_.erase(connection);
  }

  void TwytchEngine::clearModulations() {
    for (auto connection : mod_connections_) {
      Processor::Output* source = getModulationSource(connection->source);
      Processor* destination = getModulationDestination(connection->destination,
                                                        source->owner->isPolyphonic());
      destination->unplug(&connection->modulation_scale);
      source->owner->router()->removeProcessor(&connection->modulation_scale);
    }
    mod_connections_.clear();
  }

  int TwytchEngine::getNumActiveVoices() {
    return voice_handler_->getNumActiveVoices();
  }

  void TwytchEngine::process() {
    bool playing_arp = arp_on_->value();
    if (was_playing_arp_ != playing_arp)
      arpeggiator_->allNotesOff();

    was_playing_arp_ = playing_arp;
    ProcessorRouter::process();
  }

  void TwytchEngine::allNotesOff(int sample) {
    arpeggiator_->allNotesOff(sample);
  }

  void TwytchEngine::noteOn(mopo_float note, mopo_float velocity, int sample) {
    if (arp_on_->value())
      arpeggiator_->noteOn(note, velocity, sample);
    else
      voice_handler_->noteOn(note, velocity, sample);
  }

  void TwytchEngine::noteOff(mopo_float note, int sample) {
    if (arp_on_->value())
      arpeggiator_->noteOff(note, sample);
    else
      voice_handler_->noteOff(note, sample);
  }

  void TwytchEngine::setModWheel(mopo_float value) {
    voice_handler_->setModWheel(value);
  }

  void TwytchEngine::setPitchWheel(mopo_float value) {
    voice_handler_->setPitchWheel(value);
  }

  void TwytchEngine::setAftertouch(mopo_float note, mopo_float value, int sample) {
    voice_handler_->setAftertouch(note, value, sample);
  }

  void TwytchEngine::sustainOn() {
    voice_handler_->sustainOn();
  }

  void TwytchEngine::sustainOff() {
    voice_handler_->sustainOff();
  }
} // namespace mopo
