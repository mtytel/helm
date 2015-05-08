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

#ifndef VALUE_BRIDGE_H
#define VALUE_BRIDGE_H

#include "JuceHeader.h"
#include "mopo.h"

class ValueBridge : public AudioProcessorParameter {
  public:
    ValueBridge(std::string name, mopo::Value* value) :
        AudioProcessorParameter(), name_(name), value_(value) { }

    float getValue() const override { return value_->value(); }
    void setValue(float value) override { return value_->set(value); }

    float getDefaultValue() const override { return 0.0; }

    String getName(int maximumStringLength) const override {
      return name_;
    }

    String getLabel() const override {
      return "";
    }

    float getValueForText(const String &text) const override {
      return 0.0f;
    }
  
    bool isAutomatable() const override { return true; }

  private:
    String name_;
    mopo::Value* value_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueBridge)
};

#endif // VALUE_BRIDGE_H
