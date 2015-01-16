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

#include "trigger_operators.h"

namespace mopo {

  TriggerCombiner::TriggerCombiner() : Processor(2, 1) { }

  void TriggerCombiner::process() {
    outputs_->at(0)->clearTrigger();

    if (inputs_->at(0)->source->triggered) {
      outputs_->at(0)->trigger(inputs_->at(0)->source->trigger_value,
                           inputs_->at(0)->source->trigger_offset);
    }
    else if (inputs_->at(1)->source->triggered) {
      outputs_->at(0)->trigger(inputs_->at(1)->source->trigger_value,
                           inputs_->at(1)->source->trigger_offset);
    }
  }

  TriggerWait::TriggerWait() : Processor(kNumInputs, 1) { }

  void TriggerWait::waitTrigger(mopo_float trigger_value) {
    waiting_ = true;
    trigger_value_ = trigger_value;
  }

  void TriggerWait::sendTrigger(int trigger_offset) {
    if (waiting_)
      outputs_->at(0)->trigger(trigger_value_, trigger_offset);
    waiting_ = false;
  }

  void TriggerWait::process() {
    outputs_->at(0)->clearTrigger();

    if (inputs_->at(kWait)->source->triggered &&
        inputs_->at(kTrigger)->source->triggered) {

      if (inputs_->at(kWait)->source->trigger_offset <=
          inputs_->at(kTrigger)->source->trigger_offset) {
        waitTrigger(inputs_->at(kWait)->source->trigger_value);
        sendTrigger(inputs_->at(kTrigger)->source->trigger_offset);
      }
      else {
        sendTrigger(inputs_->at(kTrigger)->source->trigger_offset);
        waitTrigger(inputs_->at(kWait)->source->trigger_value);
      }
    }
    else if (inputs_->at(kWait)->source->triggered)
      waitTrigger(inputs_->at(kWait)->source->trigger_value);
    else if (inputs_->at(kTrigger)->source->triggered)
      sendTrigger(inputs_->at(kTrigger)->source->trigger_offset);
  }

  LegatoFilter::LegatoFilter() : Processor(kNumInputs, kNumOutputs),
                                 last_value_(kVoiceOff) { }

  void LegatoFilter::process() {
    outputs_->at(kRetrigger)->clearTrigger();
    outputs_->at(kRemain)->clearTrigger();
    if (!inputs_->at(kTrigger)->source->triggered)
      return;

    if (inputs_->at(kTrigger)->source->trigger_value == kVoiceOn &&
        last_value_ == kVoiceOn && inputs_->at(kLegato)->at(0)) {
      outputs_->at(kRemain)->trigger(
          inputs_->at(kTrigger)->source->trigger_value,
          inputs_->at(kTrigger)->source->trigger_offset);
    }
    else {
      outputs_->at(kRetrigger)->trigger(
          inputs_->at(kTrigger)->source->trigger_value,
          inputs_->at(kTrigger)->source->trigger_offset);
    }
    last_value_ = inputs_->at(kTrigger)->source->trigger_value;
  }

  PortamentoFilter::PortamentoFilter() : Processor(kNumInputs, 1),
                                         last_value_(kVoiceOff) { }

  void PortamentoFilter::process() {
    outputs_->at(0)->clearTrigger();
    if (!inputs_->at(kTrigger)->source->triggered)
      return;

    int state = static_cast<int>(inputs_->at(kPortamento)->at(0));
    if (inputs_->at(kTrigger)->source->trigger_value != kVoiceOff) {
      if (state == kPortamentoOff || (state == kPortamentoAuto &&
                                      last_value_ == kVoiceOff)) {
        outputs_->at(0)->trigger(inputs_->at(kTrigger)->source->trigger_value,
                             inputs_->at(kTrigger)->source->trigger_offset);
      }
    }

    last_value_ = inputs_->at(kTrigger)->source->trigger_value;
  }
} // namespace mopo
