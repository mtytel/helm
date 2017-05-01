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

#include "helm_voice_handler.h"

#include "fixed_point_oscillator.h"
#include "ladder_filter.h"
#include "noise_oscillator.h"
#include "resonance_cancel.h"
#include "helm_lfo.h"
#include "helm_oscillators.h"
#include "switch.h"

#include <sstream>

#define PITCH_MOD_RANGE 12
#define MIN_GAIN_DB -24.0
#define MAX_GAIN_DB 24.0

#define MAX_FEEDBACK_SAMPLES 8000
#define MAX_POLYPHONY 33

namespace mopo {

  namespace {
    struct FormantValues {
      cr::Value gain;
      cr::Value resonance;
      cr::Value midi_cutoff;
    };

    static const cr::Value formant_filter_types[NUM_FORMANTS] = {
      cr::Value(BiquadFilter::kGainedBandPass),
      cr::Value(BiquadFilter::kGainedBandPass),
      cr::Value(BiquadFilter::kGainedBandPass),
      cr::Value(BiquadFilter::kGainedBandPass)
    };

    static const Value formant_a_decibels(-4.0f);
    static const Value formant_e_decibels(-2.0f);
    static const Value formant_i_decibels(-2.0f);
    static const Value formant_o_decibels(-4.0f);
    static const Value formant_u_decibels(-2.0f);

    static const FormantValues formant_a[NUM_FORMANTS] = {
      {cr::Value(24), cr::Value(10), cr::Value(75.7552343327)},
      {cr::Value(18), cr::Value(12), cr::Value(84.5454706023)},
      {cr::Value(17), cr::Value(16), cr::Value(100.08500317)},
      {cr::Value(16), cr::Value(16), cr::Value(101.645729657)},
    };

    static const FormantValues formant_e[NUM_FORMANTS] = {
      {cr::Value(24), cr::Value(10), cr::Value(67.349957715)},
      {cr::Value(10), cr::Value(12), cr::Value(92.39951181)},
      {cr::Value(12), cr::Value(16), cr::Value(99.7552343327)},
      {cr::Value(10), cr::Value(16), cr::Value(103.349957715)},
    };

    static const FormantValues formant_i[NUM_FORMANTS] = {
      {cr::Value(24), cr::Value(13), cr::Value(61.7825925179)},
      {cr::Value(9), cr::Value(12), cr::Value(94.049554095)},
      {cr::Value(6), cr::Value(16), cr::Value(101.03821678)},
      {cr::Value(4), cr::Value(16), cr::Value(103.618371471)},
    };

    static const FormantValues formant_o[NUM_FORMANTS] = {
      {cr::Value(24), cr::Value(11), cr::Value(67.349957715)},
      {cr::Value(14), cr::Value(12), cr::Value(79.349957715)},
      {cr::Value(12), cr::Value(16), cr::Value(99.7552343327)},
      {cr::Value(12), cr::Value(16), cr::Value(101.03821678)},
    };

    static const FormantValues formant_u[NUM_FORMANTS] = {
      {cr::Value(24), cr::Value(11), cr::Value(65.0382167797)},
      {cr::Value(4), cr::Value(12), cr::Value(74.3695077237)},
      {cr::Value(7), cr::Value(16), cr::Value(100.408607741)},
      {cr::Value(10), cr::Value(16), cr::Value(101.645729657)},
    };
  } // namespace

  HelmVoiceHandler::HelmVoiceHandler(Processor* beats_per_second) :
      ProcessorRouter(VoiceHandler::kNumInputs, 0), VoiceHandler(MAX_POLYPHONY),
      beats_per_second_(beats_per_second) {
    output_ = new Multiply();
    registerOutput(output_->output());
  }

