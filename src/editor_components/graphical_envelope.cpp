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

#include "graphical_envelope.h"

#include "colors.h"

namespace {
  const float ATTACK_RANGE_PERCENT = 0.33f;
  const float DECAY_RANGE_PERCENT = 0.33f;
  const float HOVER_DISTANCE = 20.0f;
  const int GRID_CELL_WIDTH = 8;
  const float MARKER_WIDTH = 6.0f;
} // namespace


GraphicalEnvelope::GraphicalEnvelope() {
  attack_hover_ = false;
  decay_hover_ = false;
  sustain_hover_ = false;
  release_hover_ = false;
  mouse_down_ = false;

  attack_slider_ = nullptr;
  decay_slider_ = nullptr;
  sustain_slider_ = nullptr;
  release_slider_ = nullptr;
}

GraphicalEnvelope::~GraphicalEnvelope() { }

void GraphicalEnvelope::paint(Graphics& g) {
  static const PathStrokeType stroke(1.5f, PathStrokeType::beveled, PathStrokeType::rounded);
  static const DropShadow shadow(Colour(0xbb000000), 5, Point<int>(0, 0));

  g.fillAll(Colour(0xff424242));

  g.setColour(Colour(0xff4a4a4a));
  for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
    g.drawLine(x, 0, x, getHeight());
  for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
    g.drawLine(0, y, getWidth(), y);

  shadow.drawForPath(g, envelope_line_);
  g.setColour(Colors::graph_fill);
  g.fillPath(envelope_line_);

  g.setColour(Colour(0xff505050));
  g.drawLine(getAttackX(), 0.0f, getAttackX(), getHeight());
  g.drawLine(getDecayX(), getSustainY(), getDecayX(), getHeight());

  g.setColour(Colors::modulation);
  g.strokePath(envelope_line_, stroke);

  float hover_line_x = -20;
  if (attack_hover_)
    hover_line_x = getAttackX();
  else if (decay_hover_)
    hover_line_x = getDecayX();
  else if (release_hover_)
    hover_line_x = getReleaseX();

  g.setColour(Colour(0xbbffffff));
  g.fillRect(hover_line_x - 0.5f, 0.0f, 1.0f, 1.0f * getHeight());

  if (sustain_hover_) {
    if (mouse_down_) {
      g.setColour(Colour(0x11ffffff));
      g.fillEllipse(getDecayX() - 20.0, getSustainY() - 20.0, 40.0, 40.0);
    }

    g.setColour(Colour(0xbbffffff));
    g.drawEllipse(getDecayX() - 7.0, getSustainY() - 7.0, 14.0, 14.0, 1.0);
  }
  else if (mouse_down_) {
    g.setColour(Colour(0x11ffffff));
    g.fillRect(hover_line_x - 10.0f, 0.0f, 20.0f, 1.0f * getHeight());
  }

  g.setColour(Colors::modulation);
  g.fillEllipse(getDecayX() - MARKER_WIDTH / 2.0f, getSustainY() - MARKER_WIDTH / 2.0f,
                MARKER_WIDTH, MARKER_WIDTH);
  g.setColour(Colour(0xff000000));
  g.fillEllipse(getDecayX() - MARKER_WIDTH / 4.0f, getSustainY() - MARKER_WIDTH / 4.0f,
                MARKER_WIDTH / 2.0f, MARKER_WIDTH / 2.0f);
}

void GraphicalEnvelope::resized() {
  resetEnvelopeLine();
}

void GraphicalEnvelope::mouseMove(const MouseEvent& e) {
  float x = e.getPosition().x;
  float attack_delta = fabs(x - getAttackX());
  float decay_delta = fabs(x - getDecayX());
  float release_delta = fabs(x - getReleaseX());
  float sustain_delta = fabs(e.getPosition().y - getSustainY());

  bool a_hover = attack_delta < decay_delta && attack_delta < HOVER_DISTANCE;
  bool d_hover = !attack_hover_ && decay_delta < release_delta && decay_delta < HOVER_DISTANCE;
  bool r_hover = !decay_hover_ && release_delta < HOVER_DISTANCE;
  bool s_hover = !a_hover && !r_hover && x > getDecayX() - HOVER_DISTANCE &&
  x < getDecayX() + HOVER_DISTANCE && sustain_delta < HOVER_DISTANCE;

  if (a_hover != attack_hover_ || d_hover != decay_hover_ ||
      s_hover != sustain_hover_ || r_hover != release_hover_) {
    attack_hover_ = a_hover;
    decay_hover_ = d_hover;
    sustain_hover_ = s_hover;
    release_hover_ = r_hover;
    repaint();
  }
}

