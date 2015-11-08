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

#include "distortion.h"
#include "utils.h"

namespace mopo {

  void Distortion::processTanh() {
    mopo_float threshold = input(kThreshold)->at(0);
    mopo_float compression_size = 1.0 - threshold;
    mopo_float compression_scale = 1.0 / threshold;

    const mopo_float* audio = input(kAudio)->source->buffer;
    mopo_float* dest = output()->buffer;

    int samples = buffer_size_;
    #pragma clang loop vectorize(enable) interleave(enable)
    for (int i = 0; i < samples; ++i) {
      mopo_float val = audio[i];
      mopo_float magnitude = fabs(val);
      tmp_buffer_[i] = compression_scale * magnitude - 1.0;
    }

    for (int i = 0; i < samples; ++i)
      tmp_buffer_[i] = utils::clamp(tmp_buffer_[i], 0.0, 1.0);

    #pragma clang loop vectorize(enable) interleave(enable)
    for (int i = 0; i < samples; ++i) {
      mopo_float val = audio[i];
      mopo_float val2 = val * val;
      mopo_float compressed = val / (1 + (val2 / (3 + (val2 / (5 + val2 / (7 + val2 / 9))))));

      dest[i] = INTERPOLATE(val, compressed, tmp_buffer_[i]);
    }
  }

  void Distortion::processHardClip() {
    for (int i = 0; i < buffer_size_; ++i) {
      mopo_float val = input(kAudio)->at(i);
      output()->buffer[i] = utils::clamp(val, -1.0, 1.0);
    }
  }

  void Distortion::processVelTanh() {
    mopo_float threshold = input(kThreshold)->at(0);
    mopo_float compression_size = 1.0 - threshold;
    mopo_float compression_scale = 1.0 / compression_size;

    for (int i = 0; i < buffer_size_; ++i) {
      mopo_float val = input(kAudio)->at(i);
      mopo_float delta_in = val - past_in_;

      mopo_float normal_out = past_out_ + delta_in;
      mopo_float magnitude = fabs(normal_out);
      float drive = utils::clamp(compression_scale * magnitude - 1.0, 0.0, 1.0);
      past_out_ = INTERPOLATE(normal_out, tanh(normal_out), drive);
      output()->buffer[i] = past_out_;
      past_in_ = val;
    }
  }

  Distortion::Distortion() : Processor(Distortion::kNumInputs, 1) {
    current_type_ = kNumTypes;
    reset();
  }

  void Distortion::process() {
    current_type_ = static_cast<Type>(static_cast<int>(inputs_->at(kType)->at(0)));

    switch(current_type_) {
      case kTanh:
        processTanh();
        break;
      case kHardClip:
        processHardClip();
        break;
      case kVelTanh:
        processVelTanh();
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
