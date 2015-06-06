/* Copyright 2013-2015 Matt Tytel
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

#pragma once
#ifndef NOISE_OSCILLATOR_H
#define NOISE_OSCILLATOR_H

#include "mopo.h"

namespace mopo {

  class NoiseOscillator : public Processor {
    public:
      NoiseOscillator();

      virtual void process();
      virtual Processor* clone() const { return new NoiseOscillator(*this); }

    protected:
      void tick(int i) {
        output()->buffer[i] = Wave::whitenoise();
      }
  };
} // namespace mopo

#endif // NOISE_OSCILLATOR_H
