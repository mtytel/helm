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

#pragma once
#ifndef STUTTER_H
#define STUTTER_H

#include "memory.h"
#include "processor.h"

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

      virtual Processor* clone() const { return new Stutter(*this); }
      virtual void process();

      void tick(int i, mopo_float sample_period, mopo_float stutter_period) {
        offset_ -= 1.0;
        resample_offset_ -= 1.0;
        if (resample_offset_ <= 0.0) {
          resampling_ = true;
          resample_offset_ += sample_period;
          offset_ = stutter_period;
        }
        else if (offset_ <= 0.0) {
          resampling_ = false;
          offset_ += stutter_period;
        }

        if (resampling_) {
          mopo_float softness = input(kWindowSoftness)->at(i);
          mopo_float phase = 2.0 * PI * (sample_period - resample_offset_) /
                             stutter_period;
          mopo_float amp = std::pow(0.5 - 0.5 * cos(phase), softness);

          mopo_float audio = input(kAudio)->at(i);
          memory_->push(audio);
          output(0)->buffer[i] = amp * audio;
        }
        else {
          mopo_float softness = input(kWindowSoftness)->at(i);
          mopo_float phase1 = 2.0 * PI * offset_ / stutter_period;
          mopo_float amp = std::pow(0.5 - 0.5 * cos(phase1), softness);
          if (resample_offset_ < offset_) {
            mopo_float phase2 = 2.0 * PI * resample_offset_ / stutter_period;
            amp = std::min(amp, std::pow(0.5 - 0.5 * cos(phase2), softness));
          }
          output(0)->buffer[i] = amp * memory_->get(offset_);
        }
      }

    protected:
      Memory* memory_;
      mopo_float offset_;
      mopo_float resample_offset_;
      bool resampling_;
  };
} // namespace mopo

#endif // STUTTER_H
