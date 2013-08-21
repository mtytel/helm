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
#ifndef DELAY_H
#define DELAY_H

#include "memory.h"
#include "processor.h"

namespace laf {

  class Delay : public Processor {
    public:
      enum Inputs {
        kAudio,
        kWet,
        kDelayTime,
        kFeedback,
        kNumInputs
      };

      Delay();

      virtual Processor* clone() const { return new Delay(*this); }
      virtual void process();

    protected:
      laf_float tick(int i);

      Memory memory_;
  };
} // namespace laf

#endif // DELAY_H
