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

#include "twytch_voice_handler.h"

#include "bypass_router.h"
#include "delay.h"
#include "distortion.h"
#include "envelope.h"
#include "filter.h"
#include "formant.h"
#include "formant_manager.h"
#include "operators.h"
#include "oscillator.h"
#include "linear_slope.h"
#include "smooth_value.h"
#include "step_generator.h"
#include "twytch_oscillators.h"
#include "trigger_operators.h"
#include "utils.h"
#include "value.h"

#include <fenv.h>
#include <sstream>


#define PITCH_MOD_RANGE 12
#define MIN_GAIN_DB -24.0
#define MAX_GAIN_DB 24.0

namespace mopo {

  namespace {
    struct FormantValues {
      Value* gain;
      Value* resonance;
      Value* frequency;
    };

    static const FormantValues top_left_formants[NUM_FORMANTS] = {
      {new Value(1.0), new Value(6.0), new Value(270.0)},
      {new Value(1.0), new Value(10.0), new Value(2300.0)},
      {new Value(1.0), new Value(8.0), new Value(3000.0)},
      {new Value(0.2), new Value(15.0), new Value(500.0)},
    };

    static const FormantValues top_right_formants[NUM_FORMANTS] = {
      {new Value(1.0), new Value(6.0), new Value(270.0)},
      {new Value(1.0), new Value(12.0), new Value(500.0)},
      {new Value(1.0), new Value(8.0), new Value(2000.0)},
      {new Value(1.0), new Value(9.0), new Value(1500.0)},
    };

    static const FormantValues bottom_left_formants[NUM_FORMANTS] = {
      {new Value(1.0), new Value(6.0), new Value(270.0)},
      {new Value(1.0), new Value(4.0), new Value(2300.0)},
      {new Value(1.0), new Value(8.0), new Value(3000.0)},
      {new Value(0.2), new Value(0.5), new Value(500.0)},
    };

    static const FormantValues bottom_right_formants[NUM_FORMANTS] = {
      {new Value(0.0), new Value(6.0), new Value(270.0)},
      {new Value(0.0), new Value(12.0), new Value(500.0)},
      {new Value(0.0), new Value(8.0), new Value(3000.0)},
      {new Value(0.0), new Value(9.0), new Value(3500.0)},
    };
  } // namespace

  TwytchVoiceHandler::TwytchVoiceHandler() {
    // Create modulation and pitch wheels.
    mod_wheel_amount_ = new SmoothValue(0);
    pitch_wheel_amount_ = new SmoothValue(0);

    mod_sources_["pitch wheel"] = pitch_wheel_amount_->output();
    mod_sources_["mod wheel"] = mod_wheel_amount_->output();

    // Create all synthesizer voice components.
    createArticulation(note(), velocity(), voice_event());
    createOscillators(current_frequency_->output(),
                      amplitude_envelope_->output(Envelope::kFinished));
    createModulators(amplitude_envelope_->output(Envelope::kFinished));
    createFilter(osc_feedback_->output(0), note_from_center_->output(),
                 amplitude_envelope_->output(Envelope::kFinished), voice_event());

    Value* aftertouch_value = new Value();
    aftertouch_value->plug(aftertouch());

    addProcessor(aftertouch_value);
    mod_sources_["aftertouch"] = aftertouch_value->output();

    output_ = new Multiply();
    output_->plug(formant_container_, 0);
    output_->plug(amplitude_, 1);

    addProcessor(output_);
    addGlobalProcessor(pitch_wheel_amount_);
    addGlobalProcessor(mod_wheel_amount_);

    setVoiceOutput(output_);
    setVoiceKiller(amplitude_envelope_->output(Envelope::kValue));
  }

