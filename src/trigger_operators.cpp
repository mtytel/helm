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
    outputs_[0]->clearTrigger();

    if (inputs_[0]->source->triggered) {
      outputs_[0]->trigger(inputs_[0]->source->trigger_value,
                           inputs_[0]->source->trigger_offset);
    }
    else if (inputs_[1]->source->triggered) {
      outputs_[0]->trigger(inputs_[1]->source->trigger_value,
                           inputs_[1]->source->trigger_offset);
    }
  }

  TriggerWait::TriggerWait() : Processor(kNumInputs, 1) { }

  void TriggerWait::waitTrigger(mopo_float trigger_value) {
    waiting_ = true;
    trigger_value_ = trigger_value;
  }

  void TriggerWait::sendTrigger(int trigger_offset) {
    if (waiting_)
      outputs_[0]->trigger(trigger_value_, trigger_offset);
    waiting_ = false;
  }

  void TriggerWait::process() {
    outputs_[0]->clearTrigger();

    if (inputs_[kWait]->source->triggered &&
        inputs_[kTrigger]->source->triggered) {

      if (inputs_[kWait]->source->trigger_offset <=
          inputs_[kTrigger]->source->trigger_offset) {
        waitTrigger(inputs_[kWait]->source->trigger_value);
        sendTrigger(inputs_[kTrigger]->source->trigger_offset);
      }
      else {
        sendTrigger(inputs_[kTrigger]->source->trigger_offset);
        waitTrigger(inputs_[kWait]->source->trigger_value);
      }
    }
    else if (inputs_[kWait]->source->triggered)
      waitTrigger(inputs_[kWait]->source->trigger_value);
    else if (inputs_[kTrigger]->source->triggered)
      sendTrigger(inputs_[kTrigger]->source->trigger_offset);
  }

  LegatoFilter::LegatoFilter() : Processor(kNumInputs, kNumOutputs),
                                 last_value_(kVoiceOff) { }

  void LegatoFilter::process() {
    outputs_[kRetrigger]->clearTrigger();
    outputs_[kRemain]->clearTrigger();
    if (!inputs_[kTrigger]->source->triggered)
      return;

    if (inputs_[kTrigger]->source->trigger_value == kVoiceOn &&
        last_value_ == kVoiceOn && inputs_[kLegato]->at(0)) {
      outputs_[kRemain]->trigger(inputs_[kTrigger]->source->trigger_value,
                                 inputs_[kTrigger]->source->trigger_offset);
    }
    else {
      outputs_[kRetrigger]->trigger(inputs_[kTrigger]->source->trigger_value,
                                    inputs_[kTrigger]->source->trigger_offset);
    }
    last_value_ = inputs_[kTrigger]->source->trigger_value;
  }

  PortamentoFilter::PortamentoFilter() : Processor(kNumInputs, 1),
                                         last_value_(kVoiceOff) { }

  void PortamentoFilter::process() {
    outputs_[0]->clearTrigger();
    if (!inputs_[kTrigger]->source->triggered)
      return;

    int state = static_cast<int>(inputs_[kPortamento]->at(0));
    if (inputs_[kTrigger]->source->trigger_value != kVoiceOff) {
      if (state == kPortamentoOff || (state == kPortamentoAuto &&
                                      last_value_ == kVoiceOff)) {
        outputs_[0]->trigger(inputs_[kTrigger]->source->trigger_value,
                             inputs_[kTrigger]->source->trigger_offset);
      }
    }

    last_value_ = inputs_[kTrigger]->source->trigger_value;
  }
} // namespace mopo
