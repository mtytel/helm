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

#include <math.h>

namespace laf {

  Filter::Filter() : Processor(Filter::kNumInputs, 1) {
    current_type_ = kNumTypes;
    current_cutoff_ = 0.0;
    current_resonance_ = 0.0;

    in_0_ = in_1_ = in_2_ = 0.0;
    out_0_ = out_1_ = 0.0;
    past_in_1_ = past_in_2_ = past_out_1_ = past_out_2_ = 0.0;
  }

  void Filter::process() {
    current_type_ = static_cast<Type>(inputs_[kType]->at(0));
    computeCoefficients(current_type_, inputs_[kCutoff]->at(0),
                                       inputs_[kResonance]->at(0));

    int i = 0;
    if (inputs_[kReset]->source->triggered &&
        inputs_[kReset]->source->trigger_value == kVoiceReset) {
      int trigger_offset = inputs_[kReset]->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        outputs_[0]->buffer[i] = tick(i);

      reset();
    }
    for (; i < BUFFER_SIZE; ++i)
      outputs_[0]->buffer[i] = tick(i);
  }

  inline laf_float Filter::tick(int i) {
    laf_float input = inputs_[kAudio]->at(i);
    laf_float cutoff = inputs_[kCutoff]->at(i);
    laf_float resonance = inputs_[kResonance]->at(i);

    if (cutoff != current_cutoff_ || resonance != current_resonance_)
      computeCoefficients(current_type_, cutoff, resonance);

    laf_float out = input * in_0_ + past_in_1_ * in_1_ + past_in_2_ * in_2_ -
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

  inline void Filter::computeCoefficients(Type type,
                                          laf_float cutoff,
                                          laf_float resonance) {
    laf_float sf = 1.0 / tan(PI * cutoff / sample_rate_);
    laf_float sf_squared = sf * sf;
    laf_float norm = 1.0 + 1.0 / resonance * sf + sf_squared;

    switch(type) {
      case kLP12: {
        in_2_ = in_0_ = 1.0 / norm;
        in_1_ = 2.0 / norm;
        out_0_ = 2.0 * (1.0 - sf_squared) / norm;
        out_1_ = (1.0 - 1.0 / resonance * sf + sf_squared) / norm;
        break;
      }
      case kHP12: {
        in_2_ = in_0_ = sf_squared / norm;
        in_1_ = -2.0 * sf_squared / norm;
        out_0_ = 2.0 * (1.0 - sf_squared) / norm;
        out_1_ = (1.0 - 1.0 / resonance * sf + sf_squared) / norm;
        break;
      }
      case kBP12: {
        in_2_ = in_0_ = sf / norm;
        in_1_ = 0.0;
        out_0_ = 2.0 * (1.0 - sf_squared) / norm;
        out_1_ = (1.0 - 1.0 / resonance * sf + sf_squared) / norm;
        break;
      }
      case kAP12: {
        in_0_ = 1.0 / norm;
        in_1_ = -2.0 * (1.0 - sf_squared) / norm;
        in_2_ = (1.0 - 1.0 / resonance * sf + sf_squared) / norm;
        out_0_ = -in_1_;
        out_1_ = in_2_;
        break;
      }
      default:
        in_2_ = in_1_ = in_0_ = out_1_ = out_0_ = 0.0;
    }

    current_cutoff_ = cutoff;
    current_resonance_ = resonance;
  }
} // namespace laf
