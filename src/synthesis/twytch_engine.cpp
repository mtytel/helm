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

#include "twytch_lfo.h"
#include "twytch_voice_handler.h"
#include "switch.h"

#ifdef __APPLE__
#include <fenv.h>
#endif

#define MAX_DELAY_SAMPLES 1000000

namespace mopo {

  TwytchEngine::TwytchEngine() : was_playing_arp_(false) {
    static const Value* minutes_per_second = new Value(1.0 / 60.0);

#ifdef FE_DFL_DISABLE_SSE_DENORMS_ENV
    fesetenv(FE_DFL_DISABLE_SSE_DENORMS_ENV);
#endif

    Processor* beats_per_minute = createMonoModControl("beats_per_minute", 120.0, false);
    Multiply* beats_per_second = new Multiply();
    beats_per_second->plug(beats_per_minute, 0);
    beats_per_second->plug(minutes_per_second, 1);
    addProcessor(beats_per_second);

    // Voice Handler.
    Processor* polyphony = createMonoModControl("polyphony", 1, true);

    voice_handler_ = new TwytchVoiceHandler(beats_per_second);
    addSubmodule(voice_handler_);
    voice_handler_->setPolyphony(32);
    voice_handler_->plug(polyphony, VoiceHandler::kPolyphony);

    // Monophonic LFO 1.
    Processor* lfo_1_waveform = createMonoModControl("mono_lfo_1_waveform", Wave::kSin, true);
    Processor* lfo_1_free_frequency = createMonoModControl("mono_lfo_1_frequency", 0.0,
                                                           true, false, kExponential);
    Processor* lfo_1_free_amplitude = createMonoModControl("mono_lfo_1_amplitude", 1.0, true);
    Processor* lfo_1_frequency = createTempoSyncSwitch("mono_lfo_1", lfo_1_free_frequency,
                                                       beats_per_second, false);


    TwytchLfo* lfo_1 = new TwytchLfo();
    lfo_1->plug(lfo_1_waveform, TwytchLfo::kWaveform);
    lfo_1->plug(lfo_1_frequency, TwytchLfo::kFrequency);

    Multiply* scaled_lfo_1 = new Multiply();
    scaled_lfo_1->setControlRate();
    scaled_lfo_1->plug(lfo_1, 0);
    scaled_lfo_1->plug(lfo_1_free_amplitude, 1);

    addProcessor(lfo_1);
    addProcessor(scaled_lfo_1);
    mod_sources_["mono_lfo_1"] = scaled_lfo_1->output();
    mod_sources_["mono_lfo_1_phase"] = lfo_1->output(Oscillator::kPhase);

    // Monophonic LFO 2.
    Processor* lfo_2_waveform = createMonoModControl("mono_lfo_2_waveform", Wave::kSin, true);
    Processor* lfo_2_free_frequency = createMonoModControl("mono_lfo_2_frequency", 0.0,
                                                           true, false, kExponential);
    Processor* lfo_2_free_amplitude = createMonoModControl("mono_lfo_2_amplitude", 1.0, true);
    Processor* lfo_2_frequency = createTempoSyncSwitch("mono_lfo_2", lfo_2_free_frequency,
                                                       beats_per_second, false);

    TwytchLfo* lfo_2 = new TwytchLfo();
    lfo_2->plug(lfo_2_waveform, TwytchLfo::kWaveform);
    lfo_2->plug(lfo_2_frequency, TwytchLfo::kFrequency);

    Multiply* scaled_lfo_2 = new Multiply();
    scaled_lfo_2->setControlRate();
    scaled_lfo_2->plug(lfo_2, 0);
    scaled_lfo_2->plug(lfo_2_free_amplitude, 1);

    addProcessor(lfo_2);
    addProcessor(scaled_lfo_2);
    mod_sources_["mono_lfo_2"] = scaled_lfo_2->output();
    mod_sources_["mono_lfo_2_phase"] = lfo_2->output(Oscillator::kPhase);

    // Step Sequencer.
    Processor* num_steps = createMonoModControl("num_steps", 16, true);
    Processor* step_smoothing = createMonoModControl("step_smoothing", 0, true);
    Processor* step_free_frequency = createMonoModControl("step_frequency", 3.0,
                                                          false, false, kExponential);
    Processor* step_frequency = createTempoSyncSwitch("step_sequencer", step_free_frequency,
                                                      beats_per_second, false);

    StepGenerator* step_sequencer = new StepGenerator(MAX_STEPS);
    step_sequencer->plug(num_steps, StepGenerator::kNumSteps);
    step_sequencer->plug(step_frequency, StepGenerator::kFrequency);

    for (int i = 0; i < MAX_STEPS; ++i) {
      std::stringstream stream;
      stream << i;
      std::string num = stream.str();
      if (num.length() == 1)
        num = "0" + num;
      Value* step = new Value(0.0);
      controls_[std::string("step_seq_") + num] = step;
      step_sequencer->plug(step, StepGenerator::kSteps + i);
    }

    SmoothFilter* smoothed_step_sequencer = new SmoothFilter();
    smoothed_step_sequencer->plug(step_sequencer, SmoothFilter::kTarget);
    smoothed_step_sequencer->plug(step_smoothing, SmoothFilter::kHalfLife);

    addProcessor(step_sequencer);
    addProcessor(smoothed_step_sequencer);

    mod_sources_["step_sequencer"] = smoothed_step_sequencer->output();
    mod_sources_["step_sequencer_step"] = step_sequencer->output(StepGenerator::kStep);

    // Arpeggiator.
    Processor* arp_free_frequency = createMonoModControl("arp_frequency", 2.0,
                                                         true, false, kExponential);
    Processor* arp_frequency = createTempoSyncSwitch("arp", arp_free_frequency,
                                                     beats_per_second, false);
    Processor* arp_octaves = createMonoModControl("arp_octaves", 2, true);
    Processor* arp_pattern = createMonoModControl("arp_pattern", 3, true);
    Processor* arp_gate = createMonoModControl("arp_gate", 0.5, true);
    arp_on_ = new Value(0);
    arpeggiator_ = new Arpeggiator(voice_handler_);
    arpeggiator_->plug(arp_frequency, Arpeggiator::kFrequency);
    arpeggiator_->plug(arp_octaves, Arpeggiator::kOctaves);
    arpeggiator_->plug(arp_pattern, Arpeggiator::kPattern);
    arpeggiator_->plug(arp_gate, Arpeggiator::kGate);

    controls_["arp_on"] = arp_on_;

    addProcessor(arpeggiator_);
    addProcessor(voice_handler_);

    // Delay effect.
    Processor* delay_free_frequency = createMonoModControl("delay_frequency", 1.0, false,
                                                           false, kExponential);
    Processor* delay_frequency = createTempoSyncSwitch("delay", delay_free_frequency,
                                                       beats_per_second, false);
    Processor* delay_feedback = createMonoModControl("delay_feedback", -0.3, false, true);
    Processor* delay_wet = createMonoModControl("delay_dry_wet", 0.3, false, true);

    FrequencyToSamples* delay_samples = new FrequencyToSamples();
    delay_samples->plug(delay_frequency);

    Delay* delay = new Delay(MAX_DELAY_SAMPLES);
    delay->plug(voice_handler_, Delay::kAudio);
    delay->plug(delay_samples, Delay::kSampleDelay);
    delay->plug(delay_feedback, Delay::kFeedback);
    delay->plug(delay_wet, Delay::kWet);

    addProcessor(delay_samples);
    addProcessor(delay);

    Distortion* distorted_clamp = new Distortion();
    Value* distortion_type = new Value(Distortion::kTanh);
    Value* distortion_threshold = new Value(0.7);
    distorted_clamp->plug(delay, Distortion::kAudio);
    distorted_clamp->plug(distortion_type, Distortion::kType);
    distorted_clamp->plug(distortion_threshold, Distortion::kThreshold);

    // Volume.
    Processor* volume = createMonoModControl("volume", 0.6, false, true, kQuadratic);
    Multiply* scaled_audio = new Multiply();
    scaled_audio->plug(distorted_clamp, 0);
    scaled_audio->plug(volume, 1);

    addProcessor(distorted_clamp);
    addProcessor(scaled_audio);
    registerOutput(scaled_audio->output());
  }

