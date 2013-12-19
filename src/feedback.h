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
#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "processor.h"

namespace mopo {
  class Feedback : public Processor {
    public:
      Feedback() : Processor(1, 1) { }

      virtual Processor* clone() const { return new Feedback(*this); }
      virtual void process();
      virtual void refreshOutput();

      inline void tick(int i);
      inline void tickRefreshOutput(int i);

    protected:
      mopo_float buffer_[BUFFER_SIZE];
  };
} // namespace mopo

#endif // FEEDBACK_H
