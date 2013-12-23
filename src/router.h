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
#ifndef ROUTER_H
#define ROUTER_H

#include "feedback.h"
#include "mopo.h"
#include "router_base.h"

#include <algorithm>
#include <map>
#include <set>
#include <vector>

namespace mopo {

  template<class P>
  class Router : public RouterBase {
    public:
      Router(int num_inputs = 0, int num_outputs = 0) :
          RouterBase(num_inputs, num_outputs) { }
      Router(const Router& original);

      virtual Processor* clone() const { return new Router<P>(*this); }
      void process();
      void setSampleRate(int sample_rate);

      void addProcessor(P* processor);
      void removeProcessor(const P* processor);

    protected:
      std::map<const Processor*, P*> processors_;
  };

  template<class P>
  Router<P>::Router(const Router<P>& original) : RouterBase(original) {
    size_t num_processors = order_->size();
    for (size_t i = 0; i < num_processors; ++i)
      processors_[order_->at(i)] = static_cast<P*>(order_->at(i)->clone());
  }

  template<class P>
  void Router<P>::process() {
    size_t num_feedbacks = feedbacks_.size();
    for (size_t i = 0; i < num_feedbacks; ++i)
      feedbacks_[i]->refreshOutput();

    size_t num_processors = order_->size();
    for (size_t i = 0; i < num_processors; ++i) {
      const Processor* next = order_->at(i);
      if (processors_.find(next) == processors_.end())
        processors_[next] = static_cast<P*>(next->clone());
      processors_[next]->process();
    }

    for (size_t i = 0; i < num_feedbacks; ++i)
      feedbacks_[i]->process();

    MOPO_ASSERT(num_processors != 0);
  }

  template<class P>
  void Router<P>::setSampleRate(int sample_rate) {
    int num_processors = order_->size();
    for (int i = 0; i < num_processors; ++i) {
      const Processor* next = order_->at(i);
      if (processors_.find(next) == processors_.end())
        processors_[next] = static_cast<P*>(next->clone());
      processors_[next]->setSampleRate(sample_rate);
    }
  }

  template<class P>
  void Router<P>::addProcessor(P* processor) {
    MOPO_ASSERT(processor->router() == NULL ||
                processor->router() == this);
    processor->router(this);
    order_->push_back(processor);
    processors_[processor] = processor;

    for (int i = 0; i < processor->numInputs(); ++i)
      connect(processor, processor->input(i)->source, i);
  }

  template<class P>
  void Router<P>::removeProcessor(const P* processor) {
    MOPO_ASSERT(processor->router() == this);
    typename std::vector<const Processor*>::iterator pos =
        std::find(order_->begin(), order_->end(), processor);
    MOPO_ASSERT(pos != order_->end());
    order_->erase(pos, pos + 1);
    processors_.erase(processor);
  }
} // namespace mopo

#endif // ROUTER_H