  void TwytchVoiceHandler::createOscillators(Output* midi, Output* reset) {
    // Pitch bend.
    Value* pitch_bend_range = new Value(2);
    Multiply* pitch_bend = new Multiply();
    pitch_bend->plug(pitch_wheel_amount_, 0);
    pitch_bend->plug(pitch_bend_range, 1);
    Add* bent_midi = new Add();
    bent_midi->plug(midi, 0);
    bent_midi->plug(pitch_bend, 1);

    Value* pitch_mod_range = new Value(PITCH_MOD_RANGE);
    VariableAdd* midi_mod_sources = new VariableAdd();
    Multiply* midi_mod = new Multiply();
    midi_mod->plug(pitch_mod_range, 0);
    midi_mod->plug(midi_mod_sources, 1);
    Add* final_midi = new Add();
    final_midi->plug(bent_midi, 0);
    final_midi->plug(midi_mod, 1);

    addGlobalProcessor(pitch_bend);
    addProcessor(bent_midi);
    addProcessor(midi_mod_sources);
    addProcessor(midi_mod);
    addProcessor(final_midi);

    controls_["pitch bend range"] = pitch_bend_range;

    // Oscillator 1.
    oscillators_ = new TwytchOscillators();
    Value* oscillator1_waveform = new Value(Wave::kDownSaw);
    MidiScale* oscillator1_frequency = new MidiScale();
    oscillator1_frequency->plug(final_midi);
    oscillators_->plug(oscillator1_waveform, TwytchOscillators::kOscillator1Waveform);
    oscillators_->plug(reset, TwytchOscillators::kOscillator1Reset);
    oscillators_->plug(reset, TwytchOscillators::kOscillator2Reset);
    oscillators_->plug(oscillator1_frequency, TwytchOscillators::kOscillator1BaseFrequency);

    Value* cross_mod = new Value(0.15);
    VariableAdd* cross_mod_sources = new VariableAdd();
    cross_mod_sources->plugNext(cross_mod);

    oscillators_->plug(cross_mod_sources, TwytchOscillators::kOscillator1FM);
    oscillators_->plug(cross_mod_sources, TwytchOscillators::kOscillator2FM);

    addProcessor(cross_mod_sources);
    addProcessor(oscillator1_frequency);
    addProcessor(oscillators_);

    controls_["cross modulation"] = cross_mod;
    controls_["osc 1 waveform"] = oscillator1_waveform;

    // Oscillator 2.
    Value* oscillator2_waveform = new Value(Wave::kDownSaw);
    Value* oscillator2_transpose = new Value(-12);
    Value* oscillator2_tune = new Value(0.08);
    Add* oscillator2_transposed = new Add();
    oscillator2_transposed->plug(final_midi, 0);
    oscillator2_transposed->plug(oscillator2_transpose, 1);
    Add* oscillator2_midi = new Add();
    oscillator2_midi->plug(oscillator2_transposed, 0);
    oscillator2_midi->plug(oscillator2_tune, 1);

    MidiScale* oscillator2_frequency = new MidiScale();
    oscillator2_frequency->plug(oscillator2_midi);
    oscillators_->plug(oscillator2_waveform, TwytchOscillators::kOscillator2Waveform);
    oscillators_->plug(oscillator2_frequency, TwytchOscillators::kOscillator2BaseFrequency);

    addProcessor(oscillator2_transposed);
    addProcessor(oscillator2_midi);
    addProcessor(oscillator2_frequency);

    controls_["osc 2 waveform"] = oscillator2_waveform;
    controls_["osc 2 transpose"] = oscillator2_transpose;
    controls_["osc 2 tune"] = oscillator2_tune;

    // Oscillator mix.
    Value* oscillator_mix_amount = new Value(0.5);
    VariableAdd* osc_mix_sources = new VariableAdd();
    osc_mix_sources->plugNext(oscillator_mix_amount);

    Clamp* clamp_mix = new Clamp(0, 1);
    clamp_mix->plug(osc_mix_sources);
    oscillator_mix_ = new Interpolate();
    oscillator_mix_->plug(oscillators_->output(0), Interpolate::kFrom);
    oscillator_mix_->plug(oscillators_->output(1), Interpolate::kTo);
    oscillator_mix_->plug(clamp_mix, Interpolate::kFractional);

    addProcessor(oscillator_mix_);
    addProcessor(osc_mix_sources);
    addProcessor(clamp_mix);
    controls_["osc mix"] = oscillator_mix_amount;

    // Oscillator feedback.
    Value* osc_feedback_transpose = new Value(-12);
    Value* osc_feedback_amount = new Value(0.0);
    Value* osc_feedback_tune = new Value(0.00);
    Add* osc_feedback_transposed = new Add();
    osc_feedback_transposed->plug(final_midi, 0);
    osc_feedback_transposed->plug(osc_feedback_transpose, 1);
    Add* osc_feedback_midi = new Add();
    osc_feedback_midi->plug(osc_feedback_transposed, 0);
    osc_feedback_midi->plug(osc_feedback_tune, 1);

    controls_["osc feedback transpose"] = osc_feedback_transpose;
    controls_["osc feedback amount"] = osc_feedback_amount;
    controls_["osc feedback tune"] = osc_feedback_tune;

    MidiScale* osc_feedback_frequency = new MidiScale();
    osc_feedback_frequency->plug(osc_feedback_midi);
    Inverse* osc_feedback_period = new Inverse();
    osc_feedback_period->plug(osc_feedback_frequency);

    addProcessor(osc_feedback_transposed);
    addProcessor(osc_feedback_midi);
    addProcessor(osc_feedback_frequency);
    addProcessor(osc_feedback_period);

    osc_feedback_ = new Delay();
    osc_feedback_->plug(oscillator_mix_, Delay::kAudio);
    osc_feedback_->plug(osc_feedback_period, Delay::kDelayTime);
    osc_feedback_->plug(osc_feedback_amount, Delay::kFeedback);
    osc_feedback_->plug(&utils::value_half, Delay::kWet);
    addProcessor(osc_feedback_);

    mod_sources_["osc 1"] = oscillators_->getOscillator1Output();
    mod_sources_["osc 2"] = oscillators_->getOscillator2Output();

    mod_destinations_["cross modulation"] = cross_mod_sources;
    mod_destinations_["pitch"] = midi_mod_sources;
    mod_destinations_["osc mix"] = osc_mix_sources;
  }

