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
#ifndef ROUTER_BASE_H
#define ROUTER_BASE_H

#include "processor.h"

#include <algorithm>
#include <set>
#include <vector>

namespace mopo {

  class Feedback;

  class RouterBase : public Processor {
    public:
      RouterBase(int num_inputs = 0, int num_outputs = 0);
      RouterBase(const RouterBase& original);

      virtual void setSampleRate(int sample_rate) = 0;
      void addFeedback(Feedback* feedback);

      // Any time new dependencies are added into the ProcessorRouter graph, we
      // should call _connect_ on the destination Processor and source Output.
      void connect(Processor* destination, const Output* source, int index);
      // Makes sure _processor_ runs in a topologically sorted order in
      // relation to all other Processors in _this_.
      void reorder(Processor* processor);
      bool isDownstream(const Processor* first, const Processor* second);
      bool areOrdered(const Processor* first, const Processor* second);

    protected:
      // Returns the ancestor of _processor_ which is a child of _this_.
      // Returns NULL if _processor_ is not a descendant of _this_.
      virtual const Processor* getContext(const Processor* processor);
      std::set<const Processor*> getDependencies(const Processor* processor);

      std::vector<const Processor*>* order_;
      std::vector<Feedback*> feedbacks_;
  };
} // namespace mopo

#endif // ROUTER_BASE_H
