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

#include "ladder_filter.h"
#include "utils.h"

#include <cmath>

#define MIN_RESONANCE 0.0
#define MAX_RESONANCE 4.0
#define MIN_CUTTOFF 1.0

#define TWO_THERMAL_VOLTAGE 0.624

namespace mopo {

  LadderFilter::LadderFilter() : Processor(LadderFilter::kNumInputs, 1) {
    current_resonance_ = 0.0;
    current_drive_ = 1.0f;
    reset();
  }

  void LadderFilter::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kAudio));

    mopo_float cutoff = utils::clamp(input(kCutoff)->at(0), MIN_CUTTOFF, sample_rate_);

    mopo_float g = g_;
    computeCoefficients(cutoff);
    mopo_float resonance = utils::clamp(resonance_multiple_ * input(kResonance)->at(0) / 4.0,
                                        MIN_RESONANCE, MAX_RESONANCE);
    mopo_float drive = -input(kDrive)->at(0);
    mopo_float delta_drive = (drive - current_drive_) / buffer_size_;

    mopo_float delta_resonance = (resonance - current_resonance_) / buffer_size_;
    mopo_float delta_g = (g_ - g) / buffer_size_;


    const mopo_float* audio_buffer = input(kAudio)->source->buffer;
    mopo_float* dest = output()->buffer;
    double two_sr = sample_rate_ * 2.0;
    if (input(kReset)->source->triggered &&
        input(kReset)->source->trigger_value == kVoiceReset) {

      int trigger_offset = input(kReset)->source->trigger_offset;
      int i = 0;
      for (; i < trigger_offset; ++i) {
        g += delta_g;
        current_resonance_ += delta_resonance;
        current_drive_ += delta_drive;
        tick(i, dest, audio_buffer, g, current_resonance_, two_sr);
        tick(i, dest, audio_buffer, g, current_resonance_, two_sr);
      }

      reset();
      current_resonance_ = resonance;
      current_drive_ = drive;

      for (; i < buffer_size_; ++i) {
        tick(i, dest, audio_buffer, g_, resonance, two_sr);
        tick(i, dest, audio_buffer, g_, resonance, two_sr);
      }
    }
    else {
      for (int i = 0; i < buffer_size_; ++i) {
        g += delta_g;
        current_resonance_ += delta_resonance;
        current_drive_ += delta_drive;
        tick(i, dest, audio_buffer, g, current_resonance_, two_sr);
        tick(i, dest, audio_buffer, g, current_resonance_, two_sr);
      }
    }

    current_resonance_ = resonance;
    current_drive_ = drive;
  }

  inline void LadderFilter::tick(int i, mopo_float* dest, const mopo_float* audio_buffer,
                                 mopo_float g, mopo_float resonance, mopo_float two_sr) {
    mopo_float audio = audio_buffer[i] * current_drive_;

    mopo_float new_tan = utils::quickTanh((audio + resonance * v_[3]) / TWO_THERMAL_VOLTAGE);
    mopo_float delta_v0 = -g * (new_tan + tanh_v_[0]);
    v_[0] += (delta_v0 + delta_v_[0]) / two_sr;
    delta_v_[0] = delta_v0;
    tanh_v_[0] = utils::quickTanh(v_[0] / TWO_THERMAL_VOLTAGE);

    mopo_float delta_v1 = g * (tanh_v_[0] - tanh_v_[1]);
    v_[1] += (delta_v1 + delta_v_[1]) / two_sr;
    delta_v_[1] = delta_v1;
    tanh_v_[1] = utils::quickTanh(v_[1] / TWO_THERMAL_VOLTAGE);

    mopo_float delta_v2 = g * (tanh_v_[1] - tanh_v_[2]);
    v_[2] += (delta_v2 + delta_v_[2]) / two_sr;
    delta_v_[2] = delta_v2;
    tanh_v_[2] = utils::quickTanh(v_[2] / TWO_THERMAL_VOLTAGE);

    mopo_float delta_v3 = g * (tanh_v_[2] - tanh_v_[3]);
    v_[3] += (delta_v3 + delta_v_[3]) / two_sr;
    delta_v_[3] = delta_v3;
    tanh_v_[3] = utils::quickTanh(v_[3] / TWO_THERMAL_VOLTAGE);

    dest[i] = v_[3];
  }

  void LadderFilter::computeCoefficients(mopo_float cutoff) {
    MOPO_ASSERT(cutoff > 0.0);

    mopo_float delta_phase = (mopo::PI * cutoff * 0.5) / sample_rate_;
    resonance_multiple_ = 1.0 / (-1.273 * delta_phase * delta_phase + 3.5 * delta_phase + 0.7);
    g_ = mopo::PI * TWO_THERMAL_VOLTAGE * cutoff * (1.0 - delta_phase) / (1.0 + delta_phase);
  }

  void LadderFilter::reset() {
    memset(v_, 0, sizeof(v_));
    memset(delta_v_, 0, sizeof(delta_v_));
    memset(tanh_v_, 0, sizeof(tanh_v_));
  }

} // namespace mopo
