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
      sustain_(false), phase_(1.0), note_index_(-1), last_played_note_(0) {
    MOPO_ASSERT(voice_handler);
  }

  void Arpeggiator::process() {
    if (active_notes_.size() == 0) {
      if (last_played_note_ >= 0) {
        voice_handler_->noteOff(last_played_note_, 0);
        last_played_note_ = -1;
      }
      return;
    }

    mopo_float gate = input(kGate)->at(0);
    mopo_float frequency = input(kFrequency)->at(0);

    mopo_float delta_phase = frequency / sample_rate_;
    mopo_float new_phase = phase_ + buffer_size_ * delta_phase;

    if (new_phase >= gate && phase_ < gate) {
      int sample_note_off = (gate - phase_) / delta_phase;
      voice_handler_->noteOff(last_played_note_, 0);
      last_played_note_ = -1;
    }
    if (new_phase >= 1) {
      int sample_note_on = (1 - phase_) / delta_phase;
      note_index_ = (note_index_ + 1) % note_order_.size();
      mopo_float note = note_order_[note_index_];
      voice_handler_->noteOn(note, active_notes_[note], 0);
      last_played_note_ = note;
      phase_ = new_phase - 1.0;
    }
    else
      phase_ = new_phase;
  }

  void Arpeggiator::sustainOn() {
    sustain_ = true;
  }

  void Arpeggiator::sustainOff() {
    sustain_ = false;
    std::set<mopo_float>::iterator iter = sustained_notes_.begin();
    for (; iter != sustained_notes_.end(); ++iter)
      noteOff(*iter);
    sustained_notes_.clear();
  }

  void Arpeggiator::noteOn(mopo_float note, mopo_float velocity, int sample) {
    if (active_notes_.count(note))
      return;
    if (pressed_notes_.size() == 0) {
      note_index_ = -1;
      phase_ = 1.0;
    }
    active_notes_[note] = velocity;
    pressed_notes_.insert(note);
    note_order_.push_back(note);
  }

  void Arpeggiator::noteOff(mopo_float note, int sample) {
    if (pressed_notes_.count(note) == 0)
      return;

    if (sustain_)
      sustained_notes_.insert(note);
    else {
      active_notes_.erase(note);
      note_order_.erase(std::remove(note_order_.begin(), note_order_.end(), note));
    }

    pressed_notes_.erase(note);
    if (note_order_.size() == 0)
      note_index_ = -1;
    else
      note_index_ %= note_order_.size();
  }
} // namespace mopo
