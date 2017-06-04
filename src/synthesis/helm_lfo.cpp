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

#include "helm_lfo.h"
#include "common.h"
#include "utils.h"

#include <cmath>

namespace mopo {

  namespace {
    mopo_float randomLfoValue() {
      return 2.0 * rand() / RAND_MAX - 1.0;
    }
  } // namespace


  HelmLfo::HelmLfo() : Processor(kNumInputs, kNumOutputs, true), offset_(0.0),
                       last_random_value_(0.0), current_random_value_(0.0) { }

  void HelmLfo::process() {
    int num_samples = samples_to_process_;

    if (input(kReset)->source->triggered) {
      num_samples = samples_to_process_ - input(kReset)->source->trigger_offset;
      offset_ = 0.0;
      last_random_value_ = current_random_value_;
      current_random_value_ = randomLfoValue();
    }

    Wave::Type waveform = static_cast<Wave::Type>(static_cast<int>(input(kWaveform)->at(0)));
    mopo_float frequency = input(kFrequency)->at(0);
    mopo_float phase = input(kPhase)->at(0);

    offset_ += num_samples * frequency / sample_rate_;

    mopo_float offset_integral;
    offset_ = utils::mod(offset_, &offset_integral);

    mopo_float phase_integral;
    mopo_float phased_offset = utils::mod(offset_ + phase, &phase_integral);

    output(kOscPhase)->buffer[0] = phased_offset;

    if (waveform < Wave::kWhiteNoise)
      output(kValue)->buffer[0] = Wave::wave(waveform, phased_offset);
    else {
      if (offset_integral) {
        last_random_value_ = current_random_value_;
        current_random_value_ = randomLfoValue();
      }
      if (waveform == Wave::kWhiteNoise)
        output(kValue)->buffer[0] = current_random_value_;
      else {
        // Smooth random value.
        mopo_float t = (1.0 - cos(PI * phased_offset)) / 2.0;
        output(kValue)->buffer[0] = utils::interpolate(last_random_value_,
                                                       current_random_value_, t);
      }
    }
  }

  void HelmLfo::correctToTime(mopo_float samples) {
    mopo_float frequency = input(kFrequency)->at(0);
    offset_ = samples * frequency / sample_rate_;
    mopo_float integral;
    offset_ = utils::mod(offset_, &integral);
  }
} // namespace mopo
