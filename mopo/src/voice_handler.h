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
#ifndef VOICE_HANDLER_H
#define VOICE_HANDLER_H

#include "processor_router.h"
#include "value.h"

#include <map>
#include <list>

namespace mopo {

  struct VoiceState {
    VoiceEvent event;
    mopo_float note;
    mopo_float velocity;
  };

  class Voice {
    public:
      Voice(Processor* voice);

      Processor* processor() { return processor_; }
      const VoiceState* state() { return &state_; }

      void activate(mopo_float note, mopo_float velocity) {
        new_event_ = true;
        state_.event = kVoiceOn;
        state_.note = note;
        state_.velocity = velocity;
      }

      void deactivate() {
        new_event_ = true;
        state_.event = kVoiceOff;
      }

      bool hasNewEvent() {
        return new_event_;
      }

      void clearEvent() {
        new_event_ = false;
      }

    private:
      bool new_event_;
      VoiceState state_;
      Processor* processor_;
  };

  class VoiceHandler : public Processor {
    public:
      enum Inputs {
        kPolyphony,
        kNumInputs
      };

      VoiceHandler(size_t polyphony = 1);

      virtual Processor* clone() const { MOPO_ASSERT(false); return NULL; }
      virtual void process();
      virtual void setSampleRate(int sample_rate);
      virtual void setBufferSize(int buffer_size);

      void noteOn(mopo_float note, mopo_float velocity = 1);
      void noteOff(mopo_float note);
      void sustainOn();
      void sustainOff();

      Output* voice_event() { return &voice_event_; }
      Output* note() { return &note_; }
      Output* velocity() { return &velocity_; }

      void addProcessor(Processor* processor);
      void addGlobalProcessor(Processor* processor);

      void setPolyphony(size_t polyphony);

      void setVoiceOutput(const Output* output) {
        voice_output_ = output;
      }
      void setVoiceOutput(const Processor* output) {
        setVoiceOutput(output->output());
      }
      void setVoiceKiller(const Output* killer) {
        voice_killer_ = killer;
      }
      void setVoiceKiller(const Processor* killer) {
        setVoiceKiller(killer->output());
      }

    private:
      Voice* createVoice();
      void prepareVoiceTriggers(Voice* voice);
      void processVoice(Voice* voice);

      size_t polyphony_;
      bool sustain_;
      const Output* voice_output_;
      const Output* voice_killer_;
      Output voice_event_;
      Output note_;
      Output velocity_;

      std::list<mopo_float> pressed_notes_;
      std::set<Voice*> all_voices_;
      std::list<Voice*> free_voices_;
      std::list<Voice*> sustained_voices_;
      std::list<Voice*> active_voices_;

      ProcessorRouter voice_router_;
      ProcessorRouter global_router_;
  };
} // namespace mopo

#endif // VOICE_HANDLER_H
