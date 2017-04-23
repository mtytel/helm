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
#ifndef BYPASS_ROUTER_H
#define BYPASS_ROUTER_H

#include "processor_router.h"

namespace mopo {

  class BypassRouter : public ProcessorRouter {
    public:
      enum Inputs {
        kAudio,
        kOn,
        kNumInputs
      };

      BypassRouter(int num_inputs = kNumInputs, int num_outputs = 0);

      virtual Processor* clone() const override {
        return new BypassRouter(*this);
      }

      void process() override;
  };
} // namespace mopo

#endif // BYPASS_ROUTER_H
