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

#include "twytch_lfo.h"
#include "twytch_oscillators.h"

#include <sstream>

#define PITCH_MOD_RANGE 12
#define MIN_GAIN_DB -24.0
#define MAX_GAIN_DB 24.0

#define MAX_FEEDBACK_SAMPLES 20000

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

  TwytchVoiceHandler::TwytchVoiceHandler(Processor* beats_per_second) {
    beats_per_second_ = beats_per_second;
    output_ = new Multiply();
    registerOutput(output_->output());

    // Create modulation and pitch wheels.
    mod_wheel_amount_ = new SmoothValue(0);
    pitch_wheel_amount_ = new SmoothValue(0);

    mod_sources_["pitch_wheel"] = pitch_wheel_amount_->output();
    mod_sources_["mod_wheel"] = mod_wheel_amount_->output();

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

    output_->plug(formant_container_, 0);
    output_->plug(amplitude_, 1);

    addProcessor(output_);
    addGlobalProcessor(pitch_wheel_amount_);
    addGlobalProcessor(mod_wheel_amount_);

    setVoiceKiller(amplitude_envelope_->output(Envelope::kValue));
  }

  void TwytchVoiceHandler::createOscillators(Output* midi, Output* reset) {
    // Pitch bend.
    Processor* pitch_bend_range = createPolyModControl("pitch_bend_range", 2.0, false);
    Multiply* pitch_bend = new Multiply();
    pitch_bend->setControlRate();
    pitch_bend->plug(pitch_wheel_amount_, 0);
    pitch_bend->plug(pitch_bend_range, 1);
    Add* bent_midi = new Add();
    bent_midi->setControlRate();
    bent_midi->plug(midi, 0);
    bent_midi->plug(pitch_bend, 1);

    addProcessor(pitch_bend);
    addProcessor(bent_midi);

    // Oscillator 1.
    TwytchOscillators* oscillators = new TwytchOscillators();
    Processor* oscillator1_waveform = createPolyModControl("osc_1_waveform", Wave::kDownSaw, true);
    Processor* oscillator1_transpose = createPolyModControl("osc_1_transpose", 0, true);
    Processor* oscillator1_tune = createPolyModControl("osc_1_tune", 0.0, true);
    Add* oscillator1_transposed = new Add();
    oscillator1_transposed->setControlRate();
    oscillator1_transposed->plug(bent_midi, 0);
    oscillator1_transposed->plug(oscillator1_transpose, 1);
    Add* oscillator1_midi = new Add();
    oscillator1_midi->setControlRate();
    oscillator1_midi->plug(oscillator1_transposed, 0);
    oscillator1_midi->plug(oscillator1_tune, 1);

    MidiScale* oscillator1_frequency = new MidiScale();
    oscillator1_frequency->setControlRate();
    oscillator1_frequency->plug(oscillator1_midi);
    FrequencyToPhase* oscillator1_phase_inc = new FrequencyToPhase();
    oscillator1_phase_inc->setControlRate();
    oscillator1_phase_inc->plug(oscillator1_frequency);

    oscillators->plug(oscillator1_waveform, TwytchOscillators::kOscillator1Waveform);
    oscillators->plug(reset, TwytchOscillators::kReset);
    oscillators->plug(oscillator1_phase_inc, TwytchOscillators::kOscillator1PhaseInc);

    Processor* cross_mod = createPolyModControl("cross_modulation", 0.15, false);
    oscillators->plug(cross_mod, TwytchOscillators::kCrossMod);

    addProcessor(oscillator1_transposed);
    addProcessor(oscillator1_midi);
    addProcessor(oscillator1_frequency);
    addProcessor(oscillator1_phase_inc);
    addProcessor(oscillators);

    // Oscillator 2.
    Processor* oscillator2_waveform = createPolyModControl("osc_2_waveform", Wave::kDownSaw, true);
    Processor* oscillator2_transpose = createPolyModControl("osc_2_transpose", -12, true);
    Processor* oscillator2_tune = createPolyModControl("osc_2_tune", 0.08, true);
    Add* oscillator2_transposed = new Add();
    oscillator2_transposed->setControlRate();
    oscillator2_transposed->plug(bent_midi, 0);
    oscillator2_transposed->plug(oscillator2_transpose, 1);
    Add* oscillator2_midi = new Add();
    oscillator2_midi->setControlRate();
    oscillator2_midi->plug(oscillator2_transposed, 0);
    oscillator2_midi->plug(oscillator2_tune, 1);

    MidiScale* oscillator2_frequency = new MidiScale();
    oscillator2_frequency->setControlRate();
    oscillator2_frequency->plug(oscillator2_midi);
    FrequencyToPhase* oscillator2_phase_inc = new FrequencyToPhase();
    oscillator2_phase_inc->setControlRate();
    oscillator2_phase_inc->plug(oscillator2_frequency);

    oscillators->plug(oscillator2_waveform, TwytchOscillators::kOscillator2Waveform);
    oscillators->plug(oscillator2_phase_inc, TwytchOscillators::kOscillator2PhaseInc);

    addProcessor(oscillator2_transposed);
    addProcessor(oscillator2_midi);
    addProcessor(oscillator2_frequency);
    addProcessor(oscillator2_phase_inc);

    // Oscillator mix.
    Processor* oscillator_mix_amount = createPolyModControl("osc_mix", 0.5, false, true);

    Clamp* clamp_mix = new Clamp(0, 1);
    clamp_mix->plug(oscillator_mix_amount);
    Interpolate* oscillator_mix = new Interpolate();
    oscillator_mix->plug(oscillators->output(0), Interpolate::kFrom);
    oscillator_mix->plug(oscillators->output(1), Interpolate::kTo);
    oscillator_mix->plug(clamp_mix, Interpolate::kFractional);

    addProcessor(oscillator_mix);
    addProcessor(clamp_mix);

    // Oscillator feedback.
    Processor* osc_feedback_transpose = createPolyModControl("osc_feedback_transpose", -12, true);
    Processor* osc_feedback_amount = createPolyModControl("osc_feedback_amount", 0.0, false);
    Processor* osc_feedback_tune = createPolyModControl("osc_feedback_tune", 0.0, true);
    Add* osc_feedback_transposed = new Add();
    osc_feedback_transposed->setControlRate();
    osc_feedback_transposed->plug(bent_midi, 0);
    osc_feedback_transposed->plug(osc_feedback_transpose, 1);
    Add* osc_feedback_midi = new Add();
    osc_feedback_midi->setControlRate();
    osc_feedback_midi->plug(osc_feedback_transposed, 0);
    osc_feedback_midi->plug(osc_feedback_tune, 1);

    MidiScale* osc_feedback_frequency = new MidiScale();
    osc_feedback_frequency->setControlRate();
    osc_feedback_frequency->plug(osc_feedback_midi);

    FrequencyToSamples* osc_feedback_samples = new FrequencyToSamples();
    osc_feedback_samples->plug(osc_feedback_frequency);

    SampleAndHoldBuffer* osc_feedback_samples_audio = new SampleAndHoldBuffer();
    osc_feedback_samples_audio->plug(osc_feedback_samples);

    addProcessor(osc_feedback_transposed);
    addProcessor(osc_feedback_midi);
    addProcessor(osc_feedback_frequency);
    addProcessor(osc_feedback_samples);
    addProcessor(osc_feedback_samples_audio);

    osc_feedback_ = new SimpleDelay(MAX_FEEDBACK_SAMPLES);
    osc_feedback_->plug(oscillator_mix, SimpleDelay::kAudio);
    osc_feedback_->plug(osc_feedback_samples_audio, SimpleDelay::kSampleDelay);
    osc_feedback_->plug(osc_feedback_amount, SimpleDelay::kFeedback);
    addProcessor(osc_feedback_);
  }

  void TwytchVoiceHandler::createModulators(Output* reset) {
    // Poly LFO.
    Processor* lfo_waveform = createMonoModControl("poly_lfo_waveform", Wave::kSin, true);
    Processor* lfo_free_frequency = createPolyModControl("poly_lfo_frequency", 0.0,
                                                         true, false, kExponential);
    Processor* lfo_free_amplitude = createPolyModControl("poly_lfo_amplitude", 1.0, true);
    Processor* lfo_frequency = createTempoSyncSwitch("poly_lfo", lfo_free_frequency,
                                                     beats_per_second_, true);
    TwytchLfo* lfo = new TwytchLfo();
    lfo->plug(reset, TwytchLfo::kReset);
    lfo->plug(lfo_waveform, TwytchLfo::kWaveform);
    lfo->plug(lfo_frequency, TwytchLfo::kFrequency);

    Multiply* scaled_lfo = new Multiply();
    scaled_lfo->setControlRate();
    scaled_lfo->plug(lfo, 0);
    scaled_lfo->plug(lfo_free_amplitude, 1);

    addProcessor(lfo);
    addProcessor(scaled_lfo);
    mod_sources_["poly_lfo"] = scaled_lfo->output();
  }

  void TwytchVoiceHandler::createFilter(
      Output* audio, Output* keytrack, Output* reset, Output* note_event) {
    // Filter envelope.
    Processor* filter_attack = createPolyModControl("fil_attack", 0.1, false, false, kQuadratic);
    Processor* filter_decay = createPolyModControl("fil_decay", 0.9, true, false, kQuadratic);
    Processor* filter_sustain = createPolyModControl("fil_sustain", 0.3, false);
    Processor* filter_release = createPolyModControl("fil_release", 0.9, true, false, kQuadratic);

    TriggerFilter* note_off = new TriggerFilter(kVoiceOff);
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

    Processor* filter_envelope_depth = createPolyModControl("fil_env_depth", 48, false);
    Multiply* scaled_envelope = new Multiply();
    scaled_envelope->setControlRate();
    scaled_envelope->plug(filter_envelope_, 0);
    scaled_envelope->plug(filter_envelope_depth, 1);

    addProcessor(filter_envelope_);
    addProcessor(note_off);
    addProcessor(filter_env_trigger);
    addProcessor(scaled_envelope);

    // Filter.
    Value* filter_type = new Value(Filter::kLowPass);
    Processor* keytrack_amount = createPolyModControl("keytrack", 0.0, false);
    Multiply* current_keytrack = new Multiply();
    current_keytrack->setControlRate();
    current_keytrack->plug(keytrack, 0);
    current_keytrack->plug(keytrack_amount, 1);

    Processor* base_cutoff = createPolyModControl("cutoff", 80, true, true);
    Add* keytracked_cutoff = new Add();
    keytracked_cutoff->setControlRate();
    keytracked_cutoff->plug(base_cutoff, 0);
    keytracked_cutoff->plug(current_keytrack, 1);

    Add* midi_cutoff = new Add();
    midi_cutoff->setControlRate();
    midi_cutoff->plug(keytracked_cutoff, 0);
    midi_cutoff->plug(scaled_envelope, 1);

    MidiScale* frequency_cutoff = new MidiScale();
    frequency_cutoff->setControlRate();
    frequency_cutoff->plug(midi_cutoff);

    Processor* resonance = createPolyModControl("resonance", 0.5, true);
    ResonanceScale* final_resonance = new ResonanceScale();
    final_resonance->setControlRate();
    final_resonance->plug(resonance);

    Value* min_db = new Value(MIN_GAIN_DB);
    Value* max_db = new Value(MAX_GAIN_DB);
    Interpolate* decibals = new Interpolate();
    decibals->setControlRate();
    decibals->plug(min_db, Interpolate::kFrom);
    decibals->plug(max_db, Interpolate::kTo);
    decibals->plug(resonance, Interpolate::kFractional);
    MagnitudeScale* final_gain = new MagnitudeScale();
    final_gain->setControlRate();
    final_gain->plug(decibals);

    Processor* filter_saturation = createPolyModControl("filter_saturation", 0.0, true);
    MagnitudeScale* saturation_magnitude = new MagnitudeScale();
    saturation_magnitude->setControlRate();
    saturation_magnitude->plug(filter_saturation);

    LinearSmoothBuffer* smooth_saturation_magnitude = new LinearSmoothBuffer();
    smooth_saturation_magnitude->plug(saturation_magnitude);

    Multiply* saturated_audio = new Multiply();
    saturated_audio->plug(audio, 0);
    saturated_audio->plug(smooth_saturation_magnitude, 1);

    Filter* filter = new Filter();
    filter->plug(saturated_audio, Filter::kAudio);
    filter->plug(filter_type, Filter::kType);
    filter->plug(reset, Filter::kReset);
    filter->plug(frequency_cutoff, Filter::kCutoff);
    filter->plug(final_resonance, Filter::kResonance);
    filter->plug(final_gain, Filter::kGain);

    distorted_filter_ = new Distortion();
    Value* distortion_type = new Value(Distortion::kTanh);
    Value* distortion_threshold = new Value(0.5);
    distorted_filter_->plug(filter, Distortion::kAudio);
    distorted_filter_->plug(distortion_type, Distortion::kType);
    distorted_filter_->plug(distortion_threshold, Distortion::kThreshold);

    addProcessor(current_keytrack);
    addProcessor(saturated_audio);
    addProcessor(keytracked_cutoff);
    addProcessor(midi_cutoff);
    addProcessor(final_resonance);
    addProcessor(decibals);
    addProcessor(final_gain);
    addProcessor(frequency_cutoff);
    addProcessor(filter);

    addProcessor(saturation_magnitude);
    addProcessor(smooth_saturation_magnitude);
    addProcessor(distorted_filter_);

    controls_["filter_type"] = filter_type;

    mod_sources_["filter_env"] = filter_envelope_->output();

    // Stutter.
    BypassRouter* stutter_container = new BypassRouter();
    Value* stutter_on = new Value(0);
    stutter_container->plug(stutter_on, BypassRouter::kOn);
    stutter_container->plug(distorted_filter_, BypassRouter::kAudio);

    Stutter* stutter = new Stutter(44100);
    Processor* stutter_frequency = createPolyModControl("stutter_frequency", 8.0, false);
    Processor* resample_frequency = createPolyModControl("stutter_resample_frequency", 2.0, false);
    stutter_container->addProcessor(stutter);
    stutter_container->registerOutput(stutter->output());

    stutter->plug(distorted_filter_, Stutter::kAudio);
    stutter->plug(stutter_frequency, Stutter::kStutterFrequency);
    stutter->plug(resample_frequency, Stutter::kResampleFrequency);
    stutter->plug(reset, Stutter::kReset);

    controls_["stutter_on"] = stutter_on;
    addProcessor(stutter_container);

    // Formant Filter.
    formant_container_ = new BypassRouter();
    Value* formant_on = new Value(0);
    formant_container_->plug(formant_on, BypassRouter::kOn);
    formant_container_->plug(stutter_container, BypassRouter::kAudio);

    formant_filter_ = new FormantManager(NUM_FORMANTS);
    Value* formant_passthrough = new Value(0.0);
    formant_filter_->plug(stutter_container, FormantManager::kAudio);
    formant_filter_->plug(formant_passthrough, FormantManager::kPassthroughGain);
    formant_filter_->plug(reset, FormantManager::kReset);

    controls_["formant_on"] = formant_on;
    controls_["formant_passthrough"] = formant_passthrough;

    Processor* formant_x = createPolyModControl("formant_x", 0.0, true);
    Processor* formant_y = createPolyModControl("formant_y", 0.0, true);

    for (int i = 0; i < NUM_FORMANTS; ++i) {
      BilinearInterpolate* formant_gain = new BilinearInterpolate();
      formant_gain->setControlRate();
      BilinearInterpolate* formant_q = new BilinearInterpolate();
      formant_q->setControlRate();
      BilinearInterpolate* formant_frequency = new BilinearInterpolate();
      formant_frequency->setControlRate();

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
      formant_frequency->plug(bottom_right_formants[i].frequency,
                              BilinearInterpolate::kBottomRight);

      formant_gain->plug(formant_x, BilinearInterpolate::kXPosition);
      formant_q->plug(formant_x, BilinearInterpolate::kXPosition);
      formant_frequency->plug(formant_x, BilinearInterpolate::kXPosition);

      formant_gain->plug(formant_y, BilinearInterpolate::kYPosition);
      formant_q->plug(formant_y, BilinearInterpolate::kYPosition);
      formant_frequency->plug(formant_y, BilinearInterpolate::kYPosition);

      LinearSmoothBuffer* smooth_gain = new LinearSmoothBuffer();
      smooth_gain->plug(formant_gain);

      formant_filter_->getFormant(i)->plug(smooth_gain, Formant::kGain);
      formant_filter_->getFormant(i)->plug(formant_q, Formant::kResonance);
      formant_filter_->getFormant(i)->plug(formant_frequency, Formant::kFrequency);

      addProcessor(formant_gain);
      addProcessor(smooth_gain);
      addProcessor(formant_q);
      addProcessor(formant_frequency);
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
    Processor* amplitude_attack = createPolyModControl("amp_attack", 0.1,
                                                       false, false, kQuadratic);
    Processor* amplitude_decay = createPolyModControl("amp_decay", 1.0,
                                                      true, false, kQuadratic);
    Processor* amplitude_sustain = createPolyModControl("amp_sustain", 0.5, false);
    Processor* amplitude_release = createPolyModControl("amp_release", 0.7,
                                                        true, false, kQuadratic);

    amplitude_envelope_ = new Envelope();
    amplitude_envelope_->plug(legato_filter->output(LegatoFilter::kRetrigger),
                              Envelope::kTrigger);
    amplitude_envelope_->plug(amplitude_attack, Envelope::kAttack);
    amplitude_envelope_->plug(amplitude_decay, Envelope::kDecay);
    amplitude_envelope_->plug(amplitude_sustain, Envelope::kSustain);
    amplitude_envelope_->plug(amplitude_release, Envelope::kRelease);
    addProcessor(amplitude_envelope_);

    // Voice and frequency resetting logic.
    TriggerCombiner* note_change_trigger = new TriggerCombiner();
    note_change_trigger->plug(legato_filter->output(LegatoFilter::kRemain), 0);
    note_change_trigger->plug(amplitude_envelope_->output(Envelope::kFinished), 1);

    TriggerWait* note_wait = new TriggerWait();
    Value* current_note = new Value();
    note_wait->plug(note, TriggerWait::kWait);
    note_wait->plug(note_change_trigger, TriggerWait::kTrigger);
    current_note->plug(note_wait);

    Value* max_midi_invert = new Value(1.0 / (MIDI_SIZE - 1));
    Multiply* note_percentage = new Multiply();
    note_percentage->setControlRate();
    note_percentage->plug(max_midi_invert, 0);
    note_percentage->plug(current_note, 1);

    addProcessor(note_change_trigger);
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
    velocity_wait->plug(note_change_trigger, TriggerWait::kTrigger);
    current_velocity->plug(velocity_wait);

    addProcessor(velocity_wait);
    addProcessor(current_velocity);

    Processor* velocity_track_amount = createPolyModControl("velocity_track", 0.3, false);
    Interpolate* velocity_track_mult = new Interpolate();
    velocity_track_mult->plug(&utils::value_one, Interpolate::kFrom);
    velocity_track_mult->plug(current_velocity, Interpolate::kTo);
    velocity_track_mult->plug(velocity_track_amount, Interpolate::kFractional);
    addProcessor(velocity_track_mult);

    // Current amplitude using envelope and velocity.
    amplitude_ = new Multiply();
    amplitude_->plug(amplitude_envelope_->output(Envelope::kValue), 0);
    amplitude_->plug(velocity_track_mult, 1);

    addProcessor(amplitude_);

    // Portamento.
    Processor* portamento = createPolyModControl("portamento", -7.0, false, false, kExponential);
    Value* portamento_type = new Value(0);
    PortamentoFilter* portamento_filter = new PortamentoFilter();
    portamento_filter->plug(portamento_type, PortamentoFilter::kPortamento);
    portamento_filter->plug(note_change_trigger, PortamentoFilter::kFrequencyTrigger);
    portamento_filter->plug(trigger, PortamentoFilter::kVoiceTrigger);
    addProcessor(portamento_filter);

    current_frequency_ = new LinearSlope();
    current_frequency_->plug(current_note, LinearSlope::kTarget);
    current_frequency_->plug(portamento, LinearSlope::kRunSeconds);
    current_frequency_->plug(portamento_filter, LinearSlope::kTriggerJump);

    addProcessor(current_frequency_);
    controls_["portamento_type"] = portamento_type;

    mod_sources_["amplitude_env"] = amplitude_envelope_->output();
    mod_sources_["note"] = note_percentage->output();
    mod_sources_["velocity"] = current_velocity->output();
  }

  void TwytchVoiceHandler::setModWheel(mopo_float value) {
    mod_wheel_amount_->set(value);
  }

  void TwytchVoiceHandler::setPitchWheel(mopo_float value) {
    pitch_wheel_amount_->set(value);
  }
} // namespace mopo
