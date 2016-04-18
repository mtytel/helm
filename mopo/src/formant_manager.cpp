/* Copyright 2013-2016 Matt Tytel
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

#include "filter.h"
#include "operators.h"

namespace mopo {

  FormantManager::FormantManager(int num_formants) : ProcessorRouter(0, 0) {
    Bypass* audio_input = new Bypass();
    Bypass* reset_input = new Bypass();

    registerInput(audio_input->input(), kAudio);
    registerInput(reset_input->input(), kReset);

    addProcessor(audio_input);
    addProcessor(reset_input);

    Processor* audio_flow = audio_input;
    for (int i = 0; i < num_formants; ++i) {
      Filter* formant = new Filter();
      formant->plug(audio_flow, Filter::kAudio);
      formant->plug(reset_input, Filter::kReset);
      formants_.push_back(formant);
      addProcessor(formant);
      audio_flow = formant;
    }

    registerOutput(audio_flow->output());
  }
} // namespace mopo
