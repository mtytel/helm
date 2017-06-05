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

#include "text_slider.h"

#include "fonts.h"

TextSlider::TextSlider(String name) : SynthSlider(name), short_lookup_(nullptr) { }

void TextSlider::paint(Graphics& g) {
  static const PathStrokeType stroke(1.000f, PathStrokeType::curved, PathStrokeType::rounded);
  static const float text_percentage = 0.7f;

  int num_types = getMaximum() - getMinimum() + 1;
  float cell_width = float(getWidth()) / num_types;
  float height = getHeight();

  int selected = getValue();
  g.setColour(Colour(0xff424242));
  g.fillRect(selected * cell_width, 0.0f, cell_width, height);

  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(height * text_percentage));

  const std::string* lookup = short_lookup_;
  if (lookup == nullptr)
    lookup = string_lookup_;

  for (int i = 0; i < num_types; ++i) {
    if (selected == i)
      g.setColour(Colour(0xffffffff));
    else
      g.setColour(Colour(0xffaaaaaa));

    g.drawText(lookup[i], i * cell_width, 0.0f, cell_width, height, Justification::centred);
  }
}

void TextSlider::resized() {
  SynthSlider::resized();
}

void TextSlider::mouseEvent(const juce::MouseEvent &e) {
  float x = e.getPosition().getX();
  int index = x * (getMaximum() + 1) / getWidth();
  setValue(index);
}

void TextSlider::mouseDown(const juce::MouseEvent &e) {
  mouseEvent(e);
}

void TextSlider::mouseDrag(const juce::MouseEvent &e) {
  mouseEvent(e);
}
