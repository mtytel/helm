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
      enum KeyState {
        kHeld,
        kSustained,
        kReleased,
        kNumStates
      };

      Voice(Processor* voice);

      Processor* processor() { return processor_; }
      const VoiceState& state() { return state_; }
      const KeyState key_state() { return key_state_; }
      int event_sample() { return event_sample_; }

      mopo_float aftertouch() { return aftertouch_; }
      mopo_float aftertouch_sample() { return aftertouch_sample_; }

      void activate(mopo_float note, mopo_float velocity, int sample = 0) {
        event_sample_ = sample;
        state_.event = kVoiceOn;
        state_.note = note;
        state_.velocity = velocity;
        key_state_ = kHeld;
      }

      void sustain() {
        key_state_ = kSustained;
      }

      void deactivate(int sample = 0) {
        event_sample_ = sample;
        state_.event = kVoiceOff;
        key_state_ = kReleased;
      }

      bool hasNewEvent() {
        return event_sample_ >= 0;
      }

      void setAftertouch(mopo_float aftertouch, int sample = 0) {
        aftertouch_ = aftertouch;
        aftertouch_sample_ = sample;
      }

      bool hasNewAftertouch() {
        return aftertouch_sample_ >= 0;
      }

      void clearEvents() {
        event_sample_ = -1;
        aftertouch_sample_ = -1;
      }

    private:
      Voice() { }

      int event_sample_;
      VoiceState state_;
      KeyState key_state_;

      int aftertouch_sample_;
      mopo_float aftertouch_;

      Processor* processor_;
  };

  class VoiceHandler : public Processor {
    public:
      enum Inputs {
        kPolyphony,
        kNumInputs
      };

      VoiceHandler(size_t polyphony = 1);

      virtual Processor* clone() const { MOPO_ASSERT(false); return nullptr; }
      virtual void process();
      virtual void setSampleRate(int sample_rate);
      virtual void setBufferSize(int buffer_size);

      void allNotesOff(int sample = 0);
      Voice* grabVoice();
      void noteOn(mopo_float note, mopo_float velocity = 1, int sample = 0);
      void noteOff(mopo_float note, int sample = 0);
      void setAftertouch(mopo_float note, mopo_float aftertouch, int sample = 0);
      void sustainOn();
      void sustainOff(int sample = 0);

      Output* voice_event() { return &voice_event_; }
      Output* note() { return &note_; }
      Output* velocity() { return &velocity_; }
      Output* aftertouch() { return &aftertouch_; }

      void addProcessor(Processor* processor);
      void removeProcessor(Processor* processor);
      void addGlobalProcessor(Processor* processor);
      void removeGlobalProcessor(Processor* processor);

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
      Output aftertouch_;

      std::list<mopo_float> pressed_notes_;
      std::vector<std::unique_ptr<Voice> > all_voices_;

      std::list<Voice*> free_voices_;
      std::list<Voice*> active_voices_;

      ProcessorRouter voice_router_;
      ProcessorRouter global_router_;
  };
} // namespace mopo

#endif // VOICE_HANDLER_H
