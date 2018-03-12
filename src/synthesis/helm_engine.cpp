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

#include "helm_engine.h"

#include "dc_filter.h"
#include "helm_lfo.h"
#include "helm_voice_handler.h"
#include "peak_meter.h"
#include "value_switch.h"

#ifdef __APPLE__
#include <fenv.h>
#endif

#define MAX_DELAY_SAMPLES 300000

namespace mopo {

  HelmEngine::HelmEngine() : was_playing_arp_(false) {
    init();
    bps_ = controls_["beats_per_minute"];
  }

  HelmEngine::~HelmEngine() {
    while (mod_connections_.size())
      disconnectModulation(*mod_connections_.begin());
  }

  void HelmEngine::init() {
#ifdef FE_DFL_DISABLE_SSE_DENORMS_ENV
    fesetenv(FE_DFL_DISABLE_SSE_DENORMS_ENV);
#endif

    Output* beats_per_second = createMonoModControl("beats_per_minute", true);
    cr::LowerBound* beats_per_second_clamped = new cr::LowerBound(0.0);
    beats_per_second_clamped->plug(beats_per_second);
    addProcessor(beats_per_second_clamped);

    // Voice Handler.
    Output* polyphony = createMonoModControl("polyphony", true);

    voice_handler_ = new HelmVoiceHandler(beats_per_second_clamped->output());
    addSubmodule(voice_handler_);
    voice_handler_->setPolyphony(32);
    voice_handler_->plug(polyphony, VoiceHandler::kPolyphony);

    // Monophonic LFO 1.
    lfo_1_retrigger_ = createBaseControl("mono_lfo_1_retrigger");
    TriggerEquals* lfo_1_reset = new TriggerEquals(1.0);
    lfo_1_reset->plug(lfo_1_retrigger_, TriggerEquals::kCondition);
    lfo_1_reset->plug(voice_handler_->note_retrigger(), TriggerEquals::kTrigger);
    Output* lfo_1_waveform = createMonoModControl("mono_lfo_1_waveform", true);
    Output* lfo_1_free_frequency = createMonoModControl("mono_lfo_1_frequency", true);
    Output* lfo_1_amplitude = createMonoModControl("mono_lfo_1_amplitude", true);
    Output* lfo_1_frequency = createTempoSyncSwitch("mono_lfo_1", lfo_1_free_frequency->owner,
                                                    beats_per_second_clamped->output(), false);

    lfo_1_ = new HelmLfo();
    lfo_1_->plug(lfo_1_waveform, HelmLfo::kWaveform);
    lfo_1_->plug(lfo_1_frequency, HelmLfo::kFrequency);
    lfo_1_->plug(lfo_1_reset, HelmLfo::kReset);

    cr::Multiply* scaled_lfo_1 = new cr::Multiply();
    scaled_lfo_1->plug(lfo_1_, 0);
    scaled_lfo_1->plug(lfo_1_amplitude, 1);

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
    Output* lfo_2_waveform = createMonoModControl("mono_lfo_2_waveform", true);
    Output* lfo_2_free_frequency = createMonoModControl("mono_lfo_2_frequency", true);
    Output* lfo_2_amplitude = createMonoModControl("mono_lfo_2_amplitude", true);
    Output* lfo_2_frequency = createTempoSyncSwitch("mono_lfo_2", lfo_2_free_frequency->owner,
                                                    beats_per_second_clamped->output(), false);

    lfo_2_ = new HelmLfo();
    lfo_2_->plug(lfo_2_waveform, HelmLfo::kWaveform);
    lfo_2_->plug(lfo_2_frequency, HelmLfo::kFrequency);
    lfo_2_->plug(lfo_2_reset, HelmLfo::kReset);

    cr::Multiply* scaled_lfo_2 = new cr::Multiply();
    scaled_lfo_2->plug(lfo_2_, 0);
    scaled_lfo_2->plug(lfo_2_amplitude, 1);

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
    Output* num_steps = createMonoModControl("num_steps", true);
    Output* step_smoothing = createMonoModControl("step_smoothing", true);
    Output* step_free_frequency = createMonoModControl("step_frequency", true);
    Output* step_frequency = createTempoSyncSwitch("step_sequencer", step_free_frequency->owner,
                                                   beats_per_second_clamped->output(), false);

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

    cr::SmoothFilter* smoothed_step_sequencer = new cr::SmoothFilter(0.0);
    smoothed_step_sequencer->plug(step_sequencer_, cr::SmoothFilter::kTarget);
    smoothed_step_sequencer->plug(step_smoothing, cr::SmoothFilter::kHalfLife);

    addProcessor(step_sequencer_);
    addProcessor(step_sequencer_reset);
    addProcessor(smoothed_step_sequencer);

    mod_sources_["step_sequencer"] = smoothed_step_sequencer->output();
    mod_sources_["step_sequencer_step"] = step_sequencer_->output(StepGenerator::kStep);

    // Arpeggiator.
    arp_on_ = createBaseSwitchControl("arp_on");
    Output* arp_free_frequency = createMonoModControl("arp_frequency", true);
    Output* arp_frequency = createTempoSyncSwitch("arp", arp_free_frequency->owner,
                                                  beats_per_second_clamped->output(),
                                                  false, arp_on_);
    Output* arp_octaves = createMonoModControl("arp_octaves", true);
    Output* arp_pattern = createMonoModControl("arp_pattern", true);
    Output* arp_gate = createMonoModControl("arp_gate", true);
    arpeggiator_ = new Arpeggiator(voice_handler_);
    arpeggiator_->plug(arp_frequency, Arpeggiator::kFrequency);
    arpeggiator_->plug(arp_octaves, Arpeggiator::kOctaves);
    arpeggiator_->plug(arp_pattern, Arpeggiator::kPattern);
    arpeggiator_->plug(arp_gate, Arpeggiator::kGate);
    arpeggiator_->plug(arp_on_, Arpeggiator::kOn);

    addProcessor(voice_handler_);

    // Distortion
    Distortion* distortion = new Distortion();
    Value* distortion_on = createBaseControl("distortion_on");
    Value* distortion_type = createBaseControl("distortion_type");
    Output* distortion_drive = createMonoModControl("distortion_drive", true);
    Output* distortion_mix = createMonoModControl("distortion_mix", true);
    cr::MagnitudeScale* distortion_gain = new cr::MagnitudeScale();
    distortion_gain->plug(distortion_drive);

    distortion->plug(voice_handler_, Distortion::kAudio);
    distortion->plug(distortion_on, Distortion::kOn);
    distortion->plug(distortion_type, Distortion::kType);
    distortion->plug(distortion_gain, Distortion::kDrive);
    distortion->plug(distortion_mix, Distortion::kMix);
    addProcessor(distortion);
    addProcessor(distortion_gain);

    // Delay effect.
    Output* delay_free_frequency = createMonoModControl("delay_frequency", true);
    Output* delay_frequency = createTempoSyncSwitch("delay", delay_free_frequency->owner,
                                                    beats_per_second_clamped->output(), false);
    Output* delay_feedback = createMonoModControl("delay_feedback", true);
    Output* delay_wet = createMonoModControl("delay_dry_wet", true);
    Value* delay_on = createBaseControl("delay_on");

    cr::Clamp* delay_feedback_clamped = new cr::Clamp(-1, 1);
    delay_feedback_clamped->plug(delay_feedback);

    cr::SmoothFilter* delay_frequency_smoothed = new cr::SmoothFilter(1.0);
    delay_frequency_smoothed->plug(delay_frequency, cr::SmoothFilter::kTarget);
    delay_frequency_smoothed->plug(&utils::value_fifth, cr::SmoothFilter::kHalfLife);
    cr::FrequencyToSamples* delay_samples = new cr::FrequencyToSamples();
    delay_samples->plug(delay_frequency_smoothed);

    Delay* delay = new Delay(MAX_DELAY_SAMPLES);
    delay->plug(distortion, Delay::kAudio);
    delay->plug(delay_samples, Delay::kSampleDelay);
    delay->plug(delay_feedback_clamped, Delay::kFeedback);
    delay->plug(delay_wet, Delay::kWet);

    BypassRouter* delay_container = new BypassRouter();
    delay_container->plug(delay_on, BypassRouter::kOn);
    delay_container->plug(distortion, BypassRouter::kAudio);
    delay_container->addProcessor(delay_feedback_clamped);
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
    Output* reverb_feedback = createMonoModControl("reverb_feedback", true);
    Output* reverb_damping = createMonoModControl("reverb_damping", true);
    Output* reverb_wet = createMonoModControl("reverb_dry_wet", true);
    Value* reverb_on = createBaseControl("reverb_on");

    cr::Clamp* reverb_feedback_clamped = new cr::Clamp(-1, 1);
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

    // Volume.
    Output* volume = createMonoModControl("volume", true);
    LinearSmoothBuffer* smooth_volume = new LinearSmoothBuffer();
    smooth_volume->plug(volume);

    Multiply* scaled_audio_left = new Multiply();
    scaled_audio_left->plug(reverb_container->output(0), 0);
    scaled_audio_left->plug(smooth_volume, 1);

    Multiply* scaled_audio_right = new Multiply();
    scaled_audio_right->plug(reverb_container->output(1), 0);
    scaled_audio_right->plug(smooth_volume, 1);

    peak_meter_ = new PeakMeter();
    peak_meter_->plug(scaled_audio_left, 0);
    peak_meter_->plug(scaled_audio_right, 1);
    mod_sources_["peak_meter"] = peak_meter_->output();

    // Hard Clip.
    Clamp* clamp_left = new Clamp(-2.1, 2.1);
    clamp_left->plug(scaled_audio_left);

    Clamp* clamp_right = new Clamp(-2.1, 2.1);
    clamp_right->plug(scaled_audio_right);
    
    addProcessor(peak_meter_);
    addProcessor(smooth_volume);
    addProcessor(scaled_audio_left);
    addProcessor(scaled_audio_right);

    addProcessor(clamp_left);
    addProcessor(clamp_right);
    registerOutput(clamp_left->output());
    registerOutput(clamp_right->output());

    HelmModule::init();
  }