  void TwytchVoiceHandler::createModulators(Output* reset) {
    // LFO 1.
    Value* lfo1_waveform = new Value(Wave::kSin);
    Value* lfo1_frequency = new Value(2);
    lfo1_ = new Oscillator();
    lfo1_->plug(reset, Oscillator::kReset);
    lfo1_->plug(lfo1_waveform, Oscillator::kWaveform);
    lfo1_->plug(lfo1_frequency, Oscillator::kFrequency);

    addGlobalProcessor(lfo1_);
    controls_["lfo 1 waveform"] = lfo1_waveform;
    controls_["lfo 1 frequency"] = lfo1_frequency;

    // LFO 2.
    Value* lfo2_waveform = new Value(Wave::kSin);
    Value* lfo2_frequency = new Value(2);
    lfo2_ = new Oscillator();
    lfo2_->plug(reset, Oscillator::kReset);
    lfo2_->plug(lfo2_waveform, Oscillator::kWaveform);
    lfo2_->plug(lfo2_frequency, Oscillator::kFrequency);

    addProcessor(lfo2_);
    controls_["lfo 2 waveform"] = lfo2_waveform;
    controls_["lfo 2 frequency"] = lfo2_frequency;

    // Step Sequencer.
    Value* num_steps = new Value(16);
    Value* step_frequency = new Value(5.0);
    step_sequencer_ = new StepGenerator(MAX_STEPS);
    step_sequencer_->plug(num_steps, StepGenerator::kNumSteps);
    step_sequencer_->plug(step_frequency, StepGenerator::kFrequency);

    addProcessor(step_sequencer_);
    controls_["num steps"] = num_steps;
    controls_["step frequency"] = step_frequency;

    for (int i = 0; i < MAX_STEPS; ++i) {
      std::string num = std::to_string(i);
      if (num.length() == 1)
        num = "0" + num;
      Value* step = new Value(0.0);
      controls_[std::string("step seq ") + num] = step;
      step_sequencer_->plug(step, StepGenerator::kSteps + i);
    }

    // Modulation sources/destinations.
    mod_sources_["lfo 1"] = lfo1_->output();
    mod_sources_["lfo 2"] = lfo2_->output();
    mod_sources_["step sequencer"] = step_sequencer_->output();
  }

