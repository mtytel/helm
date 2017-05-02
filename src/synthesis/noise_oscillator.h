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

#pragma once
#ifndef NOISE_OSCILLATOR_H
#define NOISE_OSCILLATOR_H

#include "mopo.h"

namespace mopo {

  const mopo_float NOISE_CONSTANT = 9.19191919191919191919191919191919191919191919;
  const int NOISE_INT_CONSTANT = NOISE_CONSTANT;

  class NoiseOscillator : public Processor {
    public:
      enum Inputs {
        kReset,
        kNumInputs
      };

      NoiseOscillator();

      virtual void process();
      virtual Processor* clone() const { return new NoiseOscillator(*this); }

    protected:
      inline void tick(int i) {
        current_noise_value_ *= current_noise_value_;
        current_noise_value_ = current_noise_value_ - floor(current_noise_value_);

        output()->buffer[i] = 2.0 * current_noise_value_ - 1.0;

        current_noise_value_ += NOISE_INT_CONSTANT;
      }

      mopo_float current_noise_value_;
  };
} // namespace mopo

#endif // NOISE_OSCILLATOR_H
