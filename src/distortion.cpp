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

#include "distortion.h"
#include "utils.h"

namespace mopo {

  namespace {
    inline mopo_float linearFold(mopo_float t) {
      mopo_float adjust = 0.25 * t + 0.75;
      mopo_float range = adjust - trunc(adjust);
      return fabs(2.0 - 4.0 * range) - 1.0;
    }
  } // namespace

  void Distortion::processSoftClip() {
    const mopo_float* audio = input(kAudio)->source->buffer;
    const mopo_float* drive = input(kDrive)->source->buffer;
    const mopo_float* mix = input(kMix)->source->buffer;
    mopo_float* dest = output()->buffer;
    int buffer_size = buffer_size_;

    #pragma clang loop vectorize(enable) interleave(enable)
    for (int i = 0; i < buffer_size; ++i) {
      mopo_float distort = drive[i] * audio[i];
      dest[i] = utils::interpolate(audio[i], utils::quickTanh(distort), mix[i]);
    }
  }

  void Distortion::processHardClip() {
    const mopo_float* audio = input(kAudio)->source->buffer;
    const mopo_float* drive = input(kDrive)->source->buffer;
    const mopo_float* mix = input(kMix)->source->buffer;
    mopo_float* dest = output()->buffer;
    int buffer_size = buffer_size_;

    for (int i = 0; i < buffer_size; ++i) {
      mopo_float distort = utils::clamp(drive[i] * audio[i], -1.0, 1.0);
      dest[i] = utils::interpolate(audio[i], distort, mix[i]);
    }
  }

  void Distortion::processLinearFold() {
    const mopo_float* audio = input(kAudio)->source->buffer;
    const mopo_float* drive = input(kDrive)->source->buffer;
    const mopo_float* mix = input(kMix)->source->buffer;
    mopo_float* dest = output()->buffer;
    int buffer_size = buffer_size_;

#pragma clang loop vectorize(enable) interleave(enable)
    for (int i = 0; i < buffer_size; ++i) {
      mopo_float distort = linearFold(drive[i] * audio[i]);
      dest[i] = utils::interpolate(audio[i], distort, mix[i]);
    }
  }

  void Distortion::processSinFold() {
    const mopo_float* audio = input(kAudio)->source->buffer;
    const mopo_float* drive = input(kDrive)->source->buffer;
    const mopo_float* mix = input(kMix)->source->buffer;
    mopo_float* dest = output()->buffer;
    int buffer_size = buffer_size_;

#pragma clang loop vectorize(enable) interleave(enable)
    for (int i = 0; i < buffer_size; ++i) {
      mopo_float distort = linearFold(drive[i] * audio[i]);
      dest[i] = utils::interpolate(audio[i], distort, mix[i]);
    }
  }

  Distortion::Distortion() : Processor(Distortion::kNumInputs, 1) {
    current_type_ = kNumTypes;
    reset();
  }

  void Distortion::process() {
    current_type_ = static_cast<Type>(static_cast<int>(inputs_->at(kType)->at(0)));

    switch(current_type_) {
      case kSoftClip:
        processSoftClip();
        break;
      case kHardClip:
        processHardClip();
        break;
      case kLinearFold:
        processLinearFold();
        break;
      case kSinFold:
        processSinFold();
        break;
      default:
        output()->clearBuffer();
    }
  }

  void Distortion::reset() {
    past_in_ = 0.0;
    past_out_ = 0.0;
  }
} // namespace mopo
