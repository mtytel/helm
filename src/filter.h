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
#ifndef FILTER_H
#define FILTER_H

#include "processor.h"

namespace laf {

  class Filter : public Processor {
    public:
      enum Inputs {
        kAudio,
        kType,
        kCutoff,
        kResonance,
        kReset,
        kNumInputs
      };

      enum Type {
        kLP12,
        kHP12,
        kBP12,
        kAP12,
        kNumTypes,
      };

      Filter();

      virtual Processor* clone() const { return new Filter(*this); }
      virtual void process();

    private:
      laf_float tick(int i);
      void reset();
      void computeCoefficients(Type type, laf_float cutoff,
                                          laf_float resonance);

      Type current_type_;
      laf_float current_cutoff_, current_resonance_;

      laf_float in_0_, in_1_, in_2_;
      laf_float out_0_, out_1_;

      laf_float past_in_1_, past_in_2_;
      laf_float past_out_1_, past_out_2_;
  };
} // namespace laf

#endif // FILTER_H