  void HelmVoiceHandler::init() {
    // Create modulation and pitch wheels per channel.
    choose_pitch_wheel_ = new Switch();
    choose_pitch_wheel_->plug(channel(), Switch::kSource);

    Switch* choose_mod_wheel = new Switch();
    choose_mod_wheel->plug(channel(), Switch::kSource);

    for (int i = 0; i < mopo::NUM_MIDI_CHANNELS; ++i) {
      pitch_wheel_amounts_[i] = new cr::Value(0);
      choose_pitch_wheel_->plugNext(pitch_wheel_amounts_[i]);

      mod_wheel_amounts_[i] = new cr::Value(0);
      choose_mod_wheel->plugNext(mod_wheel_amounts_[i]);

      addGlobalProcessor(pitch_wheel_amounts_[i]);
      addGlobalProcessor(mod_wheel_amounts_[i]);
    }

    addProcessor(choose_pitch_wheel_);
    addProcessor(choose_mod_wheel);

    mod_sources_["pitch_wheel"] = choose_pitch_wheel_->output();
    mod_sources_["mod_wheel"] = choose_mod_wheel->output();

    // Create all synthesizer voice components.
    createArticulation(note(), last_note(), velocity(), voice_event());
    createOscillators(current_frequency_->output(),
                      amplitude_envelope_->output(Envelope::kFinished));
    createModulators(amplitude_envelope_->output(Envelope::kFinished));
    createFilter(osc_feedback_->output(0), note_from_center_->output(),
                 amplitude_envelope_->output(Envelope::kFinished));

    Value* aftertouch_value = new cr::Value();
    aftertouch_value->plug(aftertouch());

    addProcessor(aftertouch_value);
    mod_sources_["aftertouch"] = aftertouch_value->output();

    output_->plug(formant_container_, 0);
    output_->plug(amplitude_, 1);

    addProcessor(output_);

    setVoiceKiller(amplitude_envelope_->output(Envelope::kValue));

    HelmModule::init();
    setupPolyModulationReadouts();
  }

