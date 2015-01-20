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

#include "arpeggiator.h"

#include "utils.h"
#include "voice_handler.h"

namespace mopo {

  Arpeggiator::Arpeggiator(VoiceHandler* voice_handler) :
      Processor(kNumInputs, 1), voice_handler_(voice_handler),
      sustain_(false), phase_(0.0), note_index_(0.0) {
    MOPO_ASSERT(voice_handler);
  }

  void Arpeggiator::process() {
    mopo_float gate = input(kGate)->at(0);
    mopo_float frequency = input(kFrequency)->at(0);

    mopo_float delta_phase = frequency / sample_rate_;
    mopo_float new_phase = phase_ + buffer_size_ * delta_phase;

    if (new_phase >= gate) {
      int sample_note_off = (gate - phase_) / delta_phase;
      voice_handler_->noteOff(active_notes_[note_index_], sample_note_off);
    }
    if (new_phase >= 1) {
      int sample_note_on = (1 - phase_) / delta_phase;
      note_index_ = (note_index_ + 1) % active_notes_.size();
      voice_handler_->noteOn(active_notes_[note_index_], sample_note_on);
    }
  }

  void Arpeggiator::sustainOn() {
    sustain_ = true;
  }

  void Arpeggiator::sustainOff() {
    sustain_ = false;
    std::map<mopo_float, mopo_float>::iterator iter = sustained_notes_.begin();
    for (; iter != sustained_notes_.end(); ++iter)
      std::remove(active_notes_.begin(), active_notes_.end(), iter->first);
    sustained_notes_.clear();
  }

  void Arpeggiator::noteOn(mopo_float note, mopo_float velocity, int sample) {
    pressed_notes_[note] = velocity;
    active_notes_.push_back(note);
  }

  void Arpeggiator::noteOff(mopo_float note, int sample) {
    if (pressed_notes_.count(note) == 0)
      return;

    if (sustain_)
      sustained_notes_[note] = pressed_notes_[note];
    else
      std::remove(active_notes_.begin(), active_notes_.end(), note);

    pressed_notes_.erase(note);
  }
} // namespace mopo