  void TwytchVoiceHandler::createFilter(
      Output* audio, Output* keytrack, Output* reset, Output* note_event) {
    // Filter envelope.
    Value* filter_attack = new Value(0.0);
    Value* filter_decay = new Value(0.3);
    Value* filter_sustain = new Value(0.3);
    Value* filter_release = new Value(0.3);

    TriggerFilter* note_off = new TriggerFilter(VoiceEvent::kVoiceOff);
    note_off->plug(note_event);
    TriggerCombiner* filter_env_trigger = new TriggerCombiner();
    filter_env_trigger->plug(note_off, 0);
    filter_env_trigger->plug(reset, 1);

    filter_envelope_ = new Envelope();
    filter_envelope_->plug(filter_attack, Envelope::kAttack);
    filter_envelope_->plug(filter_decay, Envelope::kDecay);
    filter_envelope_->plug(filter_sustain, Envelope::kSustain);
    filter_envelope_->plug(filter_release, Envelope::kRelease);
    filter_envelope_->plug(filter_env_trigger, Envelope::kTrigger);

    Value* filter_envelope_depth = new Value(36);
    Multiply* scaled_envelope = new Multiply();
    scaled_envelope->plug(filter_envelope_, 0);
    scaled_envelope->plug(filter_envelope_depth, 1);

    addProcessor(filter_envelope_);
    addProcessor(note_off);
    addProcessor(filter_env_trigger);
    addProcessor(scaled_envelope);

    controls_["fil attack"] = filter_attack;
    controls_["fil decay"] = filter_decay;
    controls_["fil sustain"] = filter_sustain;
    controls_["fil release"] = filter_release;
    controls_["fil env depth"] = filter_envelope_depth;

    // Filter.
    Value* filter_type = new Value(Filter::kLowPass);
    Value* keytrack_amount = new Value(0);
    Multiply* current_keytrack = new Multiply();
    current_keytrack->plug(keytrack, 0);
    current_keytrack->plug(keytrack_amount, 1);

    SmoothValue* base_cutoff = new SmoothValue(80);
    Add* keytracked_cutoff = new Add();
    keytracked_cutoff->plug(base_cutoff, 0);
    keytracked_cutoff->plug(current_keytrack, 1);

    Add* midi_cutoff = new Add();
    midi_cutoff->plug(keytracked_cutoff, 0);
    midi_cutoff->plug(scaled_envelope, 1);

    VariableAdd* cutoff_mod_sources = new VariableAdd();
    Value* cutoff_mod_scale = new Value(MIDI_SIZE / 2);
    Multiply* cutoff_modulation_scaled = new Multiply();
    cutoff_modulation_scaled->plug(cutoff_mod_sources, 0);
    cutoff_modulation_scaled->plug(cutoff_mod_scale, 1);
    Add* midi_cutoff_modulated = new Add();
    midi_cutoff_modulated->plug(midi_cutoff, 0);
    midi_cutoff_modulated->plug(cutoff_modulation_scaled, 1);

    MidiScale* frequency_cutoff = new MidiScale();
    frequency_cutoff->plug(midi_cutoff_modulated);

    Value* resonance = new Value(0.5);

    VariableAdd* resonance_sources = new VariableAdd();
    resonance_sources->plugNext(resonance);
    ResonanceScale* final_resonance = new ResonanceScale();
    Value* min_db = new Value(MIN_GAIN_DB);
    Value* max_db = new Value(MAX_GAIN_DB);
    Interpolate* decibals = new Interpolate();
    decibals->plug(min_db, Interpolate::kFrom);
    decibals->plug(max_db, Interpolate::kTo);
    decibals->plug(resonance, Interpolate::kFractional);
    MagnitudeScale* final_gain = new MagnitudeScale();
    final_resonance->plug(resonance_sources);
    final_gain->plug(decibals);

    Value* filter_saturation = new Value(0.0);
    VariableAdd* saturation_mod_sources = new VariableAdd();
    Value* max_saturation = new Value(60.0);
    Multiply* saturation_mod = new Multiply();
    saturation_mod->plug(max_saturation, 0);
    saturation_mod->plug(saturation_mod_sources, 1);
    Add* total_saturation = new Add();
    total_saturation->plug(saturation_mod, 0);
    total_saturation->plug(filter_saturation, 1);
    MagnitudeScale* saturation_magnitude = new MagnitudeScale();
    saturation_magnitude->plug(total_saturation);

    Multiply* saturated_audio = new Multiply();
    saturated_audio->plug(audio, 0);
    saturated_audio->plug(saturation_magnitude, 1);

    filter_ = new Filter();
    filter_->plug(saturated_audio, Filter::kAudio);
    filter_->plug(filter_type, Filter::kType);
    filter_->plug(reset, Filter::kReset);
    filter_->plug(frequency_cutoff, Filter::kCutoff);
    filter_->plug(final_resonance, Filter::kResonance);
    filter_->plug(final_gain, Filter::kGain);

    distorted_filter_ = new Distortion();
    Value* distortion_type = new Value(Distortion::kTanh);
    Value* distortion_threshold = new Value(0.5);
    distorted_filter_->plug(filter_, Distortion::kAudio);
    distorted_filter_->plug(distortion_type, Distortion::kType);
    distorted_filter_->plug(distortion_threshold, Distortion::kThreshold);

    addGlobalProcessor(base_cutoff);
    addProcessor(current_keytrack);
    addProcessor(saturated_audio);
    addProcessor(keytracked_cutoff);
    addProcessor(midi_cutoff);
    addProcessor(cutoff_mod_sources);
    addProcessor(cutoff_modulation_scaled);
    addProcessor(midi_cutoff_modulated);
    addProcessor(resonance_sources);
    addProcessor(final_resonance);
    addProcessor(decibals);
    addProcessor(final_gain);
    addProcessor(frequency_cutoff);
    addProcessor(filter_);

    addProcessor(saturation_mod_sources);
    addProcessor(saturation_mod);
    addProcessor(total_saturation);
    addProcessor(saturation_magnitude);
    addProcessor(distorted_filter_);

    controls_["filter type"] = filter_type;
    controls_["filter saturation"] = filter_saturation;
    controls_["cutoff"] = base_cutoff;
    controls_["keytrack"] = keytrack_amount;
    controls_["resonance"] = resonance;

    mod_sources_["filter env"] = filter_envelope_->output();
    mod_destinations_["cutoff"] = cutoff_mod_sources;
    mod_destinations_["resonance"] = resonance_sources;
    mod_destinations_["saturation"] = saturation_mod_sources;

    // Formant Filter.
    formant_container_ = new BypassRouter();
    Value* formant_bypass = new Value(1);
    formant_container_->plug(formant_bypass, BypassRouter::kBypass);
    formant_container_->plug(distorted_filter_, BypassRouter::kAudio);

    formant_filter_ = new FormantManager(NUM_FORMANTS);
    Value* formant_passthrough = new Value(0.0);
    formant_filter_->plug(distorted_filter_, FormantManager::kAudio);
    formant_filter_->plug(formant_passthrough, FormantManager::kPassthroughGain);
    formant_filter_->plug(reset, FormantManager::kReset);

    controls_["formant bypass"] = formant_bypass;
    controls_["formant passthrough"] = formant_passthrough;

    SmoothValue* formant_x = new SmoothValue(0.0);
    SmoothValue* formant_y = new SmoothValue(0.0);

    addGlobalProcessor(formant_x);
    addGlobalProcessor(formant_y);

    controls_["formant x"] = formant_x;
    controls_["formant y"] = formant_y;

    for (int i = 0; i < NUM_FORMANTS; ++i) {
      BilinearInterpolate* formant_gain = new BilinearInterpolate();
      BilinearInterpolate* formant_q = new BilinearInterpolate();
      BilinearInterpolate* formant_frequency = new BilinearInterpolate();

      formant_gain->plug(top_left_formants[i].gain, BilinearInterpolate::kTopLeft);
      formant_gain->plug(top_right_formants[i].gain, BilinearInterpolate::kTopRight);
      formant_gain->plug(bottom_left_formants[i].gain, BilinearInterpolate::kBottomLeft);
      formant_gain->plug(bottom_right_formants[i].gain, BilinearInterpolate::kBottomRight);

      formant_q->plug(top_left_formants[i].resonance, BilinearInterpolate::kTopLeft);
      formant_q->plug(top_right_formants[i].resonance, BilinearInterpolate::kTopRight);
      formant_q->plug(bottom_left_formants[i].resonance, BilinearInterpolate::kBottomLeft);
      formant_q->plug(bottom_right_formants[i].resonance, BilinearInterpolate::kBottomRight);

      formant_frequency->plug(top_left_formants[i].frequency, BilinearInterpolate::kTopLeft);
      formant_frequency->plug(top_right_formants[i].frequency, BilinearInterpolate::kTopRight);
      formant_frequency->plug(bottom_left_formants[i].frequency, BilinearInterpolate::kBottomLeft);
      formant_frequency->plug(bottom_right_formants[i].frequency, BilinearInterpolate::kBottomRight);

      formant_gain->plug(formant_x, BilinearInterpolate::kXPosition);
      formant_q->plug(formant_x, BilinearInterpolate::kXPosition);
      formant_frequency->plug(formant_x, BilinearInterpolate::kXPosition);

      formant_gain->plug(formant_y, BilinearInterpolate::kYPosition);
      formant_q->plug(formant_y, BilinearInterpolate::kYPosition);
      formant_frequency->plug(formant_y, BilinearInterpolate::kYPosition);

      formant_filter_->getFormant(i)->plug(formant_gain, Formant::kGain);
      formant_filter_->getFormant(i)->plug(formant_q, Formant::kResonance);
      formant_filter_->getFormant(i)->plug(formant_frequency, Formant::kFrequency);

      addProcessor(formant_gain);
      addProcessor(formant_q);
      addProcessor(formant_frequency);

      /*
      controls_[std::string("formant gain ") + std::to_string(i)] = formant_gain;
      controls_[std::string("formant resonance ") + std::to_string(i)] = formant_q;
      controls_[std::string("formant frequency ") + std::to_string(i)] = formant_frequency;
       */
    }

    formant_container_->addProcessor(formant_filter_);
    formant_container_->registerOutput(formant_filter_->output());

    addProcessor(formant_container_);
  }

