/* Copyright 2013-2017 Matt Tytel
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
    output()->clearTrigger();

    if (input(0)->source->triggered) {
      output()->trigger(input(0)->source->trigger_value,
                        input(0)->source->trigger_offset);
    }
    else if (input(1)->source->triggered) {
      output()->trigger(input(1)->source->trigger_value,
                        input(1)->source->trigger_offset);
    }
  }

  TriggerWait::TriggerWait() : Processor(kNumInputs, 1) { }

  void TriggerWait::waitTrigger(mopo_float trigger_value) {
    waiting_ = true;
    trigger_value_ = trigger_value;
  }

  void TriggerWait::sendTrigger(int trigger_offset) {
    if (waiting_)
      output()->trigger(trigger_value_, trigger_offset);
    waiting_ = false;
  }

  void TriggerWait::process() {
    output()->clearTrigger();

    if (input(kWait)->source->triggered &&
        input(kTrigger)->source->triggered) {

      if (input(kWait)->source->trigger_offset <=
          input(kTrigger)->source->trigger_offset) {
        waitTrigger(input(kWait)->source->trigger_value);
        sendTrigger(input(kTrigger)->source->trigger_offset);
      }
      else {
        sendTrigger(input(kTrigger)->source->trigger_offset);
        waitTrigger(input(kWait)->source->trigger_value);
      }
    }
    else if (input(kWait)->source->triggered)
      waitTrigger(input(kWait)->source->trigger_value);
    else if (input(kTrigger)->source->triggered)
      sendTrigger(input(kTrigger)->source->trigger_offset);
  }

  TriggerFilter::TriggerFilter(mopo_float trigger_filter) :
      Processor(kNumInputs, 1), trigger_filter_(trigger_filter) {

  }

  void TriggerFilter::process() {
    output()->clearTrigger();

    if (input(kTrigger)->source->triggered) {
      mopo_float trigger_value = input(kTrigger)->source->trigger_value;
      if (trigger_value == trigger_filter_) {
        output()->trigger(trigger_value,
                          input(kTrigger)->source->trigger_offset);
      }
    }
  }

  void TriggerEquals::process() {
    output()->clearTrigger();

    if (input(kTrigger)->source->triggered && input(kCondition)->at(0) == value_) {
      mopo_float trigger_value = input(kTrigger)->source->trigger_value;
      output()->trigger(trigger_value,
                        input(kTrigger)->source->trigger_offset);
    }
  }

  void TriggerNonZero::process() {
    output()->clearTrigger();

    if (input(kTrigger)->source->triggered && input(kCondition)->at(0)) {
      mopo_float trigger_value = input(kTrigger)->source->trigger_value;
      output()->trigger(trigger_value,
                        input(kTrigger)->source->trigger_offset);
    }
  }

  LegatoFilter::LegatoFilter() : Processor(kNumInputs, kNumOutputs),
                                 last_value_(kVoiceOff) { }

  void LegatoFilter::process() {
    output(kRetrigger)->clearTrigger();
    output(kRemain)->clearTrigger();
    if (!input(kTrigger)->source->triggered)
      return;

    if (input(kTrigger)->source->trigger_value == kVoiceOn &&
        last_value_ == kVoiceOn && input(kLegato)->at(0)) {
      output(kRemain)->trigger(
          input(kTrigger)->source->trigger_value,
          input(kTrigger)->source->trigger_offset);
    }
    else {
      output(kRetrigger)->trigger(
          input(kTrigger)->source->trigger_value,
          input(kTrigger)->source->trigger_offset);
    }
    last_value_ = input(kTrigger)->source->trigger_value;
  }

  PortamentoFilter::PortamentoFilter() : Processor(kNumInputs, 1),
                                         released_(true) { }

  void PortamentoFilter::updateReleased() {
    if (!input(kVoiceTrigger)->source->triggered)
      return;

    int voice_trigger = input(kVoiceTrigger)->source->trigger_value;
    if (voice_trigger == kVoiceOff)
      released_ = true;
  }

  void PortamentoFilter::updateTrigger() {
    output()->clearTrigger();
    if (!input(kFrequencyTrigger)->source->triggered)
      return;

    int state = static_cast<int>(input(kPortamento)->at(0));
    if (state == kPortamentoOff || (state == kPortamentoAuto && released_)) {
      output()->trigger(input(kFrequencyTrigger)->source->trigger_value,
                        input(kFrequencyTrigger)->source->trigger_offset);
      released_ = false;
    }
  }

  void PortamentoFilter::process() {
    updateTrigger();
    updateReleased();
  }
} // namespace mopo
