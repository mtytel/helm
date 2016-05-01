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

#pragma once
#ifndef ENVELOPE_H
#define ENVELOPE_H

#include "processor.h"
#include "utils.h"

namespace mopo {

  // An ADSR (Attack, Decay, Sustain, Release) Envelope.
  // The attack is a linear scale and completes in the exact amount of time.
  // The decay and release are exponential and get down to the _CLOSE_ENOUGH_
  // level in the specified amount of time.
  //
  // The reason for this is that technically the decay and release
  // take an extremely long time to finish because they are exponential. But
  // users are used to specifying the amount of time the decay or release take
  // so we make this compromise of _CLOSE_ENOUGH_.
  class Envelope : public Processor {
    public:
      enum Inputs {
        kAttack,
        kDecay,
        kSustain,
        kRelease,
        kTrigger,
        kNumInputs
      };

      enum Outputs {
        kValue,
        kFinished,
        kNumOutputs
      };

      enum State {
        kAttacking,
        kDecaying,
        kReleasing,
        kKilling,
      };

      Envelope();
      virtual ~Envelope() { }

      virtual Processor* clone() const override { return new Envelope(*this); }
      void process() override;
      void trigger(mopo_float event, int offset);

      void tickRelease(int i, mopo_float* out_buffer) {
        current_value_ *= release_decay_;
        out_buffer[i] = current_value_;
      }

      void tickDecay(int i, mopo_float* out_buffer) {
        current_value_ = INTERPOLATE(input(kSustain)->at(i), current_value_, decay_decay_);
        out_buffer[i] = current_value_;
      }

      void tick(int i, mopo_float* out_buffer) {
        if (state_ == kAttacking) {
          if (input(kAttack)->at(i) <= 0)
            current_value_ = 1;
          else {
            mopo_float change = 1.0 / (sample_rate_ * input(kAttack)->at(i));
            current_value_ = utils::clamp(current_value_ + change, 0.0, 1.0);
          }
          if (current_value_ >= 0.999)
            state_ = kDecaying;
        }
        else if (state_ == kKilling) {
          current_value_ -= kill_decrement_;
          if (current_value_ <= 0) {
            current_value_ = 0.0;
            output(kFinished)->trigger(kVoiceReset, i);
            state_ = next_life_state_;
          }
        }
        out_buffer[i] = current_value_;
      }

    protected:
      State state_;
      State next_life_state_;
      mopo_float current_value_;
      mopo_float decay_decay_;
      mopo_float release_decay_;
      mopo_float kill_decrement_;
  };
} // namespace mopo

#endif // ENVELOPE_H
