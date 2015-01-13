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

#include "delay.h"
#include "envelope.h"
#include "filter.h"
#include "operators.h"
#include "oscillator.h"
#include "processor_router.h"
#include "linear_slope.h"
#include "smooth_value.h"
#include "twytch_strings.h"
#include "trigger_operators.h"
#include "value.h"

#include <sstream>

#define PITCH_MOD_RANGE 12

namespace mopo {

  TwytchOscillators::TwytchOscillators() : TickRouter(0, 0) {
    oscillator1_ = new Oscillator();
    oscillator2_ = new Oscillator();
    frequency1_ = new Multiply();
    frequency2_ = new Multiply();
    freq_mod1_ = new Multiply();
    freq_mod2_ = new Multiply();
    normalized_fm1_ = new Add();
    normalized_fm2_ = new Add();
    Value* one = new Value(1);

    addProcessor(normalized_fm1_);
    addProcessor(normalized_fm2_);
    addProcessor(frequency1_);
    addProcessor(frequency2_);
    addProcessor(freq_mod1_);
    addProcessor(freq_mod2_);
    addProcessor(oscillator1_);
    addProcessor(oscillator2_);

    registerInput(oscillator1_->input(Oscillator::kWaveform), kOscillator1Waveform);
    registerInput(oscillator2_->input(Oscillator::kWaveform), kOscillator2Waveform);
    registerInput(oscillator1_->input(Oscillator::kReset), kOscillator1Reset);
    registerInput(oscillator2_->input(Oscillator::kReset), kOscillator2Reset);
    registerInput(frequency1_->input(0), kOscillator1BaseFrequency);
    registerInput(frequency2_->input(0), kOscillator2BaseFrequency);
    registerInput(freq_mod1_->input(0), kOscillator1FM);
    registerInput(freq_mod2_->input(0), kOscillator2FM);

    normalized_fm1_->plug(freq_mod1_, 0);
    normalized_fm2_->plug(freq_mod2_, 0);
    normalized_fm1_->plug(one, 1);
    normalized_fm2_->plug(one, 1);
    frequency1_->plug(normalized_fm1_, 1);
    frequency2_->plug(normalized_fm2_, 1);
    oscillator1_->plug(frequency1_, Oscillator::kFrequency);
    oscillator2_->plug(frequency2_, Oscillator::kFrequency);
    freq_mod2_->plug(oscillator1_, 1);
    freq_mod1_->plug(oscillator2_, 1);

    registerOutput(oscillator1_->output());
    registerOutput(oscillator2_->output());
  }

  void TwytchOscillators::process() {
    int num_feedbacks = feedback_order_->size();
    for (int i = 0; i < num_feedbacks; ++i)
      feedback_processors_[feedback_order_->at(i)]->tickBeginRefreshOutput();

    oscillator1_->preprocess();
    oscillator2_->preprocess();
    tick(0);

    for (int i = 1; i < buffer_size_; ++i) {
      for (int f = 0; f < num_feedbacks; ++f)
        feedback_processors_[feedback_order_->at(f)]->tickRefreshOutput(i);

      tick(i);

      for (int f = 0; f < num_feedbacks; ++f)
        feedback_processors_[feedback_order_->at(f)]->tick(i);
    }
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
    VariableAdd* midi_mod_sources = new VariableAdd(MOD_MATRIX_SIZE);
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
    oscillators_->plug(reset, TwytchOscillators::kOscillator1Waveform);
    oscillators_->plug(reset, TwytchOscillators::kOscillator2Waveform);
    oscillators_->plug(oscillator1_frequency, TwytchOscillators::kOscillator1BaseFrequency);

    Value* cross_mod = new Value(0.15);
    VariableAdd* cross_mod_mod_sources = new VariableAdd(MOD_MATRIX_SIZE);
    Add* cross_mod_total = new Add();
    cross_mod_total->plug(cross_mod, 0);
    cross_mod_total->plug(cross_mod_mod_sources, 1);

    oscillators_->plug(cross_mod_total, TwytchOscillators::kOscillator1FM);
    oscillators_->plug(cross_mod_total, TwytchOscillators::kOscillator2FM);

    addProcessor(cross_mod_mod_sources);
    addProcessor(cross_mod_total);
    addProcessor(oscillator1_frequency);
    addProcessor(oscillators_);

    controls_["cross modulation"] = cross_mod;

    std::vector<std::string> wave_strings = std::vector<std::string>(
        TwytchStrings::wave_strings_,
        TwytchStrings::wave_strings_ + Wave::kNumWaveforms);
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
    VariableAdd* mix_mod_sources = new VariableAdd(MOD_MATRIX_SIZE);
    Add* mix_total = new Add();
    mix_total->plug(oscillator_mix_amount, 0);
    mix_total->plug(mix_mod_sources, 1);

    Clamp* clamp_mix = new Clamp(0, 1);
    clamp_mix->plug(mix_total);
    oscillator_mix_ = new Interpolate();
    oscillator_mix_->plug(oscillators_->output(0), Interpolate::kFrom);
    oscillator_mix_->plug(oscillators_->output(1), Interpolate::kTo);
    oscillator_mix_->plug(clamp_mix, Interpolate::kFractional);

    addProcessor(oscillator_mix_);
    addProcessor(mix_mod_sources);
    addProcessor(mix_total);
    addProcessor(clamp_mix);
    controls_["osc mix"] = oscillator_mix_amount;

    // LFO 1.
    Value* lfo1_waveform = new Value(Wave::kSin);
    Value* lfo1_frequency = new Value(2);
    lfo1_ = new Oscillator();
    lfo1_->plug(reset, Oscillator::kReset);
    lfo1_->plug(lfo1_waveform, Oscillator::kWaveform);
    lfo1_->plug(lfo1_frequency, Oscillator::kFrequency);

    addProcessor(lfo1_);
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

    // Modulation sources/destinations.
    mod_sources_["lfo 1"] = lfo1_->output();
    mod_sources_["lfo 2"] = lfo2_->output();

    mod_destinations_["cross modulation"] = cross_mod_mod_sources;
    mod_destinations_["pitch"] = midi_mod_sources;
    mod_destinations_["osc mix"] = mix_mod_sources;
  }

