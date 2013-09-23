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
#ifndef MONO_PANNER_H
#define MONO_PANNER_H

#include "processor.h"

namespace laf {

  class MonoPanner : public Processor {
    public:
      enum Inputs {
        kAudio,
        kPan,
        kNumInputs
      };

      enum Outputs {
        kLeft,
        kRight,
        kNumOutputs
      };

      MonoPanner();

      virtual Processor* clone() const { return new MonoPanner(*this); }
      virtual void process();
  };
} // namespace laf

#endif // MONO_PANNER_H
