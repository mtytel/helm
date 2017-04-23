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
#ifndef TICK_ROUTER_H
#define TICK_ROUTER_H

#include "processor_router.h"

namespace mopo {

  class TickRouter : public ProcessorRouter {
    public:
      TickRouter(int num_inputs = 0, int num_outputs = 0) :
          ProcessorRouter(num_inputs, num_outputs) { }

      virtual void process() override = 0;
      virtual void tick(int i) = 0;
  };
} // namespace mopo

#endif // PROCESSOR_ROUTER_H
