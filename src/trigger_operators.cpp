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
    output(0)->clearTrigger();

    if (input(0)->source->triggered) {
      output(0)->trigger(input(0)->source->trigger_value,
                         input(0)->source->trigger_offset);
    }
    else if (input(1)->source->triggered) {
      output(0)->trigger(input(1)->source->trigger_value,
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
      output(0)->trigger(trigger_value_, trigger_offset);
    waiting_ = false;
  }

  void TriggerWait::process() {
    output(0)->clearTrigger();

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

  void PortamentoFilter::process() {
    output(0)->clearTrigger();
    if (!input(kTrigger)->source->triggered)
      return;

    int state = static_cast<int>(input(kPortamento)->at(0));
    int trigger_value = input(kTrigger)->source->trigger_value;
    if (trigger_value == kVoiceOff)
      released_ = true;
    else if (state == kPortamentoOff || (state == kPortamentoAuto &&
                                         released_)) {
      output(0)->trigger(input(kTrigger)->source->trigger_value,
                         input(kTrigger)->source->trigger_offset);
    }

    if (trigger_value == kVoiceOn)
      released_ = false;
  }
} // namespace mopo
