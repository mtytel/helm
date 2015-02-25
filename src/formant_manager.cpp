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

#include "formant_manager.h"

#include "formant.h"
#include "operators.h"

namespace mopo {

  FormantManager::FormantManager(int num_formants) :
      ProcessorRouter(kNumInputs, 0) {
    Multiply* passthrough = new Multiply();
    passthrough->registerInput(input(kAudio), 0);
    passthrough->registerInput(input(kPassthroughGain), 1);

    VariableAdd* total = new VariableAdd();
    total->plugNext(passthrough);

    for (int i = 0; i < num_formants; ++i) {
      Formant* formant = new Formant();
      formant->registerInput(input(kAudio), Formant::kAudio);
      formants_.push_back(formant);
      addProcessor(formant);
      total->plugNext(formant);
    }

    addProcessor(passthrough);
    addProcessor(total);
    registerOutput(total->output());
  }
} // namespace mopo
