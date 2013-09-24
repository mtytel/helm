/* Copyright 2013 Little IO
 *
 * laf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * laf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with laf.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "operators.h"

#include "midi_lookup.h"

namespace laf {

  UnaryOperator::UnaryOperator() : Processor(1, 1) { }

  BinaryOperator::BinaryOperator() : Processor(2, 1) { }

  void Clamp::process() {
    for (int i = 0; i < BUFFER_SIZE; ++i)
      outputs_[0]->buffer[i] = CLAMP(inputs_[0]->at(i), -1, 1);
  }

  void Negate::process() {
    for (int i = 0; i < BUFFER_SIZE; ++i)
      outputs_[0]->buffer[i] = -inputs_[0]->at(i);
  }

  void LinearScale::process() {
    for (int i = 0; i < BUFFER_SIZE; ++i)
      outputs_[0]->buffer[i] = scale_ * inputs_[0]->at(i);
  }

  void MidiScale::process() {
    for (int i = 0; i < BUFFER_SIZE; ++i) {
      outputs_[0]->buffer[i] =
          MidiLookup::centsLookup(CENTS_PER_NOTE * inputs_[0]->at(i));
    }
  }

  void Add::process() {
    for (int i = 0; i < BUFFER_SIZE; ++i)
      outputs_[0]->buffer[i] = inputs_[0]->at(i) + inputs_[1]->at(i);
  }

  void Subtract::process() {
    for (int i = 0; i < BUFFER_SIZE; ++i)
      outputs_[0]->buffer[i] = inputs_[0]->at(i) - inputs_[1]->at(i);
  }

  void Multiply::process() {
    for (int i = 0; i < BUFFER_SIZE; ++i)
      outputs_[0]->buffer[i] = inputs_[0]->at(i) * inputs_[1]->at(i);
  }

  Interpolate::Interpolate() : Processor(kNumInputs, 1) { }

  void Interpolate::process() {
    for (int i = 0; i < BUFFER_SIZE; ++i) {
      outputs_[0]->buffer[i] = INTERPOLATE(inputs_[kFrom]->at(i),
                                           inputs_[kTo]->at(i),
                                           inputs_[kFractional]->at(i));
    }
  }
} // namespace laf
