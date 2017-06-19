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

#include "reverb.h"

#include "operators.h"
#include "reverb_all_pass.h"
#include "reverb_comb.h"
#include "reverb_tuning.h"

namespace mopo {

  Reverb::Reverb() : ProcessorRouter(kNumInputs, 2), current_dry_(0.0), current_wet_(0.0) {
    static const Value gain(FIXED_GAIN);
    
    Bypass* audio_input = new Bypass();
    LinearSmoothBuffer* feedback_input = new LinearSmoothBuffer();
    LinearSmoothBuffer* damping_input = new LinearSmoothBuffer();

    registerInput(audio_input->input(), kAudio);
    registerInput(feedback_input->input(), kFeedback);
    registerInput(damping_input->input(), kDamping);

    Multiply* gained_input = new Multiply();
    gained_input->plug(audio_input, 0);
    gained_input->plug(&gain, 1);

    addProcessor(audio_input);
    addProcessor(gained_input);
    addProcessor(feedback_input);
    addProcessor(damping_input);

    VariableAdd* left_comb_total = new VariableAdd(NUM_COMB);
    for (int i = 0; i < NUM_COMB; ++i) {
      ReverbComb* comb = new ReverbComb(1 + mopo::MAX_SAMPLE_RATE * COMB_TUNINGS[i]);
      Value* time = new cr::Value(COMB_TUNINGS[i]);
      addIdleProcessor(time);
      cr::TimeToSamples* samples = new cr::TimeToSamples();
      samples->plug(time);

      comb->plug(gained_input, ReverbComb::kAudio);
      comb->plug(samples, ReverbComb::kSampleDelay);
      comb->plug(feedback_input, ReverbComb::kFeedback);
      comb->plug(damping_input, ReverbComb::kDamping);
      left_comb_total->plugNext(comb);
      addProcessor(samples);
      addProcessor(comb);
    }

    VariableAdd* right_comb_total = new VariableAdd(NUM_COMB);
    for (int i = 0; i < NUM_COMB; ++i) {
      mopo_float tuning = COMB_TUNINGS[i] + STEREO_SPREAD;
      ReverbComb* comb = new ReverbComb(1 + mopo::MAX_SAMPLE_RATE * tuning);
      Value* time = new cr::Value(tuning);
      addIdleProcessor(time);
      cr::TimeToSamples* samples = new cr::TimeToSamples();
      samples->plug(time);

      comb->plug(gained_input, ReverbComb::kAudio);
      comb->plug(samples, ReverbComb::kSampleDelay);
      comb->plug(feedback_input, ReverbComb::kFeedback);
      comb->plug(damping_input, ReverbComb::kDamping);
      right_comb_total->plugNext(comb);
      addProcessor(samples);
      addProcessor(comb);
    }

    addProcessor(left_comb_total);
    addProcessor(right_comb_total);

    reverb_wet_left_ = left_comb_total;
    for (int i = 0; i < NUM_ALL_PASS; ++i) {
      ReverbAllPass* all_pass = new ReverbAllPass(1 + mopo::MAX_SAMPLE_RATE * ALL_PASS_TUNINGS[i]);
      Value* time = new cr::Value(ALL_PASS_TUNINGS[i]);
      addIdleProcessor(time);
      cr::TimeToSamples* samples = new cr::TimeToSamples();
      samples->plug(time);

      all_pass->plug(reverb_wet_left_, ReverbAllPass::kAudio);
      all_pass->plug(samples, ReverbAllPass::kSampleDelay);
      all_pass->plug(&utils::value_half, ReverbAllPass::kFeedback);

      addProcessor(all_pass);
      addProcessor(samples);
      reverb_wet_left_ = all_pass;
    }

    reverb_wet_right_ = right_comb_total;
    for (int i = 0; i < NUM_ALL_PASS; ++i) {
      mopo_float tuning = ALL_PASS_TUNINGS[i] + STEREO_SPREAD;
      ReverbAllPass* all_pass = new ReverbAllPass(1 + mopo::MAX_SAMPLE_RATE * tuning);
      Value* time = new cr::Value(tuning);
      addIdleProcessor(time);
      cr::TimeToSamples* samples = new cr::TimeToSamples();
      samples->plug(time);

      all_pass->plug(reverb_wet_right_, ReverbAllPass::kAudio);
      all_pass->plug(samples, ReverbAllPass::kSampleDelay);
      all_pass->plug(&utils::value_half, ReverbAllPass::kFeedback);

      addProcessor(all_pass);
      addProcessor(samples);
      reverb_wet_right_ = all_pass;
    }
  }

  void Reverb::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kAudio));

    ProcessorRouter::process();
    const mopo_float* audio = input(kAudio)->source->buffer;
    const mopo_float* left_wet_audio = reverb_wet_left_->output()->buffer;
    const mopo_float* right_wet_audio = reverb_wet_right_->output()->buffer;
    mopo_float* dest_left = output(0)->buffer;
    mopo_float* dest_right = output(1)->buffer;

    mopo_float wet_in = utils::clamp(input(kWet)->at(0), 0.0, 1.0);
    mopo_float next_wet = sqrt(wet_in);
    mopo_float next_dry = sqrt(1.0 - wet_in);
    mopo_float wet_inc = (next_wet - current_wet_) / buffer_size_;
    mopo_float dry_inc = (next_dry - current_dry_) / buffer_size_;

    VECTORIZE_LOOP
    for (int i = 0; i < buffer_size_; ++i) {
      mopo_float dry = current_dry_ + i * dry_inc;
      mopo_float wet = current_wet_ + i * wet_inc;
      dest_left[i] = dry * audio[i] + wet * left_wet_audio[i];
      dest_right[i] = dry * audio[i] + wet * right_wet_audio[i];
    }

    current_dry_ = next_dry;
    current_wet_ = next_wet;
  }
} // namespace mopo