  void HelmVoiceHandler::createOscillators(Output* midi, Output* reset) {
    // Pitch bend.
    Processor* pitch_bend_range = createPolyModControl("pitch_bend_range", true);
    cr::Multiply* pitch_bend = new cr::Multiply();
    pitch_bend->plug(choose_pitch_wheel_, 0);
    pitch_bend->plug(pitch_bend_range, 1);
    cr::Add* bent_midi = new cr::Add();
    bent_midi->plug(midi, 0);
    bent_midi->plug(pitch_bend, 1);

    addProcessor(pitch_bend);
    addProcessor(bent_midi);

    // Oscillator 1.
    HelmOscillators* oscillators = new HelmOscillators();
    Processor* oscillator1_waveform = createPolyModControl("osc_1_waveform", true);
    Processor* oscillator1_transpose = createPolyModControl("osc_1_transpose", true);
    Processor* oscillator1_tune = createPolyModControl("osc_1_tune", true);
    Processor* oscillator1_unison_voices = createPolyModControl("osc_1_unison_voices", true);
    Processor* oscillator1_unison_detune = createPolyModControl("osc_1_unison_detune", true);
    Processor* oscillator1_unison_harmonize = createBaseControl("unison_1_harmonize");

    cr::Add* oscillator1_transposed = new cr::Add();
    oscillator1_transposed->plug(bent_midi, 0);
    oscillator1_transposed->plug(oscillator1_transpose, 1);
    cr::Add* oscillator1_midi = new cr::Add();
    oscillator1_midi->plug(oscillator1_transposed, 0);
    oscillator1_midi->plug(oscillator1_tune, 1);

    cr::MidiScale* oscillator1_frequency = new cr::MidiScale();
    oscillator1_frequency->plug(oscillator1_midi);
    cr::FrequencyToPhase* oscillator1_phase_inc = new cr::FrequencyToPhase();
    oscillator1_phase_inc->plug(oscillator1_frequency);

    LinearSmoothBuffer* oscillator1_phase_inc_smooth = new LinearSmoothBuffer();
    oscillator1_phase_inc_smooth->plug(oscillator1_phase_inc, LinearSmoothBuffer::kValue);
    oscillator1_phase_inc_smooth->plug(reset, LinearSmoothBuffer::kTrigger);

    oscillators->plug(oscillator1_waveform, HelmOscillators::kOscillator1Waveform);
    oscillators->plug(reset, HelmOscillators::kReset);
    oscillators->plug(oscillator1_phase_inc_smooth, HelmOscillators::kOscillator1PhaseInc);
    oscillators->plug(oscillator1_unison_detune, HelmOscillators::kUnisonDetune1);
    oscillators->plug(oscillator1_unison_voices, HelmOscillators::kUnisonVoices1);
    oscillators->plug(oscillator1_unison_harmonize, HelmOscillators::kHarmonize1);

    Processor* cross_mod = createPolyModControl("cross_modulation", false, true);
    oscillators->plug(cross_mod, HelmOscillators::kCrossMod);

    addProcessor(oscillator1_transposed);
    addProcessor(oscillator1_midi);
    addProcessor(oscillator1_frequency);
    addProcessor(oscillator1_phase_inc);
    addProcessor(oscillator1_phase_inc_smooth);
    addProcessor(oscillators);

    // Oscillator 2.
    Processor* oscillator2_waveform = createPolyModControl("osc_2_waveform", true);
    Processor* oscillator2_transpose = createPolyModControl("osc_2_transpose", true);
    Processor* oscillator2_tune = createPolyModControl("osc_2_tune", true);
    Processor* oscillator2_unison_voices = createPolyModControl("osc_2_unison_voices", true);
    Processor* oscillator2_unison_detune = createPolyModControl("osc_2_unison_detune", true);
    Processor* oscillator2_unison_harmonize = createBaseControl("unison_2_harmonize");

    cr::Add* oscillator2_transposed = new cr::Add();
    oscillator2_transposed->plug(bent_midi, 0);
    oscillator2_transposed->plug(oscillator2_transpose, 1);
    cr::Add* oscillator2_midi = new cr::Add();
    oscillator2_midi->plug(oscillator2_transposed, 0);
    oscillator2_midi->plug(oscillator2_tune, 1);

    cr::MidiScale* oscillator2_frequency = new cr::MidiScale();
    oscillator2_frequency->plug(oscillator2_midi);
    cr::FrequencyToPhase* oscillator2_phase_inc = new cr::FrequencyToPhase();
    oscillator2_phase_inc->plug(oscillator2_frequency);

    LinearSmoothBuffer* oscillator2_phase_inc_smooth = new LinearSmoothBuffer();
    oscillator2_phase_inc_smooth->plug(oscillator2_phase_inc, LinearSmoothBuffer::kValue);
    oscillator2_phase_inc_smooth->plug(reset, LinearSmoothBuffer::kTrigger);

    oscillators->plug(oscillator2_waveform, HelmOscillators::kOscillator2Waveform);
    oscillators->plug(oscillator2_phase_inc_smooth, HelmOscillators::kOscillator2PhaseInc);
    oscillators->plug(oscillator2_unison_detune, HelmOscillators::kUnisonDetune2);
    oscillators->plug(oscillator2_unison_voices, HelmOscillators::kUnisonVoices2);
    oscillators->plug(oscillator2_unison_harmonize, HelmOscillators::kHarmonize2);

    addProcessor(oscillator2_transposed);
    addProcessor(oscillator2_midi);
    addProcessor(oscillator2_frequency);
    addProcessor(oscillator2_phase_inc);
    addProcessor(oscillator2_phase_inc_smooth);

    // Oscillator mix.
    Processor* osc_1_amplitude = createPolyModControl("osc_1_volume", true, false);
    LinearSmoothBuffer* smooth_osc_1_amp = new LinearSmoothBuffer();
    smooth_osc_1_amp->plug(osc_1_amplitude, LinearSmoothBuffer::kValue);
    smooth_osc_1_amp->plug(reset, LinearSmoothBuffer::kTrigger);
    oscillators->plug(smooth_osc_1_amp, HelmOscillators::kOscillator1Amplitude);

    Processor* osc_2_amplitude = createPolyModControl("osc_2_volume", true, false);
    LinearSmoothBuffer* smooth_osc_2_amp = new LinearSmoothBuffer();
    smooth_osc_2_amp->plug(osc_2_amplitude, LinearSmoothBuffer::kValue);
    smooth_osc_2_amp->plug(reset, LinearSmoothBuffer::kTrigger);
    oscillators->plug(smooth_osc_2_amp, HelmOscillators::kOscillator2Amplitude);

    addProcessor(smooth_osc_1_amp);
    addProcessor(smooth_osc_2_amp);

    // Sub Oscillator.
    cr::Add* sub_midi = new cr::Add();
    static const cr::Value sub_transpose(-2 * NOTES_PER_OCTAVE);
    sub_midi->plug(bent_midi, 0);
    sub_midi->plug(&sub_transpose, 1);

    cr::MidiScale* sub_frequency = new cr::MidiScale();
    sub_frequency->plug(sub_midi);
    cr::FrequencyToPhase* sub_phase_inc = new cr::FrequencyToPhase();
    sub_phase_inc->plug(sub_frequency);

    Processor* sub_waveform = createPolyModControl("sub_waveform", true);
    Processor* sub_shuffle = createPolyModControl("sub_shuffle", false, true);
    FixedPointOscillator* sub_oscillator = new FixedPointOscillator();
    sub_oscillator->plug(sub_phase_inc, FixedPointOscillator::kPhaseInc);
    sub_oscillator->plug(sub_shuffle, FixedPointOscillator::kShuffle);
    sub_oscillator->plug(sub_waveform, FixedPointOscillator::kWaveform);
    sub_oscillator->plug(reset, FixedPointOscillator::kReset);

    Processor* sub_volume = createPolyModControl("sub_volume", true, false);
    Multiply* scaled_sub_oscillator = new Multiply();

    LinearSmoothBuffer* smooth_sub_volume = new LinearSmoothBuffer();
    smooth_sub_volume->plug(sub_volume, LinearSmoothBuffer::kValue);
    smooth_sub_volume->plug(reset, LinearSmoothBuffer::kTrigger);
    scaled_sub_oscillator->plug(sub_oscillator, 0);
    scaled_sub_oscillator->plug(smooth_sub_volume, 1);

    addProcessor(sub_midi);
    addProcessor(sub_frequency);
    addProcessor(sub_phase_inc);
    addProcessor(sub_oscillator);
    addProcessor(smooth_sub_volume);
    addProcessor(scaled_sub_oscillator);

    Add *oscillator_sum = new Add();
    oscillator_sum->plug(oscillators, 0);
    oscillator_sum->plug(scaled_sub_oscillator, 1);

    addProcessor(oscillator_sum);

    // Noise Oscillator.
    NoiseOscillator* noise_oscillator = new NoiseOscillator();
    noise_oscillator->plug(reset, NoiseOscillator::kReset);

    Processor* noise_volume = createPolyModControl("noise_volume", false);
    Multiply* scaled_noise_oscillator = new Multiply();
    scaled_noise_oscillator->plug(noise_oscillator, 0);
    scaled_noise_oscillator->plug(noise_volume, 1);

    addProcessor(noise_oscillator);
    addProcessor(scaled_noise_oscillator);

    Add *oscillator_noise_sum = new Add();
    oscillator_noise_sum->plug(oscillator_sum, 0);
    oscillator_noise_sum->plug(scaled_noise_oscillator, 1);

    addProcessor(oscillator_noise_sum);

    // Oscillator feedback.
    Processor* osc_feedback_transpose = createPolyModControl("osc_feedback_transpose", true);
    Processor* osc_feedback_amount = createPolyModControl("osc_feedback_amount", false, true);
    Processor* osc_feedback_tune = createPolyModControl("osc_feedback_tune", true);

    cr::Add* osc_feedback_transposed = new cr::Add();
    osc_feedback_transposed->plug(bent_midi, 0);
    osc_feedback_transposed->plug(osc_feedback_transpose, 1);

    cr::Add* osc_feedback_midi = new cr::Add();
    osc_feedback_midi->plug(osc_feedback_transposed, 0);
    osc_feedback_midi->plug(osc_feedback_tune, 1);

    cr::MidiScale* osc_feedback_frequency = new cr::MidiScale();
    osc_feedback_frequency->plug(osc_feedback_midi);

    cr::FrequencyToSamples* osc_feedback_samples = new cr::FrequencyToSamples();
    osc_feedback_samples->plug(osc_feedback_frequency);

    LinearSmoothBuffer* osc_feedback_samples_audio = new LinearSmoothBuffer();
    osc_feedback_samples_audio->plug(osc_feedback_samples, LinearSmoothBuffer::kValue);
    osc_feedback_samples_audio->plug(reset, LinearSmoothBuffer::kTrigger);
    addProcessor(osc_feedback_transposed);
    addProcessor(osc_feedback_midi);
    addProcessor(osc_feedback_frequency);
    addProcessor(osc_feedback_samples);
    addProcessor(osc_feedback_samples_audio);

    Clamp* osc_feedback_amount_clamped = new Clamp();
    osc_feedback_amount_clamped->plug(osc_feedback_amount);

    osc_feedback_ = new SimpleDelay(MAX_FEEDBACK_SAMPLES);
    osc_feedback_->plug(oscillator_noise_sum, SimpleDelay::kAudio);
    osc_feedback_->plug(osc_feedback_samples_audio, SimpleDelay::kSampleDelay);
    osc_feedback_->plug(osc_feedback_amount_clamped, SimpleDelay::kFeedback);
    addProcessor(osc_feedback_);
    addProcessor(osc_feedback_amount_clamped);
  }

