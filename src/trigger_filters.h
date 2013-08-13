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
#ifndef TRIGGER_FILTERS_H
#define TRIGGER_FILTERS_H

#include "processor.h"

namespace laf {

  class LegatoFilter : public Processor {
    public:
      enum Inputs {
        kLegato,
        kTrigger,
        kNumInputs
      };

      LegatoFilter();

      virtual Processor* clone() const { return new LegatoFilter(*this); }

      void process();

    private:
      laf_sample last_value_;
  };
} // namespace laf

#endif // TRIGGER_FILTERS_H
