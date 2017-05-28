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
#ifndef DISTORTION_H
#define DISTORTION_H

#include "processor.h"

#include <complex>

namespace mopo {

  class Distortion : public Processor {
    public:
      enum Inputs {
        kAudio,
        kOn,
        kType,
        kDrive,
        kMix,
        kNumInputs
      };

      enum Type {
        kSoftClip,
        kHardClip,
        kLinearFold,
        kSinFold,
        kNumTypes
      };

      Distortion();
      virtual ~Distortion() { }

      virtual Processor* clone() const override {
        return new Distortion(*this);
      }

      virtual void process() override;

      void processSoftClip();
      void processHardClip();
      void processLinearFold();
      void processSinFold();

    private:
      mopo_float last_mix_;
      mopo_float last_drive_;
  };
} // namespace mopo

#endif // DISTORTION_H
