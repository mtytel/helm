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
#ifndef PHASE_PULL_OSCILLATOR_H
#define PHASE_PULL_OSCILLATOR_H

#include "processor.h"
#include "wave.h"

namespace mopo {

  // A processor that produces an oscillation stream based on the input
  // frequency, and waveform. This oscillator will correct it's phase to
  // where it 'should' be, so under extreme modulation, will eventually
  // maintain the correct number of phase cycles.
  class PhasePullOscillator : public Processor {

    public:
      enum Inputs {
        kTargetFrequency,
        kFrequency,
        kPhasePullStrength,
        kWaveform,
        kReset,
        kNumInputs
      };

      PhasePullOscillator();

      virtual Processor* clone() const { return new PhasePullOscillator(*this); }
      void preprocess();
      void process();

      inline void tick(int i) {
        mopo_float target_frequency = inputs_[kTargetFrequency]->at(i);
        mopo_float frequency = inputs_[kFrequency]->at(i);
        mopo_float phase_pull_strength = inputs_[kPhasePullStrength]->at(i);
        mopo_float integral;

        mopo_float target_delta = (target_cycles_ - cycles_) + (target_offset_ - offset_);
        velocity_to_target_ += target_delta * phase_pull_strength;
        velocity_to_target_ *= 0.9;

        target_offset_ += 1.0 + target_frequency / sample_rate_;
        target_offset_ = modf(target_offset_, &integral);
        target_cycles_ += integral - 1.0;

        offset_ += 1.0 + frequency / sample_rate_ + velocity_to_target_;
        offset_ = modf(offset_, &integral);
        cycles_ += integral - 1.0;

        outputs_[0]->buffer[i] =
            Wave::blwave(waveform_, offset_, frequency);
      }

    protected:
      mopo_float target_cycles_;
      mopo_float target_offset_;
      mopo_float cycles_;
      mopo_float offset_;
      mopo_float velocity_to_target_;
      Wave::Type waveform_;
  };
} // namespace mopo

#endif // PHASE_PULL_OSCILLATOR_H
