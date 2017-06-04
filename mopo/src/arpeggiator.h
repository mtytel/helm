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

#pragma once
#ifndef ARPEGGIATOR_H
#define ARPEGGIATOR_H

#include "circular_queue.h"
#include "note_handler.h"
#include "processor.h"
#include "value.h"

#include <list>
#include <map>
#include <set>
#include <vector>

namespace mopo {

  class Arpeggiator : public Processor, public NoteHandler {
    public:
      enum Pattern {
        kUp,
        kDown,
        kUpDown,
        kAsPlayed,
        kRandom,
        kNumTypes
      };

      enum Inputs {
        kFrequency,
        kGate,
        kPattern,
        kOctaves,
        kOn,
        kNumInputs
      };

      Arpeggiator(NoteHandler* note_handler);

      virtual Processor* clone() const override {
        MOPO_ASSERT(false);
        return 0;
      }

      virtual void process() override;

      int getNumNotes() { return pressed_notes_.size(); }
      CircularQueue<mopo_float>& getPressedNotes();
      std::pair<mopo_float, mopo_float> getNextNote();
      void addNoteToPatterns(mopo_float note);
      void removeNoteFromPatterns(mopo_float note);

      void allNotesOff(int sample = 0) override;
      void noteOn(mopo_float note, mopo_float velocity = 1,
                  int sample = 0, int channel = 0) override;
      VoiceEvent noteOff(mopo_float note, int sample = 0) override;
      void sustainOn();
      void sustainOff();

    private:
      Arpeggiator() : Processor(0, 0) { }

      NoteHandler* note_handler_;

      bool sustain_;
      mopo_float phase_;
      int note_index_;
      int current_octave_;
      bool octave_up_;
      mopo_float last_played_note_;

      std::vector<mopo_float> as_played_;
      std::vector<mopo_float> ascending_;
      std::vector<mopo_float> decending_;

      std::map<mopo_float, mopo_float> active_notes_;
      CircularQueue<mopo_float> pressed_notes_;
      CircularQueue<mopo_float> sustained_notes_;
  };
} // namespace mopo

#endif // ARPEGGIATOR_H
