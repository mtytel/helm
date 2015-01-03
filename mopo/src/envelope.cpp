/* Copyright 2013-2014 Little IO
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

#include <cmath>

#define KILL_TIME 0.04

namespace mopo {

  Envelope::Envelope() :
      Processor(kNumInputs, kNumOutputs), state_(kReleasing),
      current_value_(0), decay_decay_(0), release_decay_(0), kill_decrement_(0) { }

  void Envelope::trigger(mopo_float event, int offset) {
    if (event == kVoiceOn)
      state_ = kKilling;
    else if (event == kVoiceOff) {
      state_ = kReleasing;
      outputs_[kFinished]->trigger(kVoiceOff, offset);
    }
    else if (event == kVoiceReset) {
      state_ = kAttacking;
      current_value_ = 0.0;
    }
  }

  void Envelope::process() {
    kill_decrement_ = 1.0 / (KILL_TIME * sample_rate_);
    outputs_[kFinished]->clearTrigger();
    // Only update decay and release rate once per buffer.
    mopo_float decay_samples =
        sample_rate_ * inputs_[kDecay]->at(buffer_size_ - 1);
    decay_decay_ = pow(CLOSE_ENOUGH, 1.0 / decay_samples);

    mopo_float release_samples =
        sample_rate_ * inputs_[kRelease]->at(buffer_size_ - 1);
    release_decay_ = pow(CLOSE_ENOUGH, 1.0 / release_samples);

    int i = 0;
    if (inputs_[kTrigger]->source->triggered) {
      int trigger_offset = inputs_[kRelease]->source->trigger_offset;

      for (; i < trigger_offset; ++i)
        outputs_[kValue]->buffer[i] = tick(i);

      trigger(inputs_[kTrigger]->source->trigger_value, trigger_offset);
    }

    for (; i < buffer_size_; ++i)
      outputs_[kValue]->buffer[i] = tick(i);
  }

  inline mopo_float Envelope::tick(int i) {
    if (state_ == kAttacking) {
      if (inputs_[kAttack]->at(i) <= 0)
        current_value_ = 1;
      else {
        mopo_float change = 1.0 / (sample_rate_ * inputs_[kAttack]->at(i));
        current_value_ = CLAMP(current_value_ + change, 0, 1);
      }
      if (current_value_ >= 1)
        state_ = kDecaying;
    }
    else if (state_ == kDecaying) {
      current_value_ = INTERPOLATE(inputs_[kSustain]->at(i), current_value_,
                                   decay_decay_);
    }
    else if (state_ == kKilling) {
      current_value_ -= kill_decrement_;
      if (current_value_ <= 0) {
        outputs_[kFinished]->trigger(kVoiceReset, i);
        state_ = kAttacking;
      }
    }
    else if (state_ == kReleasing)
      current_value_ *= release_decay_;
    return current_value_;
  }
} // namespace mopo
