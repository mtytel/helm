/* Copyright 2013-2016 Matt Tytel
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

#include "xy_pad.h"
#include "utils.h"

#define GRID_CELL_WIDTH 8
#define GRID_CELL_HEIGHT 16

XYPad::XYPad() {
  mouse_down_ = false;
  x_slider_ = nullptr;
  y_slider_ = nullptr;
  active_ = true;
  setOpaque(true);
}

XYPad::~XYPad() { }

void XYPad::paintBackground(Graphics& g) {
  g.fillAll(Colour(0xff424242));

  g.setColour(Colour(0xff4a4a4a));
  for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
    g.drawLine(x, 0, x, getHeight());
  for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
    g.drawLine(0, y, getWidth(), y);
}

void XYPad::paint(Graphics& g) {
  static const PathStrokeType stroke(1.5f, PathStrokeType::beveled, PathStrokeType::rounded);
  static const DropShadow shadow(Colour(0xbb000000), 5, Point<int>(0, 0));

  g.drawImage(background_,
              0, 0, getWidth(), getHeight(),
              0, 0, background_.getWidth(), background_.getHeight());

  float x = x_slider_->getValue() * getWidth();
  float y = (1.0f - y_slider_->getValue()) * getHeight();

  Path target;
  target.addEllipse(x - 6.0f, y - 6.0f, 12.0f, 12.0f);
  shadow.drawForPath(g, target);

  g.setColour(Colour(0xff565656));
  g.fillPath(target);

  if (active_)
    g.setColour(Colour(0xff03a9f4));
  else
    g.setColour(Colour(0xff777777));
  g.strokePath(target, stroke);
  g.fillEllipse(x - 1.0f, y - 1.0f, 2.0f, 2.0f);

  if (mouse_down_) {
    g.setColour(Colour(0x11ffffff));
    g.fillEllipse(x - 20.0, y - 20.0, 40.0, 40.0);
  }
}

void XYPad::resized() {
  const Desktop::Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  float scale = display.scale;
  background_ = Image(Image::RGB, scale * getWidth(), scale * getHeight(), true);
  Graphics g(background_);
  g.addTransform(AffineTransform::scale(scale, scale));
  paintBackground(g);
}

void XYPad::mouseDown(const MouseEvent& e) {
  setSlidersFromPosition(e.getPosition());
  mouse_down_ = true;
  repaint();
}

void XYPad::mouseDrag(const MouseEvent& e) {
  setSlidersFromPosition(e.getPosition());
}

void XYPad::mouseUp(const MouseEvent& e) {
  mouse_down_ = false;
  repaint();
}

void XYPad::setSlidersFromPosition(Point<int> position) {
  if (x_slider_) {
    double percent = mopo::utils::clamp((1.0 * position.x) / getWidth(), 0.0, 1.0);
    x_slider_->setValue(percent);
  }
  if (y_slider_) {
    double percent = mopo::utils::clamp(1.0 - (1.0 * position.y) / getHeight(), 0.0, 1.0);
    y_slider_->setValue(percent);
  }
}

void XYPad::sliderValueChanged(Slider* moved_slider) {
  repaint();
}

void XYPad::setXSlider(Slider* slider) {
  if (x_slider_)
    x_slider_->removeListener(this);

  x_slider_ = slider;
  x_slider_->addListener(this);
  repaint();
}

void XYPad::setYSlider(Slider* slider) {
  if (y_slider_)
    y_slider_->removeListener(this);
  y_slider_ = slider;
  y_slider_->addListener(this);
  repaint();
}

void XYPad::setActive(bool active) {
  active_ = active;
  repaint();
}
