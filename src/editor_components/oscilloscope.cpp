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

#include "oscilloscope.h"

#include "colors.h"
#include "helm_common.h"

#define FRAMES_PER_SECOND 15
#define GRID_CELL_WIDTH 8
#define PADDING_X -2
#define PADDING_Y 5

Oscilloscope::Oscilloscope() : output_memory_(nullptr) { }

Oscilloscope::~Oscilloscope() { }

void Oscilloscope::paint(Graphics& g) {
  static const DropShadow shadow(Colour(0xbb000000), 5, Point<int>(0, 0));
  g.drawImageWithin(background_,
                    0, 0, getWidth(), getHeight(), RectanglePlacement());

  shadow.drawForPath(g, wave_path_);

  g.setColour(Colors::graph_fill);
  g.fillPath(wave_path_);
  g.setColour(Colour(0xffaaaaaa));
  g.strokePath(wave_path_, PathStrokeType(1.0f, PathStrokeType::beveled, PathStrokeType::rounded));
}

void Oscilloscope::paintBackground(Graphics& g) {
  static const DropShadow shadow(Colour(0xbb000000), 5, Point<int>(0, 0));

  g.fillAll(Colour(0xff424242));

  g.setColour(Colour(0xff4a4a4a));
  for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
    g.drawLine(x, 0, x, getHeight());
  for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
    g.drawLine(0, y, getWidth(), y);
}


void Oscilloscope::resized() {
  const Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  float scale = display.scale;
  background_ = Image(Image::RGB, scale * getWidth(), scale * getHeight(), true);
  Graphics g(background_);
  g.addTransform(AffineTransform::scale(scale, scale));
  paintBackground(g);
}

void Oscilloscope::resetWavePath() {
  if (output_memory_ == nullptr)
    return;

  wave_path_.clear();

  float draw_width = getWidth() - 2.0f * PADDING_X;
  float draw_height = getHeight() - 2.0f * PADDING_Y;

  wave_path_.startNewSubPath(PADDING_X, getHeight() / 2.0f);
  for (int i = 0; i < mopo::MEMORY_RESOLUTION; ++i) {
    float t = (1.0f * i) / mopo::MEMORY_RESOLUTION;
    float val = output_memory_[i];
    if (val != val)
      val = 0.0f;
    wave_path_.lineTo(PADDING_X + t * draw_width, PADDING_Y + draw_height * ((1.0f - val) / 2.0f));
  }

  wave_path_.lineTo(getWidth() - PADDING_X, getHeight() / 2.0f);
}

void Oscilloscope::timerCallback() {
  resetWavePath();
  repaint();
}

void Oscilloscope::showRealtimeFeedback(bool show_feedback) {
  if (show_feedback)
    startTimerHz(FRAMES_PER_SECOND);
  else {
    stopTimer();
    wave_path_.clear();
    repaint();
  }
}
