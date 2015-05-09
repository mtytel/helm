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
#ifndef TWYTCH_VOICE_HANDLER_H
#define TWYTCH_VOICE_HANDLER_H

#include "mopo.h"
#include "twytch_common.h"
#include "twytch_module.h"

#include <vector>

namespace mopo {
  class BypassRouter;
  class Delay;
  class Distortion;
  class Envelope;
  class Filter;
  class FormantManager;
  class LinearSlope;
  class Oscillator;
  class OscillatorFeedback;
  class SmoothValue;
  class StepGenerator;
  class TriggerCombiner;
  class TwytchOscillators;

  // The voice handler duplicates processors to produce polyphony.
  // Everything in the synthesizer we want per-voice instances of must be
  // contained in here.
  class TwytchVoiceHandler : public virtual VoiceHandler, public virtual TwytchModule {
    public:
      TwytchVoiceHandler(Processor* beats_per_second);
      virtual ~TwytchVoiceHandler() { } // Should probably delete things.

      void init() override;

      void setModWheel(mopo_float value);
      void setPitchWheel(mopo_float value);

    private:
      // Create the portamento, legato, amplifier envelope and other processors
      // that effect how voices start and turn into other notes.
      void createArticulation(Output* note, Output* velocity, Output* trigger);

      // Create the oscillators and hook up frequency controls.
      void createOscillators(Output* frequency, Output* reset);

      // Create the LFOs, Step Sequencers, etc.
      void createModulators(Output* reset);

      // Create the filter and filter envelope.
      void createFilter(Output* audio, Output* keytrack, Output* reset, Output* note_event);

      Processor* beats_per_second_;

      Add* note_from_center_;
      SmoothValue* mod_wheel_amount_;
      SmoothValue* pitch_wheel_amount_;
      LinearSlope* current_frequency_;
      Envelope* amplitude_envelope_;
      Multiply* amplitude_;

      Distortion* distorted_filter_;
      FormantManager* formant_filter_;
      Envelope* filter_envelope_;
      BypassRouter* formant_container_;
      OscillatorFeedback* osc_feedback_;

      Multiply* output_;
  };
} // namespace mopo

#endif // TWYTCH_VOICE_HANDLER_H
