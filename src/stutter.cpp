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

#include "stutter.h"
#include "utils.h"

#define MIN_SOFTNESS 0.00001

namespace mopo {

  namespace {
    inline mopo_float computeAmplitude(mopo_float offset, mopo_float period, mopo_float softness) {
      mopo_float progress = offset / period;
      mopo_float phase_setup = std::fabs(utils::interpolate(-softness, softness, progress));
      mopo_float phase = utils::clamp(phase_setup - softness + PI, 0.0, PI);
      return 0.5 * cos(phase) + 0.5;
    }
  } // namespace

  Stutter::Stutter(int size) : Processor(Stutter::kNumInputs, 1),
      memory_(nullptr), size_(size), offset_(0.0), memory_offset_(0.0), resample_countdown_(0.0),
      last_softness_(0.0), last_stutter_period_(0.0), resampling_(true) {
  }

  Stutter::~Stutter() {
    delete memory_;
  }

  Stutter::Stutter(const Stutter& other) : Processor(other) {
    this->memory_ = nullptr;
    this->size_ = other.size_;
    this->offset_ = other.offset_;
    this->memory_offset_ = 0.0;
    this->resample_countdown_ = other.resample_countdown_;
    this->last_softness_ = other.last_softness_;
    this->last_stutter_period_ = other.last_stutter_period_;
    this->resampling_ = other.resampling_;
  }

  void Stutter::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kAudio));

    // A hack to save memory until stutter is used.
    if (memory_ == nullptr)
      memory_ = new Memory(size_);

    mopo_float max_memory_write = memory_->getSize();
    mopo_float* dest = output()->buffer;

    mopo_float sample_period = sample_rate_ / input(kResampleFrequency)->at(0);
    mopo_float end_stutter_period = sample_rate_ / input(kStutterFrequency)->at(0);
    end_stutter_period = utils::min(sample_period, end_stutter_period);
    end_stutter_period = utils::min(max_memory_write, end_stutter_period);
    mopo_float read_softenss = utils::max(input(kWindowSoftness)->at(0), MIN_SOFTNESS);
    mopo_float end_softness = PI * utils::max(1.0, 1.0 / read_softenss);

    mopo_float stutter_period = end_stutter_period;
    if (last_stutter_period_)
      stutter_period = last_stutter_period_;

    mopo_float stutter_period_diff = (end_stutter_period - stutter_period) / buffer_size_;

    mopo_float softness = last_softness_;
    mopo_float softness_diff = (end_softness - last_softness_) / buffer_size_;

    if (input(kReset)->source->triggered) {
    }

    int buffer_size = buffer_size_;
    int trigger_offset = -1;
    if (input(kReset)->source->triggered) {
      trigger_offset = input(kReset)->source->trigger_offset;
      buffer_size = trigger_offset;
    }
    else if (resample_countdown_ > sample_period)
      resample_countdown_ = sample_period;

    int i = 0;
    while (i < buffer_size_) {
      if (resampling_) {
        int max_samples = std::ceil(stutter_period - offset_);
        int samples = std::min(buffer_size, i + max_samples);
        int num_samples = samples - i;

        for (; i < samples; ++i) {
          stutter_period += stutter_period_diff;
          softness += softness_diff;

          mopo_float audio = input(kAudio)->at(i);
          memory_->push(audio);

          mopo_float amp = computeAmplitude(offset_, stutter_period, softness);
          dest[i] = amp * audio;
          offset_ += 1.0;
        }
        resample_countdown_ -= num_samples;
        memory_offset_ += num_samples;
      }
      else {
        int max_samples = std::ceil(std::min(stutter_period - offset_, resample_countdown_));
        int samples = std::min(buffer_size, i + max_samples);

        if (memory_offset_ < max_memory_write) {
          int mem_samples = std::min<int>(max_memory_write - memory_offset_, samples);
          for (int j = i; j < mem_samples; ++j)
            memory_->push(input(kAudio)->at(j));
          memory_offset_ += (mem_samples - i);
        }

        for (; i < samples; ++i) {
          resample_countdown_ -= 1.0;

          stutter_period += stutter_period_diff;
          softness += softness_diff;

          mopo_float offset = std::min(offset_, resample_countdown_);
          offset = std::min(stutter_period - offset_, offset);
          mopo_float amp = computeAmplitude(offset, stutter_period, softness);

          dest[i] = amp * memory_->get(memory_offset_ - offset_);
          offset_ += 1.0;
        }

        if (resample_countdown_ <= 0.0)
          startResampling(sample_period);
      }

      if (offset_ >= stutter_period) {
        resampling_ = false;
        offset_ = 0.0;
      }

      if (buffer_size == trigger_offset) {
        buffer_size = buffer_size_;
        startResampling(sample_period);

        stutter_period = end_stutter_period;
        stutter_period_diff = 0.0;
        softness = end_softness;
        softness_diff = 0.0;
      }
    }
    last_stutter_period_ = end_stutter_period;
    last_softness_ = end_softness;
  }
} // namespace mopo
