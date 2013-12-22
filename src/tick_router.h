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
#ifndef TICK_ROUTER_H
#define TICK_ROUTER_H

#include "processor_router.h"

namespace mopo {

  class Feedback;

  template<class P>
  class TickRouter : public Router<P> {
    public:
      virtual Processor* clone() const { return new TickRouter(*this); }
      virtual void process();
  };

  template<class P>
  void TickRouter<P>::process() {
    size_t num_processors = order_->size();
    std::vector<P*> process_list;
    process_list.reserve(num_processors);

    for (size_t p = 0; p < num_processors; ++p) {
      const Processor* next = order_->at(i);
      if (processors_.find(next) == processors_.end())
        processors_[next] = next->clone();

      process_list[p] = processors_[next];
    }

    size_t num_feedbacks = feedbacks_.size();
    for (size_t i = 0; i < num_feedbacks; ++i)
      feedbacks_[i]->tickBeginRefreshOutput();

    for (int i = 0; i < BUFFER_SIZE - 1; ++i) {
      for (size_t p = 0; p < num_processors; ++p)
        process_list[p]->tick(i);

      for (size_t f = 0; f < num_feedbacks; ++f)
        feedbacks_[f]->tick(i);

      for (size_t f = 0; f < num_feedbacks; ++f)
        feedbacks_[f]->tickRefreshOutput(i);
    }

    for (size_t p = 0; p < num_processors; ++p)
      process_list[p]->tick(BUFFER_SIZE - 1);

    for (size_t f = 0; f < num_feedbacks; ++f)
      feedbacks_[f]->tick(BUFFER_SIZE - 1);

    MOPO_ASSERT(num_processors != 0);
  }
} // namespace mopo

#endif // TICK_ROUTER_H
