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

#include "state_variable_filter.h"
#include "utils.h"

#include <cmath>

#define MIN_RESONANCE 0.1
#define MAX_RESONANCE 16.0
#define MIN_CUTTOFF 1.0

namespace mopo {

  StateVariableFilter::StateVariableFilter() : Processor(StateVariableFilter::kNumInputs, 1) {
    a1_ = a2_ = a3_ = 0.0;
    m0_ = m1_ = m2_ = 0.0;
    v0_a_ = v1_a_ = v2_a_ = 0.0;
    v0_b_ = v1_b_ = v2_b_ = 0.0;
    last_out_a_ = last_out_b_ = 0.0;
    reset();
  }

  void StateVariableFilter::process() {
    Type type = static_cast<Type>(static_cast<int>(input(kType)->at(0)));
    mopo_float cutoff = utils::clamp(input(kCutoff)->at(0), MIN_CUTTOFF, sample_rate_);
    mopo_float resonance = utils::clamp(input(kResonance)->at(0),
                                        MIN_RESONANCE, MAX_RESONANCE);
    computeCoefficients(type, cutoff, resonance, input(kGain)->at(0), true);

    const mopo_float* audio_buffer = input(kAudio)->source->buffer;
    mopo_float* dest = output()->buffer;
    process24db(audio_buffer, dest);
  }

  void StateVariableFilter::process12db(const mopo_float* audio_buffer, mopo_float* dest) {
    if (inputs_->at(kReset)->source->triggered &&
        inputs_->at(kReset)->source->trigger_value == kVoiceReset) {
      int trigger_offset = inputs_->at(kReset)->source->trigger_offset;
      int i = 0;
      for (; i < trigger_offset; ++i) {
        tick(i, dest, audio_buffer);
      }

      reset();

      for (; i < buffer_size_; ++i)
        tick(i, dest, audio_buffer);
    }
    else {
      for (int i = 0; i < buffer_size_; ++i) {
        tick(i, dest, audio_buffer);
      }
    }
  }

  void StateVariableFilter::process24db(const mopo_float* audio_buffer, mopo_float* dest) {
    if (inputs_->at(kReset)->source->triggered &&
        inputs_->at(kReset)->source->trigger_value == kVoiceReset) {
      int trigger_offset = inputs_->at(kReset)->source->trigger_offset;
      int i = 0;
      for (; i < trigger_offset; ++i) {
        tick24db(i, dest, audio_buffer);
      }

      reset();

      for (; i < buffer_size_; ++i)
        tick24db(i, dest, audio_buffer);
    }
    else {
      for (int i = 0; i < buffer_size_; ++i) {
        tick24db(i, dest, audio_buffer);
      }
    }
  }

  void StateVariableFilter::computeCoefficients(Type type,
                                                mopo_float cutoff,
                                                mopo_float resonance,
                                                mopo_float gain,
                                                bool db24) {
    MOPO_ASSERT(resonance > 0.0);
    MOPO_ASSERT(cutoff > 0.0);
    MOPO_ASSERT(gain >= 0.0);

    gain = sqrt(gain);
    if (db24) {
      gain = sqrt(gain);
      resonance = sqrt(resonance);
    }

    mopo_float g = tan(PI * utils::min(cutoff / sample_rate_, 0.5));
    mopo_float k = 1.0 / resonance;

    switch(type) {
      case kLowPass: {
        m0_ = 0.0;
        m1_ = 0.0;
        m2_ = 1.0;
        break;
      }
      case kHighPass: {
        m0_ = 1.0;
        m1_ = -k;
        m2_ = -1.0;
        break;
      }
      case kBandPass: {
        m0_ = 0.0;
        m1_ = 1.0;
        m2_ = 0.0;
        break;
      }
      case kAllPass: {
        m0_ = 1.0;
        m1_ = -2.0 * k;
        m2_ = 0.0;
        break;
      }
      case kLowShelf: {
        g /= sqrt(gain);
        m0_ = 1.0;
        m1_ = k * (gain - 1.0);
        m2_ = gain * gain - 1.0;
        break;
      }
      case kHighShelf: {
        g *= sqrt(gain);
        m0_ = gain * gain;
        m1_ = k * gain * (1.0 - gain);
        m2_ = 1.0 - gain * gain;
        break;
      }
      case kBandShelf: {
        k /= gain;
        m0_ = 1.0;
        m1_ = k * (gain * gain - 1);
        m2_ = 0.0;
        break;
      }
      case kNotch: {
        break;
      }
      default: {
        m0_ = 0.0;
        m1_ = 0.0;
        m2_ = 0.0;
      }
    }
    
    a1_ = 1.0 / (1.0 + g * (g + k));
    a2_ = g * a1_;
    a3_ = g * a2_;
  }

  inline void StateVariableFilter::tick(int i, mopo_float* dest, const mopo_float* audio_buffer) {
    mopo_float audio = audio_buffer[i];

    mopo_float v3_a = audio - ic2eq_a_;
    v1_a_ = a1_ * ic1eq_a_ + a2_ * v3_a;
    v2_a_ = ic2eq_a_ + a2_ * ic1eq_a_ + a3_ * v3_a;
    ic1eq_a_ = 2.0 * v1_a_ - ic1eq_a_;
    ic2eq_a_ = 2.0 * v2_a_ - ic2eq_a_;

    mopo_float new_out = m0_ * audio + m1_ * v1_a_ + m2_ * v2_a_;
    mopo_float delta = new_out - last_out_a_;
    last_out_a_ = tanh(last_out_a_) + delta;
    dest[i] = last_out_a_;
  }

  inline void StateVariableFilter::tick24db(int i, mopo_float* dest, const mopo_float* audio_buffer) {
    mopo_float audio = audio_buffer[i];

    mopo_float v3_a = audio - ic2eq_a_;
    v1_a_ = a1_ * ic1eq_a_ + a2_ * v3_a;
    v2_a_ = ic2eq_a_ + a2_ * ic1eq_a_ + a3_ * v3_a;
    ic1eq_a_ = 2.0 * v1_a_ - ic1eq_a_;
    ic2eq_a_ = 2.0 * v2_a_ - ic2eq_a_;
    mopo_float new_stage1_out = m0_ * audio + m1_ * v1_a_ + m2_ * v2_a_;
    mopo_float delta_a = new_stage1_out - last_out_a_;
    last_out_a_ = tanh(last_out_a_) + delta_a;

    mopo_float v3_b = last_out_a_ - ic2eq_b_;
    v1_b_ = a1_ * ic1eq_b_ + a2_ * v3_b;
    v2_b_ = ic2eq_b_ + a2_ * ic1eq_b_ + a3_ * v3_b;
    ic1eq_b_ = 2.0 * v1_b_ - ic1eq_b_;
    ic2eq_b_ = 2.0 * v2_b_ - ic2eq_b_;

    mopo_float new_stage2_out = m0_ * last_out_a_ + m1_ * v1_b_ + m2_ * v2_b_;
    mopo_float delta_b = new_stage2_out - last_out_b_;
    last_out_b_ = tanh(last_out_b_) + delta_b;
    dest[i] = last_out_b_;
  }

  void StateVariableFilter::reset() {
    ic1eq_a_ = 0.0;
    ic2eq_a_ = 0.0;
    ic1eq_b_ = 0.0;
    ic2eq_b_ = 0.0;
  }

} // namespace mopo
