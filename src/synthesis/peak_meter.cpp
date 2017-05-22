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

  PeakMeter::PeakMeter() : Processor(1, 1), current_db_(0.0) {
    min_gain_ = utils::dbToGain(MIN_DB);
  }

  void PeakMeter::process() {

    mopo_float peak = utils::peak(input()->source->buffer, buffer_size_);
    mopo_float db = utils::gainToDb(std::max(min_gain_, peak));

    mopo_float exponent = buffer_size_ * (1.0 * mopo::DEFAULT_SAMPLE_RATE) / sample_rate_;
    mopo_float delta = fabs(db - current_db_);
    mopo_float decay = pow(1.0 - PEAK_DECAY * (1.0 + DELTA_SCALE * delta), exponent);
    current_db_ *= decay;

    current_db_ = std::max(current_db_, db - MIN_DB);
    output()->buffer[0] = current_db_ + MIN_DB;
  }
} // namespace mopo
