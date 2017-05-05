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

#pragma once
#ifndef FORMANT_MANAGER_H
#define FORMANT_MANAGER_H

#include "processor_router.h"

#include <complex>

namespace mopo {

  class BiquadFilter;

  class FormantManager : public ProcessorRouter {
    public:
      enum Inputs {
        kAudio,
        kReset,
        kNumInputs
      };

      FormantManager(int num_formants = 4);

      virtual Processor* clone() const override {
        return new FormantManager(*this);
      }

      BiquadFilter* getFormant(int index = 0) { return formants_[index]; }
      int num_formants() { return formants_.size(); }

      std::complex<mopo_float> getResponse(mopo_float frequency);

      mopo_float getAmplitudeResponse(mopo_float frequency) {
        return std::abs(getResponse(frequency));
      }

      mopo_float getPhaseResponse(mopo_float frequency) {
        return std::arg(getResponse(frequency));
      }

    protected:
      std::vector<BiquadFilter*> formants_;
  };
} // namespace mopo

#endif // FORMANT_MANAGER_H
