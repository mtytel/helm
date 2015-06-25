/* Copyright 2013-2015 Matt Tytel
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

#include "oscilloscope.h"

#define FRAMES_PER_SECOND 15
#define MAX_RESOLUTION 54
#define GRID_CELL_WIDTH 8
#define PADDING_X -2
#define PADDING_Y 5

Oscilloscope::Oscilloscope(int num_samples) {
  output_memory_ = nullptr;
  samples_to_show_ = num_samples;
  setFramesPerSecond(FRAMES_PER_SECOND);
}

Oscilloscope::~Oscilloscope() { }

void Oscilloscope::paint(Graphics& g) {
  static const DropShadow shadow(Colour(0xbb000000), 5, Point<int>(0, 0));
  g.fillAll(Colour(0xff424242));

  g.setColour(Colour(0xff4a4a4a));
  for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
    g.drawLine(x, 0, x, getHeight());
  for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
    g.drawLine(0, y, getWidth(), y);

  shadow.drawForPath(g, wave_path_);

  g.setColour(Colour(0xff565656));
  g.fillPath(wave_path_);
  g.setColour(Colour(0xffaaaaaa));
  g.strokePath(wave_path_, PathStrokeType(1.0f, PathStrokeType::beveled, PathStrokeType::rounded));
}

void Oscilloscope::resized() {
  resetWavePath();
}

void Oscilloscope::resetWavePath() {
  if (output_memory_ == nullptr)
    return;

  wave_path_.clear();

  float draw_width = getWidth() - 2.0f * PADDING_X;
  float draw_height = getHeight() - 2.0f * PADDING_Y;

  wave_path_.startNewSubPath(PADDING_X, getHeight() / 2.0f);
  int inc = samples_to_show_ / MAX_RESOLUTION;
  for (int i = samples_to_show_; i >= 0; i -= inc) {
    float t = (samples_to_show_ - 1.0f * i) / samples_to_show_;
    float val = output_memory_->get(i);
    if (val != val)
      val = 0.0f;
    wave_path_.lineTo(PADDING_X + t * draw_width, PADDING_Y + draw_height * ((1.0f - val) / 2.0f));
  }

  wave_path_.lineTo(getWidth() - PADDING_X, getHeight() / 2.0f);
}

void Oscilloscope::update() {
  resetWavePath();
}
