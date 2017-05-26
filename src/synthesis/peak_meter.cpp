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

#include "peak_meter.h"
#include "utils.h"

#define PEAK_DECAY 0.00003
#define DELTA_SCALE 20.0
#define MIN_MOVEMENT 0.00002

namespace mopo {

  PeakMeter::PeakMeter() : Processor(2, 1), current_peak_left_(0.0), current_peak_right_(0.0) {
  }

  void PeakMeter::process() {
    mopo_float peak_left = utils::peak(input(0)->source->buffer, buffer_size_, 8);
    mopo_float peak_right = utils::peak(input(1)->source->buffer, buffer_size_, 8);

    mopo_float exponent = buffer_size_ * (1.0 * mopo::DEFAULT_SAMPLE_RATE) / sample_rate_;
    mopo_float movement = MIN_MOVEMENT * exponent;

    mopo_float delta_left = fabs(peak_left - current_peak_left_);
    mopo_float delta_right = fabs(peak_right - current_peak_right_);
    mopo_float decay_left = pow(1.0 - PEAK_DECAY * (1.0 + DELTA_SCALE * delta_left), exponent);
    mopo_float decay_right = pow(1.0 - PEAK_DECAY * (1.0 + DELTA_SCALE * delta_right), exponent);
    current_peak_left_ *= decay_left;
    current_peak_right_ *= decay_right;

    current_peak_left_ = utils::max(current_peak_left_ - movement, peak_left);
    current_peak_right_ = utils::max(current_peak_right_ - movement, peak_right);
    output()->buffer[0] = current_peak_left_;
    output()->buffer[1] = current_peak_right_;
  }
} // namespace mopo
