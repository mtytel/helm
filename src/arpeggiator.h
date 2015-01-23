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

#pragma once
#ifndef ARPEGGIATOR_H
#define ARPEGGIATOR_H

#include "processor.h"
#include "value.h"

#include <map>
#include <set>
#include <vector>

namespace mopo {

  class VoiceHandler;

  class Arpeggiator : public Processor {
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
        kNumInputs
      };

      Arpeggiator(VoiceHandler* voice_handler);

      virtual Processor* clone() const { MOPO_ASSERT(false); return nullptr; }
      virtual void process();

      int getNumNotes() { return pressed_notes_.size(); }
      std::pair<mopo_float, mopo_float> getNextNote();
      void addNoteToPatterns(mopo_float note);
      void removeNoteFromPatterns(mopo_float note);

      void allNotesOff(int sample = 0);
      void noteOn(mopo_float note, mopo_float velocity = 1, int sample = 0);
      void noteOff(mopo_float note, int sample = 0);
      void sustainOn();
      void sustainOff();

    private:
      Arpeggiator() : Processor(0, 0) { }

      VoiceHandler* voice_handler_;

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
      std::set<mopo_float> pressed_notes_;
      std::set<mopo_float> sustained_notes_;
  };
} // namespace mopo

#endif // ARPEGGIATOR_H
