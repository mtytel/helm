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

#include "graphical_step_sequencer.h"

#include "colors.h"
#include "synth_gui_interface.h"

#define FRAMES_PER_SECOND 24

GraphicalStepSequencer::GraphicalStepSequencer() {
  num_steps_slider_ = nullptr;
  step_generator_output_ = nullptr;
  last_step_ = -1;
  highlighted_step_ = -1;
  num_steps_ = 1;

  setOpaque(true);
}

GraphicalStepSequencer::~GraphicalStepSequencer() { }

void GraphicalStepSequencer::paintBackground(Graphics& g) {
  static const DropShadow shadow(Colour(0xbb000000), 1, Point<int>(0, 0));
  if (sequence_.size() == 0 || num_steps_slider_ == nullptr)
    return;

  g.fillAll(Colour(0xff424242));

  float x_inc = getWidth() / (1.0f * num_steps_);
  float line_width = 1.5f * getHeight() / 80.0f;
  g.setColour(Colour(0xff545454));
  for (int i = 1; i * x_inc < getWidth(); ++i)
    g.drawLine(i * x_inc, 0, i * x_inc, getHeight());

  // Draw shadows.
  float x = 0.0f;
  for (int i = 0; i < num_steps_; ++i) {
    float val = sequence_[i]->getValue();
    float bar_position = (getHeight() - 1.0f) * ((1.0f - val) / 2.0f);
    Rectangle<int> rect(x, bar_position, x_inc, 1);
    shadow.drawForRectangle(g, rect);
    x += x_inc;
  }

  // Draw bars.
  x = 0.0f;
  for (int i = 0; i < num_steps_; ++i) {
    float val = sequence_[i]->getValue();
    float bar_position = (getHeight() - 1.0f) * ((1.0f - val) / 2.0f);
    g.setColour(Colors::graph_fill);

    if (val >= 0)
      g.fillRect(x, bar_position, x_inc, proportionOfHeight(0.5f) - bar_position);
    else {
      float half_height = proportionOfHeight(0.5f);
      g.fillRect(x, half_height, x_inc, bar_position - half_height);
    }

    g.setColour(Colors::modulation);
    g.fillRect(x, bar_position, x_inc, line_width);

    x += x_inc;
  }
}

void GraphicalStepSequencer::paint(Graphics& g) {
  if (sequence_.size() == 0 || num_steps_slider_ == nullptr)
    return;

  g.drawImage(background_,
              0, 0, getWidth(), getHeight(),
              0, 0, background_.getWidth(), background_.getHeight());

  float step_width = getWidth() / (1.0f * num_steps_);

  if (highlighted_step_ >= 0) {
    g.setColour(Colour(0x11ffffff));
    g.fillRect(highlighted_step_ * step_width, 0.0f, step_width, 1.0f * getHeight());
  }

  if (last_step_ >= 0) {
    g.setColour(Colour(0x08ffffff));
    g.fillRect(last_step_ * step_width, 0.0f, step_width, 1.0f * getHeight());
  }
}

void GraphicalStepSequencer::resized() {
  ensureMinSize();

  const Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  float scale = display.scale;
  background_ = Image(Image::RGB, scale * getWidth(), scale * getHeight(), true);
  resetBackground();
}

void GraphicalStepSequencer::mouseMove(const MouseEvent& e) {
  updateHover(getHoveredStep(e.getPosition()));
}

void GraphicalStepSequencer::mouseExit(const MouseEvent& e) {
  updateHover(-1);
}

void GraphicalStepSequencer::mouseDown(const MouseEvent& e) {
  last_edit_position_ = e.getPosition();
  updateHover(getHoveredStep(e.getPosition()));
  changeStep(e);
}

void GraphicalStepSequencer::mouseDrag(const MouseEvent& e) {
  updateHover(getHoveredStep(e.getPosition()));
  changeStep(e);
  last_edit_position_ = e.getPosition();
}

