/* Copyright 2013 Little IO
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

#define CLOSE_ENOUGH (1.0 / 256.0)

namespace mopo {

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

      virtual Processor* clone() const { return new Envelope(*this); }
      void process();
      void trigger(mopo_float event, int offset);

    private:
      mopo_float tick(int i);

      State state_;
      mopo_float current_value_;
      mopo_float decay_decay_;
      mopo_float release_decay_;
  };
} // namespace mopo

#endif // ENVELOPE_H
