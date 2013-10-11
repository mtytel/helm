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

#include "processor_router.h"

#include "laf.h"

#include <algorithm>
#include <stdio.h>
#include <vector>

namespace laf {
  ProcessorRouter::ProcessorRouter(int num_inputs, int num_outputs) :
      Processor(num_inputs, num_outputs) {
    order_ = new std::vector<const Processor*>;
  }

  Processor* ProcessorRouter::clone() const {
    ProcessorRouter* clone = new ProcessorRouter(*this);

    size_t num_processors = order_->size();
    for (size_t i = 0; i < num_processors; ++i)
      clone->processors_[order_->at(i)] = order_->at(i)->clone();

    return clone;
  }

  void ProcessorRouter::process() {
    int num_processors = order_->size();
    for (int i = 0; i < num_processors; ++i) {
      const Processor* next = order_->at(i);
      if (processors_.find(next) == processors_.end())
        processors_[next] = next->clone();
      processors_[next]->process();
    }
  }

  void ProcessorRouter::setSampleRate(int sample_rate) {
    int num_processors = order_->size();
    for (int i = 0; i < num_processors; ++i) {
      const Processor* next = order_->at(i);
      if (processors_.find(next) == processors_.end())
        processors_[next] = next->clone();
      processors_[next]->setSampleRate(sample_rate);
    }
  }

  void ProcessorRouter::addProcessor(Processor* processor) {
    LAF_ASSERT(!processor->router());
    processor->router(this);
    order_->push_back(processor);
    processors_[processor] = processor;

    reorder(processor);
  }

  void ProcessorRouter::removeProcessor(const Processor* processor) {
    LAF_ASSERT(processor->router() == this);
    std::vector<const Processor*>::iterator pos =
        std::find(order_->begin(), order_->end(), processor);
    LAF_ASSERT(pos != order_->end());
    order_->erase(pos, pos + 1);
  }

  void ProcessorRouter::reorder(Processor* processor) {
    // Get all the dependencies inside this router.
    std::set<const Processor*> dependencies = getDependencies(processor);

    // Stably reorder putting dependencies first.
    std::vector<const Processor*> new_order;
    int num_processors = processors_.size();

    // First put the dependencies.
    for (int i = 0; i < num_processors; ++i) {
      if (order_->at(i) != processor &&
          dependencies.find(order_->at(i)) != dependencies.end()) {
        new_order.push_back(order_->at(i));
      }
    }

    // Then the processor if it is in this router.
    if (processors_.find(processor) != processors_.end())
      new_order.push_back(processor);

    // Then the remaining processors.
    for (int i = 0; i < num_processors; ++i) {
      if (order_->at(i) != processor &&
          dependencies.find(order_->at(i)) == dependencies.end()) {
        new_order.push_back(order_->at(i));
      }
    }

    LAF_ASSERT(new_order.size() == processors_.size());
    (*order_) = new_order;

    // Make sure our parent is ordered as well.
    if (router_)
      router_->reorder(processor);
  }

  bool ProcessorRouter::areOrdered(const Processor* first,
                                   const Processor* second) {
    const Processor* first_context = getContext(first);
    const Processor* second_context = getContext(second);

    if (first_context && second_context) {
      size_t num_processors = order_->size();
      for (size_t i = 0; i < num_processors; ++i) {
        if (order_->at(i) == first_context)
          return true;
        else if (order_->at(i) == second_context)
          return false;
      }
    }
    else if (router_)
      return router_->areOrdered(first, second);

    return true;
  }

  const Processor* ProcessorRouter::getContext(const Processor* processor) {
    const Processor* context = processor;
    while (context && processors_.find(context) == processors_.end())
      context = context->router();

    return context;
  }

  std::set<const Processor*> ProcessorRouter::getDependencies(
      const Processor* processor) {
    std::vector<const Processor*> inputs;
    std::set<const Processor*> visited;
    std::set<const Processor*> dependencies;
    const Processor* context = getContext(processor);

    inputs.push_back(processor);
    for (size_t i = 0; i < inputs.size(); ++i) {
      // Find the parent that is inside this router.
      const Processor* dependency = getContext(inputs[i]);

      // If _inputs[i]_ has an ancestor in this context, then it is a
      // dependency. If it is outside this router context, we don't need to
      // check its inputs.
      if (dependency) {
        dependencies.insert(dependency);

        for (int j = 0; j < inputs[i]->numInputs(); ++j) {
          const Input* input = inputs[i]->input(j);
          if (input->source && input->source->owner &&
              visited.find(input->source->owner) == visited.end()) {
            inputs.push_back(input->source->owner);
            visited.insert(input->source->owner);
          }
        }
      }
    }

    // Make sure our context isn't listed as a dependency.
    dependencies.erase(context);

    return dependencies;
  }
} // namespace laf