  void TwytchVoiceHandler::createArticulation(
      Output* note, Output* velocity, Output* trigger) {
    // Legato.
    Value* legato = new Value(0);
    LegatoFilter* legato_filter = new LegatoFilter();
    legato_filter->plug(legato, LegatoFilter::kLegato);
    legato_filter->plug(trigger, LegatoFilter::kTrigger);

    controls_["legato"] = legato;
    addProcessor(legato_filter);

    // Amplitude envelope.
    Value* amplitude_attack = new Value(0.01);
    Value* amplitude_decay = new Value(0.7);
    Value* amplitude_sustain = new SmoothValue(0.5);
    Value* amplitude_release = new Value(0.3);

    amplitude_envelope_ = new Envelope();
    amplitude_envelope_->plug(legato_filter->output(LegatoFilter::kRetrigger),
                              Envelope::kTrigger);
    amplitude_envelope_->plug(amplitude_attack, Envelope::kAttack);
    amplitude_envelope_->plug(amplitude_decay, Envelope::kDecay);
    amplitude_envelope_->plug(amplitude_sustain, Envelope::kSustain);
    amplitude_envelope_->plug(amplitude_release, Envelope::kRelease);

    addProcessor(amplitude_envelope_);
    addGlobalProcessor(amplitude_sustain);

    controls_["amp attack"] = amplitude_attack;
    controls_["amp decay"] = amplitude_decay;
    controls_["amp sustain"] = amplitude_sustain;
    controls_["amp release"] = amplitude_release;

    // Voice and frequency resetting logic.
    note_change_trigger_ = new TriggerCombiner();
    note_change_trigger_->plug(legato_filter->output(LegatoFilter::kRemain), 0);
    note_change_trigger_->plug(amplitude_envelope_->output(Envelope::kFinished), 1);

    TriggerWait* note_wait = new TriggerWait();
    Value* current_note = new Value();
    note_wait->plug(note, TriggerWait::kWait);
    note_wait->plug(note_change_trigger_, TriggerWait::kTrigger);
    current_note->plug(note_wait);

    Value* max_midi_invert = new Value(1.0 / (MIDI_SIZE - 1));
    Multiply* note_percentage = new Multiply();
    note_percentage->plug(max_midi_invert, 0);
    note_percentage->plug(current_note, 1);

    addProcessor(note_change_trigger_);
    addProcessor(note_wait);
    addProcessor(current_note);

    // Key tracking.
    Value* center_adjust = new Value(-MIDI_SIZE / 2);
    note_from_center_ = new Add();
    note_from_center_->plug(center_adjust, 0);
    note_from_center_->plug(current_note, 1);

    addProcessor(note_from_center_);
    addProcessor(note_percentage);
    addGlobalProcessor(center_adjust);

    // Velocity tracking.
    TriggerWait* velocity_wait = new TriggerWait();
    Value* current_velocity = new Value();
    velocity_wait->plug(velocity, TriggerWait::kWait);
    velocity_wait->plug(note_change_trigger_, TriggerWait::kTrigger);
    current_velocity->plug(velocity_wait);

    addProcessor(velocity_wait);
    addProcessor(current_velocity);

    Value* velocity_track_amount = new Value(0.3);

    Interpolate* velocity_track_mult = new Interpolate();
    velocity_track_mult->plug(&utils::value_one, Interpolate::kFrom);
    velocity_track_mult->plug(current_velocity, Interpolate::kTo);
    velocity_track_mult->plug(velocity_track_amount, Interpolate::kFractional);

    addProcessor(velocity_track_mult);
    controls_["velocity track"] = velocity_track_amount;

    // Current amplitude using envelope and velocity.
    amplitude_ = new Multiply();
    amplitude_->plug(amplitude_envelope_->output(Envelope::kValue), 0);
    amplitude_->plug(velocity_track_mult, 1);

    addProcessor(amplitude_);

    // Portamento.
    Value* portamento = new Value(0.01);
    Value* portamento_type = new Value(0);
    PortamentoFilter* portamento_filter = new PortamentoFilter();
    portamento_filter->plug(portamento_type, PortamentoFilter::kPortamento);
    portamento_filter->plug(note_change_trigger_, PortamentoFilter::kFrequencyTrigger);
    portamento_filter->plug(trigger, PortamentoFilter::kVoiceTrigger);
    addProcessor(portamento_filter);

    current_frequency_ = new LinearSlope();
    current_frequency_->plug(current_note, LinearSlope::kTarget);
    current_frequency_->plug(portamento, LinearSlope::kRunSeconds);
    current_frequency_->plug(portamento_filter, LinearSlope::kTriggerJump);

    addProcessor(current_frequency_);
    controls_["portamento"] = portamento;
    controls_["portamento type"] = portamento_type;

    mod_sources_["amplitude env"] = amplitude_envelope_->output();
    mod_sources_["note"] = note_percentage->output();
    mod_sources_["velocity"] = current_velocity->output();
  }

