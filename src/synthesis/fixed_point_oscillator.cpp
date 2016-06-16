/* Copyright 2013-2016 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fixed_point_oscillator.h"

namespace mopo {

  const mopo_float FixedPointOscillator::SCALE_OUT = 0.5 / (FixedPointWaveLookup::SCALE * INT_MAX);

  FixedPointOscillator::FixedPointOscillator() : Processor(kNumInputs, 1), phase_(0) { }

  void FixedPointOscillator::process() {
    int phase_inc = UINT_MAX * input(kPhaseInc)->source->buffer[0];
    mopo_float shuffle = utils::clamp(1.0 - input(kShuffle)->source->buffer[0], 0.0, 1.0);
    mopo_float* dest = output()->buffer;

    unsigned int shuffle_index = INT_MAX * shuffle;

    int waveform = static_cast<int>(input(kWaveform)->source->buffer[0] + 0.5);
    waveform = mopo::utils::clamp(waveform, 0, FixedPointWaveLookup::kWhiteNoise - 1);
    int* wave_buffer = FixedPointWave::getBuffer(waveform, 2.0 * phase_inc);

    mopo_float first_adjust = bool(shuffle) * 2.0 / shuffle;
    mopo_float second_adjust = 1.0 / (1.0 - 0.5 * shuffle);

    if (input(kReset)->source->triggered)
      phase_ = 0;

    int i = 0;
    unsigned int buffer_size = buffer_size_;
    unsigned int current_phase = 0;
    while (i < buffer_size) {
      if (phase_ < shuffle_index) {
        unsigned int max_samples = (shuffle_index - phase_) / phase_inc + 1;
        unsigned int samples = std::min(buffer_size, i + max_samples);
        for (; i < samples; ++i) {
          phase_ += phase_inc;
          current_phase = phase_ * first_adjust;
          dest[i] = SCALE_OUT * wave_buffer[FixedPointWave::getIndex(current_phase)];
        }
      }

      unsigned int max_samples = -phase_ / phase_inc + 1;
      unsigned int samples = std::min(buffer_size, i + max_samples);
      for (; i < samples; ++i) {
        phase_ += phase_inc;
        current_phase = (phase_ - shuffle_index) * second_adjust;
        dest[i] = SCALE_OUT * wave_buffer[FixedPointWave::getIndex(current_phase)];
      }
    }
  }
} // namespace mopo
