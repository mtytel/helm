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

#include "twytch_engine.h"

#include "arpeggiator.h"
#include "delay.h"
#include "operators.h"
#include "smooth_value.h"
#include "twytch_voice_handler.h"
#include "utils.h"
#include "value.h"

#include <fenv.h>

namespace mopo {

  TwytchEngine::TwytchEngine() {
    fesetenv(FE_DFL_DISABLE_SSE_DENORMS_ENV);

    // Voice Handler.
    Value* polyphony = new Value(1);
    voice_handler_ = new TwytchVoiceHandler();
    voice_handler_->setPolyphony(64);
    voice_handler_->plug(polyphony, VoiceHandler::kPolyphony);
    controls_["polyphony"] = polyphony;

    // Arpeggiator.
    Value* arp_frequency = new Value(5.0);
    Value* arp_gate = new Value(0.5);
    arpeggiator_ = new Arpeggiator(voice_handler_);
    arpeggiator_->plug(arp_frequency, Arpeggiator::kFrequency);
    arpeggiator_->plug(arp_gate, Arpeggiator::kGate);
    controls_["arp frequency"] = arp_frequency;
    controls_["arp gate"] = arp_gate;

    addProcessor(arpeggiator_);
    addProcessor(voice_handler_);

    // Delay effect.
    SmoothValue* delay_time = new SmoothValue(0.06);
    SmoothValue* delay_feedback = new SmoothValue(-0.3);
    SmoothValue* delay_wet = new SmoothValue(0.3);

    Delay* delay = new Delay();
    delay->plug(voice_handler_, Delay::kAudio);
    delay->plug(delay_time, Delay::kDelayTime);
    delay->plug(delay_feedback, Delay::kFeedback);
    delay->plug(delay_wet, Delay::kWet);

    addProcessor(delay_time);
    addProcessor(delay_feedback);
    addProcessor(delay_wet);
    addProcessor(delay);

    controls_["delay time"] = delay_time;
    controls_["delay feedback"] = delay_feedback;
    controls_["delay dry wet"] = delay_wet;

    // Volume.
    SmoothValue* volume = new SmoothValue(0.6);
    Multiply* scaled_audio = new Multiply();
    scaled_audio->plug(delay, 0);
    scaled_audio->plug(volume, 1);
    Clamp* clamp = new Clamp();
    clamp->plug(scaled_audio);

    addProcessor(clamp);
    addProcessor(volume);
    addProcessor(scaled_audio);
    registerOutput(clamp->output());

    controls_["volume"] = volume;
  }

  control_map TwytchEngine::getControls() {
    control_map voice_controls = voice_handler_->getControls();
    voice_controls.insert(controls_.begin(), controls_.end());
    return voice_controls;
  }

  void TwytchEngine::noteOn(mopo_float note, mopo_float velocity, int sample) {
    arpeggiator_->noteOn(note, velocity, sample);
  }

  void TwytchEngine::noteOff(mopo_float note, int sample) {
    arpeggiator_->noteOff(note, sample);
  }

  void TwytchEngine::setModWheel(mopo_float value) {
    voice_handler_->setModWheel(value);
  }

  void TwytchEngine::setPitchWheel(mopo_float value) {
    voice_handler_->setPitchWheel(value);
  }

  void TwytchEngine::sustainOn() {
    voice_handler_->sustainOn();
  }

  void TwytchEngine::sustainOff() {
    voice_handler_->sustainOff();
  }
} // namespace mopo