  void TwytchVoiceHandler::setModWheel(mopo_float value) {
    mod_wheel_amount_->set(value);
  }

  void TwytchVoiceHandler::setPitchWheel(mopo_float value) {
    pitch_wheel_amount_->set(value);
  }

  void TwytchVoiceHandler::connectModulation(ModulationConnection* connection) {
    MOPO_ASSERT(mod_sources_.count(connection->source));
    MOPO_ASSERT(mod_destinations_.count(connection->destination));

    connection->modulation_scale.plug(mod_sources_[connection->source], 0);
    connection->modulation_scale.plug(&connection->amount, 1);

    mod_destinations_[connection->destination]->plugNext(&connection->modulation_scale);
    addProcessor(&connection->modulation_scale);
    mod_connections_.insert(connection);
  }

  void TwytchVoiceHandler::disconnectModulation(ModulationConnection* connection) {
    mod_destinations_[connection->destination]->unplug(&connection->modulation_scale);
    removeProcessor(&connection->modulation_scale);
    mod_connections_.erase(connection);
  }

  void TwytchVoiceHandler::clearModulations() {
    for (auto connection : mod_connections_) {
      mod_destinations_[connection->destination]->unplug(&connection->modulation_scale);
      removeProcessor(&connection->modulation_scale);
    }
    mod_connections_.clear();
  }

  std::vector<std::string> TwytchVoiceHandler::getModulationDestinations() {
    std::vector<std::string> destination_names;
    for (auto iter : mod_destinations_)
      destination_names.push_back(iter.first);
    return destination_names;
  }
} // namespace mopo
