/* Copyright 2013 Little IO
 *
 * laf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * laf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with laf.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "processor.h"
#include "wave.h"

namespace laf {

  class Oscillator : public Processor {
    public:
      enum Inputs {
        kFrequency,
        kPhase,
        kWaveType,
        kReset,
        kNumInputs
      };

      Oscillator();

      virtual Processor* clone() const { return new Oscillator(*this); }
      void process();

    protected:
      laf_sample tick(int i);
      void reset();

      laf_sample offset_;
      Wave::Type wave_type_;
  };
} // namespace laf

#endif // OSCILLATOR_H
