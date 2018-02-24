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

#include "helm_common.h"

namespace mopo {

  const ValueDetails ValueDetailsLookup::parameter_list[] = {
    { "amp_attack", 0.0, 4.0, 0, 0.109545, 0.0, 1.0,
      ValueDetails::kQuadratic, false, "secs", "Amp Attack" },
    { "amp_decay", 0.0, 4.0, 0, 1.5, 0.0, 1.0,
      ValueDetails::kQuadratic, false, "secs", "Amp Decay" },
    { "amp_release", 0.0, 4.0, 0, 0.3, 0.0, 1.0,
      ValueDetails::kQuadratic, false, "secs", "Amp Release" },
    { "amp_sustain", 0.0, 1.0, 0, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Amp Sustain" },
    { "arp_frequency", -1.0, 4.0, 0, 2.0, 0.0, 1.0,
      ValueDetails::kExponential, true, "secs", "Arp Frequency" },
    { "arp_gate", 0.0, 1.0, 0, 0.5, 0.0, 100.0,
      ValueDetails::kLinear, false, "%", "Arp Gate" },
    { "arp_octaves", 1.0, 4.0, 4, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "octaves", "Arp Octaves" },
    { "arp_on", 0.0, 1.0, 2, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Arp Switch" },
    { "arp_pattern", 0.0, 4.0, 5, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Arp Pattern"},
    { "arp_sync", 0.0, 3.0, 4, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Arp Sync" },
    { "arp_tempo", 0.0, 11.0, 12, 9.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Arp Tempo" },
    { "beats_per_minute", 0.333333333, 5.0, 0, 2.0, 0.0, 60.0,
      ValueDetails::kLinear, false, "", "Beats Per Minute" },
    { "cross_modulation", 0.0, 0.5, 0, 0.0, 0.0, 200.0,
      ValueDetails::kLinear, false, "%", "Cross Mod" },
    { "cutoff", 28.0, 127.0, 0, 80.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "semitones", "Filter Cutoff" },
    { "delay_dry_wet", 0.0, 1.0, 0, 0.5, 0.0, 100.0,
      ValueDetails::kLinear, false, "%", "Delay Mix" },
    { "delay_feedback", -1.0, 1.0, 0, 0.4, 0.0, 100.0,
      ValueDetails::kLinear, false, "%", "Delay Feedback" },
    { "delay_frequency", -2.0, 5.0, 0, 2.0, 0.0, 1.0,
      ValueDetails::kExponential, true, "secs", "Delay Frequency" },
    { "delay_on", 0.0, 1.0, 2, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Delay Switch" },
    { "delay_sync", 0.0, 3.0, 4, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Delay Sync" },
    { "delay_tempo", 0.0, 11.0, 12, 9.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Delay Tempo" },
    { "distortion_on", 0.0, 1.0, 2, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Distortion Switch" },
    { "distortion_type", 0.0, 3.0, 4, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Distortion Type" },
    { "distortion_drive", -30.0, 30.0, 0, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "dB", "Distortion Drive" },
    { "distortion_mix", 0.0, 1.0, 0, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Distortion Mix" },
    { "fil_attack", 0.0, 4.0, 0, 0.0, 0.0, 1.0,
      ValueDetails::kQuadratic, false, "secs", "Filter Attack" },
    { "fil_decay", 0.0, 4.0, 0, 1.5, 0.0, 1.0,
      ValueDetails::kQuadratic, false, "secs", "Filter Decay" },
    { "fil_env_depth", -128.0, 128.0, 0, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "semitones", "Filter Env Depth" },
    { "fil_release", 0.0, 4.0, 0, 1.5, 0.0, 1.0,
      ValueDetails::kQuadratic, false, "secs", "Filter Release" },
    { "fil_sustain", 0.0, 1.0, 0, 0.5, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Filter Sustain" },
    { "filter_saturation", -60.0, 60.0, 0, 0.0, 0.0, 1.0, // DEPRECATED
      ValueDetails::kLinear, false, "dB", "Saturation" },
    { "filter_drive", -12, 20.0, 0, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "dB", "Filter Drive" },
    { "filter_blend", 0.0, 2.0, 0, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Filter Blend" },
    { "filter_type", 0.0, 6.0, 7, 6.0, 0.0, 1.0, // DEPRECATED
      ValueDetails::kLinear, false, "", "Filter Type" },
    { "filter_style", 0.0, 2.0, 3, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Filter Style" },
    { "filter_shelf", 0.0, 2.0, 3, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Filter Shelf" },
    { "filter_on", 0.0, 1.0, 2, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Filter Switch" },
    { "formant_on", 0.0, 1.0, 2, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Formant Switch" },
    { "formant_x", 0.0, 1.0, 0, 0.5, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Formant X" },
    { "formant_y", 0.0, 1.0, 0, 0.5, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Formant Y" },
    { "keytrack", -1.0, 1.0, 0, 0.0, 0.0, 100.0,
      ValueDetails::kLinear, false, "%", "Filter Key Track" },
    { "legato", 0.0, 1.0, 2, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Legato" },
    { "mod_attack", 0.0, 4.0, 0, 0.0, 0.0, 1.0,
      ValueDetails::kQuadratic, false, "secs", "Mod Env Attack" },
    { "mod_decay", 0.0, 4.0, 0, 1.5, 0.0, 1.0,
      ValueDetails::kQuadratic, false, "secs", "Mod Env Decay" },
    { "mod_release", 0.0, 4.0, 0, 1.5, 0.0, 1.0,
      ValueDetails::kQuadratic, false, "secs", "Mod Env Release" },
    { "mod_sustain", 0.0, 1.0, 0, 0.5, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Mod Env Sustain" },
    { "mono_lfo_1_amplitude", -1.0, 1.0, 0, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Mono LFO 1 Amp" },
    { "mono_lfo_1_frequency", -7.0, 6.0, 0, 1.0, 0.0, 1.0,
      ValueDetails::kExponential, true, "secs", "Mono LFO 1 Frequency" },
    { "mono_lfo_1_retrigger", 0.0, 2.0, 3, 2.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Mono LFO 1 Retrigger" },
    { "mono_lfo_1_sync", 0.0, 3.0, 4, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Mono LFO 1 Sync" },
    { "mono_lfo_1_tempo", 0.0, 11.0, 12, 6.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Mono LFO 1 Tempo" },
    { "mono_lfo_1_waveform", 0.0, 12.0, 13, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Mono LFO 1 Waveform" },
    { "mono_lfo_2_amplitude", -1.0, 1.0, 0, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Mono LFO 2 Amp" },
    { "mono_lfo_2_frequency", -7.0, 6.0, 0, 1.0, 0.0, 1.0,
      ValueDetails::kExponential, true, "secs", "Mono LFO 2 Frequency" },
    { "mono_lfo_2_retrigger", 0.0, 2.0, 3, 2.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Mono LFO 2 Retrigger" },
    { "mono_lfo_2_sync", 0.0, 3.0, 4, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Mono LFO 2 Sync" },
    { "mono_lfo_2_tempo", 0.0, 11.0, 12, 7.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Mono LFO 2 Tempo" },
    { "mono_lfo_2_waveform", 0.0, 12.0, 13, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Mono LFO 2 Waveform" },
    { "noise_volume", 0.0, 1.0, 0, 0.0, 0.0, 1.0,
      ValueDetails::kQuadratic, false, "", "Noise Volume" },
    { "num_steps", 1.0, 32.0, 32, 8.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Num Steps" },
    { "osc_1_transpose", -48.0, 48.0, 97, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "semitones", "Osc 1 Transpose" },
    { "osc_1_tune", -1.0, 1.0, 0, 0.0, 0.0, 100.0,
      ValueDetails::kLinear, false, "cents", "Osc 1 Tune" },
    { "osc_1_unison_detune", 0.0, 100.0, 0, 10.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "cents", "Osc 1 Unison Detune" },
    { "osc_1_unison_voices", 1.0, 15.0, 8, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "v", "Osc 1 Unison Voices" },
    { "osc_1_volume", 0.0, 1.0, 0, 0.5477225575, 0.0, 1.0,
      ValueDetails::kQuadratic, false, "", "Osc 1 Volume" },
    { "osc_1_waveform", 0.0, 10.0, 11, 4.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Osc 1 Waveform" },
    { "osc_2_transpose", -48.0, 48.0, 97, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "semitones", "Osc 2 Transpose" },
    { "osc_2_tune", -1.0, 1.0, 0, 0.0, 0.0, 100.0,
      ValueDetails::kLinear, false, "cents", "Osc 2 Tune" },
    { "osc_2_unison_detune", 0.0, 100.0, 0, 10.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "cents", "Osc 2 Unison Detune" },
    { "osc_2_unison_voices", 1.0, 15.0, 8, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "v", "Osc 2 Unison Voices" },
    { "osc_2_volume", 0.0, 1.0, 0, 0.5477225575, 0.0, 1.0,
      ValueDetails::kQuadratic, false, "", "Osc 2 Volume" },
    { "osc_2_waveform", 0.0, 10.0, 11, 4.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Osc 2 Waveform" },
    { "osc_feedback_amount", -1.0, 1.0, 0, 0.0, 0.0, 100.0,
      ValueDetails::kLinear, false, "%", "Osc Feedback Amount" },
    { "osc_feedback_transpose", -24.0, 24.0, 49, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "semitones", "Osc Feedback Transpose" },
    { "osc_feedback_tune", -1.0, 1.0, 0, 0.0, 0.0, 100.0,
      ValueDetails::kLinear, false, "cents", "Osc Feedback Tune" },
    { "osc_mix", 0.0, 1.0, 0, 0.5, 0.0, 1.0, // DEPRECATED
      ValueDetails::kLinear, false, "", "Osc Mix" },
    { "pitch_bend_range", 0.0, 48.0, 49, 2.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "semitones", "Pitch Bend Range" },
    { "poly_lfo_amplitude", -1.0, 1.0, 0, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Poly LFO Amp" },
    { "poly_lfo_frequency", -7.0, 6.0, 0, 1.0, 0.0, 1.0,
      ValueDetails::kExponential, true, "secs", "Poly LFO Frequency" },
    { "poly_lfo_sync", 0.0, 3.0, 4, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Poly LFO Sync" },
    { "poly_lfo_tempo", 0.0, 11.0, 12, 7.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Poly LFO Tempo" },
    { "poly_lfo_waveform", 0.0, 12.0, 13, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Poly LFO Waveform" },
    { "polyphony", 1.0, 32.0, 32, 4.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "voices", "Polyphony" },
    { "portamento", -9.0, -1.0, 0, -7.0, 0.0, 12.0,
      ValueDetails::kExponential, false, "s/oct", "Portamento" },
    { "portamento_type", 0.0, 2.0, 3, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Portamento Type" },
    { "resonance", 0.0, 1.0, 0, 0.5, 0.0, 100.0,
      ValueDetails::kLinear, false, "%", "Filter Resonance" },
    { "reverb_damping", 0.0, 1.0, 0, 0.5, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Reverb Damping" },
    { "reverb_dry_wet", 0.0, 1.0, 0, 0.5, 0.0, 100.0,
      ValueDetails::kLinear, false, "%", "Reverb Mix" },
    { "reverb_feedback", 0.8, 1.0, 0, 0.9, 0.0, 100.0,
      ValueDetails::kLinear, false, "%", "Reverb Feedback" },
    { "reverb_on", 0.0, 1.0, 2, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Reverb Switch" },
    { "step_frequency", -5.0, 6.0, 0, 2.0, 0.0, 1.0,
      ValueDetails::kExponential, true, "secs", "Step Frequency" },
    { "step_seq_00", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 1" },
    { "step_seq_01", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 2" },
    { "step_seq_02", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 3" },
    { "step_seq_03", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 4" },
    { "step_seq_04", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 5" },
    { "step_seq_05", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 6" },
    { "step_seq_06", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 7" },
    { "step_seq_07", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 8" },
    { "step_seq_08", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 9" },
    { "step_seq_09", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 10" },
    { "step_seq_10", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 11" },
    { "step_seq_11", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 12" },
    { "step_seq_12", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 13" },
    { "step_seq_13", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 14" },
    { "step_seq_14", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 15" },
    { "step_seq_15", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 16" },
    { "step_seq_16", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 17" },
    { "step_seq_17", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 18" },
    { "step_seq_18", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 19" },
    { "step_seq_19", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 20" },
    { "step_seq_20", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 21" },
    { "step_seq_21", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 22" },
    { "step_seq_22", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 23" },
    { "step_seq_23", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 24" },
    { "step_seq_24", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 25" },
    { "step_seq_25", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 26" },
    { "step_seq_26", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 27" },
    { "step_seq_27", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 28" },
    { "step_seq_28", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 29" },
    { "step_seq_29", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 30" },
    { "step_seq_30", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 31" },
    { "step_seq_31", -1.0, 1.0, 0, 0.0, 0.0, 1.0, ValueDetails::kLinear, false, "", "Step 32" },
    { "step_sequencer_retrigger", 0.0, 2.0, 3, 2.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Step Retrigger" },
    { "step_sequencer_sync", 0.0, 3.0, 4, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Step Sync" },
    { "step_sequencer_tempo", 0.0, 11.0, 12, 7.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Step Tempo" },
    { "step_smoothing", 0.0, 0.5, 0, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Step Smoothing" },
    { "stutter_frequency", 0.0, 7.0, 0, 3.0, 0.0, 1.0,
      ValueDetails::kExponential, true, "secs", "Stutter Frequency" },
    { "stutter_sync", 0.0, 3.0, 4, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Stutter Sync" },
    { "stutter_tempo", 6.0, 11.0, 6, 8.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Stutter Tempo" },
    { "stutter_on", 0.0, 1.0, 2, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Stutter Switch" },
    { "stutter_resample_frequency", -7.0, 4.0, 0, 1.0, 0.0, 1.0,
      ValueDetails::kExponential, true, "secs", "Resample Frequency" },
    { "stutter_resample_sync", 0.0, 3.0, 4, 1.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Stutter Sync" },
    { "stutter_resample_tempo", 0.0, 11.0, 12, 6.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Stutter Tempo" },
    { "stutter_softness", 0.0, 1.0, 0, 0.2, 0.0, 100.0,
      ValueDetails::kLinear, false, "%", "Stutter Softness" },
    { "sub_shuffle", 0.0, 1.0, 0, 0.0, 0.0, 100.0,
      ValueDetails::kLinear, false, "%", "Sub Osc Shuffle" },
    { "sub_octave", 0.0, 1.0, 2, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Sub Octave Down" },
    { "sub_volume", 0.0, 1.0, 0, 0.0, 0.0, 1.0,
      ValueDetails::kQuadratic, false, "", "Sub Osc Volume" },
    { "sub_waveform", 0.0, 10.0, 11, 2.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Sub Osc Waveform" },
    { "unison_1_harmonize", 0.0, 1.0, 2, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Osc 1 Harmonize" },
    { "unison_2_harmonize", 0.0, 1.0, 2, 0.0, 0.0, 1.0,
      ValueDetails::kLinear, false, "", "Osc 2 Harmonize" },
    { "velocity_track", -1.0, 1.0, 0, 0.0, 0.0, 100.0,
      ValueDetails::kLinear, false, "%", "Velocity Track" },
    { "volume", 0.0, 1.4143, 0, 0.7071068, 0.0, 1.0,
      ValueDetails::kQuadratic, false, "", "Volume" },
  };

  ModulationConnectionBank::ModulationConnectionBank() {
    allocateMoreConnections();
  }

  ModulationConnectionBank::~ModulationConnectionBank() {
    for (ModulationConnection* connection : all_connections_)
      delete connection;
  }

  ModulationConnection* ModulationConnectionBank::get(const std::string& from,
                                                      const std::string& to) {
    if (available_connections_.size() == 0)
      allocateMoreConnections();

    ModulationConnection* connection = available_connections_.front();
    available_connections_.pop_front();
    connection->resetConnection(from, to);
    return connection;
  }

  void ModulationConnectionBank::recycle(ModulationConnection* connection) {
    available_connections_.push_back(connection);
  }

  void ModulationConnectionBank::allocateMoreConnections() {
    for (int i = 0; i < DEFAULT_MODULATION_CONNECTIONS; ++i) {
      ModulationConnection* connection = new ModulationConnection();
      available_connections_.push_back(connection);
      all_connections_.push_back(connection);
    }
  }

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
