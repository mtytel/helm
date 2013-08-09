/* Copyright 2013 Little IO
 *
 * laf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * laf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with laf.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef VOICE_HANDLER_H
#define VOICE_HANDLER_H

#include "processor_router.h"
#include "value.h"

#include <map>
#include <list>

namespace laf {

  class Voice {
    public:
      Voice(Processor* voice);

      void activate(laf_sample note, laf_sample velocity);
      void deactivate();

      laf_sample note() { return note_; }
      laf_sample velocity() { return velocity_; }
      Processor* voice() { return voice_; }

    private:
      laf_sample note_;
      laf_sample velocity_;
      Processor* voice_;
  };

  class VoiceHandler : public Processor {
    public:
      VoiceHandler(int polyphony = 1);

      virtual Processor* clone() const { LAF_ASSERT(false); }
      virtual void process();
      virtual void setSampleRate(int sample_rate);

      void noteOn(laf_sample note, laf_sample velocity = 1);
      void noteOff(laf_sample note);
      void sustainOn();
      void sustainOff();

      Value* note() { return &note_; }
      Value* velocity() { return &velocity_; }

      void addProcessor(Processor* processor);
      void setPolyphony(int polyphony);
      void setVoiceOutput(const Processor* output) { voice_output_ = output; }
      void setVoiceKiller(const Processor* killer) { voice_killer_ = killer; }

    private:
      Voice* createVoice();
      void processVoice(Voice* voice);

      int polyphony_;
      bool sustain_;
      const Processor* voice_output_;
      const Processor* voice_killer_;
      Value note_;
      Value velocity_;

      std::set<Voice*> all_voices_;
      std::list<Voice*> free_voices_;
      std::list<Voice*> released_voices_;
      std::list<Voice*> sustained_voices_;
      std::list<Voice*> active_voices_;
      ProcessorRouter router_;
  };
} // namespace laf

#endif // VOICE_HANDLER_H
