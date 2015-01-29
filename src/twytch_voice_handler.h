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

#include "operators.h"
#include "twytch_common.h"
#include "voice_handler.h"

#include <vector>

namespace mopo {
  class Delay;
  class Envelope;
  class Filter;
  class LinearSlope;
  class Oscillator;
  class SmoothValue;
  class StepGenerator;
  class TwytchOscillators;

  // The voice handler duplicates processors to produce polyphony.
  // Everything in the synthesizer we want per-voice instances of must be
  // contained in here.
  class TwytchVoiceHandler : public VoiceHandler {
    public:
      TwytchVoiceHandler();
      virtual ~TwytchVoiceHandler() { } // Should probably delete things.

      control_map getControls() { return controls_; }

      void setModWheel(mopo_float value);
      void setPitchWheel(mopo_float value);

      void connectModulation(std::string from, std::string to, Value* scale);
      void disconnectModulation(std::string to, Value* scale);

    private:
      // Create the portamento, legato, amplifier envelope and other processors
      // that effect how voices start and turn into other notes.
      void createArticulation(Output* note, Output* velocity, Output* trigger);

      // Create the oscillators and hook up frequency controls.
      void createOscillators(Output* frequency, Output* reset);

      // Create the filter and filter envelope.
      void createFilter(Output* audio, Output* keytrack, Output* reset);

      // Create the modulation matrix.
      void createModMatrix();

      Add* note_from_center_;
      SmoothValue* mod_wheel_amount_;
      SmoothValue* pitch_wheel_amount_;
      LinearSlope* current_frequency_;
      Envelope* amplitude_envelope_;
      Multiply* amplitude_;

      TwytchOscillators* oscillators_;
      Delay* osc_feedback_;
      Oscillator* lfo1_;
      Oscillator* lfo2_;
      StepGenerator* step_sequencer_;
      Interpolate* oscillator_mix_;

      Filter* filter_;
      Envelope* filter_envelope_;

      Multiply* output_;

      control_map controls_;
      output_map mod_sources_;
      input_map mod_destinations_;
      std::map<Value*, Multiply*> modulation_lookup_;
  };
} // namespace mopo

#endif // TWYTCH_VOICE_HANDLER_H
