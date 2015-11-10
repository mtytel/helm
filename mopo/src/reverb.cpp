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
    Bypass* audio_input = new Bypass();
    Bypass* feedback_input = new Bypass();
    Bypass* damping_input = new Bypass();
    Bypass* wet_input = new Bypass();

    registerInput(audio_input->input(), kAudio);
    registerInput(feedback_input->input(), kFeedback);
    registerInput(damping_input->input(), kDamping);
    registerInput(wet_input->input(), kWet);

    Multiply* gained_input = new Multiply();
    gained_input->plug(audio_input, 0);
    gained_input->plug(gain, 1);

    VariableAdd* left_comb_total = new VariableAdd();
    for (int i = 0; i < NUM_COMB; ++i) {
      ReverbComb* comb = new ReverbComb(REVERB_MAX_MEMORY);
      Value* time = new Value(COMB_TUNINGS[i]);
      TimeToSamples* samples = new TimeToSamples();
      samples->setControlRate();
      samples->plug(time);

      comb->plug(damping_input, ReverbComb::kDamping);
      comb->plug(feedback_input, ReverbComb::kFeedback);
      comb->plug(gained_input, ReverbComb::kAudio);
      comb->plug(samples, ReverbComb::kSampleDelay);
      addProcessor(samples);
      addProcessor(comb);
      left_comb_total->plugNext(comb);
    }

    VariableAdd* right_comb_total = new VariableAdd();
    for (int i = 0; i < NUM_COMB; ++i) {
      ReverbComb* comb = new ReverbComb(REVERB_MAX_MEMORY);
      Value* time = new Value(COMB_TUNINGS[i] + STEREO_SPREAD);
      TimeToSamples* samples = new TimeToSamples();
      samples->setControlRate();
      samples->plug(time);

      comb->plug(damping_input, ReverbComb::kDamping);
      comb->plug(feedback_input, ReverbComb::kFeedback);
      comb->plug(gained_input, ReverbComb::kAudio);
      comb->plug(samples, ReverbComb::kSampleDelay);
      addProcessor(samples);
      addProcessor(comb);
      right_comb_total->plugNext(comb);
    }

    addProcessor(audio_input);
    addProcessor(gained_input);
    addProcessor(feedback_input);
    addProcessor(wet_input);
    addProcessor(damping_input);
    addProcessor(left_comb_total);
    addProcessor(right_comb_total);

    Processor* left_audio = left_comb_total;
    for (int i = 0; i < NUM_ALL_PASS; ++i) {
      ReverbAllPass* all_pass = new ReverbAllPass(REVERB_MAX_MEMORY);
      Value* time = new Value(ALL_PASS_TUNINGS[i]);
      TimeToSamples* samples = new TimeToSamples();
      samples->setControlRate();
      samples->plug(time);

      all_pass->plug(left_audio, ReverbAllPass::kAudio);
      all_pass->plug(samples, ReverbAllPass::kSampleDelay);
      all_pass->plug(&utils::value_half, ReverbAllPass::kFeedback);

      addProcessor(all_pass);
      addProcessor(samples);
      left_audio = all_pass;
    }

    Processor* right_audio = right_comb_total;
    for (int i = 0; i < NUM_ALL_PASS; ++i) {
      ReverbAllPass* all_pass = new ReverbAllPass(REVERB_MAX_MEMORY);
      Value* time = new Value(ALL_PASS_TUNINGS[i] + STEREO_SPREAD);
      TimeToSamples* samples = new TimeToSamples();
      samples->setControlRate();
      samples->plug(time);

      all_pass->plug(right_audio, ReverbAllPass::kAudio);
      all_pass->plug(samples, ReverbAllPass::kSampleDelay);
      all_pass->plug(&utils::value_half, ReverbAllPass::kFeedback);

      addProcessor(all_pass);
      addProcessor(samples);
      right_audio = all_pass;
    }

    Interpolate* left_output = new Interpolate();
    left_output->plug(audio_input, Interpolate::kFrom);
    left_output->plug(left_audio, Interpolate::kTo);
    left_output->plug(wet_input, Interpolate::kFractional);

    Interpolate* right_output = new Interpolate();
    right_output->plug(audio_input, Interpolate::kFrom);
    right_output->plug(right_audio, Interpolate::kTo);
    right_output->plug(wet_input, Interpolate::kFractional);

    addProcessor(left_output);
    addProcessor(right_output);

    registerOutput(left_output->output());
    registerOutput(right_output->output());
  }
} // namespace mopo