  void HelmVoiceHandler::createModulators(Output* reset) {
    // Poly LFO.
    Processor* lfo_waveform = createPolyModControl("poly_lfo_waveform", true);
    Processor* lfo_free_frequency = createPolyModControl("poly_lfo_frequency", true, false);
    Processor* lfo_free_amplitude = createPolyModControl("poly_lfo_amplitude", true);
    Processor* lfo_frequency = createTempoSyncSwitch("poly_lfo", lfo_free_frequency,
                                                     beats_per_second_, true);
    HelmLfo* lfo = new HelmLfo();
    lfo->plug(reset, HelmLfo::kReset);
    lfo->plug(lfo_waveform, HelmLfo::kWaveform);
    lfo->plug(lfo_frequency, HelmLfo::kFrequency);

    cr::Multiply* scaled_lfo = new cr::Multiply();
    scaled_lfo->plug(lfo, 0);
    scaled_lfo->plug(lfo_free_amplitude, 1);

    addProcessor(lfo);
    addProcessor(scaled_lfo);
    mod_sources_["poly_lfo"] = scaled_lfo->output();

    // Extra Envelope.
    Processor* mod_attack = createPolyModControl("mod_attack", false, false);
    Processor* mod_decay = createPolyModControl("mod_decay", true, false);
    Processor* mod_sustain = createPolyModControl("mod_sustain", false);
    Processor* mod_release = createPolyModControl("mod_release", true, false);

    extra_envelope_ = new Envelope();
    extra_envelope_->plug(mod_attack, Envelope::kAttack);
    extra_envelope_->plug(mod_decay, Envelope::kDecay);
    extra_envelope_->plug(mod_sustain, Envelope::kSustain);
    extra_envelope_->plug(mod_release, Envelope::kRelease);
    extra_envelope_->plug(env_trigger_, Envelope::kTrigger);

    addProcessor(extra_envelope_);
    mod_sources_["mod_envelope"] = extra_envelope_->output();
  }

