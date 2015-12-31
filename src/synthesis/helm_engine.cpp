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

#include "helm_engine.h"

#include "dc_filter.h"
#include "helm_lfo.h"
#include "helm_voice_handler.h"
#include "switch.h"

#ifdef __APPLE__
#include <fenv.h>
#endif

#define MAX_DELAY_SAMPLES 1000000

namespace mopo {

  HelmEngine::HelmEngine() : was_playing_arp_(false) {
    init();
  }

  void HelmEngine::init() {
    static const Value* minutes_per_second = new Value(1.0 / 60.0);

#ifdef FE_DFL_DISABLE_SSE_DENORMS_ENV
    fesetenv(FE_DFL_DISABLE_SSE_DENORMS_ENV);
#endif

    Processor* beats_per_minute = createMonoModControl("beats_per_minute", false);
    Multiply* beats_per_second = new Multiply();
    beats_per_second->plug(beats_per_minute, 0);
    beats_per_second->plug(minutes_per_second, 1);
    addProcessor(beats_per_second);

    // Voice Handler.
    Processor* polyphony = createMonoModControl("polyphony", true);

    voice_handler_ = new HelmVoiceHandler(beats_per_second);
    addSubmodule(voice_handler_);
    voice_handler_->setPolyphony(32);
    voice_handler_->plug(polyphony, VoiceHandler::kPolyphony);

    // Monophonic LFO 1.
    lfo_1_retrigger_ = createBaseControl("mono_lfo_1_retrigger");
    TriggerEquals* lfo_1_reset = new TriggerEquals(1.0);
    lfo_1_reset->plug(lfo_1_retrigger_, TriggerEquals::kCondition);
    lfo_1_reset->plug(voice_handler_->note_retrigger(), TriggerEquals::kTrigger);
    Processor* lfo_1_waveform = createMonoModControl("mono_lfo_1_waveform", true);
    Processor* lfo_1_free_frequency = createMonoModControl("mono_lfo_1_frequency", true);
    Processor* lfo_1_free_amplitude = createMonoModControl("mono_lfo_1_amplitude", true);
    Processor* lfo_1_frequency = createTempoSyncSwitch("mono_lfo_1", lfo_1_free_frequency,
                                                       beats_per_second, false);

    lfo_1_ = new HelmLfo();
    lfo_1_->plug(lfo_1_waveform, HelmLfo::kWaveform);
    lfo_1_->plug(lfo_1_frequency, HelmLfo::kFrequency);
    lfo_1_->plug(lfo_1_reset, HelmLfo::kReset);

    Multiply* scaled_lfo_1 = new Multiply();
    scaled_lfo_1->setControlRate();
    scaled_lfo_1->plug(lfo_1_, 0);
    scaled_lfo_1->plug(lfo_1_free_amplitude, 1);

    addProcessor(lfo_1_);
    addProcessor(lfo_1_reset);
    addProcessor(scaled_lfo_1);
    mod_sources_["mono_lfo_1"] = scaled_lfo_1->output();
    mod_sources_["mono_lfo_1_phase"] = lfo_1_->output(Oscillator::kPhase);

    // Monophonic LFO 2.
    lfo_2_retrigger_ = createBaseControl("mono_lfo_2_retrigger");
    TriggerEquals* lfo_2_reset = new TriggerEquals(1.0);
    lfo_2_reset->plug(lfo_2_retrigger_, TriggerEquals::kCondition);
    lfo_2_reset->plug(voice_handler_->note_retrigger(), TriggerEquals::kTrigger);
    Processor* lfo_2_waveform = createMonoModControl("mono_lfo_2_waveform", true);
    Processor* lfo_2_free_frequency = createMonoModControl("mono_lfo_2_frequency", true);
    Processor* lfo_2_free_amplitude = createMonoModControl("mono_lfo_2_amplitude", true);
    Processor* lfo_2_frequency = createTempoSyncSwitch("mono_lfo_2", lfo_2_free_frequency,
                                                       beats_per_second, false);

    lfo_2_ = new HelmLfo();
    lfo_2_->plug(lfo_2_waveform, HelmLfo::kWaveform);
    lfo_2_->plug(lfo_2_frequency, HelmLfo::kFrequency);
    lfo_2_->plug(lfo_2_reset, HelmLfo::kReset);

    Multiply* scaled_lfo_2 = new Multiply();
    scaled_lfo_2->setControlRate();
    scaled_lfo_2->plug(lfo_2_, 0);
    scaled_lfo_2->plug(lfo_2_free_amplitude, 1);

    addProcessor(lfo_2_);
    addProcessor(lfo_2_reset);
    addProcessor(scaled_lfo_2);
    mod_sources_["mono_lfo_2"] = scaled_lfo_2->output();
    mod_sources_["mono_lfo_2_phase"] = lfo_2_->output(Oscillator::kPhase);

    // Step Sequencer.
    step_sequencer_retrigger_ = createBaseControl("step_sequencer_retrigger");
    TriggerEquals* step_sequencer_reset = new TriggerEquals(1.0);
    step_sequencer_reset->plug(step_sequencer_retrigger_, TriggerEquals::kCondition);
    step_sequencer_reset->plug(voice_handler_->note_retrigger(), TriggerEquals::kTrigger);
    Processor* num_steps = createMonoModControl("num_steps", true);
    Processor* step_smoothing = createMonoModControl("step_smoothing", true);
    Processor* step_free_frequency = createMonoModControl("step_frequency", true);
    Processor* step_frequency = createTempoSyncSwitch("step_sequencer", step_free_frequency,
                                                      beats_per_second, false);

    step_sequencer_ = new StepGenerator(MAX_STEPS);
    step_sequencer_->plug(step_sequencer_reset, StepGenerator::kReset);
    step_sequencer_->plug(num_steps, StepGenerator::kNumSteps);
    step_sequencer_->plug(step_frequency, StepGenerator::kFrequency);

    for (int i = 0; i < MAX_STEPS; ++i) {
      std::stringstream stream;
      stream << i;
      std::string num = stream.str();
      if (num.length() == 1)
        num = "0" + num;
      Processor* step = createBaseControl(std::string("step_seq_") + num);
      step_sequencer_->plug(step, StepGenerator::kSteps + i);
    }

    SmoothFilter* smoothed_step_sequencer = new SmoothFilter();
    smoothed_step_sequencer->plug(step_sequencer_, SmoothFilter::kTarget);
    smoothed_step_sequencer->plug(step_smoothing, SmoothFilter::kHalfLife);

    addProcessor(step_sequencer_);
    addProcessor(step_sequencer_reset);
    addProcessor(smoothed_step_sequencer);

    mod_sources_["step_sequencer"] = smoothed_step_sequencer->output();
    mod_sources_["step_sequencer_step"] = step_sequencer_->output(StepGenerator::kStep);

    // Arpeggiator.
    Processor* arp_free_frequency = createMonoModControl("arp_frequency", true);
    Processor* arp_frequency = createTempoSyncSwitch("arp", arp_free_frequency,
                                                     beats_per_second, false);
    Processor* arp_octaves = createMonoModControl("arp_octaves", true);
    Processor* arp_pattern = createMonoModControl("arp_pattern", true);
    Processor* arp_gate = createMonoModControl("arp_gate", true);
    arp_on_ = createBaseControl("arp_on");
    arpeggiator_ = new Arpeggiator(voice_handler_);
    arpeggiator_->plug(arp_frequency, Arpeggiator::kFrequency);
    arpeggiator_->plug(arp_octaves, Arpeggiator::kOctaves);
    arpeggiator_->plug(arp_pattern, Arpeggiator::kPattern);
    arpeggiator_->plug(arp_gate, Arpeggiator::kGate);

    addProcessor(arpeggiator_);
    addProcessor(voice_handler_);

    // Delay effect.
    Processor* delay_free_frequency = createMonoModControl("delay_frequency", true);
    Processor* delay_frequency = createTempoSyncSwitch("delay", delay_free_frequency,
                                                       beats_per_second, false);
    Processor* delay_feedback = createMonoModControl("delay_feedback", false, true);
    Processor* delay_wet = createMonoModControl("delay_dry_wet", false, true);
    Value* delay_on = createBaseControl("delay_on");

    Clamp* delay_feedback_clamped = new Clamp(-1, 1);
    delay_feedback_clamped->plug(delay_feedback);

    SampleAndHoldBuffer* delay_frequency_audio_rate = new SampleAndHoldBuffer();
    delay_frequency_audio_rate->plug(delay_frequency);

    SmoothFilter* delay_frequency_smoothed = new SmoothFilter();
    delay_frequency_smoothed->plug(delay_frequency_audio_rate, SmoothFilter::kTarget);
    delay_frequency_smoothed->plug(&utils::value_half, SmoothFilter::kHalfLife);
    FrequencyToSamples* delay_samples = new FrequencyToSamples();
    delay_samples->plug(delay_frequency_smoothed);

    Delay* delay = new Delay(MAX_DELAY_SAMPLES);
    delay->plug(voice_handler_, Delay::kAudio);
    delay->plug(delay_samples, Delay::kSampleDelay);
    delay->plug(delay_feedback_clamped, Delay::kFeedback);
    delay->plug(delay_wet, Delay::kWet);

    BypassRouter* delay_container = new BypassRouter();
    delay_container->plug(delay_on, BypassRouter::kOn);
    delay_container->plug(voice_handler_, BypassRouter::kAudio);
    delay_container->addProcessor(delay_feedback_clamped);
    delay_container->addProcessor(delay_frequency_audio_rate);
    delay_container->addProcessor(delay_frequency_smoothed);
    delay_container->addProcessor(delay_samples);
    delay_container->addProcessor(delay);
    delay_container->registerOutput(delay->output());

    addProcessor(delay_container);

    // DC Blocker.
    DcFilter* dc_filter = new DcFilter();
    dc_filter->plug(delay_container, DcFilter::kAudio);

    addProcessor(dc_filter);

    // Reverb Effect.
    Processor* reverb_feedback = createMonoModControl("reverb_feedback", false, true);
    Processor* reverb_damping = createMonoModControl("reverb_damping", false, true);
    Processor* reverb_wet = createMonoModControl("reverb_dry_wet", false, true);
    Value* reverb_on = createBaseControl("reverb_on");

    Clamp* reverb_feedback_clamped = new Clamp(-1, 1);
    reverb_feedback_clamped->plug(reverb_feedback);

    Reverb* reverb = new Reverb();
    reverb->plug(dc_filter, Reverb::kAudio);
    reverb->plug(reverb_feedback_clamped, Reverb::kFeedback);
    reverb->plug(reverb_damping, Reverb::kDamping);
    reverb->plug(reverb_wet, Reverb::kWet);

    BypassRouter* reverb_container = new BypassRouter();
    reverb_container->plug(reverb_on, BypassRouter::kOn);
    reverb_container->plug(dc_filter, BypassRouter::kAudio);
    reverb_container->addProcessor(reverb);
    reverb_container->addProcessor(reverb_feedback_clamped);
    reverb_container->registerOutput(reverb->output(0));
    reverb_container->registerOutput(reverb->output(1));

    addProcessor(reverb_container);

    // Soft Clipping.
    Distortion* distorted_clamp_left = new Distortion();
    Value* distortion_type = new Value(Distortion::kTanh);
    Value* distortion_threshold = new Value(0.7);
    distorted_clamp_left->plug(reverb_container->output(0), Distortion::kAudio);
    distorted_clamp_left->plug(distortion_type, Distortion::kType);
    distorted_clamp_left->plug(distortion_threshold, Distortion::kThreshold);

    Distortion* distorted_clamp_right = new Distortion();
    distorted_clamp_right->plug(reverb_container->output(1), Distortion::kAudio);
    distorted_clamp_right->plug(distortion_type, Distortion::kType);
    distorted_clamp_right->plug(distortion_threshold, Distortion::kThreshold);

    // Volume.
    Processor* volume = createMonoModControl("volume", false, true);
    Multiply* scaled_audio_left = new Multiply();
    scaled_audio_left->plug(distorted_clamp_left, 0);
    scaled_audio_left->plug(volume, 1);

    Multiply* scaled_audio_right = new Multiply();
    scaled_audio_right->plug(distorted_clamp_right, 0);
    scaled_audio_right->plug(volume, 1);

    addProcessor(distorted_clamp_left);
    addProcessor(distorted_clamp_right);
    addProcessor(scaled_audio_left);
    addProcessor(scaled_audio_right);
    registerOutput(scaled_audio_left->output());
    registerOutput(scaled_audio_right->output());

    HelmModule::init();
  }

