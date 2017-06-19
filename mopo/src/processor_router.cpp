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

#include "processor_router.h"

#include "feedback.h"

#include <algorithm>
#include <vector>

namespace mopo {

  ProcessorRouter::ProcessorRouter(int num_inputs, int num_outputs) :
      Processor(num_inputs, num_outputs),
      global_order_(new std::vector<const Processor*>()),
      global_feedback_order_(new std::vector<const Feedback*>()),
      global_changes_(new int(0)), local_changes_(0) {
  }

  ProcessorRouter::ProcessorRouter(const ProcessorRouter& original) :
      Processor(original), global_order_(original.global_order_),
      global_feedback_order_(original.global_feedback_order_),
      global_changes_(original.global_changes_),
      local_changes_(original.local_changes_) {
    local_order_.assign(global_order_->size(), 0);
    local_feedback_order_.assign(global_feedback_order_->size(), 0);

    size_t num_processors = global_order_->size();
    for (size_t i = 0; i < num_processors; ++i) {
      const Processor* next = global_order_->at(i);
      Processor* clone = next->clone();
      local_order_[i] = clone;
      processors_[next] = clone;
    }

    size_t num_feedbacks = global_feedback_order_->size();
    for (size_t i = 0; i < num_feedbacks; ++i) {
      const Feedback* next = global_feedback_order_->at(i);
      Feedback* clone = new Feedback(*next);
      local_feedback_order_[i] = clone;
      feedback_processors_[next] = clone;
    }
  }

  ProcessorRouter::~ProcessorRouter() {
    for (Processor* processor : local_order_)
      delete processor;
    for (Feedback* feedback : local_feedback_order_)
      delete feedback;

    for (Processor* processor : idle_processors_) {
      processor->destroy();
      delete processor;
    }
  }

  void ProcessorRouter::process() {
    updateAllProcessors();

    // First make sure all the Feedback loops are ready to be read.
    int num_feedbacks = local_feedback_order_.size();
    for (int i = 0; i < num_feedbacks; ++i)
      local_feedback_order_[i]->refreshOutput();

    // Run all the main processors.
    int num_processors = local_order_.size();
    for (int i = 0; i < num_processors; ++i) {
      if (local_order_[i]->enabled())
        local_order_[i]->process();
    }

    // Store the outputs into the Feedback objects for next time.
    for (int i = 0; i < num_feedbacks; ++i) {
      if (global_feedback_order_->at(i)->enabled())
        local_feedback_order_[i]->process();
    }

    MOPO_ASSERT(num_processors != 0);
  }

  void ProcessorRouter::destroy() {
    for (Processor* processor : local_order_)
      processor->destroy();

    delete global_order_;
    delete global_feedback_order_;
    delete global_changes_;
    Processor::destroy();
  }

  void ProcessorRouter::setSampleRate(int sample_rate) {
    Processor::setSampleRate(sample_rate);
    updateAllProcessors();

    int num_processors = local_order_.size();
    for (int i = 0; i < num_processors; ++i)
      local_order_[i]->setSampleRate(sample_rate);

    int num_feedbacks = local_feedback_order_.size();
    for (int i = 0; i < num_feedbacks; ++i)
      local_feedback_order_[i]->setSampleRate(sample_rate);
  }

  void ProcessorRouter::setBufferSize(int buffer_size) {
    Processor::setBufferSize(buffer_size);
    updateAllProcessors();

    int num_processors = local_order_.size();
    for (int i = 0; i < num_processors; ++i)
      local_order_[i]->setBufferSize(buffer_size);

    int num_feedbacks = local_feedback_order_.size();
    for (int i = 0; i < num_feedbacks; ++i)
      local_feedback_order_[i]->setBufferSize(buffer_size);
  }

  void ProcessorRouter::addProcessor(Processor* processor) {
    MOPO_ASSERT(processor->router() == 0 || processor->router() == this);
    (*global_changes_)++;
    local_changes_++;

    processor->router(this);
    processor->setBufferSize(getBufferSize());
    global_order_->push_back(processor);
    processors_[processor] = processor;
    local_order_.push_back(processor);

    for (int i = 0; i < processor->numInputs(); ++i)
      connect(processor, processor->input(i)->source, i);
  }

  void ProcessorRouter::addIdleProcessor(Processor *processor) {
    idle_processors_.push_back(processor);
  }

  void ProcessorRouter::removeProcessor(const Processor* processor) {
    for (int i = 0; i < processor->numInputs(); ++i)
      disconnect(processor, processor->input(i)->source);

    MOPO_ASSERT(processor->router() == this);
    (*global_changes_)++;
    local_changes_++;
    std::vector<const Processor*>::iterator pos =
        std::find(global_order_->begin(), global_order_->end(), processor);
    MOPO_ASSERT(pos != global_order_->end());
    global_order_->erase(pos, pos + 1);

    std::vector<Processor*>::iterator local_pos =
        std::find(local_order_.begin(), local_order_.end(), processor);
    MOPO_ASSERT(local_pos != local_order_.end());
    local_order_.erase(local_pos, local_pos + 1);

    processors_.erase(processor);
  }

  void ProcessorRouter::connect(Processor* destination,
                                const Output* source, int index) {
    if (isDownstream(destination, source->owner)) {
      // We are introducing a cycle so insert a Feedback node.
      Feedback* feedback = nullptr;
      if (source->owner->isControlRate() || destination->isControlRate())
        feedback = new cr::Feedback();
      else
        feedback = new Feedback();
      feedback->plug(source);
      destination->plug(feedback, index);
      addFeedback(feedback);
    }
    else {
      // Not introducing a cycle so just make sure _destination_ is in order.
      reorder(destination);
    }
  }