  void HelmVoiceHandler::createFilter(
      Output* audio, Output* keytrack, Output* reset) {
    // Filter envelope.
    Processor* filter_attack = createPolyModControl("fil_attack", false, false);
    Processor* filter_decay = createPolyModControl("fil_decay", true, false);
    Processor* filter_sustain = createPolyModControl("fil_sustain", false);
    Processor* filter_release = createPolyModControl("fil_release", true, false);

    filter_envelope_ = new Envelope();
    filter_envelope_->plug(filter_attack, Envelope::kAttack);
    filter_envelope_->plug(filter_decay, Envelope::kDecay);
    filter_envelope_->plug(filter_sustain, Envelope::kSustain);
    filter_envelope_->plug(filter_release, Envelope::kRelease);
    filter_envelope_->plug(env_trigger_, Envelope::kTrigger);

    Processor* filter_envelope_depth = createPolyModControl("fil_env_depth", false);
    cr::Multiply* scaled_envelope = new cr::Multiply();
    scaled_envelope->plug(filter_envelope_, 0);
    scaled_envelope->plug(filter_envelope_depth, 1);

    addProcessor(filter_envelope_);
    addProcessor(scaled_envelope);

    // Filter.
    Processor* filter_type = createBaseControl("filter_type");
    Processor* keytrack_amount = createPolyModControl("keytrack", false);
    cr::Multiply* current_keytrack = new cr::Multiply();
    current_keytrack->plug(keytrack, 0);
    current_keytrack->plug(keytrack_amount, 1);

    Processor* base_cutoff = createPolyModControl("cutoff", true, true);
    cr::Add* keytracked_cutoff = new cr::Add();
    keytracked_cutoff->plug(base_cutoff, 0);
    keytracked_cutoff->plug(current_keytrack, 1);

    cr::Add* midi_cutoff = new cr::Add();
    midi_cutoff->plug(keytracked_cutoff, 0);
    midi_cutoff->plug(scaled_envelope, 1);

    cr::MidiScale* frequency_cutoff = new cr::MidiScale();
    frequency_cutoff->plug(midi_cutoff);

    Processor* resonance = createPolyModControl("resonance", true);
    cr::ResonanceScale* scaled_resonance = new cr::ResonanceScale();
    scaled_resonance->plug(resonance);

    ResonanceCancel* final_resonance = new ResonanceCancel();
    final_resonance->plug(scaled_resonance, ResonanceCancel::kResonance);
    final_resonance->plug(filter_type, ResonanceCancel::kFilterType);

    static const cr::Value min_db(MIN_GAIN_DB);
    static const cr::Value max_db(MAX_GAIN_DB);
    Interpolate* decibels = new Interpolate();
    decibels->setControlRate();
    decibels->plug(&min_db, Interpolate::kFrom);
    decibels->plug(&max_db, Interpolate::kTo);
    decibels->plug(resonance, Interpolate::kFractional);
    cr::MagnitudeScale* final_gain = new cr::MagnitudeScale();
    final_gain->plug(decibels);

    Processor* filter_saturation = createPolyModControl("filter_saturation", true);
    cr::MagnitudeScale* saturation_magnitude = new cr::MagnitudeScale();
    saturation_magnitude->plug(filter_saturation);

    LinearSmoothBuffer* smooth_saturation_magnitude = new LinearSmoothBuffer();
    smooth_saturation_magnitude->plug(saturation_magnitude);
    smooth_saturation_magnitude->plug(reset, LinearSmoothBuffer::kTrigger);

    Multiply* saturated_audio = new Multiply();
    saturated_audio->plug(audio, 0);
    saturated_audio->plug(smooth_saturation_magnitude, 1);

    LadderFilter* ladder = new LadderFilter();
    ladder->plug(saturated_audio, LadderFilter::kAudio);
    ladder->plug(reset, LadderFilter::kReset);
    ladder->plug(frequency_cutoff, LadderFilter::kCutoff);
    ladder->plug(final_resonance, LadderFilter::kResonance);

    StateVariableFilter* filter = new StateVariableFilter();
    filter->plug(saturated_audio, BiquadFilter::kAudio);
    filter->plug(filter_type, BiquadFilter::kType);
    filter->plug(reset, BiquadFilter::kReset);
    filter->plug(frequency_cutoff, BiquadFilter::kCutoff);
    filter->plug(final_resonance, BiquadFilter::kResonance);
    filter->plug(final_gain, BiquadFilter::kGain);

    addProcessor(current_keytrack);
    addProcessor(saturated_audio);
    addProcessor(keytracked_cutoff);
    addProcessor(midi_cutoff);
    addProcessor(scaled_resonance);
    addProcessor(final_resonance);
    addProcessor(decibels);
    addProcessor(final_gain);
    addProcessor(frequency_cutoff);
    addProcessor(filter);
    addProcessor(ladder);

    addProcessor(saturation_magnitude);
    addProcessor(smooth_saturation_magnitude);

    mod_sources_["fil_envelope"] = filter_envelope_->output();

    // Stutter.
    BypassRouter* stutter_container = new BypassRouter();
    addProcessor(stutter_container);

    Processor* stutter_on = createBaseControl("stutter_on");
    stutter_container->plug(stutter_on, BypassRouter::kOn);
    stutter_container->plug(filter, BypassRouter::kAudio);

    Stutter* stutter = new Stutter(STUTTER_MAX_SAMPLES);
    Processor* stutter_free_frequency = createPolyModControl("stutter_frequency", true);
    Processor* stutter_frequency = createTempoSyncSwitch("stutter", stutter_free_frequency,
                                                         beats_per_second_, true);
    Processor* resample_free_frequency = createPolyModControl("stutter_resample_frequency", true);
    Processor* resample_frequency = createTempoSyncSwitch("stutter_resample",
                                                          resample_free_frequency,
                                                          beats_per_second_, true);

    Processor* stutter_softness = createPolyModControl("stutter_softness", true);

    stutter_container->addProcessor(stutter);
    stutter_container->registerOutput(stutter->output());

    stutter->plug(filter, Stutter::kAudio);
    stutter->plug(stutter_frequency, Stutter::kStutterFrequency);
    stutter->plug(resample_frequency, Stutter::kResampleFrequency);
    stutter->plug(stutter_softness, Stutter::kWindowSoftness);
    stutter->plug(reset, Stutter::kReset);

    // Formant Filter.
    formant_container_ = new BypassRouter();
    addProcessor(formant_container_);

    Processor* formant_on = createBaseControl("formant_on");
    formant_container_->plug(formant_on, BypassRouter::kOn);
    formant_container_->plug(stutter_container, BypassRouter::kAudio);

    formant_filter_ = new FormantManager(NUM_FORMANTS);
    formant_filter_->plug(stutter_container, FormantManager::kAudio);
    formant_filter_->plug(reset, FormantManager::kReset);

    Processor* formant_x = createPolyModControl("formant_x", false, true);
    Processor* formant_y = createPolyModControl("formant_y", false, true);

    for (int i = 0; i < NUM_FORMANTS; ++i) {
      BilinearInterpolate* formant_gain = new BilinearInterpolate();
      formant_gain->setControlRate();
      BilinearInterpolate* formant_q = new BilinearInterpolate();
      formant_q->setControlRate();
      BilinearInterpolate* formant_midi = new BilinearInterpolate();
      formant_midi->setControlRate();

      formant_gain->plug(&formant_a[i].gain, BilinearInterpolate::kTopLeft);
      formant_gain->plug(&formant_o[i].gain, BilinearInterpolate::kTopRight);
      formant_gain->plug(&formant_i[i].gain, BilinearInterpolate::kBottomLeft);
      formant_gain->plug(&formant_e[i].gain, BilinearInterpolate::kBottomRight);

      formant_q->plug(&formant_a[i].resonance, BilinearInterpolate::kTopLeft);
      formant_q->plug(&formant_o[i].resonance, BilinearInterpolate::kTopRight);
      formant_q->plug(&formant_i[i].resonance, BilinearInterpolate::kBottomLeft);
      formant_q->plug(&formant_e[i].resonance, BilinearInterpolate::kBottomRight);

      formant_midi->plug(&formant_a[i].midi_cutoff, BilinearInterpolate::kTopLeft);
      formant_midi->plug(&formant_o[i].midi_cutoff, BilinearInterpolate::kTopRight);
      formant_midi->plug(&formant_i[i].midi_cutoff, BilinearInterpolate::kBottomLeft);
      formant_midi->plug(&formant_e[i].midi_cutoff, BilinearInterpolate::kBottomRight);

      formant_gain->plug(formant_x, BilinearInterpolate::kXPosition);
      formant_q->plug(formant_x, BilinearInterpolate::kXPosition);
      formant_midi->plug(formant_x, BilinearInterpolate::kXPosition);

      formant_gain->plug(formant_y, BilinearInterpolate::kYPosition);
      formant_q->plug(formant_y, BilinearInterpolate::kYPosition);
      formant_midi->plug(formant_y, BilinearInterpolate::kYPosition);

      cr::MagnitudeScale* formant_magnitude = new cr::MagnitudeScale();
      formant_magnitude->plug(formant_gain);

      cr::MidiScale* formant_frequency = new cr::MidiScale();
      formant_frequency->plug(formant_midi);

      formant_filter_->getFormant(i)->plug(&formant_filter_types[i], BiquadFilter::kType);
      formant_filter_->getFormant(i)->plug(formant_magnitude, BiquadFilter::kGain);
      formant_filter_->getFormant(i)->plug(formant_q, BiquadFilter::kResonance);
      formant_filter_->getFormant(i)->plug(formant_frequency, BiquadFilter::kCutoff);

      addProcessor(formant_gain);
      addProcessor(formant_magnitude);
      addProcessor(formant_q);
      addProcessor(formant_midi);
      addProcessor(formant_frequency);
    }

    BilinearInterpolate* formant_decibels = new BilinearInterpolate();
    formant_decibels->plug(&formant_a_decibels, BilinearInterpolate::kTopLeft);
    formant_decibels->plug(&formant_o_decibels, BilinearInterpolate::kTopRight);
    formant_decibels->plug(&formant_i_decibels, BilinearInterpolate::kBottomLeft);
    formant_decibels->plug(&formant_e_decibels, BilinearInterpolate::kBottomRight);
    formant_decibels->plug(formant_x, BilinearInterpolate::kXPosition);
    formant_decibels->plug(formant_y, BilinearInterpolate::kYPosition);

    MagnitudeScale* formant_total_gain = new MagnitudeScale();
    formant_total_gain->plug(formant_decibels);

    Multiply* formant_output = new Multiply();
    formant_output->plug(formant_total_gain, 0);
    formant_output->plug(formant_filter_, 1);

    formant_container_->addProcessor(formant_decibels);
    formant_container_->addProcessor(formant_total_gain);
    formant_container_->addProcessor(formant_filter_);
    formant_container_->addProcessor(formant_output);
    formant_container_->registerOutput(formant_output->output());
  }

