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

#include "helm_common.h"

namespace mopo {

  const ValueDetails ValueDetailsLookup::parameter_list[] = {
    { "amp_attack", 0.0, 4.0, 0, 0.1, 1.0, ValueDetails::kQuadratic, "secs" },
    { "amp_decay", 0.0, 4.0, 0, 1.5, 1.0, ValueDetails::kQuadratic, "secs" },
    { "amp_release", 0.0, 4.0, 0, 0.3, 1.0, ValueDetails::kQuadratic, "secs" },
    { "amp_sustain", 0.0, 1.0, 0, 1.0, 1.0, ValueDetails::kLinear, "" },
    { "arp_frequency", -1.0, 4.0, 0, 2.0, 1.0, ValueDetails::kExponential, "Hz" },
    { "arp_gate", 0.0, 1.0, 0, 0.5, 1.0, ValueDetails::kLinear, "" },
    { "arp_octaves", 1.0, 4.0, 4, 1.0, 1.0, ValueDetails::kLinear, "octaves" },
    { "arp_on", 0.0, 1.0, 2, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "arp_pattern", 0.0, 4.0, 5, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "arp_sync", 0.0, 3.0, 4, 1.0, 1.0, ValueDetails::kLinear, "" },
    { "arp_tempo", 0.0, 11.0, 12, 9.0, 1.0, ValueDetails::kLinear, "" },
    { "beats_per_minute", 20.0, 300.0, 0, 120.0, 1.0, ValueDetails::kLinear, "bpm" },
    { "cross_modulation", 0.0, 0.5, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "cutoff", 28.0, 127.0, 0, 80.0, 1.0, ValueDetails::kLinear, "" },
    { "delay_dry_wet", 0.0, 1.0, 0, 0.5, 1.0, ValueDetails::kLinear, "" },
    { "delay_feedback", -1.0, 1.0, 0, 0.4, 100.0, ValueDetails::kLinear, "%" },
    { "delay_frequency", -2.0, 5.0, 0, 2.0, 1.0, ValueDetails::kExponential, "Hz" },
    { "delay_on", 0.0, 1.0, 2, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "delay_sync", 0.0, 3.0, 4, 1.0, 1.0, ValueDetails::kLinear, "" },
    { "delay_tempo", 0.0, 11.0, 12, 9.0, 1.0, ValueDetails::kLinear, "" },
    { "fil_attack", 0.0, 4.0, 0, 0.1, 1.0, ValueDetails::kQuadratic, "secs" },
    { "fil_decay", 0.0, 4.0, 0, 1.5, 1.0, ValueDetails::kQuadratic, "secs" },
    { "fil_env_depth", -128.0, 128.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "fil_release", 0.0, 4.0, 0, 1.5, 1.0, ValueDetails::kQuadratic, "secs" },
    { "fil_sustain", 0.0, 1.0, 0, 0.5, 1.0, ValueDetails::kLinear, "" },
    { "filter_saturation", -60.0, 60.0, 0, 0.0, 1.0, ValueDetails::kLinear, "dB" },
    { "filter_type", 0.0, 6.0, 7, 6.0, 1.0, ValueDetails::kLinear, "" },
    { "formant_on", 0.0, 1.0, 2, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "formant_x", 0.0, 1.0, 0, 0.5, 1.0, ValueDetails::kLinear, "" },
    { "formant_y", 0.0, 1.0, 0, 0.5, 1.0, ValueDetails::kLinear, "" },
    { "keytrack", -1.0, 1.0, 0, 0.0, 100.0, ValueDetails::kLinear, "%" },
    { "legato", 0.0, 1.0, 2, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "mono_lfo_1_amplitude", -1.0, 1.0, 0, 1.0, 1.0, ValueDetails::kLinear, "" },
    { "mono_lfo_1_frequency", -7.0, 6.0, 0, 1.0, 1.0, ValueDetails::kExponential, "Hz" },
    { "mono_lfo_1_sync", 0.0, 3.0, 4, 1.0, 1.0, ValueDetails::kLinear, "" },
    { "mono_lfo_1_tempo", 0.0, 11.0, 12, 6.0, 1.0, ValueDetails::kLinear, "" },
    { "mono_lfo_1_waveform", 0.0, 11.0, 12, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "mono_lfo_2_amplitude", -1.0, 1.0, 0, 1.0, 1.0, ValueDetails::kLinear, "" },
    { "mono_lfo_2_frequency", -7.0, 6.0, 0, 1.0, 1.0, ValueDetails::kExponential, "Hz" },
    { "mono_lfo_2_sync", 0.0, 3.0, 4, 1.0, 1.0, ValueDetails::kLinear, "" },
    { "mono_lfo_2_tempo", 0.0, 11.0, 12, 7.0, 1.0, ValueDetails::kLinear, "" },
    { "mono_lfo_2_waveform", 0.0, 11.0, 12, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "noise_volume", 0.0, 1.0, 0, 0.0, 1.0, ValueDetails::kQuadratic, "" },
    { "num_steps", 1.0, 32.0, 32, 8.0, 1.0, ValueDetails::kLinear, "" },
    { "osc_1_transpose", -48.0, 48.0, 97, 0.0, 1.0, ValueDetails::kLinear, "semitones" },
    { "osc_1_tune", -1.0, 1.0, 0, 0.0, 100.0, ValueDetails::kLinear, "cents" },
    { "osc_1_unison_detune", 0.0, 100.0, 0, 0.0, 1.0, ValueDetails::kLinear, "cents" },
    { "osc_1_unison_voices", 1.0, 15.0, 8, 1.0, 1.0, ValueDetails::kLinear, "v" },
    { "osc_1_waveform", 0.0, 10.0, 11, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "osc_2_transpose", -48.0, 48.0, 97, 0.0, 1.0, ValueDetails::kLinear, "semitones" },
    { "osc_2_tune", -1.0, 1.0, 0, 0.0, 100.0, ValueDetails::kLinear, "cents" },
    { "osc_2_unison_detune", 0.0, 100.0, 0, 0.0, 1.0, ValueDetails::kLinear, "cents" },
    { "osc_2_unison_voices", 1.0, 15.0, 8, 1.0, 1.0, ValueDetails::kLinear, "v" },
    { "osc_2_waveform", 0.0, 10.0, 11, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "osc_feedback_amount", -1.0, 1.0, 0, 0.0, 100.0, ValueDetails::kLinear, "%" },
    { "osc_feedback_transpose", -24.0, 24.0, 49, 0.0, 1.0, ValueDetails::kLinear, "semitones" },
    { "osc_feedback_tune", -1.0, 1.0, 0, 0.0, 100.0, ValueDetails::kLinear, "cents" },
    { "osc_mix", 0.0, 1.0, 0, 0.5, 1.0, ValueDetails::kLinear, "" },
    { "pitch_bend_range", 0.0, 24.0, 25, 2.0, 1.0, ValueDetails::kLinear, "semitones" },
    { "poly_lfo_amplitude", -1.0, 1.0, 0, 1.0, 1.0, ValueDetails::kLinear, "" },
    { "poly_lfo_frequency", -7.0, 6.0, 0, 1.0, 1.0, ValueDetails::kExponential, "Hz" },
    { "poly_lfo_sync", 0.0, 3.0, 4, 1.0, 1.0, ValueDetails::kLinear, "" },
    { "poly_lfo_tempo", 0.0, 11.0, 12, 7.0, 1.0, ValueDetails::kLinear, "" },
    { "poly_lfo_waveform", 0.0, 11.0, 12, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "polyphony", 1.0, 32.0, 32, 1.0, 1.0, ValueDetails::kLinear, "voices" },
    { "portamento", -9.0, -1.0, 0, -9.0, 1.0, ValueDetails::kExponential, "s/oct" },
    { "portamento_type", 0.0, 2.0, 3, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "resonance", 0.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_frequency", -5.0, 6.0, 0, 2.0, 1.0, ValueDetails::kExponential, "Hz" },
    { "reverb_damping", 0.0, 1.0, 0, 0.5, 1.0, ValueDetails::kLinear, "" },
    { "reverb_dry_wet", 0.0, 1.0, 0, 0.5, 1.0, ValueDetails::kLinear, "" },
    { "reverb_feedback", 0.8, 1.0, 0, 0.9, 100.0, ValueDetails::kLinear, "%" },
    { "reverb_on", 0.0, 1.0, 2, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_00", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_01", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_02", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_03", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_04", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_05", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_06", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_07", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_08", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_09", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_10", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_11", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_12", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_13", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_14", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_15", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_16", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_17", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_18", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_19", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_20", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_21", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_22", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_23", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_24", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_25", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_26", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_27", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_28", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_29", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_30", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_seq_31", -1.0, 1.0, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "step_sequencer_sync", 0.0, 3.0, 4, 1.0, 1.0, ValueDetails::kLinear, "" },
    { "step_sequencer_tempo", 0.0, 11.0, 12, 7.0, 1.0, ValueDetails::kLinear, "" },
    { "step_smoothing", 0.0, 0.5, 0, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "stutter_frequency", -7.0, 6.0, 0, 3.0, 1.0, ValueDetails::kExponential, "Hz" },
    { "stutter_on", 0.0, 1.0, 2, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "stutter_resample_frequency", -7.0, 6.0, 0, 1.0, 1.0, ValueDetails::kExponential, "Hz" },
    { "sub_shuffle", 0.0, 1.0, 0, 0.0, 100.0, ValueDetails::kLinear, "%" },
    { "sub_transpose", -12.0, 12.0, 25, 0.0, 1.0, ValueDetails::kLinear, "semitones" },
    { "sub_tune", -1.0, 1.0, 0, 0.0, 100.0, ValueDetails::kLinear, "cents" },
    { "sub_volume", 0.0, 1.0, 0, 0.0, 1.0, ValueDetails::kQuadratic, "" },
    { "sub_waveform", 0.0, 10.0, 11, 2.0, 1.0, ValueDetails::kLinear, "" },
    { "unison_1_harmonize", 0.0, 1.0, 2, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "unison_2_harmonize", 0.0, 1.0, 2, 0.0, 1.0, ValueDetails::kLinear, "" },
    { "velocity_track", -1.0, 1.0, 0, 0.0, 100.0, ValueDetails::kLinear, "%" },
    { "volume", 0.0, 1.0, 0, 0.75, 1.0, ValueDetails::kQuadratic, "" },
  };

  ValueDetailsLookup::ValueDetailsLookup() {
    int num_parameters = sizeof(parameter_list) / sizeof(ValueDetails);
    for (int i = 0; i < num_parameters; ++i) {
      details_lookup_[parameter_list[i].name] = parameter_list[i];

      MOPO_ASSERT(parameter_list[i].default_value <= parameter_list[i].max);
      MOPO_ASSERT(parameter_list[i].default_value >= parameter_list[i].min);
    }
  }

  ValueDetailsLookup Parameters::lookup_;

} // namespace mopo