void GraphicalEnvelope::mouseExit(const MouseEvent& e) {
  attack_hover_ = false;
  decay_hover_ = false;
  sustain_hover_ = false;
  release_hover_ = false;
  repaint();
}

void GraphicalEnvelope::mouseDown(const MouseEvent& e) {
  mouse_down_ = true;
  repaint();
}

void GraphicalEnvelope::mouseDrag(const MouseEvent& e) {
  if (attack_hover_)
    setAttackX(e.getPosition().x);
  else if (decay_hover_)
    setDecayX(e.getPosition().x);
  else if (release_hover_)
    setReleaseX(e.getPosition().x);

  if (sustain_hover_)
    setSustainY(e.getPosition().y);

  if (attack_hover_ || decay_hover_ || sustain_hover_ || release_hover_) {
    resetEnvelopeLine();
    repaint();
  }
}

void GraphicalEnvelope::mouseUp(const MouseEvent& e) {
  mouse_down_ = false;
  repaint();
}

void GraphicalEnvelope::guiChanged(SynthSlider* slider) {
  resetEnvelopeLine();
  repaint();
}

float GraphicalEnvelope::getAttackX() {
  if (!attack_slider_)
    return 0.0;

  double percent = attack_slider_->valueToProportionOfLength(attack_slider_->getValue());
  return 1 + (getWidth() - 1) * percent * ATTACK_RANGE_PERCENT;
}

float GraphicalEnvelope::getDecayX() {
  if (!decay_slider_)
    return 0.0;

  double percent = decay_slider_->valueToProportionOfLength(decay_slider_->getValue());
  return getAttackX() + getWidth() * percent * DECAY_RANGE_PERCENT;
}

float GraphicalEnvelope::getSustainY() {
  if (!sustain_slider_)
    return 0.0;

  double percent = sustain_slider_->valueToProportionOfLength(sustain_slider_->getValue());
  return getHeight() * (1.0 - percent);
}

float GraphicalEnvelope::getReleaseX() {
  if (!release_slider_)
    return 0.0;

  double percent = release_slider_->valueToProportionOfLength(release_slider_->getValue());
  return getDecayX() + getWidth() * percent * (1.0 - DECAY_RANGE_PERCENT - ATTACK_RANGE_PERCENT);
}

void GraphicalEnvelope::setAttackX(double x) {
  if (!attack_slider_)
    return;

  double percent = x / (getWidth() * ATTACK_RANGE_PERCENT);
  attack_slider_->setValue(attack_slider_->proportionOfLengthToValue(percent));
}

void GraphicalEnvelope::setDecayX(double x) {
  if (!decay_slider_)
    return;

  double percent = (x - getAttackX()) / (getWidth() * DECAY_RANGE_PERCENT);
  decay_slider_->setValue(decay_slider_->proportionOfLengthToValue(percent));
}

void GraphicalEnvelope::setSustainY(double y) {
  if (!sustain_slider_)
    return;

  sustain_slider_->setValue(sustain_slider_->proportionOfLengthToValue(1.0 - y / getHeight()));
}

void GraphicalEnvelope::setReleaseX(double x) {
  if (!release_slider_)
    return;

  double percent = (x - getDecayX()) /
  (getWidth() * (1.0 - DECAY_RANGE_PERCENT - ATTACK_RANGE_PERCENT));
  release_slider_->setValue(release_slider_->proportionOfLengthToValue(percent));
}

void GraphicalEnvelope::setAttackSlider(SynthSlider* attack_slider) {
  attack_slider_ = attack_slider;
  attack_slider_->addSliderListener(this);
  resetEnvelopeLine();
  repaint();
}

void GraphicalEnvelope::setDecaySlider(SynthSlider* decay_slider) {
  decay_slider_ = decay_slider;
  decay_slider_->addSliderListener(this);
  resetEnvelopeLine();
  repaint();
}

void GraphicalEnvelope::setSustainSlider(SynthSlider* sustain_slider) {
  sustain_slider_ = sustain_slider;
  sustain_slider_->addSliderListener(this);
  resetEnvelopeLine();
  repaint();
}

void GraphicalEnvelope::setReleaseSlider(SynthSlider* release_slider) {
  release_slider_ = release_slider;
  release_slider_->addSliderListener(this);
  resetEnvelopeLine();
  repaint();
}

void GraphicalEnvelope::resetEnvelopeLine() {
  envelope_line_.clear();
  envelope_line_.startNewSubPath(1, getHeight());
  envelope_line_.lineTo(getAttackX(), 0.0f);
  envelope_line_.quadraticTo(0.5f * (getAttackX() + getDecayX()), getSustainY(),
                             getDecayX(), getSustainY());

  envelope_line_.quadraticTo(0.5f * (getReleaseX() + getDecayX()), getHeight(),
                             getReleaseX(), getHeight());
}
