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

#pragma once
#ifndef TWYTCH_COMMON_H
#define TWYTCH_COMMON_H

#include "mopo.h"
#include "value.h"
#include "operators.h"

#include <map>
#include <string>

namespace mopo {

  struct ValueDetails {
    std::string name;
  } typedef ValueDetails;

  static const std::string parameter_list[] = {
    "amp_attack",
    "amp_decay",
    "amp_release",
    "amp_sustain",
    "arp_frequency",
    "arp_gate",
    "arp_octaves",
    "arp_on",
    "arp_pattern",
    "arp_sync",
    "arp_tempo",
    "beats_per_minute",
    "cross_modulation",
    "cutoff",
    "delay_dry_wet",
    "delay_feedback",
    "delay_frequency",
    "delay_sync",
    "delay_tempo",
    "fil_attack",
    "fil_decay",
    "fil_env_depth",
    "fil_release",
    "fil_sustain",
    "filter_saturation",
    "filter_type",
    "formant_on",
    "formant_passthrough",
    "formant_x",
    "formant_y",
    "keytrack",
    "legato",
    "mono_lfo_1_amplitude",
    "mono_lfo_1_frequency",
    "mono_lfo_1_sync",
    "mono_lfo_1_tempo",
    "mono_lfo_1_waveform",
    "mono_lfo_2_amplitude",
    "mono_lfo_2_frequency",
    "mono_lfo_2_sync",
    "mono_lfo_2_tempo",
    "mono_lfo_2_waveform",
    "num_steps",
    "osc_1_transpose",
    "osc_1_tune",
    "osc_1_unison_detune",
    "osc_1_unison_voices",
    "osc_1_waveform",
    "osc_2_transpose",
    "osc_2_tune",
    "osc_2_unison_detune",
    "osc_2_unison_voices",
    "osc_2_waveform",
    "osc_feedback_amount",
    "osc_feedback_transpose",
    "osc_feedback_tune",
    "osc_mix",
    "pitch_bend_range",
    "poly_lfo_amplitude",
    "poly_lfo_frequency",
    "poly_lfo_sync",
    "poly_lfo_tempo",
    "poly_lfo_waveform",
    "polyphony",
    "portamento",
    "portamento_type",
    "resonance",
    "step_frequency",
    "step_seq_00",
    "step_seq_01",
    "step_seq_02",
    "step_seq_03",
    "step_seq_04",
    "step_seq_05",
    "step_seq_06",
    "step_seq_07",
    "step_seq_08",
    "step_seq_09",
    "step_seq_10",
    "step_seq_11",
    "step_seq_12",
    "step_seq_13",
    "step_seq_14",
    "step_seq_15",
    "step_seq_16",
    "step_seq_17",
    "step_seq_18",
    "step_seq_19",
    "step_seq_20",
    "step_seq_21",
    "step_seq_22",
    "step_seq_23",
    "step_seq_24",
    "step_seq_25",
    "step_seq_26",
    "step_seq_27",
    "step_seq_28",
    "step_seq_29",
    "step_seq_30",
    "step_seq_31",
    "step_sequencer_sync",
    "step_sequencer_tempo",
    "step_smoothing",
    "stutter_frequency",
    "stutter_on",
    "stutter_resample_frequency",
    "unison_1_harmonize",
    "unison_2_harmonize",
    "velocity_track",
    "volume"
  };

  namespace strings {

    const std::string off_on[] = {
      "off",
      "on"
    };

    const std::string off_auto_on[] = {
      "off",
      "auto",
      "on"
    };

    const std::string arp_patterns[] = {
      "up",
      "down",
      "up-down",
      "as played",
      "random"
    };

    const std::string freq_sync_styles[] = {
      "Hertz",
      "Tempo",
      "Tempo Dotted",
      "Tempo Triplets"
    };

    const std::string filter_types[] = {
      "low pass",
      "high pass",
      "band pass",
      "notch",
      "low shelf",
      "high shelf",
      "all pass"
    };

    const std::string waveforms[] = {
      "sin",
      "triangle",
      "square",
      "saw up",
      "saw down",
      "3 step",
      "4 step",
      "8 step",
      "3 pyramid",
      "5 pyramid",
      "9 pyramid",
      "white noise"
    };

    const std::string synced_frequencies[] = {
      "32/1",
      "16/1",
      "8/1",
      "4/1",
      "2/1",
      "1/1",
      "1/2",
      "1/4",
      "1/8",
      "1/16",
      "1/32",
      "1/64",
    };
  } // namespace strings

  const mopo_float MAX_STEPS = 32;
  const int NUM_FORMANTS = 4;

  typedef std::map<std::string, Value*> control_map;
  typedef std::map<char, std::string> midi_learn_map;
  typedef std::map<std::string, Processor*> input_map;
  typedef std::map<std::string, Processor::Output*> output_map;

  const mopo::Value synced_freq_ratios[] = {
    Value(1.0 / 128.0),
    Value(1.0 / 64.0),
    Value(1.0 / 32.0),
    Value(1.0 / 16.0),
    Value(1.0 / 8.0),
    Value(1.0 / 4.0),
    Value(1.0 / 2.0),
    Value(1.0),
    Value(2.0),
    Value(4.0),
    Value(8.0),
    Value(16.0),
  };

  struct ModulationConnection {
    ModulationConnection(std::string from, std::string to) :
        source(from), destination(to) { }

    std::string source;
    std::string destination;
    Value amount;
    Multiply modulation_scale;
  };
} // namespace mopo

#endif // TWYTCH_COMMON_H
