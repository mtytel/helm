/* Copyright 2013-2016 Matt Tytel
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
      Processor(kNumInputs, kNumOutputs),
      state_(kReleasing), next_life_state_(kReleasing),
      current_value_(0), decay_decay_(0), release_decay_(0),
      kill_decrement_(0) { }

  void Envelope::trigger(mopo_float event, int offset) {
    if (event == kVoiceOn) {
      state_ = kKilling;
      next_life_state_ = kAttacking;
    }
    else if (event == kVoiceOff) {
      if (state_ == kKilling)
        next_life_state_ = kReleasing;
      else
        state_ = kReleasing;
    }
    else if (event == kVoiceReset) {
      state_ = kAttacking;
      current_value_ = 0.0;
    }
    else if (event == kVoiceKill) {
      state_ = kKilling;
      next_life_state_ = kReleasing;
    }
  }

  void Envelope::process() {
    kill_decrement_ = 1.0 / (VOICE_KILL_TIME * sample_rate_);
    output(kFinished)->clearTrigger();

    // Only update rates once per buffer.
    mopo_float attack = input(kAttack)->at(0);
    mopo_float attack_increment = bool(attack) * 1.0 / (sample_rate_ * attack);

    mopo_float decay_samples = sample_rate_ * input(kDecay)->at(0);
    decay_decay_ = SampleDecayLookup::sampleDecayLookup(decay_samples);

    mopo_float release_samples = sample_rate_ * input(kRelease)->at(0);
    release_decay_ = SampleDecayLookup::sampleDecayLookup(release_samples);

    if (input(kTrigger)->source->triggered) {
      int trigger_offset = input(kRelease)->source->trigger_offset;
      trigger(input(kTrigger)->source->trigger_value, trigger_offset);
    }

    mopo_float* out_buffer = output(kValue)->buffer;
    mopo_float current_value = current_value_;
    
    int i = 0;
    while (i < buffer_size_) {
      if (state_ == kReleasing) {
        for (; i < buffer_size_; ++i) {
          current_value *= release_decay_;
          out_buffer[i] = current_value;
        }
      }
      else if (state_ == kDecaying) {
        for (; i < buffer_size_; ++i) {
          current_value = INTERPOLATE(input(kSustain)->at(i), current_value, decay_decay_);
          out_buffer[i] = current_value;
        }
      }
      else if (state_ == kAttacking) {
        if (attack <= 0.0) {
          current_value = 1.0;
          out_buffer[i++] = current_value;
          state_ = kDecaying;
        }
        else {
          for (; i < buffer_size_; ++i) {
            current_value = current_value + attack_increment;

            if (current_value >= ATTACK_DONE) {
              state_ = kDecaying;
              current_value = 1.0;
              out_buffer[i++] = current_value;
              break;
            }
            out_buffer[i] = current_value;
          }
        }
      }
      else if (state_ == kKilling) {
        for (; i < buffer_size_; ++i) {
          current_value -= kill_decrement_;
          if (current_value <= 0) {
            current_value = 0.0;
            output(kFinished)->trigger(kVoiceReset, i);
            state_ = next_life_state_;
            out_buffer[i++] = current_value;
            break;
          }
          out_buffer[i] = current_value;
        }
      }
    }
    current_value_ = current_value;
  }
} // namespace mopo
