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

#include "filter_selector.h"

#define TYPE_PADDING_X 7.0f
#define TYPE_PADDING_Y 3.0f

FilterSelector::FilterSelector(String name) : SynthSlider(name) { }

void FilterSelector::paint(Graphics& g) {
  static const PathStrokeType stroke(1.000f, PathStrokeType::curved, PathStrokeType::rounded);

  int selected = getValue();
  int num_types = getMaximum() - getMinimum() + 1;
  float cell_width = float(getWidth()) / num_types;
  g.setColour(Colour(0xff424242));
  g.fillRect(selected * cell_width, 0.0f, cell_width, float(getHeight()));

  g.setColour(selected == 0 ? Colour(0xffffffff) : Colour(0xffaaaaaa));
  g.strokePath(low_pass_, stroke);

  g.setColour(selected == 1 ? Colour(0xffffffff) : Colour(0xffaaaaaa));
  g.strokePath(high_pass_, stroke);

  g.setColour(selected == 2 ? Colour(0xffffffff) : Colour(0xffaaaaaa));
  g.strokePath(band_pass_, stroke);

  g.setColour(selected == 3 ? Colour(0xffffffff) : Colour(0xffaaaaaa));
  g.strokePath(low_shelf_, stroke);

  g.setColour(selected == 4 ? Colour(0xffffffff) : Colour(0xffaaaaaa));
  g.strokePath(high_shelf_, stroke);

  g.setColour(selected == 5 ? Colour(0xffffffff) : Colour(0xffaaaaaa));
  g.strokePath(band_shelf_, stroke);

  g.setColour(selected == 6 ? Colour(0xffffffff) : Colour(0xffaaaaaa));
  g.strokePath(all_pass_, stroke);
}

void FilterSelector::resized() {
  SynthSlider::resized();
  int num_types = getMaximum() - getMinimum() + 1;
  float cell_width = float(getWidth()) / num_types;
  float type_width = cell_width - 2 * TYPE_PADDING_X;
  float type_height = getHeight() - 2 * TYPE_PADDING_Y;

  resizeLowPass(0.0f * cell_width + TYPE_PADDING_X, TYPE_PADDING_Y, type_width, type_height);
  resizeHighPass(1.0f * cell_width + TYPE_PADDING_X, TYPE_PADDING_Y, type_width, type_height);
  resizeBandPass(2.0f * cell_width + TYPE_PADDING_X, TYPE_PADDING_Y, type_width, type_height);
  resizeLowShelf(3.0f * cell_width + TYPE_PADDING_X, TYPE_PADDING_Y, type_width, type_height);
  resizeHighShelf(4.0f * cell_width + TYPE_PADDING_X, TYPE_PADDING_Y, type_width, type_height);
  resizeBandShelf(5.0f * cell_width + TYPE_PADDING_X, TYPE_PADDING_Y, type_width, type_height);
  resizeAllPass(6.0f * cell_width + TYPE_PADDING_X, TYPE_PADDING_Y, type_width, type_height);
}

void FilterSelector::mouseEvent(const juce::MouseEvent &e) {
  float x = e.getPosition().getX();
  int index = x * (getMaximum() + 1) / getWidth();
  setValue(index);
}

void FilterSelector::mouseDown(const juce::MouseEvent &e) {
  mouseEvent(e);
}

void FilterSelector::mouseDrag(const juce::MouseEvent &e) {
  mouseEvent(e);
}

void FilterSelector::resizeLowPass(float x, float y, float width, float height) {
  low_pass_.clear();
  low_pass_.startNewSubPath(x, y + height / 2.0f);
  low_pass_.lineTo(x + width / 2.0f, y + height / 2.0f);
  low_pass_.lineTo(x + 4.0f * width / 6.0f, y + height / 4.0f);
  low_pass_.lineTo(x + width, y + height);
}

void FilterSelector::resizeHighPass(float x, float y, float width, float height) {
  high_pass_.clear();
  high_pass_.startNewSubPath(x + width, y + height / 2.0f);
  high_pass_.lineTo(x + width / 2.0f, y + height / 2.0f);
  high_pass_.lineTo(x + 2.0f * width / 6.0f, y + height / 4.0f);
  high_pass_.lineTo(x, y + height);
}

void FilterSelector::resizeBandPass(float x, float y, float width, float height) {
  band_pass_.clear();
  band_pass_.startNewSubPath(x, y + height);
  band_pass_.lineTo(x + width / 4.0f, y + 3.0f * height / 5.0f);
  band_pass_.lineTo(x + width / 2.0f, y);
  band_pass_.lineTo(x + 3.0f * width / 4.0f, y + 3.0f * height / 5.0f);
  band_pass_.lineTo(x + width, y + height);
}

void FilterSelector::resizeLowShelf(float x, float y, float width, float height) {
  low_shelf_.clear();
  low_shelf_.startNewSubPath(x, y + height / 4.0f);
  low_shelf_.lineTo(x + width / 3.0f, y + height / 4.0f);
  low_shelf_.lineTo(x + 2.0f * width / 3.0f, y + 3.0f * height / 4.0f);
  low_shelf_.lineTo(x + width, y + 3.0f * height / 4.0f);
}

void FilterSelector::resizeHighShelf(float x, float y, float width, float height) {
  high_shelf_.clear();
  high_shelf_.startNewSubPath(x, y + 3.0f * height / 4.0f);
  high_shelf_.lineTo(x + width / 3.0f, y + 3.0f * height / 4.0f);
  high_shelf_.lineTo(x + 2.0f * width / 3.0f, y + height / 4.0f);
  high_shelf_.lineTo(x + width, y + height / 4.0f);
}

void FilterSelector::resizeBandShelf(float x, float y, float width, float height) {
  band_shelf_.clear();
  band_shelf_.startNewSubPath(x, y + 3.0f * height / 4.0f);
  band_shelf_.lineTo(x + width / 4.0f, y + 3.0f * height / 4.0f);
  band_shelf_.lineTo(x + width / 2.0f, y + height / 4.0f);
  band_shelf_.lineTo(x + 3.0f * width / 4.0f, y + 3.0f * height / 4.0f);
  band_shelf_.lineTo(x + width, y + 3.0f * height / 4.0f);
}

void FilterSelector::resizeAllPass(float x, float y, float width, float height) {
  all_pass_.clear();
  all_pass_.startNewSubPath(x, y + height / 2.0f);
  all_pass_.lineTo(x + width, y + height / 2.0f);
}