  void TwytchVoiceHandler::createFilter(
      Output* audio, Output* keytrack, Output* reset) {
    // Filter envelope.
    Value* filter_attack = new Value(0.0);
    Value* filter_decay = new Value(0.3);
    Value* filter_sustain = new Value(0.0);
    Value* filter_release = new Value(0.3);

    filter_envelope_ = new Envelope();
    filter_envelope_->plug(filter_attack, Envelope::kAttack);
    filter_envelope_->plug(filter_decay, Envelope::kDecay);
    filter_envelope_->plug(filter_sustain, Envelope::kSustain);
    filter_envelope_->plug(filter_release, Envelope::kRelease);
    filter_envelope_->plug(reset, Envelope::kTrigger);

    Value* filter_envelope_depth = new Value(12);
    Multiply* scaled_envelope = new Multiply();
    scaled_envelope->plug(filter_envelope_, 0);
    scaled_envelope->plug(filter_envelope_depth, 1);

    addProcessor(filter_envelope_);
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

    SmoothValue* base_cutoff = new SmoothValue(92);
    Add* keytracked_cutoff = new Add();
    keytracked_cutoff->plug(base_cutoff, 0);
    keytracked_cutoff->plug(current_keytrack, 1);

    Add* midi_cutoff = new Add();
    midi_cutoff->plug(keytracked_cutoff, 0);
    midi_cutoff->plug(scaled_envelope, 1);

    VariableAdd* cutoff_mod_sources = new VariableAdd(MOD_MATRIX_SIZE);
    Value* cutoff_mod_scale = new Value(MIDI_SIZE / 2);
    Multiply* cutoff_modulation_scaled = new Multiply();
    cutoff_modulation_scaled->plug(cutoff_mod_sources, 0);
    cutoff_modulation_scaled->plug(cutoff_mod_scale, 1);
    Add* midi_cutoff_modulated = new Add();
    midi_cutoff_modulated->plug(midi_cutoff, 0);
    midi_cutoff_modulated->plug(cutoff_modulation_scaled, 1);

    MidiScale* frequency_cutoff = new MidiScale();
    frequency_cutoff->plug(midi_cutoff_modulated);

    Value* resonance = new Value(3);

    VariableAdd* resonance_mod_sources = new VariableAdd(MOD_MATRIX_SIZE);
    Value* resonance_mod_scale = new Value(8);
    Multiply* resonance_modulation_scaled = new Multiply();
    resonance_modulation_scaled->plug(resonance_mod_sources, 0);
    resonance_modulation_scaled->plug(resonance_mod_scale, 1);
    Add* resonance_modulated = new Add();
    resonance_modulated->plug(resonance, 0);
    resonance_modulated->plug(resonance_modulation_scaled, 1);
    Clamp* clamp_resonance = new Clamp(0.5, 30);
    clamp_resonance->plug(resonance_modulated);

    filter_ = new Filter();
    filter_->plug(audio, Filter::kAudio);
    filter_->plug(filter_type, Filter::kType);
    filter_->plug(reset, Filter::kReset);
    filter_->plug(frequency_cutoff, Filter::kCutoff);
    filter_->plug(clamp_resonance, Filter::kResonance);

    addGlobalProcessor(base_cutoff);
    addProcessor(current_keytrack);
    addProcessor(keytracked_cutoff);
    addProcessor(midi_cutoff);
    addProcessor(cutoff_mod_sources);
    addProcessor(cutoff_modulation_scaled);
    addProcessor(midi_cutoff_modulated);
    addProcessor(resonance_mod_sources);
    addProcessor(resonance_modulation_scaled);
    addProcessor(resonance_modulated);
    addProcessor(clamp_resonance);
    addProcessor(frequency_cutoff);
    addProcessor(filter_);

    std::vector<std::string> filter_strings = std::vector<std::string>(
        TwytchStrings::filter_strings_,
        TwytchStrings::filter_strings_ + Filter::kNumTypes);
    controls_["filter type"] = filter_type;
    controls_["cutoff"] = base_cutoff;
    controls_["keytrack"] = keytrack_amount;
    controls_["resonance"] = resonance;

    mod_sources_["filter env"] = filter_envelope_->output();
    mod_destinations_["cutoff"] = cutoff_mod_sources;
    mod_destinations_["resonance"] = resonance_mod_sources;
  }