  void ProcessorRouter::disconnect(const Processor* destination,
                                   const Output* source) {
    if (isDownstream(destination, source->owner)) {
      // We're fine unless there is a cycle and need to delete a Feedback node.
      for (int i = 0; i < destination->numInputs(); ++i) {
        const Processor* owner = destination->input(i)->source->owner;

        if (feedback_processors_.find(owner) != feedback_processors_.end()) {
          Feedback* feedback = feedback_processors_[owner];
          if (feedback->input()->source == source)
            removeFeedback(feedback_processors_[owner]);
          destination->input(i)->source = &Processor::null_source_;
        }
      }
    }
  }

  void ProcessorRouter::reorder(Processor* processor) {
    (*global_changes_)++;
    local_changes_++;

    // Get all the dependencies inside this router.
    std::set<const Processor*> dependencies = getDependencies(processor);

    // Stably reorder putting dependencies first.
    std::vector<const Processor*> new_order;
    new_order.reserve(global_order_->size());
    int num_processors = processors_.size();

    // First put the dependencies.
    for (int i = 0; i < num_processors; ++i) {
      if (global_order_->at(i) != processor &&
          dependencies.find(global_order_->at(i)) != dependencies.end()) {
        new_order.push_back(global_order_->at(i));
      }
    }

    // Then the processor if it is in this router.
    if (processors_.find(processor) != processors_.end())
      new_order.push_back(processor);

    // Then the remaining processors.
    for (int i = 0; i < num_processors; ++i) {
      if (global_order_->at(i) != processor &&
          dependencies.find(global_order_->at(i)) == dependencies.end()) {
        new_order.push_back(global_order_->at(i));
      }
    }

    MOPO_ASSERT(new_order.size() == processors_.size());
    (*global_order_) = new_order;

    // Make sure our parent is ordered as well.
    if (router_)
      router_->reorder(processor);
  }

  bool ProcessorRouter::isDownstream(const Processor* first,
                                     const Processor* second) const {
    std::set<const Processor*> dependencies = getDependencies(second);
    return dependencies.find(first) != dependencies.end();
  }

  bool ProcessorRouter::areOrdered(const Processor* first,
                                   const Processor* second) const {
    const Processor* first_context = getContext(first);
    const Processor* second_context = getContext(second);

    if (first_context && second_context) {
      size_t num_processors = global_order_->size();
      for (size_t i = 0; i < num_processors; ++i) {
        if (global_order_->at(i) == first_context)
          return true;
        else if (global_order_->at(i) == second_context)
          return false;
      }
    }
    else if (router_)
      return router_->areOrdered(first, second);

    return true;
  }

  bool ProcessorRouter::isPolyphonic(const Processor* processor) const {
    if (router_)
      return router_->isPolyphonic(this);
    return false;
  }

  ProcessorRouter* ProcessorRouter::getMonoRouter() {
    if (isPolyphonic(this))
      return router_->getMonoRouter();
    return this;
  }

  ProcessorRouter* ProcessorRouter::getPolyRouter() {
    return this;
  }

  void ProcessorRouter::addFeedback(Feedback* feedback) {
    feedback->router(this);
    global_feedback_order_->push_back(feedback);
    local_feedback_order_.push_back(feedback);
    feedback_processors_[feedback] = feedback;
  }

  void ProcessorRouter::removeFeedback(Feedback* feedback) {
    std::vector<const Feedback*>::iterator pos =
        std::find(global_feedback_order_->begin(),
                  global_feedback_order_->end(), feedback);
    MOPO_ASSERT(pos != global_feedback_order_->end());
    global_feedback_order_->erase(pos, pos + 1);

    std::vector<Feedback*>::iterator local_pos =
        std::find(local_feedback_order_.begin(),
                  local_feedback_order_.end(), feedback);
    MOPO_ASSERT(local_pos != local_feedback_order_.end());
    local_feedback_order_.erase(local_pos, local_pos + 1);

    feedback_processors_.erase(feedback);
  }

  void ProcessorRouter::updateAllProcessors() {
    if (local_changes_ == *global_changes_)
      return;

    local_order_.assign(global_order_->size(), 0);
    local_feedback_order_.assign(global_feedback_order_->size(), 0);

    size_t num_processors = global_order_->size();
    for (int i = 0; i < num_processors; ++i) {
      const Processor* next = global_order_->at(i);
      if (processors_.count(next) == 0)
        processors_[next] = next->clone();
      local_order_[i] = processors_[next];
    }

    size_t num_feedbacks = global_feedback_order_->size();
    for (int i = 0; i < num_feedbacks; ++i) {
      const Feedback* next = global_feedback_order_->at(i);
      if (feedback_processors_.count(next) == 0)
        feedback_processors_[next] = new Feedback(*next);
      local_feedback_order_[i] = feedback_processors_[next];
    }

    local_changes_ = *global_changes_;
  }

  const Processor* ProcessorRouter::getContext(const Processor* processor)
      const {
    const Processor* context = processor;
    while (context && processors_.find(context) == processors_.end())
      context = context->router();

    return context;
  }

  std::set<const Processor*> ProcessorRouter::getDependencies(
      const Processor* processor) const {
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
