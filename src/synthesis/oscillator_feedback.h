/* Copyright 2013-2015 Matt Tytel
 *
 * twytch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * twytch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with twytch.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef OSCILLATOR_FEEDBACK_H
#define OSCILLATOR_FEEDBACK_H

#include "twytch_module.h"

namespace mopo {

  class OscillatorFeedback : public TwytchModule {
    public:
      OscillatorFeedback(Output* audio, Output* midi);

      void init() override;

      virtual Processor* clone() const { return new OscillatorFeedback(*this); }

    protected:
      Output* in_audio_;
      Output* in_midi_;
      SimpleDelay* feedback_;
  };
} // namespace mopo

#endif // OSCILLATOR_FEEDBACK_H