  void HelmVoiceHandler::createArticulation(
      Output* note, Output* last_note, Output* velocity, Output* trigger) {
    // Legato.
    legato_ = createBaseControl("legato");
    LegatoFilter* legato_filter = new LegatoFilter();
    legato_filter->plug(legato_, LegatoFilter::kLegato);
    legato_filter->plug(trigger, LegatoFilter::kTrigger);

    addProcessor(legato_filter);

    // Amplitude envelope.
    Processor* amplitude_attack = createPolyModControl("amp_attack", false, false);
    Processor* amplitude_decay = createPolyModControl("amp_decay", true, false);
    Processor* amplitude_sustain = createPolyModControl("amp_sustain", false);
    Processor* amplitude_release = createPolyModControl("amp_release", true, false);

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

    static const cr::Value max_midi_invert(1.0 / (MIDI_SIZE - 1));
    cr::Multiply* note_percentage = new cr::Multiply();
    note_percentage->plug(&max_midi_invert, 0);
    note_percentage->plug(current_note, 1);

    addProcessor(note_change_trigger);
    addProcessor(note_wait);
    addProcessor(current_note);

    // Key tracking.
    static const Value center_adjust(-MIDI_SIZE / 2);
    note_from_center_ = new cr::Add();
    note_from_center_->plug(&center_adjust, 0);
    note_from_center_->plug(current_note, 1);

    addProcessor(note_from_center_);
    addProcessor(note_percentage);

    // Velocity tracking.
    TriggerWait* velocity_wait = new TriggerWait();
    Value* current_velocity = new Value();
    velocity_wait->plug(velocity, TriggerWait::kWait);
    velocity_wait->plug(note_change_trigger, TriggerWait::kTrigger);
    current_velocity->plug(velocity_wait);

    addProcessor(velocity_wait);
    addProcessor(current_velocity);

    Processor* velocity_track_amount = createPolyModControl("velocity_track", false);
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
    Processor* portamento = createPolyModControl("portamento", false, false);
    Processor* portamento_type = createBaseControl("portamento_type");

    current_frequency_ = new PortamentoSlope();
    current_frequency_->plug(current_note, PortamentoSlope::kTarget);
    current_frequency_->plug(portamento_type, PortamentoSlope::kPortamentoType);
    current_frequency_->plug(note_pressed(), PortamentoSlope::kNoteNumber);
    current_frequency_->plug(portamento, PortamentoSlope::kRunSeconds);
    current_frequency_->plug(note_pressed(), PortamentoSlope::kTriggerJump);
    current_frequency_->plug(last_note, PortamentoSlope::kTriggerStart);

    addProcessor(current_frequency_);

    mod_sources_["amp_envelope"] = amplitude_envelope_->output();
    mod_sources_["note"] = note_percentage->output();
    mod_sources_["velocity"] = current_velocity->output();

    // Envelope Trigger.
    TriggerFilter* note_off = new TriggerFilter(kVoiceOff);
    note_off->plug(trigger);
    env_trigger_ = new TriggerCombiner();
    env_trigger_->plug(note_off, 0);
    env_trigger_->plug(amplitude_envelope_->output(Envelope::kFinished), 1);

    addProcessor(note_off);
    addProcessor(env_trigger_);
  }

