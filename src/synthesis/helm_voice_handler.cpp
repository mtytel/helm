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
#include "gate.h"
#include "ladder_filter.h"
#include "noise_oscillator.h"
#include "resonance_cancel.h"
#include "helm_lfo.h"
#include "helm_oscillators.h"
#include "trigger_random.h"
#include "value_switch.h"

#include <sstream>

#define PITCH_MOD_RANGE 12
#define MIN_GAIN_DB -24.0
#define MAX_GAIN_DB 24.0

#define MAX_FEEDBACK_SAMPLES 8000

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

  HelmVoiceHandler::HelmVoiceHandler(Output* beats_per_second) :
      ProcessorRouter(VoiceHandler::kNumInputs, 0), VoiceHandler(MAX_POLYPHONY),
      beats_per_second_(beats_per_second) {
    output_ = new Multiply();
    registerOutput(output_->output());
  }

  void HelmVoiceHandler::init() {
    // Create modulation and pitch wheels per channel.
    choose_pitch_wheel_ = new Gate();
    choose_pitch_wheel_->plug(channel(), Gate::kChoice);

    Gate* choose_mod_wheel = new Gate();
    choose_mod_wheel->plug(channel(), Gate::kChoice);

    for (int i = 0; i < mopo::NUM_MIDI_CHANNELS; ++i) {
      pitch_wheel_amounts_[i] = new cr::Value(0);
      choose_pitch_wheel_->plugNext(pitch_wheel_amounts_[i]);

      mod_wheel_amounts_[i] = new cr::Value(0);
      choose_mod_wheel->plugNext(mod_wheel_amounts_[i]);

      addGlobalProcessor(pitch_wheel_amounts_[i]);
      addGlobalProcessor(mod_wheel_amounts_[i]);
    }

    getMonoRouter()->addProcessor(choose_pitch_wheel_);
    getMonoRouter()->addProcessor(choose_mod_wheel);

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

    setVoiceKiller(amplitude_->output());

    HelmModule::init();
    setupPolyModulationReadouts();
  }

  void HelmVoiceHandler::createOscillators(Output* midi, Output* reset) {
    // Pitch bend.
    Output* pitch_bend_range = createPolyModControl("pitch_bend_range", true);
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
    Output* oscillator1_waveform = createPolyModControl("osc_1_waveform", true);
    Output* oscillator1_transpose = createPolyModControl("osc_1_transpose", true);
    Output* oscillator1_tune = createPolyModControl("osc_1_tune", true);
    Output* oscillator1_unison_voices = createPolyModControl("osc_1_unison_voices", true);
    Output* oscillator1_unison_detune = createPolyModControl("osc_1_unison_detune", true);
    Value* oscillator1_unison_harmonize = createBaseControl("unison_1_harmonize");

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

    Output* cross_mod = createPolyModControl("cross_modulation", true);
    oscillators->plug(cross_mod, HelmOscillators::kCrossMod);

    addProcessor(oscillator1_transposed);
    addProcessor(oscillator1_midi);
    addProcessor(oscillator1_frequency);
    addProcessor(oscillator1_phase_inc);
    addProcessor(oscillator1_phase_inc_smooth);
    addProcessor(oscillators);

    // Oscillator 2.
    Output* oscillator2_waveform = createPolyModControl("osc_2_waveform", true);
    Output* oscillator2_transpose = createPolyModControl("osc_2_transpose", true);
    Output* oscillator2_tune = createPolyModControl("osc_2_tune", true);
    Output* oscillator2_unison_voices = createPolyModControl("osc_2_unison_voices", true);
    Output* oscillator2_unison_detune = createPolyModControl("osc_2_unison_detune", true);
    Value* oscillator2_unison_harmonize = createBaseControl("unison_2_harmonize");

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
    Output* osc_1_amplitude = createPolyModControl("osc_1_volume", true);
    LinearSmoothBuffer* smooth_osc_1_amp = new LinearSmoothBuffer();
    smooth_osc_1_amp->plug(osc_1_amplitude, LinearSmoothBuffer::kValue);
    smooth_osc_1_amp->plug(reset, LinearSmoothBuffer::kTrigger);
    oscillators->plug(smooth_osc_1_amp, HelmOscillators::kOscillator1Amplitude);

    Output* osc_2_amplitude = createPolyModControl("osc_2_volume", true);
    LinearSmoothBuffer* smooth_osc_2_amp = new LinearSmoothBuffer();
    smooth_osc_2_amp->plug(osc_2_amplitude, LinearSmoothBuffer::kValue);
    smooth_osc_2_amp->plug(reset, LinearSmoothBuffer::kTrigger);
    oscillators->plug(smooth_osc_2_amp, HelmOscillators::kOscillator2Amplitude);

    addProcessor(smooth_osc_1_amp);
    addProcessor(smooth_osc_2_amp);

    // Sub Oscillator.
    cr::Add* sub_midi = new cr::Add();
    static const cr::Value sub_transpose(-2 * NOTES_PER_OCTAVE);
    Value* sub_octave = createBaseControl("sub_octave");

    sub_midi->plug(bent_midi, 0);
    sub_midi->plug(&sub_transpose, 1);

    cr::MidiScale* sub_frequency = new cr::MidiScale();
    sub_frequency->plug(sub_midi);
    cr::FrequencyToPhase* sub_phase_inc = new cr::FrequencyToPhase();
    sub_phase_inc->plug(sub_frequency);

    Output* sub_waveform = createPolyModControl("sub_waveform", true);
    Output* sub_shuffle = createPolyModControl("sub_shuffle", true);
    Output* sub_volume = createPolyModControl("sub_volume", true);
    LinearSmoothBuffer* smooth_sub_volume = new LinearSmoothBuffer();
    smooth_sub_volume->plug(sub_volume, LinearSmoothBuffer::kValue);
    smooth_sub_volume->plug(reset, LinearSmoothBuffer::kTrigger);

    FixedPointOscillator* sub_oscillator = new FixedPointOscillator();
    sub_oscillator->plug(sub_phase_inc, FixedPointOscillator::kPhaseInc);
    sub_oscillator->plug(sub_shuffle, FixedPointOscillator::kShuffle);
    sub_oscillator->plug(sub_waveform, FixedPointOscillator::kWaveform);
    sub_oscillator->plug(reset, FixedPointOscillator::kReset);
    sub_oscillator->plug(sub_octave, FixedPointOscillator::kLowOctave);
    sub_oscillator->plug(smooth_sub_volume, FixedPointOscillator::kAmplitude);

    addProcessor(sub_midi);
    addProcessor(sub_frequency);
    addProcessor(sub_phase_inc);
    addProcessor(sub_oscillator);
    addProcessor(smooth_sub_volume);

    Add *oscillator_sum = new Add();
    oscillator_sum->plug(oscillators, 0);
    oscillator_sum->plug(sub_oscillator, 1);

    addProcessor(oscillator_sum);

    // Noise Oscillator.
    Output* noise_volume = createPolyModControl("noise_volume", true);
    NoiseOscillator* noise_oscillator = new NoiseOscillator();
    noise_oscillator->plug(reset, NoiseOscillator::kReset);
    noise_oscillator->plug(noise_volume, NoiseOscillator::kAmplitude);

    addProcessor(noise_oscillator);

    Add *oscillator_noise_sum = new Add();
    oscillator_noise_sum->plug(oscillator_sum, 0);
    oscillator_noise_sum->plug(noise_oscillator, 1);

    addProcessor(oscillator_noise_sum);

    // Oscillator feedback.
    Output* osc_feedback_transpose = createPolyModControl("osc_feedback_transpose", true);
    Output* osc_feedback_amount = createPolyModControl("osc_feedback_amount", true);
    Output* osc_feedback_tune = createPolyModControl("osc_feedback_tune", true);

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

    cr::Clamp* osc_feedback_amount_clamped = new cr::Clamp();
    osc_feedback_amount_clamped->plug(osc_feedback_amount);

    LinearSmoothBuffer* osc_feedback_amount_audio = new LinearSmoothBuffer();
    osc_feedback_amount_audio->plug(osc_feedback_amount_clamped, LinearSmoothBuffer::kValue);
    osc_feedback_amount_audio->plug(reset, LinearSmoothBuffer::kTrigger);

    osc_feedback_ = new SimpleDelay(MAX_FEEDBACK_SAMPLES);
    osc_feedback_->plug(oscillator_noise_sum, SimpleDelay::kAudio);
    osc_feedback_->plug(osc_feedback_samples_audio, SimpleDelay::kSampleDelay);
    osc_feedback_->plug(osc_feedback_amount_audio, SimpleDelay::kFeedback);
    osc_feedback_->plug(reset, SimpleDelay::kReset);

    addProcessor(osc_feedback_);
    addProcessor(osc_feedback_amount_clamped);
    addProcessor(osc_feedback_amount_audio);
  }

  void HelmVoiceHandler::createModulators(Output* reset) {
    // Poly LFO.
    Output* lfo_waveform = createPolyModControl("poly_lfo_waveform", true);
    Output* lfo_free_frequency = createPolyModControl("poly_lfo_frequency", true);
    Output* lfo_free_amplitude = createPolyModControl("poly_lfo_amplitude", true);
    Output* lfo_frequency = createTempoSyncSwitch("poly_lfo", lfo_free_frequency->owner,
                                                  beats_per_second_, true);
    poly_lfo_ = new HelmLfo();
    poly_lfo_->plug(reset, HelmLfo::kReset);
    poly_lfo_->plug(lfo_waveform, HelmLfo::kWaveform);
    poly_lfo_->plug(lfo_frequency, HelmLfo::kFrequency);

    cr::Multiply* scaled_lfo = new cr::Multiply();
    scaled_lfo->plug(poly_lfo_, 0);
    scaled_lfo->plug(lfo_free_amplitude, 1);

    addProcessor(poly_lfo_);
    addProcessor(scaled_lfo);
    mod_sources_["poly_lfo"] = scaled_lfo->output();
    mod_sources_["poly_lfo_amp"] = registerOutput(scaled_lfo->output());
    mod_sources_["poly_lfo_phase"] = registerOutput(poly_lfo_->output(Oscillator::kPhase));

    // Extra Envelope.
    Output* mod_attack = createPolyModControl("mod_attack", true);
    Output* mod_decay = createPolyModControl("mod_decay", true);
    Output* mod_sustain = createPolyModControl("mod_sustain", true);
    Output* mod_release = createPolyModControl("mod_release", true);

    extra_envelope_ = new Envelope();
    extra_envelope_->plug(mod_attack, Envelope::kAttack);
    extra_envelope_->plug(mod_decay, Envelope::kDecay);
    extra_envelope_->plug(mod_sustain, Envelope::kSustain);
    extra_envelope_->plug(mod_release, Envelope::kRelease);
    extra_envelope_->plug(env_trigger_, Envelope::kTrigger);

    addProcessor(extra_envelope_);
    mod_sources_["mod_envelope"] = extra_envelope_->output();
    mod_sources_["mod_envelope_amp"] = registerOutput(extra_envelope_->output(Envelope::kValue));
    mod_sources_["mod_envelope_phase"] = registerOutput(extra_envelope_->output(Envelope::kPhase));

    // Random Modulation
    TriggerRandom* random_mod = new TriggerRandom();
    random_mod->plug(reset);
    addProcessor(random_mod);
    mod_sources_["random"] = random_mod->output();
  }

  void HelmVoiceHandler::createFilter(
      Output* audio, Output* keytrack, Output* reset) {
    // Filter envelope.
    Output* filter_attack = createPolyModControl("fil_attack", true);
    Output* filter_decay = createPolyModControl("fil_decay", true);
    Output* filter_sustain = createPolyModControl("fil_sustain", true);
    Output* filter_release = createPolyModControl("fil_release", true);

    filter_envelope_ = new Envelope();
    filter_envelope_->plug(filter_attack, Envelope::kAttack);
    filter_envelope_->plug(filter_decay, Envelope::kDecay);
    filter_envelope_->plug(filter_sustain, Envelope::kSustain);
    filter_envelope_->plug(filter_release, Envelope::kRelease);
    filter_envelope_->plug(env_trigger_, Envelope::kTrigger);

    Output* filter_envelope_depth = createPolyModControl("fil_env_depth", true);
    cr::Multiply* scaled_envelope = new cr::Multiply();
    scaled_envelope->plug(filter_envelope_, 0);
    scaled_envelope->plug(filter_envelope_depth, 1);

    addProcessor(filter_envelope_);
    addProcessor(scaled_envelope);

    // Filter.
    Output* keytrack_amount = createPolyModControl("keytrack", true);
    cr::Multiply* current_keytrack = new cr::Multiply();
    current_keytrack->plug(keytrack, 0);
    current_keytrack->plug(keytrack_amount, 1);

    Output* base_cutoff = createPolyModControl("cutoff", true, true);
    cr::Add* keytracked_cutoff = new cr::Add();
    keytracked_cutoff->plug(base_cutoff, 0);
    keytracked_cutoff->plug(current_keytrack, 1);

    cr::Add* midi_cutoff = new cr::Add();
    midi_cutoff->plug(keytracked_cutoff, 0);
    midi_cutoff->plug(scaled_envelope, 1);

    cr::MidiScale* frequency_cutoff = new cr::MidiScale();
    frequency_cutoff->plug(midi_cutoff);

    Output* resonance = createPolyModControl("resonance", true);
    cr::ResonanceScale* scaled_resonance = new cr::ResonanceScale();
    scaled_resonance->plug(resonance);

    static const cr::Value min_db(MIN_GAIN_DB);
    static const cr::Value max_db(MAX_GAIN_DB);
    cr::Interpolate* decibels = new cr::Interpolate();
    decibels->plug(&min_db, cr::Interpolate::kFrom);
    decibels->plug(&max_db, cr::Interpolate::kTo);
    decibels->plug(resonance, cr::Interpolate::kFractional);
    cr::MagnitudeScale* final_gain = new cr::MagnitudeScale();
    final_gain->plug(decibels);

    Value* filter_style = createBaseControl("filter_style");
    Value* filter_shelf = createBaseControl("filter_shelf");
    Value* filter_on = createBaseControl("filter_on");
    Output* filter_drive = createPolyModControl("filter_drive", true);
    Output* filter_blend = createPolyModControl("filter_blend", true);
    cr::MagnitudeScale* drive_magnitude = new cr::MagnitudeScale();
    drive_magnitude->plug(filter_drive);

    /*
    LadderFilter* ladder_filter = new LadderFilter();
    ladder_filter->plug(audio, LadderFilter::kAudio);
    ladder_filter->plug(reset, LadderFilter::kReset);
    ladder_filter->plug(scaled_resonance, LadderFilter::kResonance);
    ladder_filter->plug(frequency_cutoff, LadderFilter::kCutoff);
    ladder_filter->plug(drive_magnitude, LadderFilter::kDrive);
    addProcessor(ladder_filter);
     */

    StateVariableFilter* filter = new StateVariableFilter();
    filter->plug(filter_on, StateVariableFilter::kOn);
    filter->plug(filter_style, StateVariableFilter::kStyle);
    filter->plug(filter_shelf, StateVariableFilter::kShelfChoice);
    filter->plug(audio, StateVariableFilter::kAudio);
    filter->plug(filter_blend, StateVariableFilter::kPassBlend);
    filter->plug(reset, StateVariableFilter::kReset);
    filter->plug(frequency_cutoff, StateVariableFilter::kCutoff);
    filter->plug(scaled_resonance, StateVariableFilter::kResonance);
    filter->plug(final_gain, StateVariableFilter::kGain);
    filter->plug(drive_magnitude, StateVariableFilter::kDrive);

    addProcessor(current_keytrack);
    addProcessor(keytracked_cutoff);
    addProcessor(midi_cutoff);
    addProcessor(scaled_resonance);
    addProcessor(decibels);
    addProcessor(final_gain);
    addProcessor(frequency_cutoff);
    addProcessor(filter);

    addProcessor(drive_magnitude);

    mod_sources_["fil_envelope"] = filter_envelope_->output();
    mod_sources_["fil_envelope_amp"] = registerOutput(filter_envelope_->output(Envelope::kValue));
    mod_sources_["fil_envelope_phase"] = registerOutput(filter_envelope_->output(Envelope::kPhase));

    // Stutter.
    BypassRouter* stutter_container = new BypassRouter();
    addProcessor(stutter_container);

    ValueSwitch* stutter_on = createBaseSwitchControl("stutter_on");
    stutter_container->plug(stutter_on, BypassRouter::kOn);
    stutter_container->plug(filter, BypassRouter::kAudio);

    Stutter* stutter = new Stutter(STUTTER_MAX_SAMPLES);
    Output* stutter_free_frequency = createPolyModControl("stutter_frequency", true);
    Output* stutter_frequency = createTempoSyncSwitch("stutter", stutter_free_frequency->owner,
                                                      beats_per_second_, true, stutter_on);
    Output* resample_free_frequency = createPolyModControl("stutter_resample_frequency", true);
    Output* resample_frequency = createTempoSyncSwitch("stutter_resample",
                                                       resample_free_frequency->owner,
                                                       beats_per_second_, true, stutter_on);

    Output* stutter_softness = createPolyModControl("stutter_softness", true);

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

    ValueSwitch* formant_on = createBaseSwitchControl("formant_on");
    formant_container_->plug(formant_on->output(ValueSwitch::kValue), BypassRouter::kOn);
    formant_container_->plug(stutter_container, BypassRouter::kAudio);

    formant_filter_ = new FormantManager(NUM_FORMANTS);
    formant_filter_->plug(stutter_container, FormantManager::kAudio);
    formant_filter_->plug(reset, FormantManager::kReset);

    Output* formant_x = createPolyModControl("formant_x", true);
    Output* formant_y = createPolyModControl("formant_y", true);

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

      formant_on->addProcessor(formant_gain);
      formant_on->addProcessor(formant_magnitude);
      formant_on->addProcessor(formant_q);
      formant_on->addProcessor(formant_midi);
      formant_on->addProcessor(formant_frequency);
    }

    BilinearInterpolate* formant_decibels = new BilinearInterpolate();
    formant_decibels->setControlRate();
    formant_decibels->plug(&formant_a_decibels, BilinearInterpolate::kTopLeft);
    formant_decibels->plug(&formant_o_decibels, BilinearInterpolate::kTopRight);
    formant_decibels->plug(&formant_i_decibels, BilinearInterpolate::kBottomLeft);
    formant_decibels->plug(&formant_e_decibels, BilinearInterpolate::kBottomRight);
    formant_decibels->plug(formant_x, BilinearInterpolate::kXPosition);
    formant_decibels->plug(formant_y, BilinearInterpolate::kYPosition);

    cr::MagnitudeScale* formant_total_gain = new cr::MagnitudeScale();
    formant_total_gain->plug(formant_decibels);

    LinearSmoothBuffer* formant_gain_smooth = new LinearSmoothBuffer();
    formant_gain_smooth->plug(formant_total_gain);

    Multiply* formant_output = new Multiply();
    formant_output->plug(formant_gain_smooth, 0);
    formant_output->plug(formant_filter_, 1);

    formant_container_->addProcessor(formant_decibels);
    formant_container_->addProcessor(formant_total_gain);
    formant_container_->addProcessor(formant_gain_smooth);
    formant_container_->addProcessor(formant_filter_);
    formant_container_->addProcessor(formant_output);
    formant_container_->registerOutput(formant_output->output());
    formant_on->set(formant_on->value());
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
    Output* amplitude_attack = createPolyModControl("amp_attack", true);
    Output* amplitude_decay = createPolyModControl("amp_decay", true);
    Output* amplitude_sustain = createPolyModControl("amp_sustain", true);
    Output* amplitude_release = createPolyModControl("amp_release", true);

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
    cr::Value* current_velocity = new cr::Value();
    velocity_wait->plug(velocity, TriggerWait::kWait);
    velocity_wait->plug(note_change_trigger, TriggerWait::kTrigger);
    current_velocity->plug(velocity_wait);

    addProcessor(velocity_wait);
    addProcessor(current_velocity);

    Output* velocity_track_amount = createPolyModControl("velocity_track", true);
    cr::Interpolate* velocity_track_mult = new cr::Interpolate();
    velocity_track_mult->plug(&utils::value_one, Interpolate::kFrom);
    velocity_track_mult->plug(current_velocity, Interpolate::kTo);
    velocity_track_mult->plug(velocity_track_amount, Interpolate::kFractional);
    addProcessor(velocity_track_mult);

    // Current amplitude using envelope and velocity.
    Processor* control_amplitude = new cr::Multiply();
    control_amplitude->plug(amplitude_envelope_->output(Envelope::kValue), 0);
    control_amplitude->plug(velocity_track_mult, 1);

    amplitude_ = new LinearSmoothBuffer();
    amplitude_->plug(control_amplitude, LinearSmoothBuffer::kValue);
    amplitude_->plug(amplitude_envelope_->output(Envelope::kFinished),
                     LinearSmoothBuffer::kTrigger);

    addProcessor(control_amplitude);
    addProcessor(amplitude_);

    // Portamento.
    Output* portamento = createPolyModControl("portamento", true);
    Value* portamento_type = createBaseControl("portamento_type");

    current_frequency_ = new PortamentoSlope();
    current_frequency_->plug(current_note, PortamentoSlope::kTarget);
    current_frequency_->plug(portamento_type, PortamentoSlope::kPortamentoType);
    current_frequency_->plug(note_pressed(), PortamentoSlope::kNoteNumber);
    current_frequency_->plug(portamento, PortamentoSlope::kRunSeconds);
    current_frequency_->plug(note_pressed(), PortamentoSlope::kTriggerJump);
    current_frequency_->plug(last_note, PortamentoSlope::kTriggerStart);

    addProcessor(current_frequency_);

    mod_sources_["amp_envelope"] = amplitude_envelope_->output();
    mod_sources_["amp_envelope_amp"] =
        registerOutput(amplitude_envelope_->output(Envelope::kValue));
    mod_sources_["amp_envelope_phase"] =
        registerOutput(amplitude_envelope_->output(Envelope::kPhase));
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

    if (getNumActiveVoices() == 0) {
      for (auto& mod_source : mod_sources_)
        mod_source.second->buffer[0] = 0.0;
    }
  }

  void HelmVoiceHandler::noteOn(mopo_float note, mopo_float velocity, int sample, int channel, mopo_float aftertouch) {
    if (getPressedNotes().size() < polyphony() || legato_->value() == 0.0)
      note_retriggered_.trigger(note, sample);
    VoiceHandler::noteOn(note, velocity, sample, channel, aftertouch);
  }

  VoiceEvent HelmVoiceHandler::noteOff(mopo_float note, int sample) {
    if (getPressedNotes().size() > polyphony() &&
        isNotePlaying(note) &&
        legato_->value() == 0.0)
      note_retriggered_.trigger(note, sample);
    return VoiceHandler::noteOff(note, sample);
  }

  bool HelmVoiceHandler::shouldAccumulate(Output* output) {
    if (output->owner == poly_lfo_ || output->owner == amplitude_envelope_ ||
        output->owner == filter_envelope_ || output->owner == extra_envelope_) {
      return false;
    }
    return VoiceHandler::shouldAccumulate(output);
  }

  void HelmVoiceHandler::setupPolyModulationReadouts() {
    output_map& poly_mods = HelmModule::getPolyModulations();

    for (auto& mod : poly_mods)
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

  output_map& HelmVoiceHandler::getPolyModulations() {
    return poly_readouts_;
  }
} // namespace mopo