  void HelmEngine::connectModulation(ModulationConnection* connection) {
    Output* source = getModulationSource(connection->source);
    bool source_poly = source->owner->isPolyphonic();
    MOPO_ASSERT(source != nullptr);

    Processor* destination = getModulationDestination(connection->destination, source_poly);
    MOPO_ASSERT(destination != nullptr);

    ValueSwitch* mono_mod_switch = getMonoModulationSwitch(connection->destination);
    MOPO_ASSERT(mono_mod_switch != nullptr);

    connection->modulation_scale.plug(source, 0);
    connection->modulation_scale.plug(&connection->amount, 1);
    source->owner->router()->addProcessor(&connection->modulation_scale);
    destination->plugNext(&connection->modulation_scale);

    mono_mod_switch->set(1);
    ValueSwitch* poly_mod_switch = getPolyModulationSwitch(connection->destination);
    if (poly_mod_switch)
      poly_mod_switch->set(1);

    mod_connections_.insert(connection);
  }

  bool HelmEngine::isModulationActive(ModulationConnection* connection) {
    return mod_connections_.count(connection);
  }

  CircularQueue<mopo_float>& HelmEngine::getPressedNotes() {
    if (arp_on_->value())
      return arpeggiator_->getPressedNotes();
    return voice_handler_->getPressedNotes();
  }

