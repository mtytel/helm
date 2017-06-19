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
#ifndef PROCESSOR_ROUTER_H
#define PROCESSOR_ROUTER_H

#include "feedback.h"
#include "processor.h"

#include <map>
#include <set>
#include <vector>

namespace mopo {

  class ProcessorRouter : public Processor {
    public:
      ProcessorRouter(int num_inputs = 0, int num_outputs = 0);
      ProcessorRouter(const ProcessorRouter& original);

      virtual ~ProcessorRouter();

      virtual Processor* clone() const override {
        return new ProcessorRouter(*this);
      }

      virtual void destroy() override;
      virtual void process() override;
      virtual void setSampleRate(int sample_rate) override;
      virtual void setBufferSize(int buffer_size) override;

      virtual void addProcessor(Processor* processor);
      virtual void addIdleProcessor(Processor* processor);
      virtual void removeProcessor(const Processor* processor);

      // Any time new dependencies are added into the ProcessorRouter graph, we
      // should call _connect_ on the destination Processor and source Output.
      void connect(Processor* destination, const Output* source, int index);
      void disconnect(const Processor* destination, const Output* source);
      bool isDownstream(const Processor* first, const Processor* second) const;
      bool areOrdered(const Processor* first, const Processor* second) const;

      virtual bool isPolyphonic(const Processor* processor) const;

      virtual ProcessorRouter* getMonoRouter();
      virtual ProcessorRouter* getPolyRouter();

    protected:
      // When we create a cycle into the ProcessorRouter graph, we must insert
      // a Feedback node and add it here.
      virtual void addFeedback(Feedback* feedback);
      virtual void removeFeedback(Feedback* feedback);

      // Makes sure _processor_ runs in a topologically sorted order in
      // relation to all other Processors in _this_.
      void reorder(Processor* processor);

      // Ensures we have all copies of all processors and feedback processors.
      virtual void updateAllProcessors();

      // Returns the ancestor of _processor_ which is a child of _this_.
      // Returns null if _processor_ is not a descendant of _this_.
      const Processor* getContext(const Processor* processor) const;
      std::set<const Processor*>
          getDependencies(const Processor* processor) const;

      std::vector<const Processor*>* global_order_;
      std::vector<Processor*> local_order_;
      std::map<const Processor*, Processor*> processors_;
      std::vector<Processor*> idle_processors_;

      std::vector<const Feedback*>* global_feedback_order_;
      std::vector<Feedback*> local_feedback_order_;
      std::map<const Processor*, Feedback*> feedback_processors_;

      int* global_changes_;
      int local_changes_;
  };
} // namespace mopo

#endif // PROCESSOR_ROUTER_H
