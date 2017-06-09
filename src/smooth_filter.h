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
#ifndef SMOOTH_FILTER_H
#define SMOOTH_FILTER_H

#include "processor.h"

namespace mopo {

  class SmoothFilter : public Processor {
    public:
      enum Inputs {
        kTarget,
        kHalfLife,
        kNumInputs
      };

      SmoothFilter(mopo_float start_value = 0.0);

      virtual Processor* clone() const override {
        return new SmoothFilter(*this);
      }

      virtual void process() override;

    private:
      mopo_float last_value_;
  };

  namespace cr {
    class SmoothFilter : public Processor {
      public:
        enum Inputs {
          kTarget,
          kHalfLife,
          kNumInputs
        };

        SmoothFilter(mopo_float start_value = 0.0);

        virtual Processor* clone() const override {
          return new SmoothFilter(*this);
        }

        virtual void process() override;
        
      private:
        mopo_float last_value_;
    };
  } // namespace cr
} // namespace mopo

#endif // FILTER_H
