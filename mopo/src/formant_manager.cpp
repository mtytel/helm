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

  FormantManager::FormantManager(int num_formants) : ProcessorRouter(0, 0) {
    Bypass* audio_input = new Bypass();
    Bypass* reset_input = new Bypass();

    Multiply* passthrough = new Multiply();
    passthrough->plug(audio_input, 0);
    registerInput(audio_input->input(), kAudio);
    registerInput(reset_input->input(), kReset);
    registerInput(passthrough->input(1), kPassthroughGain);

    VariableAdd* total = new VariableAdd();
    total->plugNext(passthrough);

    addProcessor(audio_input);
    addProcessor(reset_input);
    addProcessor(passthrough);

    for (int i = 0; i < num_formants; ++i) {
      Formant* formant = new Formant();
      formant->plug(audio_input, Formant::kAudio);
      formants_.push_back(formant);
      addProcessor(formant);
      total->plugNext(formant);
    }

    addProcessor(total);
    registerOutput(total->output());
  }
} // namespace mopo