  void HelmEngine::connectModulation(ModulationConnection* connection) {
    Processor::Output* source = getModulationSource(connection->source);
    MOPO_ASSERT(source != 0);

    Processor* destination = getModulationDestination(connection->destination,
                                                      source->owner->isPolyphonic());
    MOPO_ASSERT(destination != 0);

    connection->modulation_scale.plug(source, 0);
    connection->modulation_scale.plug(&connection->amount, 1);
    connection->modulation_scale.setControlRate(source->owner->isControlRate());
    destination->plugNext(&connection->modulation_scale);

    source->owner->router()->addProcessor(&connection->modulation_scale);
    mod_connections_.insert(connection);
  }

  std::list<mopo_float> HelmEngine::getPressedNotes() {
    if (arp_on_->value())
      return arpeggiator_->getPressedNotes();
    return voice_handler_->getPressedNotes();
  }

  void HelmEngine::disconnectModulation(ModulationConnection* connection) {
    Processor::Output* source = getModulationSource(connection->source);
    Processor* destination = getModulationDestination(connection->destination,
                                                      source->owner->isPolyphonic());
    destination->unplug(&connection->modulation_scale);

    source->owner->router()->removeProcessor(&connection->modulation_scale);
    mod_connections_.erase(connection);
  }

  void HelmEngine::clearModulations() {
    for (auto connection : mod_connections_) {
      Processor::Output* source = getModulationSource(connection->source);
      Processor* destination = getModulationDestination(connection->destination,
                                                        source->owner->isPolyphonic());
      destination->unplug(&connection->modulation_scale);
      source->owner->router()->removeProcessor(&connection->modulation_scale);
    }
    mod_connections_.clear();
  }

