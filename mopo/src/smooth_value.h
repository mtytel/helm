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

#pragma once
#ifndef SMOOTH_VALUE_H
#define SMOOTH_VALUE_H

#include "value.h"

namespace mopo {

  class SmoothValue : public Value {
    public:
      SmoothValue(mopo_float value = 0.0);

      virtual Processor* clone() const override {
        return new SmoothValue(*this);
      }

      virtual void process() override;

      virtual void setSampleRate(int sample_rate) override;

      void set(mopo_float value) override { target_value_ = value; }
      void setHard(mopo_float value) {
        Value::set(value);
        target_value_ = value;
      }

      mopo_float value() const override { return target_value_; }

    private:
      void tick(int i);

      mopo_float target_value_;
      mopo_float decay_;
  };

  namespace cr {
    class SmoothValue : public Value {
      public:
        SmoothValue(mopo_float value = 0.0);

        virtual Processor* clone() const override {
          return new SmoothValue(*this);
        }

        virtual void process() override;

        virtual void setSampleRate(int sample_rate) override;
        virtual void setBufferSize(int buffer_size) override;

        void set(mopo_float value) override { target_value_ = value; }
        void setHard(mopo_float value) {
          Value::set(value);
          target_value_ = value;
        }

        mopo_float value() const override { return target_value_; }

      private:
        void computeDecay();

        mopo_float target_value_;
        mopo_float decay_;
        int num_samples_;
    };
  } // namespace cr
} // namespace mopo

#endif // SMOOTH_VALUE_H
