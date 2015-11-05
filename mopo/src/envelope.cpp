/* Copyright 2013-2015 Matt Tytel
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

    // Only update decay and release rate once per buffer.
    mopo_float decay_samples = sample_rate_ * input(kDecay)->at(0);
    decay_samples = CLAMP(decay_samples, 1.0, decay_samples);
    decay_decay_ = pow(CLOSE_ENOUGH, 1.0 / decay_samples);

    mopo_float release_samples = sample_rate_ * input(kRelease)->at(0);
    release_samples = CLAMP(release_samples, 1.0, release_samples);
    release_decay_ = pow(CLOSE_ENOUGH, 1.0 / release_samples);

    mopo_float* out_buffer = output(kValue)->buffer;
    int i = 0;
    if (input(kTrigger)->source->triggered) {
      int trigger_offset = input(kRelease)->source->trigger_offset;

      if (state_ == kReleasing) {
        for (; i < trigger_offset; ++i)
          tickRelease(i, out_buffer);
      }
      else {
        for (; i < trigger_offset; ++i)
          tick(i, out_buffer);
      }

      trigger(input(kTrigger)->source->trigger_value, trigger_offset);
    }

    if (state_ == kReleasing) {
      for (; i < buffer_size_; ++i)
        tickRelease(i, out_buffer);
    }
    else {
      for (; i < buffer_size_; ++i)
        tick(i, out_buffer);
    }
  }
} // namespace mopo
