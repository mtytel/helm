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

#include "phaser.h"

#include "filter.h"
#include "operators.h"
#include "oscillator.h"

namespace mopo {

  Phaser::Phaser(int num_passes) : ProcessorRouter(0, 0) {
    Oscillator* oscillator = new Oscillator();
    registerInput(oscillator->input(Oscillator::kFrequency), kOscFrequency);
    registerInput(oscillator->input(Oscillator::kWaveform), kOscWaveform);
    registerInput(oscillator->input(Oscillator::kReset), kReset);

    Multiply* scaled_oscillator = new Multiply();
    registerInput(scaled_oscillator->input(0), kSemitoneSweep);
    scaled_oscillator->plug(oscillator, 1);

    Add* midi_cutoff = new Add();
    registerInput(midi_cutoff->input(0), kFilterCutoffMidi);
    midi_cutoff->plug(scaled_oscillator, 1);

    MidiScale* frequency_cutoff = new MidiScale();
    frequency_cutoff->plug(midi_cutoff);

    Filter* first_filter = new Filter();
    Value* filter_type = new Value(Filter::kAllPass);
    first_filter->plug(filter_type, Filter::kType);
    first_filter->plug(frequency_cutoff, Filter::kCutoff);
    registerInput(first_filter->input(Filter::kAudio), kAudio);
    registerInput(first_filter->input(Filter::kResonance), kFilterResonance);
    Filter* last_filter = first_filter;

    for (int i = 1; i < num_passes; ++i) {
      Filter* filter = new Filter();
      filter->registerInput(first_filter->input(Filter::kResonance),
                            Filter::kResonance);
      filter->plug(last_filter, Filter::kAudio);
      filter->plug(filter_type, Filter::kType);
      filter->plug(frequency_cutoff, Filter::kCutoff);
      addProcessor(filter);
      last_filter = filter;
    }

    Interpolate* mixer = new Interpolate();
    mixer->registerInput(first_filter->input(Filter::kAudio),
                         Interpolate::kFrom);
    mixer->plug(last_filter, Interpolate::kTo);
    registerInput(mixer->input(Interpolate::kFractional), kMix);

    addProcessor(oscillator);
    addProcessor(scaled_oscillator);
    addProcessor(midi_cutoff);
    addProcessor(frequency_cutoff);
    addProcessor(mixer);
    addProcessor(first_filter);

    registerOutput(mixer->output(), 0);
  }
} // namespace mopo
