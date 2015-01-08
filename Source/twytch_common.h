/* Copyright 2013-2014 Little IO
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
#ifndef TWYTCH_COMMON_H
#define TWYTCH_COMMON_H

#include "mopo.h"
#include "value.h"

#include <map>
#include <string>

namespace mopo {

  // A container for a given control its metadata such as maximum and minimum
  // value.
  class Control {
    public:
      Control(Value* value, mopo_float min, mopo_float max, int resolution) :
          value_(value), min_(min), max_(max),
          resolution_(resolution), midi_learn_(0) {
        current_value_ = value->value();
      }

      Control(Value* value, std::vector<std::string> strings, int resolution) :
          value_(value), min_(0), max_(resolution),
          resolution_(resolution), midi_learn_(0), display_strings_(strings) {
        current_value_ = value->value();
      }

      Control() : value_(0), min_(0), max_(0), current_value_(0),
                  resolution_(0), midi_learn_(0) { }

      mopo_float min() const { return min_; }
      mopo_float max() const { return max_; }
      int resolution() const { return resolution_; }

      mopo_float getIncrementSize() const {
        if (resolution_ == 0 || resolution_ >= MIDI_SIZE)
          return 0;
        return (max_ - min_) / resolution_;
      }

      void set(mopo_float val) {
        current_value_ = CLAMP(val, min_, max_);
        value_->set(current_value_);
      }

      mopo_float getPercentage() const {
        return (current_value_ - min_) / (max_ - min_);
      }

      void setPercentage(mopo_float percentage) {
        int index = resolution_ * percentage;
        set(min_ + index * (max_ - min_) / resolution_);
      }

      void setMidi(int midi_val) {
        setPercentage(midi_val / (MIDI_SIZE - 1.0));
      }

      void increment() {
        set(current_value_ + (max_ - min_) / resolution_);
      }

      void decrement() {
        set(current_value_ - (max_ - min_) / resolution_);
      }

      int midi_learn() const { return midi_learn_; }

      void midi_learn(float midi) { midi_learn_ = midi; }

      std::vector<std::string> display_strings() const {
        return display_strings_;
      }

      mopo_float current_value() const { return current_value_; }

      const Value* value() const { return value_; }

      bool isBipolar() const { return max_ == -min_; }

    private:
      Value* value_;
      mopo_float min_, max_, current_value_;
      int resolution_, midi_learn_;
      std::vector<std::string> display_strings_;
  };

  typedef std::map<std::string, Control*> control_map;
  typedef std::map<std::string, Processor*> input_map;
  typedef std::map<std::string, Processor::Output*> output_map;

} // namespace mopo

#endif // TWYTCH_COMMON_H
