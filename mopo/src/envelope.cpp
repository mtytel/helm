/* Copyright 2013-2017 Matt Tytel
 *
 * mopo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mopo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mopo.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "envelope.h"

#include "sample_decay_lookup.h"

#include <cmath>

#define ATTACK_DONE 0.999

namespace mopo {

  Envelope::Envelope() :
      Processor(kNumInputs, kNumOutputs, true),
      state_(kReleasing), current_value_(0.0) { }

  void Envelope::trigger(mopo_float event) {
    if (event == kVoiceOn || event == kVoiceReset) {
      state_ = kAttacking;
      current_value_ = 0.0;

      output(kFinished)->trigger(kVoiceReset);
    }
    else if (event == kVoiceOff)
      state_ = kReleasing;
    else if (event == kVoiceKill) {
      state_ = kKilling;
    }
  }

  void Envelope::process() {
    output(kFinished)->clearTrigger();

    if (input(kTrigger)->source->triggered)
      trigger(input(kTrigger)->source->trigger_value);

    output(kPhase)->buffer[0] = state_;

    int samples = 0;

    if (state_ == kAttacking) {
      mopo_float attack = utils::max(input(kAttack)->at(0), 0.000000001);
      mopo_float attack_increment = 1.0 / (sample_rate_ * attack);
      samples = (ATTACK_DONE - current_value_) / attack_increment;

      if (samples < samples_to_process_) {
        state_ = kDecaying;
        current_value_ = 1.0;
        output(kValue)->buffer[0] = current_value_;
      }
      else {
        output(kValue)->buffer[0] = current_value_;
        current_value_ += samples_to_process_ * attack_increment;
      }
    }
    if (state_ == kDecaying) {
      mopo_float decay_samples = sample_rate_ * input(kDecay)->at(0);
      mopo_float sustain = input(kSustain)->at(0);

      mopo_float decay_decay_ = SampleDecayLookup::sampleDecayLookup(decay_samples);
      mopo_float leftover_samples = samples_to_process_ - samples;
      mopo_float delta = current_value_ - sustain;
      mopo_float end_delta = delta * pow(decay_decay_, leftover_samples);

      current_value_ = sustain + end_delta;
      output(kValue)->buffer[0] = current_value_;
    }
    else if (state_ == kReleasing) {
      mopo_float release_samples = sample_rate_ * input(kRelease)->at(0);

      mopo_float release_decay = SampleDecayLookup::sampleDecayLookup(release_samples);
      mopo_float leftover_samples = samples_to_process_ - samples;
      current_value_ = current_value_ * pow(release_decay, leftover_samples);
      output(kValue)->buffer[0] = current_value_;
    }
    else if (state_ == kKilling) {
      mopo_float decrement = samples_to_process_ / (VOICE_KILL_TIME * sample_rate_);
      current_value_ = utils::max(0.0, current_value_ - decrement);
      output(kValue)->buffer[0] = current_value_;
    }
  }
} // namespace mopo
