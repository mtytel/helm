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

#include "stutter.h"

#define MIN_SOFTNESS 0.00001

namespace mopo {

  namespace {
    inline mopo_float computeAmplitude(mopo_float offset, mopo_float period, mopo_float softness) {
      mopo_float progress = offset / period;
      mopo_float phase_setup = std::fabs(INTERPOLATE(-softness, softness, progress));
      mopo_float phase = utils::clamp(phase_setup - softness + PI, 0.0, PI);
      return 0.5 * cos(phase) + 0.5;
    }
  } // namespace

  Stutter::Stutter(int size) : Processor(Stutter::kNumInputs, 1),
      offset_(0.0), memory_offset_(0.0), resample_offset_(0.0),
      last_softness_(0.0), last_stutter_period_(0.0), resampling_(true) {
    memory_ = new Memory(size);
  }

  Stutter::~Stutter() {
    delete memory_;
  }

  Stutter::Stutter(const Stutter& other) : Processor(other) {
    this->memory_ = new Memory(*other.memory_);
    this->offset_ = other.offset_;
    this->memory_offset_ = 0.0;
    this->resample_offset_ = other.resample_offset_;
    this->last_softness_ = other.last_softness_;
    this->last_stutter_period_ = other.last_stutter_period_;
    this->resampling_ = other.resampling_;
  }

  void Stutter::process() {
    mopo_float max_memory_write = memory_->getSize() / 2.0;

    mopo_float sample_period = sample_rate_ / input(kResampleFrequency)->at(0);
    mopo_float end_stutter_period = sample_rate_ / input(kStutterFrequency)->at(0);
    end_stutter_period = std::min(sample_period, end_stutter_period);
    end_stutter_period = std::min(max_memory_write, end_stutter_period);
    mopo_float read_softenss = std::max(input(kWindowSoftness)->at(0), MIN_SOFTNESS);
    mopo_float softness = PI * std::max(1.0, 1.0 / read_softenss);

    mopo_float stutter_period = end_stutter_period;
    if (last_stutter_period_)
      stutter_period = last_stutter_period_;

    mopo_float stutter_period_diff = (end_stutter_period - stutter_period) / buffer_size_;

    if (input(kReset)->source->triggered) {
      resampling_ = true;
      resample_offset_ = sample_period;
      offset_ = stutter_period;
    }
    else if (resample_offset_ > sample_period)
      resample_offset_ = sample_period;

    int i = 0;
    while (i < buffer_size_) {
      if (resampling_) {
        int max_samples = offset_ + 1;
        int samples = std::min(buffer_size_, i + max_samples);

        for (; i < samples; ++i) {
          offset_ -= 1.0;
          stutter_period += stutter_period_diff;

          mopo_float audio = input(kAudio)->at(i);
          memory_->push(audio);

          mopo_float amp = computeAmplitude(offset_, stutter_period, softness);
          output()->buffer[i] = amp * audio;
        }
        resample_offset_ -= samples;
      }

      if (!resampling_) {
        int max_samples = std::min(offset_, resample_offset_) + 1;
        int samples = std::min(buffer_size_, i + max_samples);
        if (memory_offset_ < max_memory_write) {
          int mem_samples = std::min<int>(memory_->getSize() - memory_offset_, samples);
          for (int j = i; j < mem_samples; ++j)
            memory_->push(input(kAudio)->at(j));
          memory_offset_ += mem_samples;
        }

        for (; i < samples; ++i) {
          offset_ -= 1.0;
          resample_offset_ -= 1.0;
          stutter_period += stutter_period_diff;

          mopo_float offset = std::min(offset_, resample_offset_);
          offset = std::min(stutter_period - offset_, offset);
          mopo_float amp = computeAmplitude(offset, stutter_period, softness);

          output()->buffer[i] = amp * memory_->get(offset_ + memory_offset_);
        }

        if (resample_offset_ <= 0.0) {
          resampling_ = true;
          resample_offset_ += sample_period;
          offset_ = stutter_period;
        }
      }

      if (offset_ <= 0.0) {
        if (resampling_)
          memory_offset_ = 0.0;
        resampling_ = false;
        offset_ += stutter_period;
      }
    }
    last_stutter_period_ = end_stutter_period;
  }
} // namespace mopo