  ModulationConnection* HelmEngine::getConnection(std::string source, std::string destination) {
    for (ModulationConnection* connection : mod_connections_) {
      if (connection->source == source && connection->destination == destination)
        return connection;
    }
    return nullptr;
  }

  std::vector<ModulationConnection*> HelmEngine::getSourceConnections(std::string source) {
    std::vector<ModulationConnection*> connections;
    for (ModulationConnection* connection : mod_connections_) {
      if (connection->source == source)
        connections.push_back(connection);
    }
    return connections;
  }

  std::vector<ModulationConnection*>
  HelmEngine::getDestinationConnections(std::string destination) {
    std::vector<ModulationConnection*> connections;
    for (ModulationConnection* connection : mod_connections_) {
      if (connection->destination == destination)
        connections.push_back(connection);
    }
    return connections;
  }

  int HelmEngine::getNumActiveVoices() {
    return voice_handler_->getNumActiveVoices();
  }

  void HelmEngine::process() {
    bool playing_arp = arp_on_->value();
    if (was_playing_arp_ != playing_arp)
      arpeggiator_->allNotesOff();

    was_playing_arp_ = playing_arp;
    ProcessorRouter::process();
  }

  void HelmEngine::allNotesOff(int sample) {
    arpeggiator_->allNotesOff(sample);
  }

