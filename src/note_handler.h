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
#ifndef NOTE_HANDLER_H
#define NOTE_HANDLER_H

#include "common.h"

namespace mopo {

  class NoteHandler {
    public:
      virtual ~NoteHandler() { }
      virtual void allNotesOff(int sample = 0) = 0;
      virtual void noteOn(mopo_float note, mopo_float velocity = 1,
                          int sample = 0, int channel = 0) = 0;
      virtual VoiceEvent noteOff(mopo_float note, int sample = 0) = 0;
  };
} // namespace mopo

#endif // NOTE_HANDLER_H