  void TwytchVoiceHandler::createModMatrix() {
    std::vector<std::string> source_names;
    source_names.push_back("");
    output_map::iterator s_iter = mod_sources_.begin();
    for (; s_iter != mod_sources_.end(); ++s_iter)
      source_names.push_back(s_iter->first);

    std::vector<std::string> destination_names;
    destination_names.push_back("");
    input_map::iterator d_iter = mod_destinations_.begin();
    for (; d_iter != mod_destinations_.end(); ++d_iter)
      destination_names.push_back(d_iter->first);

    for (int i = 0; i < MOD_MATRIX_SIZE; ++i) {
      mod_matrix_scales_[i] = new Value(0.0);
      mod_matrix_[i] = new Multiply();
      mod_matrix_[i]->plug(mod_matrix_scales_[i], 1);

      addGlobalProcessor(mod_matrix_scales_[i]);
      addProcessor(mod_matrix_[i]);

      MatrixSourceValue* source_value = new MatrixSourceValue(this);
      source_value->setSources(source_names);
      source_value->setModulationIndex(i);

      MatrixDestinationValue* destination_value =
          new MatrixDestinationValue(this);
      destination_value->setDestinations(destination_names);
      destination_value->setModulationIndex(i);

      std::stringstream scale_name;
      scale_name << "mod scale " << i + 1;
      controls_[scale_name.str()] = mod_matrix_scales_[i];

      std::stringstream source_name;
      source_name << "mod source " << i + 1;
      controls_[source_name.str()] = source_value;

      std::stringstream destination_name;
      destination_name << "mod destination " << i + 1;
      controls_[destination_name.str()] = destination_value;
    }
  }

  void TwytchVoiceHandler::createArticulation(
      Output* note, Output* velocity, Output* trigger) {
    // Legato.
    Value* legato = new Value(0);
    LegatoFilter* legato_filter = new LegatoFilter();
    legato_filter->plug(legato, LegatoFilter::kLegato);
    legato_filter->plug(trigger, LegatoFilter::kTrigger);

    std::vector<std::string> legato_strings = std::vector<std::string>(
        TwytchStrings::legato_strings_,
        TwytchStrings::legato_strings_ + 2);
    controls_["legato"] = legato;
    addProcessor(legato_filter);

    // Amplitude envelope.
    Value* amplitude_attack = new Value(0.01);
    Value* amplitude_decay = new Value(0.7);
    Value* amplitude_sustain = new SmoothValue(0.0);
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
    TriggerCombiner* frequency_trigger = new TriggerCombiner();
    frequency_trigger->plug(legato_filter->output(LegatoFilter::kRemain), 0);
    frequency_trigger->plug(
        amplitude_envelope_->output(Envelope::kFinished), 1);

    TriggerWait* note_wait = new TriggerWait();
    Value* current_note = new Value();
    note_wait->plug(note, TriggerWait::kWait);
    note_wait->plug(frequency_trigger, TriggerWait::kTrigger);
    current_note->plug(note_wait);

    Value* max_midi_invert = new Value(1.0 / (MIDI_SIZE - 1));
    Multiply* note_percentage = new Multiply();
    note_percentage->plug(max_midi_invert, 0);
    note_percentage->plug(current_note, 1);

    addProcessor(frequency_trigger);
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
    velocity_wait->plug(frequency_trigger, TriggerWait::kTrigger);
    current_velocity->plug(velocity_wait);

    addProcessor(velocity_wait);
    addProcessor(current_velocity);

    Value* velocity_track_amount = new Value(0.3);
    Value* one = new Value(1.0);

    Interpolate* velocity_track_mult = new Interpolate();
    velocity_track_mult->plug(one, Interpolate::kFrom);
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
    portamento_filter->plug(frequency_trigger, PortamentoFilter::kTrigger);
    addProcessor(portamento_filter);

    current_frequency_ = new LinearSlope();
    current_frequency_->plug(current_note, LinearSlope::kTarget);
    current_frequency_->plug(portamento, LinearSlope::kRunSeconds);
    current_frequency_->plug(portamento_filter, LinearSlope::kTriggerJump);

    addProcessor(current_frequency_);
    controls_["portamento"] = portamento;
    std::vector<std::string> portamento_strings = std::vector<std::string>(
        TwytchStrings::portamento_strings_,
        TwytchStrings::portamento_strings_ +
        PortamentoFilter::kNumPortamentoStates);
    controls_["portamento type"] = portamento_type;

    mod_sources_["amp env"] = amplitude_envelope_->output();
    mod_sources_["note"] = note_percentage->output();
    mod_sources_["velocity"] = current_velocity->output();
  }

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
    createFilter(oscillator_mix_->output(), note_from_center_->output(),
                 amplitude_envelope_->output(Envelope::kFinished));
    createModMatrix();

