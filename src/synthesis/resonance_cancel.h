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
#ifndef RESONANCE_CANCEL_H
#define RESONANCE_CANCEL_H

#include "processor.h"

namespace mopo {

  // Will output a resonance of 1.0 if the filter type is a shelf filter.
  class ResonanceCancel : public Processor {
    public:
      enum Inputs {
        kResonance,
        kFilterType,
        kNumInputs
      };

      ResonanceCancel();

      virtual Processor* clone() const { return new ResonanceCancel(*this); }

      void process();
  };
} // namespace mopo

#endif // RESONANCE_CANCEL_H
