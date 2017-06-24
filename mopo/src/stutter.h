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
#ifndef STUTTER_H
#define STUTTER_H

#include "memory.h"
#include "processor.h"
#include "utils.h"

namespace mopo {

  // A signal delay processor with wet/dry, delay time and feedback controls.
  // Handles fractional delay amounts through interpolation.
  class Stutter : public Processor {
    public:
      enum Inputs {
        kAudio,
        kStutterFrequency,
        kResampleFrequency,
        kWindowSoftness,
        kReset,
        kNumInputs
      };

      Stutter(int size);
      Stutter(const Stutter& other);
      virtual ~Stutter();

      virtual Processor* clone() const override { return new Stutter(*this); }
      virtual void process() override;

    protected:
      void startResampling(mopo_float sample_period) {
        resampling_ = true;
        resample_countdown_ = sample_period;
        offset_ = 0.0;
        memory_offset_ = 0.0;
      }

      Memory* memory_;
      int size_;
      mopo_float offset_;
      mopo_float memory_offset_;
      mopo_float resample_countdown_;
      mopo_float last_stutter_period_;
      mopo_float last_amplitude_;
      bool resampling_;
  };
} // namespace mopo

#endif // STUTTER_H
