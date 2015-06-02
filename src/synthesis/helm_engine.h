/* Copyright 2013-2015 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef HELM_ENGINE_H
#define HELM_ENGINE_H

#include "mopo.h"
#include "helm_common.h"
#include "helm_module.h"

namespace mopo {
  class Arpeggiator;
  class HelmVoiceHandler;
  class Value;

  // The overall helm engine. All audio processing is contained in here.
  class HelmEngine : public HelmModule {
    public:
      HelmEngine();

      void init() override;

      void process() override;

      std::set<ModulationConnection*> getModulationConnections() { return mod_connections_; }
      std::list<mopo::mopo_float> getPressedNotes();
      void connectModulation(ModulationConnection* connection);
      void disconnectModulation(ModulationConnection* connection);
      ModulationConnection* getConnection(std::string source, std::string destination);
      std::vector<ModulationConnection*> getSourceConnections(std::string source);
      std::vector<ModulationConnection*> getDestinationConnections(std::string destination);
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

    private:
      HelmVoiceHandler* voice_handler_;
      Arpeggiator* arpeggiator_;
      Value* arp_on_;
      bool was_playing_arp_;

      std::set<ModulationConnection*> mod_connections_;
  };
} // namespace mopo

#endif // HELM_ENGINE_H
