/* Copyright 2013-2015 Matt Tytel
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

#include "formant.h"

#include "filter.h"
#include "operators.h"

namespace mopo {

  Formant::Formant() : ProcessorRouter(kNumInputs, 1) {
    static const Value filter_type(Filter::kBandPass);

    Filter* filter = new Filter();
    filter->registerInput(input(kAudio), Filter::kAudio);
    filter->registerInput(input(kResonance), Filter::kResonance);
    filter->registerInput(input(kFrequency), Filter::kCutoff);
    filter->registerInput(input(kReset), Filter::kReset);
    filter->plug(&filter_type, Filter::kType);

    Multiply* total = new Multiply();
    total->registerInput(input(kGain), 0);
    total->plug(filter, 1);

    addProcessor(filter);
    addProcessor(total);
    total->registerOutput(output(), 0);
  }
} // namespace mopo
