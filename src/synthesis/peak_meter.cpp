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

#define PEAK_DECAY 0.00000005
#define DELTA_SCALE 5.0
#define MIN_DB (-80)

namespace mopo {

  PeakMeter::PeakMeter() : Processor(2, 1), current_db_left_(0.0), current_db_right_(0.0) {
    min_gain_ = utils::dbToGain(MIN_DB);
  }

  void PeakMeter::process() {
    mopo_float peak_left = utils::peak(input(0)->source->buffer, buffer_size_);
    mopo_float peak_right = utils::peak(input(1)->source->buffer, buffer_size_);
    mopo_float db_left = utils::gainToDb(std::max(min_gain_, peak_left));
    mopo_float db_right = utils::gainToDb(std::max(min_gain_, peak_right));

    mopo_float exponent = buffer_size_ * (1.0 * mopo::DEFAULT_SAMPLE_RATE) / sample_rate_;

    mopo_float delta_left = fabs(db_left - current_db_left_);
    mopo_float delta_right = fabs(db_right - current_db_right_);
    mopo_float decay_left = pow(1.0 - PEAK_DECAY * (1.0 + DELTA_SCALE * delta_left), exponent);
    mopo_float decay_right = pow(1.0 - PEAK_DECAY * (1.0 + DELTA_SCALE * delta_right), exponent);
    current_db_left_ *= decay_left;
    current_db_right_ *= decay_right;

    current_db_left_ = std::max(current_db_left_, db_left - MIN_DB);
    current_db_right_ = std::max(current_db_right_, db_right - MIN_DB);
    output()->buffer[0] = current_db_left_ + MIN_DB;
    output()->buffer[1] = current_db_right_ + MIN_DB;
  }
} // namespace mopo
