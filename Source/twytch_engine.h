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
#ifndef TWYTCH_SYNTH_H
#define TWYTCH_SYNTH_H

#include "alias.h"
#include "arpeggiator.h"
#include "delay.h"
#include "operators.h"
#include "oscillator.h"
#include "twytch_common.h"
#include "tick_router.h"
#include "voice_handler.h"

#include <vector>

#define MOD_MATRIX_SIZE 5

namespace mopo {
  class Arpeggiator;
  class Envelope;
  class Filter;
  class LinearSlope;
  class SmoothValue;
  class StepSequencer;

  // The oscillators of the synthesizer. This section of the synth is processed
  // sample by sample to allow for cross modulation.
  class TwytchOscillators : public TickRouter {
    public:

      enum Inputs {
        kOscillator1Waveform,
        kOscillator2Waveform,
        kOscillator1Reset,
        kOscillator2Reset,
        kOscillator1BaseFrequency,
        kOscillator2BaseFrequency,
        kOscillator1FM,
        kOscillator2FM,
        kNumInputs
      };
    
      TwytchOscillators();
      TwytchOscillators(const TwytchOscillators& original) :
          TickRouter(original) {
        oscillator1_ = new Oscillator(*original.oscillator1_);
        oscillator2_ = new Oscillator(*original.oscillator2_);
        frequency1_ = new Multiply(*original.frequency1_);
        frequency2_ = new Multiply(*original.frequency2_);
        freq_mod1_ = new Multiply(*original.freq_mod1_);
        freq_mod2_ = new Multiply(*original.freq_mod2_);
        normalized_fm1_ = new Add(*original.normalized_fm1_);
        normalized_fm2_ = new Add(*original.normalized_fm2_);
      }

      virtual void process();
      virtual Processor* clone() const { return new TwytchOscillators(*this); }

      // Process one sample of the oscillators. Must be done in the correct
      // order currently.
      void tick(int i) {
        freq_mod1_->tick(i);
        normalized_fm1_->tick(i);
        frequency1_->tick(i);
        oscillator1_->tick(i);
        freq_mod2_->tick(i);
        normalized_fm2_->tick(i);
        frequency2_->tick(i);
        oscillator2_->tick(i);
      }

    protected:
      Oscillator* oscillator1_;
      Oscillator* oscillator2_;
      Multiply* frequency1_;
      Multiply* frequency2_;
      Multiply* freq_mod1_;
      Multiply* freq_mod2_;
      Add* normalized_fm1_;
      Add* normalized_fm2_;
  };

  // The voice handler duplicates processors to produce polyphony.
  // Everything in the synthesizer we want per-voice instances of must be
  // contained in here.
  class TwytchVoiceHandler : public VoiceHandler {
    public:
      TwytchVoiceHandler();

      control_map getControls() { return controls_; }

      void setModWheel(mopo_float value);
      void setPitchWheel(mopo_float value);

      void setModulationSource(int index, std::string source);
      void setModulationDestination(int index, std::string destination);

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
      Interpolate* oscillator_mix_;

      Filter* filter_;
      Envelope* filter_envelope_;

      Multiply* output_;

      control_map controls_;
      output_map mod_sources_;
      input_map mod_destinations_;

      std::vector<std::string> mod_source_names_;
      std::vector<std::string> mod_destination_names_;
      Value* mod_matrix_scales_[MOD_MATRIX_SIZE];
      Multiply* mod_matrix_[MOD_MATRIX_SIZE];
      std::string current_mod_destinations_[MOD_MATRIX_SIZE];
  };

  // A modulation matrix source entry.
  class MatrixSourceValue : public Value {
    public:
      MatrixSourceValue(TwytchVoiceHandler* handler) :
          Value(0), handler_(handler), mod_index_(0) { }

      virtual Processor* clone() const { return new MatrixSourceValue(*this); }

      void setSources(const std::vector<std::string> &sources) {
        sources_ = sources;
      }

      void setModulationIndex(int mod_index) {
        mod_index_ = mod_index;
      }

      void set(mopo_float value) {
        Value::set(static_cast<int>(value));
        handler_->setModulationSource(mod_index_, sources_[value_]);
      }

    private:
      TwytchVoiceHandler* handler_;
      std::vector<std::string> sources_;
      int mod_index_;
  };

  // A modulation matrix destination entry.
  class MatrixDestinationValue : public Value {
    public:
      MatrixDestinationValue(TwytchVoiceHandler* handler) :
          Value(0), handler_(handler), mod_index_(0) { }

      virtual Processor* clone() const {
        return new MatrixDestinationValue(*this);
      }

      void setDestinations(const std::vector<std::string> &destinations) {
        destinations_ = destinations;
      }

      void setModulationIndex(int mod_index) {
        mod_index_ = mod_index;
      }

      void set(mopo_float value) {
        Value::set(static_cast<int>(value));
        handler_->setModulationDestination(mod_index_, destinations_[value_]);
      }

    private:
      TwytchVoiceHandler* handler_;
      std::vector<std::string> destinations_;
      int mod_index_;
  };

  // The overall twytch engine. All audio processing is contained in here.
  class TwytchEngine : public ProcessorRouter {
    public:
      TwytchEngine();

      control_map getControls();

      void setModWheel(mopo_float value) {
        voice_handler_->setModWheel(value);
      }
      void setPitchWheel(mopo_float value) {
        voice_handler_->setPitchWheel(value);
      }

      // Keyboard events.
      void noteOn(mopo_float note, mopo_float velocity = 1.0, int sample = 0);
      void noteOff(mopo_float note, int sample = 0);

      // Sustain pedal events.
      void sustainOn() { voice_handler_->sustainOn(); }
      void sustainOff() { voice_handler_->sustainOff(); }

    private:
      TwytchVoiceHandler* voice_handler_;
      Arpeggiator* arpeggiator_;

      control_map controls_;
  };
} // namespace mopo

#endif // TWYTCH_SYNTH_H