    output_ = new Multiply();
    output_->plug(filter_->output(), 0);
    output_->plug(amplitude_, 1);

    addProcessor(output_);
    addGlobalProcessor(pitch_wheel_amount_);
    addGlobalProcessor(mod_wheel_amount_);

    setVoiceOutput(output_);
    setVoiceKiller(amplitude_envelope_->output(Envelope::kValue));
  }

  TwytchEngine::TwytchEngine() {
    // Voice Handler.
    Value* polyphony = new Value(1);
    voice_handler_ = new TwytchVoiceHandler();
    voice_handler_->setPolyphony(64);
    voice_handler_->plug(polyphony, VoiceHandler::kPolyphony);

    addProcessor(voice_handler_);
    controls_["polyphony"] = polyphony;

    // Delay effect.
    SmoothValue* delay_time = new SmoothValue(0.06);
    SmoothValue* delay_feedback = new SmoothValue(-0.3);
    SmoothValue* delay_wet = new SmoothValue(0.3);

    Delay* delay = new Delay();
    delay->plug(voice_handler_, Delay::kAudio);
    delay->plug(delay_time, Delay::kDelayTime);
    delay->plug(delay_feedback, Delay::kFeedback);
    delay->plug(delay_wet, Delay::kWet);

    addProcessor(delay_time);
    addProcessor(delay_feedback);
    addProcessor(delay_wet);
    addProcessor(delay);

    controls_["delay time"] = delay_time;
    controls_["delay feedback"] = delay_feedback;
    controls_["delay dry/wet"] = delay_wet;

    // Volume.
    SmoothValue* volume = new SmoothValue(0.6);
    Multiply* scaled_audio = new Multiply();
    scaled_audio->plug(delay, 0);
    scaled_audio->plug(volume, 1);
    Clamp* clamp = new Clamp();
    clamp->plug(scaled_audio);

    addProcessor(clamp);
    addProcessor(volume);
    addProcessor(scaled_audio);
    registerOutput(clamp->output());

    controls_["volume"] = volume;
  }

  control_map TwytchEngine::getControls() {
    control_map voice_controls = voice_handler_->getControls();
    voice_controls.insert(controls_.begin(), controls_.end());
    return voice_controls;
  }

  void TwytchEngine::noteOn(mopo_float note, mopo_float velocity) {
    voice_handler_->noteOn(note, velocity);
  }

  void TwytchEngine::noteOff(mopo_float note) {
    voice_handler_->noteOff(note);
  }

  void TwytchVoiceHandler::setModWheel(mopo_float value) {
    mod_wheel_amount_->set(value);
  }

  void TwytchVoiceHandler::setPitchWheel(mopo_float value) {
    pitch_wheel_amount_->set(value);
  }

  void TwytchVoiceHandler::setModulationSource(int matrix_index,
                                                 std::string source) {
    mod_matrix_[matrix_index]->unplugIndex(0);
    if (source.length())
      mod_matrix_[matrix_index]->plug(mod_sources_[source], 0);
  }

  void TwytchVoiceHandler::setModulationDestination(
      int matrix_index, std::string destination) {
    std::string current = current_mod_destinations_[matrix_index];

    // First unplug the previous destination.
    if (current.length())
      mod_destinations_[current]->unplug(mod_matrix_[matrix_index]);

    // Then plug in to the new destination.
    current_mod_destinations_[matrix_index] = destination;
    if (destination.length())
      mod_destinations_[destination]->plugNext(mod_matrix_[matrix_index]);
  }
} // namespace mopo
