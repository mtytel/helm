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
#ifndef PROCESSOR_ROUTER_H
#define PROCESSOR_ROUTER_H

#include "processor.h"

#include <map>
#include <set>
#include <vector>

namespace laf {

  class ProcessorRouter : public Processor {
    public:
      ProcessorRouter(int num_inputs = 0, int num_outputs = 0);

      virtual Processor* clone() const;
      virtual void process();
      virtual void setSampleRate(int sample_rate);

      virtual void addProcessor(Processor* processor);
      virtual void removeProcessor(const Processor* processor);

      // Makes sure _processor_ runs in a topologically sorted order in
      // relation to all other Processors in _this_.
      // Any time new dependencies are added into the ProcessorRouter graph, we
      // should call _reorder_ on the source Processors.
      void reorder(Processor* processor);
      bool areOrdered(const Processor* first, const Processor* second);

    protected:
      // Returns the ancestor of _processor_ which is a child of _this_.
      // Returns NULL if _processor_ is not a descendant of _this_.
      const Processor* getContext(const Processor* processor);
      std::set<const Processor*> getDependencies(const Processor* processor);

      std::vector<const Processor*>* order_;
      std::map<const Processor*, Processor*> processors_;
  };
} // namespace laf

#endif // PROCESSOR_ROUTER_H
