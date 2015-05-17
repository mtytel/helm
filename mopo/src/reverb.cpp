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

#include "reverb.h"

#include "operators.h"
#include "reverb_all_pass.h"
#include "reverb_comb.h"
#include "reverb_tuning.h"

namespace mopo {

  Reverb::Reverb() : ProcessorRouter(kNumInputs, 0) {
    static const Value* gain = new Value(FIXED_GAIN);
    Multiply* gained_input = new Multiply();
    Bypass* feedback_input = new Bypass();
    Bypass* damping_input = new Bypass();

    registerInput(gained_input->input(0), kAudio);
    registerInput(feedback_input->input(), kFeedback);
    registerInput(damping_input->input(), kDamping);

    gained_input->plug(gain, 1);
    VariableAdd* comb_total = new VariableAdd();

    for (int i = 0; i < NUM_COMB; ++i) {
      ReverbComb* comb = new ReverbComb(REVERB_MAX_MEMORY);
      Value* time = new Value(COMB_TUNINGS[i]);
      TimeToSamples* samples = new TimeToSamples();
      samples->plug(time);

      comb->plug(damping_input, ReverbComb::kDamping);
      comb->plug(feedback_input, ReverbComb::kFeedback);
      comb->plug(gained_input, ReverbComb::kAudio);
      comb->plug(samples, ReverbComb::kSampleDelay);
      addProcessor(samples);
      addProcessor(comb);
      comb_total->plugNext(comb);
    }

    addProcessor(gained_input);
    addProcessor(feedback_input);
    addProcessor(damping_input);
    addProcessor(comb_total);

    Processor* audio = comb_total;
    for (int i = 0; i < NUM_ALL_PASS; ++i) {
      ReverbAllPass* all_pass = new ReverbAllPass(REVERB_MAX_MEMORY);
      Value* time = new Value(ALL_PASS_TUNINGS[i]);
      TimeToSamples* samples = new TimeToSamples();
      samples->plug(time);

      all_pass->plug(audio, ReverbAllPass::kAudio);
      all_pass->plug(samples, ReverbAllPass::kSampleDelay);
      all_pass->plug(&utils::value_half, ReverbAllPass::kFeedback);

      addProcessor(all_pass);
      addProcessor(samples);
      audio = all_pass;
    }

    registerOutput(audio->output());
  }
} // namespace mopo
