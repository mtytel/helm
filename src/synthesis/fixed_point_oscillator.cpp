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

  FixedPointOscillator::FixedPointOscillator() : Processor(kNumInputs, 1) {
    phase_ = 0;
  }

  void FixedPointOscillator::process() {
    int phase_inc = UINT_MAX * input(kPhaseInc)->source->buffer[0];

    int waveform = static_cast<int>(input(kWaveform)->source->buffer[0] + 0.5);
    waveform = mopo::utils::clamp(waveform, 0, FixedPointWaveLookup::kWhiteNoise - 1);
    int* wave_buffer = FixedPointWave::getBuffer(waveform, 2.0 * phase_inc);

    mopo_float shuffle = 1.0 - input(kShuffle)->source->buffer[0];
    mopo_float shuffle_end = 1.0 - input(kShuffle)->source->buffer[buffer_size_ - 1];
    unsigned int shuffle_index = INT_MAX * shuffle;
    unsigned int shuffle_end_index = INT_MAX * shuffle_end;
    mopo_float shuffle_delta = (shuffle_end - shuffle) / buffer_size_;
    int shuffle_index_delta = (shuffle_end_index - shuffle_index) / buffer_size_;

    int i = 0;
    if (input(kReset)->source->triggered) {
      int trigger_offset = input(kReset)->source->trigger_offset;
      for (; i < trigger_offset; ++i) {
        tick(i, wave_buffer, phase_inc, shuffle, shuffle_index);
        shuffle += shuffle_delta;
        shuffle_index += shuffle_index_delta;
      }

      phase_ = 0;
    }
    for (; i < buffer_size_; ++i) {
      tick(i, wave_buffer, phase_inc, shuffle, shuffle_index);
      shuffle += shuffle_delta;
      shuffle_index += shuffle_index_delta;
    }
  }
} // namespace mopo
