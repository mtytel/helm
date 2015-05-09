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

#include "oscillator_feedback.h"

#define MAX_FEEDBACK_SAMPLES 20000

namespace mopo {

  OscillatorFeedback::OscillatorFeedback(Output* audio, Output* midi) :
      ProcessorRouter(0, 0), in_audio_(audio), in_midi_(midi) {
    feedback_ = new SimpleDelay(MAX_FEEDBACK_SAMPLES);
    registerOutput(feedback_->output());
  }

  void OscillatorFeedback::init() {
    Processor* osc_feedback_transpose = createPolyModControl("osc_feedback_transpose", -12, true);
    Processor* osc_feedback_amount = createPolyModControl("osc_feedback_amount", 0.0, false);
    Processor* osc_feedback_tune = createPolyModControl("osc_feedback_tune", 0.0, true);
    Add* osc_feedback_transposed = new Add();
    osc_feedback_transposed->setControlRate();
    osc_feedback_transposed->plug(in_midi_, 0);
    osc_feedback_transposed->plug(osc_feedback_transpose, 1);
    Add* osc_feedback_midi = new Add();
    osc_feedback_midi->setControlRate();
    osc_feedback_midi->plug(osc_feedback_transposed, 0);
    osc_feedback_midi->plug(osc_feedback_tune, 1);

    MidiScale* osc_feedback_frequency = new MidiScale();
    osc_feedback_frequency->setControlRate();
    osc_feedback_frequency->plug(osc_feedback_midi);

    FrequencyToSamples* osc_feedback_samples = new FrequencyToSamples();
    osc_feedback_samples->plug(osc_feedback_frequency);

    SampleAndHoldBuffer* osc_feedback_samples_audio = new SampleAndHoldBuffer();
    osc_feedback_samples_audio->plug(osc_feedback_samples);

    feedback_->plug(in_audio_, SimpleDelay::kAudio);
    feedback_->plug(osc_feedback_samples_audio, SimpleDelay::kSampleDelay);
    feedback_->plug(osc_feedback_amount, SimpleDelay::kFeedback);

    addProcessor(osc_feedback_transposed);
    addProcessor(osc_feedback_midi);
    addProcessor(osc_feedback_frequency);
    addProcessor(osc_feedback_samples);
    addProcessor(osc_feedback_samples_audio);
    addProcessor(feedback_);
  }
} // namespace mopo