  void HelmVoiceHandler::process() {
    setLegato(legato_->output()->buffer[0]);
    VoiceHandler::process();
    note_retriggered_.clearTrigger();
  }

  void HelmVoiceHandler::noteOn(mopo_float note, mopo_float velocity, int sample, int channel) {
    if (getPressedNotes().size() < polyphony() || legato_->value() == 0.0)
      note_retriggered_.trigger(note, sample);
    VoiceHandler::noteOn(note, velocity, sample, channel);
  }

  VoiceEvent HelmVoiceHandler::noteOff(mopo_float note, int sample) {
    if (getPressedNotes().size() > polyphony() &&
        isNotePlaying(note) &&
        legato_->value() == 0.0)
      note_retriggered_.trigger(note, sample);
    return VoiceHandler::noteOff(note, sample);
  }

  void HelmVoiceHandler::setupPolyModulationReadouts() {
    output_map poly_mods = HelmModule::getPolyModulations();

    for (auto mod : poly_mods)
      poly_readouts_[mod.first] = registerOutput(mod.second);
  }

  void HelmVoiceHandler::setModWheel(mopo_float value, int channel) {
    MOPO_ASSERT(channel >= 1 && channel <= mopo::NUM_MIDI_CHANNELS);
    mod_wheel_amounts_[channel - 1]->set(value);
  }

  void HelmVoiceHandler::setPitchWheel(mopo_float value, int channel) {
    MOPO_ASSERT(channel >= 1 && channel <= mopo::NUM_MIDI_CHANNELS);
    pitch_wheel_amounts_[channel - 1]->set(value);
  }

  output_map HelmVoiceHandler::getPolyModulations() {
    return poly_readouts_;
  }
} // namespace mopo
