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

#pragma once
#ifndef TWYTCH_ENGINE_H
#define TWYTCH_ENGINE_H

#include "mopo.h"
#include "twytch_common.h"
#include "twytch_module.h"

namespace mopo {
  class Arpeggiator;
  class TwytchVoiceHandler;
  class Value;

  // The overall twytch engine. All audio processing is contained in here.
  class TwytchEngine : public ProcessorRouter, public TwytchModule {
    public:
      TwytchEngine();

      void process() override;

      std::set<ModulationConnection*> getModulationConnections() { return mod_connections_; }
      std::list<mopo::mopo_float> getPressedNotes();
      void connectModulation(ModulationConnection* connection);
      void disconnectModulation(ModulationConnection* connection);
      void clearModulations();
      int getNumActiveVoices();

      // Keyboard events.
      void allNotesOff(int sample = 0);
      void noteOn(mopo_float note, mopo_float velocity = 1.0, int sample = 0);
      void noteOff(mopo_float note, int sample = 0);
      void setModWheel(mopo_float value);
      void setPitchWheel(mopo_float value);
      void setBpm(mopo_float bpm);
      void setAftertouch(mopo_float note, mopo_float value, int sample = 0);

      // Sustain pedal events.
      void sustainOn();
      void sustainOff();

      // Twytch Module.
      ProcessorRouter* getMonoRouter() override { return this; }
      ProcessorRouter* getPolyRouter() override { return 0; }

    private:
      TwytchVoiceHandler* voice_handler_;
      Arpeggiator* arpeggiator_;
      Value* arp_on_;
      bool was_playing_arp_;

      std::set<ModulationConnection*> mod_connections_;
  };
} // namespace mopo

#endif // TWYTCH_ENGINE_H