  void HelmEngine::disconnectModulation(ModulationConnection* connection) {
    Output* source = getModulationSource(connection->source);
    bool source_poly = source->owner->isPolyphonic();

    Processor* destination = getModulationDestination(connection->destination, source_poly);
    Processor* mono_destination = getMonoModulationDestination(connection->destination);
    Processor* poly_destination = getPolyModulationDestination(connection->destination);
    MOPO_ASSERT(destination != nullptr);

    destination->unplug(&connection->modulation_scale);

    if (mono_destination->connectedInputs() == 1 &&
        (poly_destination == nullptr || poly_destination->connectedInputs() == 0)) {
      ValueSwitch* mono_mod_switch = getMonoModulationSwitch(connection->destination);
      mono_mod_switch->set(0);

      ValueSwitch* poly_mod_switch = getPolyModulationSwitch(connection->destination);
      if (poly_mod_switch)
        poly_mod_switch->set(0);
    }

    source->owner->router()->removeProcessor(&connection->modulation_scale);
    mod_connections_.erase(connection);
  }

  int HelmEngine::getNumActiveVoices() {
    return voice_handler_->getNumActiveVoices();
  }

  mopo_float HelmEngine::getLastActiveNote() const {
    return voice_handler_->getLastActiveNote();
  }

  void HelmEngine::process() {
    bool playing_arp = arp_on_->value();
    if (was_playing_arp_ != playing_arp)
      arpeggiator_->allNotesOff();

    was_playing_arp_ = playing_arp;
    arpeggiator_->process();
    ProcessorRouter::process();

    if (getNumActiveVoices() == 0) {
      for (auto& modulation : mod_connections_)
        modulation->modulation_scale.process();
    }
  }

  void HelmEngine::setBufferSize(int buffer_size) {
    ProcessorRouter::setBufferSize(buffer_size);
    arpeggiator_->setBufferSize(buffer_size);
  }

  void HelmEngine::setSampleRate(int sample_rate) {
    ProcessorRouter::setSampleRate(sample_rate);
    arpeggiator_->setSampleRate(sample_rate);
  }

  void HelmEngine::allNotesOff(int sample) {
    arpeggiator_->allNotesOff(sample);
  }

  void HelmEngine::noteOn(mopo_float note, mopo_float velocity, int sample, int channel, mopo_float aftertouch) {
    if (arp_on_->value())
      arpeggiator_->noteOn(note, velocity, sample, channel, aftertouch);
    else
      voice_handler_->noteOn(note, velocity, sample, channel, aftertouch);
  }

  VoiceEvent HelmEngine::noteOff(mopo_float note, int sample) {
    if (arp_on_->value())
      return arpeggiator_->noteOff(note, sample);
    return voice_handler_->noteOff(note, sample);
  }

  void HelmEngine::setModWheel(mopo_float value, int channel) {
    voice_handler_->setModWheel(value, channel);
  }

  void HelmEngine::setPitchWheel(mopo_float value, int channel) {
    voice_handler_->setPitchWheel(value, channel);
  }
  
  void HelmEngine::setPressure(mopo_float value, int channel, int sample) {
    if (arp_on_->value())
      arpeggiator_->setPressure(value, channel, sample);
    voice_handler_->setPressure(value, channel, sample);
  }

  void HelmEngine::setAftertouch(mopo_float note, mopo_float value, int sample) {
    // TODO: take channel into account
    if (arp_on_->value())
      arpeggiator_->setAftertouch(note, value, sample);
    voice_handler_->setAftertouch(note, value, sample);
  }

  void HelmEngine::setBpm(mopo_float bpm) {
    mopo_float bps = bpm / 60.0;
    if (bps_->value() != bps)
      bps_->set(bps);
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
