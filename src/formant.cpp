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

  Formant::Formant() : ProcessorRouter(0, 0) {
    Filter* filter = new Filter();
    registerInput(filter->input(Filter::kAudio), kAudio);
    registerInput(filter->input(Filter::kResonance), kResonance);
    registerInput(filter->input(Filter::kCutoff), kFrequency);
    registerInput(filter->input(Filter::kReset), kReset);
    registerInput(filter->input(Filter::kType), kType);

    Multiply* total = new Multiply();
    registerInput(total->input(0), kGain);
    total->plug(filter, 1);

    addProcessor(filter);
    addProcessor(total);
    registerOutput(total->output());
  }
} // namespace mopo
