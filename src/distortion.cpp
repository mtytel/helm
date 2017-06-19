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
      mopo_float range = adjust - floor(adjust);
      return fabs(2.0 - 4.0 * range) - 1.0;
    }

    inline mopo_float sinFold(mopo_float t) {
      mopo_float adjust = -0.25 * t + 0.5;
      mopo_float range = adjust - floor(adjust);
      return utils::quickSin1(range);
    }
  } // namespace


  Distortion::Distortion() :
      Processor(Distortion::kNumInputs, 1), last_mix_(0.0), last_drive_(0.0) { }

  void Distortion::processSoftClip() {
    const mopo_float* audio = input(kAudio)->source->buffer;
    mopo_float next_drive = input(kDrive)->at(0);
    mopo_float mult_drive = (next_drive - last_drive_) / buffer_size_;
    mopo_float next_mix = input(kMix)->at(0);
    mopo_float mult_mix = (next_mix - last_mix_) / buffer_size_;

    mopo_float* dest = output()->buffer;
    int buffer_size = buffer_size_;

    VECTORIZE_LOOP
    for (int i = 0; i < buffer_size; ++i) {
      mopo_float mix = last_mix_ + i * mult_mix;
      mopo_float drive = last_drive_ + i * mult_drive;
      mopo_float distort = drive * audio[i];
      dest[i] = utils::interpolate(audio[i], utils::quickTanh(distort), mix);
    }

    last_mix_ = next_mix;
    last_drive_ = next_drive;
  }

  void Distortion::processHardClip() {
    const mopo_float* audio = input(kAudio)->source->buffer;
    mopo_float next_drive = input(kDrive)->at(0);
    mopo_float mult_drive = (next_drive - last_drive_) / buffer_size_;
    mopo_float next_mix = input(kMix)->at(0);
    mopo_float mult_mix = (next_mix - last_mix_) / buffer_size_;

    mopo_float* dest = output()->buffer;
    int buffer_size = buffer_size_;

    for (int i = 0; i < buffer_size; ++i) {
      mopo_float mix = last_mix_ + i * mult_mix;
      mopo_float drive = last_drive_ + i * mult_drive;
      mopo_float distort = utils::clamp(drive * audio[i], -1.0, 1.0);
      dest[i] = utils::interpolate(audio[i], distort, mix);
    }

    last_mix_ = next_mix;
    last_drive_ = next_drive;
  }

  void Distortion::processLinearFold() {
    const mopo_float* audio = input(kAudio)->source->buffer;
    mopo_float next_drive = input(kDrive)->at(0);
    mopo_float mult_drive = (next_drive - last_drive_) / buffer_size_;
    mopo_float next_mix = input(kMix)->at(0);
    mopo_float mult_mix = (next_mix - last_mix_) / buffer_size_;

    mopo_float* dest = output()->buffer;
    int buffer_size = buffer_size_;

    VECTORIZE_LOOP
    for (int i = 0; i < buffer_size; ++i) {
      mopo_float mix = last_mix_ + i * mult_mix;
      mopo_float drive = last_drive_ + i * mult_drive;
      mopo_float distort = linearFold(drive * audio[i]);
      dest[i] = utils::interpolate(audio[i], distort, mix);
    }

    last_mix_ = next_mix;
    last_drive_ = next_drive;
  }

  void Distortion::processSinFold() {
    const mopo_float* audio = input(kAudio)->source->buffer;
    mopo_float next_drive = input(kDrive)->at(0);
    mopo_float mult_drive = (next_drive - last_drive_) / buffer_size_;
    mopo_float next_mix = input(kMix)->at(0);
    mopo_float mult_mix = (next_mix - last_mix_) / buffer_size_;

    mopo_float* dest = output()->buffer;
    int buffer_size = buffer_size_;

    VECTORIZE_LOOP
    for (int i = 0; i < buffer_size; ++i) {
      mopo_float mix = last_mix_ + i * mult_mix;
      mopo_float drive = last_drive_ + i * mult_drive;
      mopo_float distort = sinFold(drive * audio[i]);
      dest[i] = utils::interpolate(audio[i], distort, mix);
    }

    last_mix_ = next_mix;
    last_drive_ = next_drive;
  }

  void Distortion::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kAudio));

    Type type = static_cast<Type>(static_cast<int>(input(kType)->at(0)));
    if (input(kOn)->at(0) == 0.0) {
      utils::copyBuffer(output()->buffer, input(kAudio)->source->buffer, buffer_size_);
      return;
    }

    switch(type) {
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
        utils::copyBuffer(output()->buffer, input(kAudio)->source->buffer, buffer_size_);
    }
  }
} // namespace mopo
