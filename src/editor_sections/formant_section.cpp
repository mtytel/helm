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

#include "formant_section.h"

#include "synth_button.h"
#include "synth_slider.h"

#define SLIDER_WIDTH 10

FormantSection::FormantSection(String name) : SynthSection(name) {
  addSlider(x_ = new SynthSlider("formant_x"));
  x_->setSliderStyle(Slider::LinearBar);
  x_->setPopupPlacement(BubbleComponent::below, 0);

  addSlider(y_ = new SynthSlider("formant_y"));
  y_->setSliderStyle(Slider::LinearBarVertical);
  y_->setPopupPlacement(BubbleComponent::right, 0);

  addAndMakeVisible(xy_pad_ = new XYPad());
  xy_pad_->setXSlider(x_);
  xy_pad_->setYSlider(y_);

  addButton(on_ = new SynthButton("formant_on"));
  setActivator(on_);
}

FormantSection::~FormantSection() {
  on_ = nullptr;
  x_ = nullptr;
  y_ = nullptr;
  xy_pad_ = nullptr;
}

void FormantSection::paintBackground(Graphics& g) {
  static const DropShadow component_shadow(Colour(0x88000000), 2, Point<int>(0, 1));
  SynthSection::paintBackground(g);
  component_shadow.drawForRectangle(g, xy_pad_->getBounds());
}

void FormantSection::resized() {
  int title_width = getTitleWidth();
  on_->setBounds(size_ratio_ * 2.0f, 0, title_width, title_width);

  int slider_width = size_ratio_ * SLIDER_WIDTH;
  x_->setBounds(0, getHeight() - slider_width, getWidth() - slider_width, slider_width);
  y_->setBounds(getWidth() - slider_width, title_width, slider_width,
                getHeight() - title_width - slider_width);
  xy_pad_->setBounds(0, title_width, getWidth() - slider_width,
                     getHeight() - title_width - slider_width);

  SynthSection::resized();
}

void FormantSection::setActive(bool active) {
  SynthSection::setActive(active);
  xy_pad_->setActive(active);
}
