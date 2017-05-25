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

#pragma once
#ifndef ALIAS_H
#define ALIAS_H

#include "processor.h"
#include "utils.h"

namespace mopo {

  // A signal delay processor with wet/dry, delay time and feedback controls.
  // Handles fractional delay amounts through interpolation.
  class Alias : public Processor {
    public:
      enum Inputs {
        kAudio,
        kWet,
        kFrequency,
        kNumInputs
      };

      Alias();

      virtual Processor* clone() const override { return new Alias(*this); }
      virtual void process() override;

      void tick(int i) {
        mopo_float audio = input(kAudio)->at(i);
        mopo_float wet = input(kWet)->at(i);
        mopo_float period = sample_rate_ / input(kFrequency)->at(i);

        static_samples_ += 1.0;
        if (static_samples_ >= period) {
          static_samples_ -= period;
          current_sample_ = audio;
        }

        output(0)->buffer[i] = utils::interpolate(audio, current_sample_, wet);
      }

    protected:
      mopo_float current_sample_;
      mopo_float static_samples_;
  };
} // namespace mopo

#endif // ALIAS_H
