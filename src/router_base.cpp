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

#include "router_base.h"

#include "feedback.h"

namespace mopo {

  RouterBase::RouterBase(int num_inputs, int num_outputs) :
      Processor(num_inputs, num_outputs) {
    order_ = new std::vector<const Processor*>;
  }

  RouterBase::RouterBase(const RouterBase& original) :
      Processor(original), order_(original.order_) {
    size_t num_feedbacks = original.feedbacks_.size();
    for (size_t i = 0; i < num_feedbacks; ++i)
      feedbacks_[i] = static_cast<Feedback*>(original.feedbacks_[i]->clone());
  }

  void RouterBase::addFeedback(Feedback* feedback) {
    MOPO_ASSERT(feedback->router() == NULL || feedback->router() == this);
    feedback->router(this);
    feedbacks_.push_back(feedback);
  }

  void RouterBase::connect(Processor* destination,
                           const Output* source, int index) {
    if (isDownstream(destination, source->owner)) {
      // We are introducing a cycle so insert a Feedback node.
      Feedback* feedback = new Feedback();
      feedback->plug(source);
      destination->plug(feedback, index);
      addFeedback(feedback);
      MOPO_ASSERT(false);
    }
    else {
      // Not introducing a cycle so just make sure _destination_ is in order.
      reorder(destination);
    }
  }

  void RouterBase::reorder(Processor* processor) {
    // Get all the dependencies inside this router.
    std::set<const Processor*> dependencies = getDependencies(processor);

    // Stably reorder putting dependencies first.
    std::vector<const Processor*> new_order;
    int num_processors = order_->size();
    new_order.reserve(num_processors);

    // First put the dependencies.
    for (int i = 0; i < num_processors; ++i) {
      if (order_->at(i) != processor &&
          dependencies.find(order_->at(i)) != dependencies.end()) {
        new_order.push_back(order_->at(i));
      }
    }

    // Then the processor, if it is in this router.
    if (std::find(order_->begin(), order_->end(), processor) != order_->end())
      new_order.push_back(processor);

    // Then the remaining processors.
    for (int i = 0; i < num_processors; ++i) {
      if (order_->at(i) != processor &&
          dependencies.find(order_->at(i)) == dependencies.end()) {
        new_order.push_back(order_->at(i));
      }
    }

    MOPO_ASSERT(new_order.size() == order_->size());
    (*order_) = new_order;

    // Make sure our parent is ordered as well.
    if (router_)
      router_->reorder(processor);
  }

  bool RouterBase::isDownstream(const Processor* first,
                                const Processor* second) {
    std::set<const Processor*> dependencies = getDependencies(second);
    return dependencies.find(first) != dependencies.end();
  }

  bool RouterBase::areOrdered(const Processor* first, const Processor* second) {
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

  const Processor* RouterBase::getContext(const Processor* processor) {
    const Processor* context = processor;
    while (context && std::find(order_->begin(), order_->end(), context) ==
                      order_->end()) {
      context = context->router();
    }

    return context;
  }

  std::set<const Processor*> RouterBase::getDependencies(
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
} // namespace mopo