void GraphicalStepSequencer::timerCallback() {
  if (step_generator_output_) {
    int new_step = step_generator_output_->buffer[0];
    if (new_step != last_step_) {
      last_step_ = new_step;
      repaint();
    }
  }
}

void GraphicalStepSequencer::setStepSliders(std::vector<Slider*> sliders) {
  sequence_ = sliders;
  for (int i = 0; i < sliders.size(); ++i)
    sequence_[i]->addListener(this);
  ensureMinSize();
  resetBackground();
}

void GraphicalStepSequencer::sliderValueChanged(Slider* moved_slider) {
  ensureMinSize();
  resetBackground();
}

void GraphicalStepSequencer::guiChanged(SynthSlider* moved_slider) {
  ensureMinSize();
  resetBackground();
}

void GraphicalStepSequencer::setNumStepsSlider(SynthSlider* num_steps_slider) {
  if (num_steps_slider_)
    num_steps_slider_->removeListener(this);
  num_steps_slider_ = num_steps_slider;
  num_steps_slider_->addSliderListener(this);

  ensureMinSize();
  resetBackground();
}

void GraphicalStepSequencer::ensureMinSize() {
  if (num_steps_slider_ == nullptr)
    return;

  num_steps_ = num_steps_slider_->getValue();
}

void GraphicalStepSequencer::resetBackground() {
  if (!background_.isValid())
    return;

  ensureMinSize();
  const Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  float scale = display.scale;
  Graphics g(background_);
  g.addTransform(AffineTransform::scale(scale, scale));
  paintBackground(g);
  repaint();
}

// Sets the height of the steps based on mouse positions.
// If the mouse skipped over some steps between last mouseDrag event, we'll interpolate
// and set all the steps between to appropriate values.
void GraphicalStepSequencer::changeStep(const MouseEvent& e) {
  Point<int> mouse_position = e.getPosition();
  int from_step = getHoveredStep(last_edit_position_);
  int selected_step = getHoveredStep(mouse_position);

  float x = mouse_position.x;
  float y = mouse_position.y;
  float x_delta = last_edit_position_.x - x;
  float y_delta = last_edit_position_.y - y;
  float slope = y_delta == 0 ? 0 : y_delta / x_delta;
  float next_x = getWidth() * (1.0f * selected_step) / num_steps_;
  int direction = -1;

  if (selected_step < from_step) {
    direction = 1;
    next_x += getWidth() * 1.0f / num_steps_;
  }
  float inc_x = next_x - x;

  for (int step = selected_step; step != from_step + direction; step += direction) {
    if (step >= 0 && step < num_steps_) {
      float new_value = -2.0f * y / getHeight() + 1.0f;
      new_value = std::max(std::min(new_value, 1.0f), -1.0f);
      new_value = sequence_[step]->snapValue(new_value, Slider::DragMode::absoluteDrag);
      sequence_[step]->setValue(new_value);
    }
    y += inc_x * slope;
    inc_x = direction * getWidth() * 1.0f / num_steps_;
  }
  resetBackground();
}

int GraphicalStepSequencer::getHoveredStep(Point<int> position) {
  return floorf(num_steps_ * (1.0f * position.x) / getWidth());
}

void GraphicalStepSequencer::updateHover(int step_index) {
  if (step_index == highlighted_step_)
    return;
  highlighted_step_ = step_index;
  repaint();
}

void GraphicalStepSequencer::showRealtimeFeedback(bool show_feedback) {
  if (show_feedback) {
    if (step_generator_output_ == nullptr) {
      SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
      startTimerHz(FRAMES_PER_SECOND);
      if (parent)
        step_generator_output_ = parent->getSynth()->getModSource(getName().toStdString());
    }
  }
  else {
    stopTimer();
    step_generator_output_ = nullptr;
    last_step_ = -1;
    repaint();
  }
}
