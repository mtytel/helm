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
#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "processor.h"

namespace mopo {

  // A special processor for the purpose of feedback loops in the signal flow.
  // Feedback can be used for batch buffer feedback processing or sample by
  // sample feedback processing.
  class Feedback : public Processor {
    public:
      Feedback() : Processor(1, 1) {
        memset(buffer_, 0, MAX_BUFFER_SIZE * sizeof(mopo_float));
      }

      virtual ~Feedback() { }

      virtual Processor* clone() const override { return new Feedback(*this); }
      virtual void process() override;
      virtual void refreshOutput();

      inline void tick(int i) {
        buffer_[i] = input(0)->source->buffer[i];
      }

      inline void tickBeginRefreshOutput() {
        output(0)->buffer[0] = buffer_[buffer_size_ - 1];
      }

      inline void tickRefreshOutput(int i) {
        MOPO_ASSERT(i > 0 && i < buffer_size_);
        output(0)->buffer[i] = buffer_[i - 1];
      }

    protected:
      mopo_float buffer_[MAX_BUFFER_SIZE];
  };
} // namespace mopo

#endif // FEEDBACK_H
