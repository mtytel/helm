/* Copyright 2013-2016 Matt Tytel
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

#include "filter.h"
#include "utils.h"

#include <cmath>

#define MIN_RESONANCE 0.1
#define MAX_RESONANCE 16.0
#define MIN_CUTTOFF 1.0

namespace mopo {

  Filter::Filter() : Processor(Filter::kNumInputs, 1) {
    current_type_ = kNumTypes;
    current_cutoff_ = 0.0;
    current_resonance_ = 0.0;

    target_in_0_ = 1.0;
    target_in_1_ = target_in_2_ = 0.0;
    target_out_1_ = target_out_2_ = 0.0;

    in_0_ = 1.0;
    in_1_ = in_2_ = 0.0;
    out_1_ = out_2_ = 0.0;

    past_in_1_ = past_in_2_ = past_out_1_ = past_out_2_ = 0.0;
  }

  std::complex<mopo_float> Filter::getResponse(mopo_float frequency) {
    static const std::complex<mopo_float> one(1.0, 0.0);
    const mopo_float phase_delta = 2.0 * PI * frequency / sample_rate_;
    const std::complex<mopo_float> freq_tick1 = std::polar(mopo_float(1.0), -phase_delta);
    const std::complex<mopo_float> freq_tick2 = std::polar(mopo_float(1.0), -2 * phase_delta);

    return (target_in_0_ * one + target_in_1_ * freq_tick1 + target_in_2_ * freq_tick2) /
           (one + target_out_1_ * freq_tick1 + target_out_2_ * freq_tick2);
  }

  void Filter::process() {
    current_type_ = static_cast<Type>(static_cast<int>(input(kType)->at(0)));
    mopo_float cutoff = utils::clamp(input(kCutoff)->at(0), MIN_CUTTOFF, sample_rate_);
    mopo_float resonance = utils::clamp(input(kResonance)->at(0),
                                        MIN_RESONANCE, MAX_RESONANCE);
    computeCoefficients(current_type_, cutoff, resonance, input(kGain)->at(0));

    mopo_float delta_in_0 = (target_in_0_ - in_0_) / buffer_size_;
    mopo_float delta_in_1 = (target_in_1_ - in_1_) / buffer_size_;
    mopo_float delta_in_2 = (target_in_2_ - in_2_) / buffer_size_;
    mopo_float delta_out_1 = (target_out_1_ - out_1_) / buffer_size_;
    mopo_float delta_out_2 = (target_out_2_ - out_2_) / buffer_size_;

    const mopo_float* audio_buffer = input(kAudio)->source->buffer;
    mopo_float* dest = output()->buffer;
    if (inputs_->at(kReset)->source->triggered &&
        inputs_->at(kReset)->source->trigger_value == kVoiceReset) {
      int trigger_offset = inputs_->at(kReset)->source->trigger_offset;
      int i = 0;
      for (; i < trigger_offset; ++i) {
        in_0_ += delta_in_0;
        in_1_ += delta_in_1;
        in_2_ += delta_in_2;
        out_1_ += delta_out_1;
        out_2_ += delta_out_2;
        tick(i, dest, audio_buffer);
      }

      reset();

      for (; i < buffer_size_; ++i)
        tick(i, dest, audio_buffer);
    }
    else {
      for (int i = 0; i < buffer_size_; ++i) {
        in_0_ += delta_in_0;
        in_1_ += delta_in_1;
        in_2_ += delta_in_2;
        out_1_ += delta_out_1;
        out_2_ += delta_out_2;
        tick(i, dest, audio_buffer);
      }
    }
  }

  void Filter::reset() {
    past_in_1_ = past_in_2_ = past_out_1_ = past_out_2_ = 0.0;

    in_0_ = target_in_0_;
    in_1_ = target_in_1_;
    in_2_ = target_in_2_;
    out_1_ = target_out_1_;
    out_2_ = target_out_2_;
  }

} // namespace mopo
