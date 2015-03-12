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

#include "twytch_oscillators.h"

namespace mopo {

  TwytchOscillators::TwytchOscillators() : TickRouter(0, 0) {
    oscillator1_ = new Oscillator();
    oscillator2_ = new Oscillator();
    frequency1_ = new Multiply();
    frequency2_ = new Multiply();
    freq_mod1_ = new Multiply();
    freq_mod2_ = new Multiply();
    normalized_fm1_ = new Add();
    normalized_fm2_ = new Add();

    addProcessor(normalized_fm1_);
    addProcessor(normalized_fm2_);
    addProcessor(frequency1_);
    addProcessor(frequency2_);
    addProcessor(freq_mod1_);
    addProcessor(freq_mod2_);
    addProcessor(oscillator1_);
    addProcessor(oscillator2_);

    registerInput(oscillator1_->input(Oscillator::kWaveform), kOscillator1Waveform);
    registerInput(oscillator2_->input(Oscillator::kWaveform), kOscillator2Waveform);
    registerInput(oscillator1_->input(Oscillator::kReset), kOscillator1Reset);
    registerInput(oscillator2_->input(Oscillator::kReset), kOscillator2Reset);
    registerInput(frequency1_->input(0), kOscillator1BaseFrequency);
    registerInput(frequency2_->input(0), kOscillator2BaseFrequency);
    registerInput(freq_mod1_->input(0), kOscillator1FM);
    registerInput(freq_mod2_->input(0), kOscillator2FM);

    normalized_fm1_->plug(freq_mod1_, 0);
    normalized_fm2_->plug(freq_mod2_, 0);
    normalized_fm1_->plug(&utils::value_one, 1);
    normalized_fm2_->plug(&utils::value_one, 1);
    frequency1_->plug(normalized_fm1_, 1);
    frequency2_->plug(normalized_fm2_, 1);
    oscillator1_->plug(frequency1_, Oscillator::kFrequency);
    oscillator2_->plug(frequency2_, Oscillator::kFrequency);
    freq_mod2_->plug(oscillator1_, 1);
    freq_mod1_->plug(oscillator2_, 1);

    registerOutput(oscillator1_->output());
    registerOutput(oscillator2_->output());
  }

  void TwytchOscillators::process() {
    int num_feedbacks = feedback_order_->size();
    for (int i = 0; i < num_feedbacks; ++i)
      feedback_processors_[feedback_order_->at(i)]->tickBeginRefreshOutput();

    oscillator1_->preprocess();
    oscillator2_->preprocess();
    tick(0);

    for (int i = 1; i < buffer_size_; ++i) {
      for (int f = 0; f < num_feedbacks; ++f)
        feedback_processors_[feedback_order_->at(f)]->tickRefreshOutput(i);

      tick(i);

      for (int f = 0; f < num_feedbacks; ++f)
        feedback_processors_[feedback_order_->at(f)]->tick(i);
    }
  }
} // namespace mopo
