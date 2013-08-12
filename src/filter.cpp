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

#include "filter.h"

namespace laf {

  Filter::Filter() : Processor(Filter::kNumInputs, 1) {
    past_in_1_ = past_in_2_ = past_out_1_ = past_out_2_ = 0;
  }

  void Filter::process() {
    int i = 0;
    if (inputs_[kReset]->source->triggered) {
      int trigger_offset = inputs_[kReset]->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        outputs_[0]->buffer[i] = tick(i);

      reset();
    }
    for (; i < BUFFER_SIZE; ++i)
      outputs_[0]->buffer[i] = tick(i);
  }

  inline laf_sample Filter::tick(int i) {
    laf_sample input = inputs_[kAudio]->at(i);
    laf_sample cutoff = inputs_[kCutoff]->at(i);
    laf_sample resonance = inputs_[kResonance]->at(i);

    if (cutoff != last_cutoff_ || resonance != last_resonance_)
      computeCoefficients(cutoff, resonance);

    laf_sample out = input * in_0_ + past_in_1_ * in_1_ + past_in_2_ * in_2_ -
                     past_out_1_ * out_0_ - past_out_2_ * out_1_;
    past_in_2_ = past_in_1_;
    past_in_1_ = input;
    past_out_2_ = past_out_1_;
    past_out_1_ = out;
    return out;
  }

  inline void Filter::reset() {
    past_in_1_ = past_in_2_ = past_out_1_ = past_out_2_ = 0;
  }

  inline void Filter::computeCoefficients(laf_sample cutoff,
                                          laf_sample resonance) {
    switch(type_) {
      case kLP12: {
        last_cutoff_ = cutoff;
        last_resonance_ = resonance;
        laf_sample sf = 1.0 / tan(PI * cutoff / sample_rate_);
        laf_sample sf_squared = sf * sf;
        laf_sample norm = 1 + 1 / resonance * sf + sf_squared;

        in_2_ = in_0_ = 1 / norm;
        in_1_ = 2.0 / norm;
        out_0_ = 2.0 * (1 - sf_squared) / norm;
        out_1_ = (1 - 1 / resonance * sf + sf_squared) / norm;
        break;
      }
      case kHP12: {
        laf_sample sf = 1.0 / tan(PI * cutoff / 44100.0);
        laf_sample sf_squared = sf * sf;
        laf_sample norm = 1 + 1 / resonance * sf + sf_squared;

        in_0_ = sf_squared / norm;
        in_1_ = -2.0 * sf_squared / norm;
        in_2_ = sf_squared / norm;
        out_0_ = 2.0 * (1 - sf_squared) / norm;
        out_1_ = (1 - 1 / resonance * sf + sf_squared) / norm;
        break;
      }
    }

    last_cutoff_ = cutoff;
    last_resonance_ = resonance;
  }
} // namespace laf