  void HelmEngine::noteOn(mopo_float note, mopo_float velocity, int sample, int channel) {
    if (arp_on_->value())
      arpeggiator_->noteOn(note, velocity, sample);
    else
      voice_handler_->noteOn(note, velocity, sample, channel);
  }

  VoiceEvent HelmEngine::noteOff(mopo_float note, int sample) {
    if (arp_on_->value())
      return arpeggiator_->noteOff(note, sample);
    return voice_handler_->noteOff(note, sample);
  }

  void HelmEngine::setModWheel(mopo_float value, int channel) {
    voice_handler_->setModWheel(value);
  }

  void HelmEngine::setPitchWheel(mopo_float value, int channel) {
    voice_handler_->setPitchWheel(value);
  }

  void HelmEngine::setAftertouch(mopo_float note, mopo_float value, int sample) {
    voice_handler_->setAftertouch(note, value, sample);
  }

  void HelmEngine::setBpm(mopo_float bpm) {
    controls_["beats_per_minute"]->set(bpm);
  }

  void HelmEngine::correctToTime(mopo_float samples) {
    HelmModule::correctToTime(samples);
    if (lfo_1_retrigger_->value() == 2.0)
      lfo_1_->correctToTime(samples);
    if (lfo_2_retrigger_->value() == 2.0)
      lfo_2_->correctToTime(samples);
    step_sequencer_->correctToTime(samples);
  }

  void HelmEngine::sustainOn() {
    voice_handler_->sustainOn();
  }

  void HelmEngine::sustainOff() {
    voice_handler_->sustainOff();
  }
} // namespace mopo
