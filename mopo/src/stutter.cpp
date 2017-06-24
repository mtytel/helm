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
      last_stutter_period_(0.0), last_amplitude_(0.0), resampling_(true) {
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
    this->last_amplitude_ = 0.0;
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
    const mopo_float* audio = input(kAudio)->source->buffer;

    mopo_float sample_period = sample_rate_ / input(kResampleFrequency)->at(0);
    mopo_float end_stutter_period = sample_rate_ / input(kStutterFrequency)->at(0);
    end_stutter_period = utils::min(sample_period, end_stutter_period);
    end_stutter_period = utils::min(max_memory_write, end_stutter_period);
    mopo_float read_softness = utils::max(input(kWindowSoftness)->at(0), MIN_SOFTNESS);
    mopo_float end_softness = PI * utils::max(1.0, 1.0 / read_softness);

    mopo_float stutter_period = end_stutter_period;
    if (last_stutter_period_)
      stutter_period = last_stutter_period_;

    mopo_float stutter_period_diff = (end_stutter_period - stutter_period) / buffer_size_;

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
        stutter_period += num_samples * stutter_period_diff;
        offset_ += num_samples;
        mopo_float end_amplitude = computeAmplitude(offset_, stutter_period, end_softness);
        mopo_float amplitude = last_amplitude_;
        mopo_float amplitude_diff = (end_amplitude - amplitude) / num_samples;

        for (; i < samples; ++i) {
          mopo_float audio_in = audio[i];
          memory_->push(audio_in);

          amplitude += amplitude_diff;
          dest[i] = amplitude * audio_in;
        }

        resample_countdown_ -= num_samples;
        memory_offset_ += num_samples;
        last_amplitude_ = end_amplitude;
      }
      else {
        int max_samples = std::ceil(std::min(stutter_period - offset_, resample_countdown_));
        int samples = std::min(buffer_size, i + max_samples);
        int num_samples = samples - i;
        if (memory_offset_ < max_memory_write) {
          int mem_samples = std::min<int>(max_memory_write - memory_offset_, samples);
          memory_->pushBlock(audio + i, std::max<int>(0, mem_samples - i));
          memory_offset_ += (mem_samples - i);
        }

        resample_countdown_ -= num_samples;
        mopo_float next_offset = offset_ + num_samples;

        stutter_period += num_samples * stutter_period_diff;
        mopo_float end_offset = std::min(stutter_period - next_offset, next_offset);
        end_offset = std::min(end_offset, resample_countdown_);
        mopo_float end_amplitude = computeAmplitude(end_offset, stutter_period, end_softness);
        mopo_float amplitude = last_amplitude_;
        mopo_float amplitude_diff = (end_amplitude - amplitude) / num_samples;

        mopo_float* playback_dest = dest + i;
        for (int s = 0; s < num_samples; ++s) {
          amplitude += amplitude_diff;
          playback_dest[s] = amplitude * memory_->get(memory_offset_ - offset_ - s);
        }

        i = samples;
        offset_ = next_offset;
        last_amplitude_ = end_amplitude;

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
      }
    }
    last_stutter_period_ = end_stutter_period;
  }
} // namespace mopo
