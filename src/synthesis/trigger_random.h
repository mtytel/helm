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
#ifndef TRIGGER_RANDOM_H
#define TRIGGER_RANDOM_H

#include "processor.h"

namespace mopo {

  class TriggerRandom : public Processor {
    public:
      enum Inputs {
        kAudio,
        kReset,
        kNumInputs
      };

      TriggerRandom();
      virtual ~TriggerRandom() { }

      virtual Processor* clone() const { return new TriggerRandom(*this); }
      virtual void process();

    private:
      mopo_float value_;
  };
} // namespace mopo

#endif // TRIGGER_RANDOM_H
