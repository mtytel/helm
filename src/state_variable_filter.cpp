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
    target_m0_ = target_m1_ = target_m2_ = 0.0;

    last_in_ = last_distort_ = 0.0;

    drive_ = target_drive_ = 0.0;
    last_style_ = kNumStyles;
    last_shelf_ = kNumShelves;
    reset();
  }

  void StateVariableFilter::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kAudio));

    const mopo_float* audio_buffer = input(kAudio)->source->buffer;
    mopo_float* dest = output()->buffer;

    if (input(kOn)->at(0) == 0.0) {
      processAllPass(audio_buffer, dest);
      return;
    }

    Styles style = static_cast<Styles>(static_cast<int>(input(kStyle)->at(0)));
    bool db24 = style == k24dB;

    mopo_float cutoff = utils::clamp(input(kCutoff)->at(0), MIN_CUTTOFF, sample_rate_);
    mopo_float resonance = utils::clamp(input(kResonance)->at(0),
                                        MIN_RESONANCE, MAX_RESONANCE);
    target_drive_ = input(kDrive)->at(0);

    if (style == kShelf) {
      Shelves shelf_choice = static_cast<Shelves>(static_cast<int>(input(kShelfChoice)->at(0)));
      computeShelfCoefficients(shelf_choice, cutoff, input(kGain)->at(0));
    }
    else {
      mopo_float blend = input(kPassBlend)->at(0);
      computePassCoefficients(blend, cutoff, resonance, db24);
    }

    if (style != last_style_) {
      reset();
      last_style_ = style;
    }

    if (db24)
      process24db(audio_buffer, dest);
    else
      process12db(audio_buffer, dest);
  }

  void StateVariableFilter::process12db(const mopo_float* audio_buffer, mopo_float* dest) {
    mopo_float delta_m0 = (target_m0_ - m0_) / buffer_size_;
    mopo_float delta_m1 = (target_m1_ - m1_) / buffer_size_;
    mopo_float delta_m2 = (target_m2_ - m2_) / buffer_size_;
    mopo_float delta_drive = (target_drive_ - drive_) / buffer_size_;

    if (input(kReset)->source->triggered &&
        input(kReset)->source->trigger_value == kVoiceReset) {
      int trigger_offset = input(kReset)->source->trigger_offset;
      int i = 0;
      for (; i < trigger_offset; ++i) {
        m0_ += delta_m0;
        m1_ += delta_m1;
        m2_ += delta_m2;
        drive_ += delta_drive;
        tick(i, dest, audio_buffer);
      }

      reset();

      for (; i < buffer_size_; ++i)
        tick(i, dest, audio_buffer);
    }
    else {
      for (int i = 0; i < buffer_size_; ++i) {
        m0_ += delta_m0;
        m1_ += delta_m1;
        m2_ += delta_m2;
        drive_ += delta_drive;
        tick(i, dest, audio_buffer);
      }
    }
  }

  void StateVariableFilter::process24db(const mopo_float* audio_buffer, mopo_float* dest) {
    mopo_float delta_m0 = (target_m0_ - m0_) / buffer_size_;
    mopo_float delta_m1 = (target_m1_ - m1_) / buffer_size_;
    mopo_float delta_m2 = (target_m2_ - m2_) / buffer_size_;

    mopo_float delta_drive = (target_drive_ - drive_) / buffer_size_;

    if (inputs_->at(kReset)->source->triggered &&
        inputs_->at(kReset)->source->trigger_value == kVoiceReset) {
      int trigger_offset = inputs_->at(kReset)->source->trigger_offset;
      int i = 0;
      for (; i < trigger_offset; ++i) {
        m0_ += delta_m0;
        m1_ += delta_m1;
        m2_ += delta_m2;
        drive_ += delta_drive;
        tick24db(i, dest, audio_buffer);
      }

      reset();

      for (; i < buffer_size_; ++i)
        tick24db(i, dest, audio_buffer);
    }
    else {
      for (int i = 0; i < buffer_size_; ++i) {
        m0_ += delta_m0;
        m1_ += delta_m1;
        m2_ += delta_m2;
        drive_ += delta_drive;
        tick24db(i, dest, audio_buffer);
      }
    }

    m1_ = target_m1_;
  }

  void StateVariableFilter::processAllPass(const mopo_float* audio_buffer, mopo_float* dest) {
    reset();
    utils::copyBuffer(dest, audio_buffer, buffer_size_);
  }

  void StateVariableFilter::computePassCoefficients(mopo_float blend,
                                                    mopo_float cutoff,
                                                    mopo_float resonance,
                                                    bool db24) {
    MOPO_ASSERT(resonance > 0.0);
    MOPO_ASSERT(cutoff > 0.0);

    if (db24)
      resonance = sqrt(resonance);

    mopo_float g = tan(PI * utils::min(cutoff / sample_rate_, 0.5));
    mopo_float k = 1.0 / resonance;

    mopo_float low_pass_amount = sqrt(utils::clamp(1.0 - blend, 0.0, 1.0));
    mopo_float band_pass_amount = sqrt(utils::clamp(1.0 - fabs(blend - 1.0), 0.0, 1.0));
    mopo_float high_pass_amount = sqrt(utils::clamp(blend - 1.0, 0.0, 1.0));

    target_m0_ = 0.0;
    target_m1_ = band_pass_amount;
    target_m2_ = low_pass_amount;

    target_m0_ += high_pass_amount;
    target_m1_ += -k * high_pass_amount;
    target_m2_ += -high_pass_amount;
    
    a1_ = 1.0 / (1.0 + g * (g + k));
    a2_ = g * a1_;
    a3_ = g * a2_;
  }

  void StateVariableFilter::computeShelfCoefficients(Shelves choice,
                                                     mopo_float cutoff,
                                                     mopo_float gain) {
    MOPO_ASSERT(gain >= 0.0);
    MOPO_ASSERT(cutoff > 0.0);

    gain = sqrt(gain);

    mopo_float g = tan(PI * utils::min(cutoff / sample_rate_, 0.5));
    mopo_float k = 1.0;

    switch(choice) {
      case kLowShelf: {
        g /= sqrt(gain);
        target_m0_ = 1.0;
        target_m1_ = k * (gain - 1.0);
        target_m2_ = gain * gain - 1.0;
        break;
      }
      case kBandShelf: {
        k /= gain;
        target_m0_ = 1.0;
        target_m1_ = k * (gain * gain - 1.0);
        target_m2_ = 0.0;
        break;
      }
      case kHighShelf: {
        g *= sqrt(gain);
        target_m0_ = gain * gain;
        target_m1_ = k * gain * (1.0 - gain);
        target_m2_ = 1.0 - gain * gain;
        break;
      }
      default: {
        target_m0_ = 0.0;
        target_m1_ = 0.0;
        target_m2_ = 0.0;
      }
    }

    a1_ = 1.0 / (1.0 + g * (g + k));
    a2_ = g * a1_;
    a3_ = g * a2_;

    if (last_shelf_ != choice) {
      reset();
      last_shelf_ = choice;
    }
  }

  inline void StateVariableFilter::tick(int i, mopo_float* dest, const mopo_float* audio_buffer) {
    mopo_float audio = utils::quickTanh(drive_ * audio_buffer[i]);

    mopo_float v3_a = audio - ic2eq_a_;
    mopo_float v1_a = a1_ * ic1eq_a_ + a2_ * v3_a;
    mopo_float v2_a = ic2eq_a_ + a2_ * ic1eq_a_ + a3_ * v3_a;
    ic1eq_a_ = 2.0 * v1_a - ic1eq_a_;
    ic2eq_a_ = 2.0 * v2_a - ic2eq_a_;

    dest[i] = m0_ * audio + m1_ * v1_a + m2_ * v2_a;
  }

  inline void StateVariableFilter::tick24db(int i, mopo_float* dest,
                                            const mopo_float* audio_buffer) {
    mopo_float audio = drive_ * audio_buffer[i];

    mopo_float v3_a = audio - ic2eq_a_;
    mopo_float v1_a = a1_ * ic1eq_a_ + a2_ * v3_a;
    mopo_float v2_a = ic2eq_a_ + a2_ * ic1eq_a_ + a3_ * v3_a;
    ic1eq_a_ = 2.0 * v1_a - ic1eq_a_;
    ic2eq_a_ = 2.0 * v2_a - ic2eq_a_;
    mopo_float out_a = m0_ * audio + m1_ * v1_a + m2_ * v2_a;

    mopo_float distort = utils::quickTanh(out_a);

    mopo_float v3_b = distort - ic2eq_b_;
    mopo_float v1_b = a1_ * ic1eq_b_ + a2_ * v3_b;
    mopo_float v2_b = ic2eq_b_ + a2_ * ic1eq_b_ + a3_ * v3_b;
    ic1eq_b_ = 2.0 * v1_b - ic1eq_b_;
    ic2eq_b_ = 2.0 * v2_b - ic2eq_b_;

    dest[i] = m0_ * distort + m1_ * v1_b + m2_ * v2_b;
  }

  void StateVariableFilter::reset() {
    ic1eq_a_ = 0.0;
    ic2eq_a_ = 0.0;
    ic1eq_b_ = 0.0;
    ic2eq_b_ = 0.0;
    m0_ = target_m0_;
    m1_ = target_m1_;
    m2_ = target_m2_;
    drive_ = target_drive_;
    last_in_ = last_distort_ = 0.0;
  }

} // namespace mopo
