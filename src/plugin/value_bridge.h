/* Copyright 2013-2017 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VALUE_BRIDGE_H
#define VALUE_BRIDGE_H

#include "JuceHeader.h"
#include "mopo.h"
#include "helm_common.h"

class ValueBridge : public AudioProcessorParameter {
  public:
    class Listener {
      public:
        virtual ~Listener() { }
        virtual void parameterChanged(std::string name, mopo::mopo_float value) = 0;
    };

    ValueBridge(std::string name, mopo::Value* value) :
        AudioProcessorParameter(), name_(name), value_(value), listener_(nullptr),
        source_changed_(false) {
      details_ = mopo::Parameters::getDetails(name);
      span_ = details_.max - details_.min;
    }

    float getValue() const override {
      return convertToPluginValue(value_->value());
    }

    void setValue(float value) override {
      if (listener_ && !source_changed_) {
        source_changed_ = true;
        mopo::mopo_float synth_value = convertToSynthValue(value);
        listener_->parameterChanged(name_.toStdString(), synth_value);
        source_changed_ = false;
      }
    }

    void setListener(Listener* listener) {
      listener_ = listener;
    }

    float getDefaultValue() const override {
      return convertToPluginValue(details_.default_value);
    }

    String getName(int maximumStringLength) const override {
      return name_.substring(0, maximumStringLength);
    }

    String getLabel() const override {
      return "";
    }

    String getText(float value, int maximumStringLength) const override {
      return String(getSkewedValue()).substring(0, maximumStringLength);
    }

    float getValueForText(const String &text) const override {
      return text.getFloatValue();
    }

    bool isAutomatable() const override {
      return true;
    }

    // Converts internal value to value from 0.0 to 1.0.
    float convertToPluginValue(mopo::mopo_float synth_value) const {
      return (synth_value - details_.min) / span_;
    }

    // Converts from value from 0.0 to 1.0 to internal synth value.
    float convertToSynthValue(mopo::mopo_float plugin_value) const {
      mopo::mopo_float percent = plugin_value;
      if (details_.steps) {
        mopo::mopo_float max_step = details_.steps - 1;
        percent = floor(percent * max_step + 0.5) / max_step;
      }

      return percent * span_ + details_.min;
    }

    void setValueNotifyHost(float new_value) {
      if (!source_changed_) {
        source_changed_ = true;
        setValueNotifyingHost(new_value);
        source_changed_ = false;
      }
    }

  private:
    float getSkewedValue() const {
      switch (details_.display_skew) {
        case mopo::ValueDetails::kQuadratic:
          return powf(value_->value(), 2.0f);
        case mopo::ValueDetails::kExponential:
          return powf(2.0f, value_->value());
        default:
          return value_->value();
      }
    }

    String name_;
    mopo::ValueDetails details_;
    mopo::mopo_float span_;
    mopo::Value* value_;
    Listener* listener_;
    bool source_changed_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueBridge)
};

#endif // VALUE_BRIDGE_H