  void TwytchEngine::connectModulation(ModulationConnection* connection) {
    Processor::Output* source = getModulationSource(connection->source);
    Processor* destination = getModulationDestination(connection->destination,
                                                      source->owner->isPolyphonic());
    MOPO_ASSERT(source != 0);
    MOPO_ASSERT(destination != 0);

    connection->modulation_scale.plug(source, 0);
    connection->modulation_scale.plug(&connection->amount, 1);
    connection->modulation_scale.setControlRate(source->owner->isControlRate());
    destination->plugNext(&connection->modulation_scale);

    source->owner->router()->addProcessor(&connection->modulation_scale);
    mod_connections_.insert(connection);
  }

  std::list<mopo_float> TwytchEngine::getPressedNotes() {
    if (arp_on_->value())
      return arpeggiator_->getPressedNotes();
    return voice_handler_->getPressedNotes();
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

  ModulationConnection* TwytchEngine::getConnection(std::string source, std::string destination) {
    for (ModulationConnection* connection : mod_connections_) {
      if (connection->source == source && connection->destination == destination)
        return connection;
    }
    return nullptr;
  }

  std::vector<ModulationConnection*> TwytchEngine::getSourceConnections(std::string source) {
    std::vector<ModulationConnection*> connections;
    for (ModulationConnection* connection : mod_connections_) {
      if (connection->source == source)
        connections.push_back(connection);
    }
    return connections;
  }

  std::vector<ModulationConnection*>
  TwytchEngine::getDestinationConnections(std::string destination) {
    std::vector<ModulationConnection*> connections;
    for (ModulationConnection* connection : mod_connections_) {
      if (connection->destination == destination)
        connections.push_back(connection);
    }
    return connections;
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

  void TwytchEngine::setBpm(mopo_float bpm) {
    controls_["beats_per_minute"]->set(bpm);
  }

  void TwytchEngine::sustainOn() {
    voice_handler_->sustainOn();
  }

  void TwytchEngine::sustainOff() {
    voice_handler_->sustainOff();
  }
} // namespace mopo
