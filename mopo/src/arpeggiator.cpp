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

#include "arpeggiator.h"

#include "utils.h"

#include <algorithm>
#include <cstdlib>

namespace mopo {

  namespace {
    const float MIN_VOICE_TIME = 0.01;
  } // namespace

  Arpeggiator::Arpeggiator(NoteHandler* note_handler) :
      Processor(kNumInputs, 1), note_handler_(note_handler),
      sustain_(false), phase_(1.0), note_index_(-1),
      current_octave_(0), octave_up_(true), last_played_note_(0) {
    MOPO_ASSERT(note_handler);
    pressed_notes_.reserve(MIDI_SIZE);
    sustained_notes_.reserve(MIDI_SIZE);
  }

  void Arpeggiator::process() {
    if (input(kOn)->at(0) == 0.0)
      return;
    
    mopo_float frequency = input(kFrequency)->at(0);
    mopo_float min_gate = (MIN_VOICE_TIME + VOICE_KILL_TIME) * frequency;
    mopo_float gate = utils::interpolate(min_gate, 1.0, input(kGate)->at(0));

    mopo_float delta_phase = frequency / sample_rate_;
    mopo_float new_phase = phase_ + buffer_size_ * delta_phase;

    // If we're past the gate phase and we're playing a note, turn it off.
    if (new_phase >= gate && last_played_note_ >= 0) {
      int offset = utils::iclamp((gate - phase_) / delta_phase, 0, buffer_size_ - 1);
      note_handler_->noteOff(last_played_note_, offset);
      last_played_note_ = -1;
    }

    // Check if it's time to play the next note.
    if (getNumNotes() && new_phase >= 1) {
      int offset = utils::iclamp((1 - phase_) / delta_phase, 0, buffer_size_ - 1);
      std::pair<mopo_float, mopo_float> note = getNextNote();
      note_handler_->noteOn(note.first, note.second, offset);
      last_played_note_ = note.first;
      phase_ = new_phase - 1.0;
    }
    else
      phase_ = new_phase;
  }

  std::pair<mopo_float, mopo_float> Arpeggiator::getNextNote() {
    int octaves = utils::imax(1, input(kOctaves)->at(0));
    Pattern type =
        static_cast<Pattern>(static_cast<int>(input(kPattern)->at(0)));
    std::vector<mopo_float>* pattern = &as_played_;

    note_index_++;

    switch (type) {
      case kUp:
        pattern = &ascending_;
        octave_up_ = true;
        break;
      case kDown:
        pattern = &decending_;
        octave_up_ = false;
        break;
      case kRandom:
        pattern = &ascending_;
        note_index_ = rand() % ascending_.size();
        current_octave_ = rand() % octaves;
        break;
      case kUpDown:
        if (note_index_ >= ascending_.size() - 1) {
          if ((current_octave_ == 0 && !octave_up_) ||
              (current_octave_ >= octaves - 1 && octave_up_)) {
            note_index_ = 0;
            octave_up_ = !octave_up_;
          }
        }
        pattern = octave_up_ ? &ascending_ : &decending_;
        break;
      case kAsPlayed:
        pattern = &as_played_;
        break;
      default:
        break;
    }

    if (note_index_ >= pattern->size()) {
      note_index_ = 0;
      if (octave_up_)
        current_octave_ = (current_octave_ + 1) % octaves;
      else
        current_octave_ = (current_octave_ + octaves - 1) % octaves;
    }
    mopo_float base_note = pattern->at(note_index_);
    mopo_float note = base_note + mopo::NOTES_PER_OCTAVE * current_octave_;
    mopo_float velocity = active_notes_[base_note];
    return std::pair<mopo_float, mopo_float>(note, velocity);
  }

  CircularQueue<mopo_float>& Arpeggiator::getPressedNotes() {
    return pressed_notes_;
  }

  void Arpeggiator::addNoteToPatterns(mopo_float note) {
    as_played_.push_back(note);
    ascending_.push_back(note);
    std::sort(ascending_.begin(), ascending_.end());
    decending_.push_back(note);
    std::sort(decending_.rbegin(), decending_.rend());
  }

  void Arpeggiator::removeNoteFromPatterns(mopo_float note) {
    as_played_.erase(
        std::remove(as_played_.begin(), as_played_.end(), note));
    ascending_.erase(
        std::remove(ascending_.begin(), ascending_.end(), note));
    decending_.erase(
        std::remove(decending_.begin(), decending_.end(), note));
  }

  void Arpeggiator::sustainOn() {
    sustain_ = true;
  }

  void Arpeggiator::sustainOff() {
    sustain_ = false;
    for (mopo_float note : sustained_notes_)
      noteOff(note);
    sustained_notes_.clear();
  }

  void Arpeggiator::allNotesOff(int sample) {
    active_notes_.clear();
    pressed_notes_.clear();
    sustained_notes_.clear();
    ascending_.clear();
    decending_.clear();
    as_played_.clear();
    note_handler_->allNotesOff();
  }

  void Arpeggiator::noteOn(mopo_float note, mopo_float velocity, int sample, int channel) {
    if (active_notes_.count(note))
      return;
    if (pressed_notes_.size() == 0) {
      note_index_ = -1;
      current_octave_ = 0;
      phase_ = 1.0;
    }
    active_notes_[note] = velocity;
    pressed_notes_.push_back(note);
    addNoteToPatterns(note);
  }

  VoiceEvent Arpeggiator::noteOff(mopo_float note, int sample) {
    if (pressed_notes_.count(note) == 0)
      return kVoiceOff;

    if (sustain_)
      sustained_notes_.push_back(note);
    else {
      active_notes_.erase(note);
      removeNoteFromPatterns(note);
    }

    pressed_notes_.remove(note);
    return kVoiceOff;
  }
} // namespace mopo
