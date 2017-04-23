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
#ifndef REVERB_TUNING_H
#define REVERB_TUNING_H

namespace mopo {

  const int REVERB_MAX_MEMORY = 7192;
  const int NUM_COMB = 8;
  const int NUM_ALL_PASS = 4;
  const mopo_float FIXED_GAIN = 0.015f;
  const float scalewet = 3.0f;
  const float scaledry = 2.0f;
  const float scaledamp = 0.4f;
  const float scaleroom = 0.28f;
  const float offsetroom = 0.7f;
  const float initialroom = 0.5f;
  const float initialdamp = 0.5f;
  const float initialwet = 1.0f / scalewet;
  const float initialdry = 0.0f;
  const float initialwidth = 1.0f;
  const mopo_float STEREO_SPREAD = 0.00052154195;

  const mopo_float COMB_TUNINGS[NUM_COMB] = {
    0.02530612244,
    0.02693877551,
    0.02895691609,
    0.03074829931,
    0.03224489795,
    0.0338095238,
    0.03530612244,
    0.03666666666
  };

  const mopo_float ALL_PASS_TUNINGS[NUM_ALL_PASS] = {
    0.01260770975,
    0.01,
    0.0077324263,
    0.00510204081
  };

} // namespace mopo

#endif // REVERB_TUNING_H
