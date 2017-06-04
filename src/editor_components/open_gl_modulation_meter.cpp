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

#include "open_gl_modulation_meter.h"

#include "mopo.h"
#include "synth_gui_interface.h"
#include "shaders.h"
#include "text_look_and_feel.h"

OpenGLModulationMeter::OpenGLModulationMeter(const mopo::Output* mono_total,
                                             const mopo::Output* poly_total,
                                             const SynthSlider* slider,
                                             float* vertices) :
        mono_total_(mono_total), poly_total_(poly_total), destination_(slider), vertices_(vertices),
        current_value_(0.0), knob_percent_(0.0), mod_percent_(0.0),
        full_radius_(0.0), outer_radius_(0.0),
        left_(0.0f), right_(0.0), top_(0.0), bottom_(0.0) {
  static const float initial_rotary_vertices[24] {
    0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f
  };

  static const float initial_horizontal_vertices[24] {
    0.0f, 0.0f, 0.0f, 0.0f, -10.0f, 10.0f,
    0.0f, 0.0f, 0.0f, 0.0f, -10.0f, 10.0f,
    0.0f, 0.0f, 0.0f, 0.0f, -10.0f, 10.0f,
    0.0f, 0.0f, 0.0f, 0.0f, -10.0f, 10.0f
  };

  static const float initial_vertical_vertices[24] {
    0.0f, 0.0f, 0.0f, 0.0f, -10.0f, 10.0f,
    0.0f, 0.0f, 0.0f, 0.0f, -10.0f, 10.0f,
    0.0f, 0.0f, 0.0f, 0.0f, -10.0f, 10.0f,
    0.0f, 0.0f, 0.0f, 0.0f, -10.0f, 10.0f
  };

  rotary_ = destination_->isRotary() &&
            &destination_->getLookAndFeel() != TextLookAndFeel::instance();

  if (rotary_)
    memcpy(vertices_, initial_rotary_vertices, sizeof(initial_rotary_vertices));
  else if (destination_->isHorizontal())
    memcpy(vertices_, initial_horizontal_vertices, sizeof(initial_horizontal_vertices));
  else
    memcpy(vertices_, initial_vertical_vertices, sizeof(initial_vertical_vertices));

  setInterceptsMouseClicks(false, false);
  updateDrawing();
}

OpenGLModulationMeter::~OpenGLModulationMeter() { }

void OpenGLModulationMeter::paint(Graphics& g) { }

void OpenGLModulationMeter::resized() {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent) {
    std::vector<mopo::ModulationConnection*> connections;
    connections = parent->getSynth()->getSourceConnections(getName().toStdString());
    setModulated(connections.size());
  }

  if (isVisible())
    setVertices();
  else
    collapseVertices();
}

void OpenGLModulationMeter::setVisible(bool should_be_visible) {
  if (should_be_visible)
    setVertices();
  else
    collapseVertices();

  Component::setVisible(should_be_visible);
}

void OpenGLModulationMeter::setVertices() {
  Rectangle<int> parent_bounds = getParentComponent()->getBounds();
  Rectangle<int> bounds = getBounds();
  float left = bounds.getX();
  float right = bounds.getRight();
  float top = parent_bounds.getHeight() - bounds.getY();
  float bottom = parent_bounds.getHeight() - bounds.getBottom();

  if (!destination_->isRotary()) {
    if (destination_->isHorizontal()) {
      bottom += getHeight() / 2.0f - SynthSlider::linear_rail_width;
      top -= getHeight() / 2.0f - SynthSlider::linear_rail_width;
    }
    else {
      left += getWidth() / 2.0f - SynthSlider::linear_rail_width;
      right -= getWidth() / 2.0f - SynthSlider::linear_rail_width;
    }
  }

  left_ = 2.0f * left / parent_bounds.getWidth() - 1.0f;
  right_ = 2.0f * right / parent_bounds.getWidth() - 1.0f;
  top_ = 2.0f * top / parent_bounds.getHeight() - 1.0f;
  bottom_ = 2.0f * bottom / parent_bounds.getHeight() - 1.0f;
}

void OpenGLModulationMeter::collapseVertices() {
  left_ = right_ = top_ = bottom_= 0.0f;
  vertices_[0] = vertices_[6] = 0.0f;
  vertices_[12] = vertices_[18] = 0.0f;
  vertices_[1] = vertices_[19] = 0.0f;
  vertices_[7] = vertices_[13] = 0.0f;
}

void OpenGLModulationMeter::updateDrawing() {
  if (mono_total_) {
    current_value_ = mono_total_->buffer[0];
    if (poly_total_)
      current_value_ += poly_total_->buffer[0];
  }

  double range = destination_->getMaximum() - destination_->getMinimum();
  double value = (current_value_ - destination_->getMinimum()) / range;
  double new_mod_percent = mopo::utils::clamp(value , 0.0, 1.0);
  double new_knob_percent = (destination_->getValue() - destination_->getMinimum()) / range;

  if (new_mod_percent != mod_percent_ || new_knob_percent != knob_percent_) {
    mod_percent_ = new_mod_percent;
    knob_percent_ = new_knob_percent;

    float min_percent = std::min(mod_percent_, knob_percent_);
    float max_percent = std::max(mod_percent_, knob_percent_);

    if (rotary_) {
      float angle = SynthSlider::rotary_angle;

      float min_radians = mopo::utils::interpolate(-angle, angle, min_percent);
      float max_radians = mopo::utils::interpolate(-angle, angle, max_percent);

      vertices_[0] = vertices_[6] = left_;
      vertices_[12] = vertices_[18] = right_;
      vertices_[1] = vertices_[19] = top_;
      vertices_[7] = vertices_[13] = bottom_;

      for (int i = 0; i < 4; ++i) {
        vertices_[4 + 6 * i] = min_radians;
        vertices_[5 + 6 * i] = max_radians;
      }
    }
    else if (destination_->isHorizontal()) {
      float start = mopo::utils::interpolate(left_, right_, min_percent);
      vertices_[0] = vertices_[6] = start;

      float end = mopo::utils::interpolate(left_, right_, max_percent);
      vertices_[12] = vertices_[18] = end;

      vertices_[1] = vertices_[19] = top_;
      vertices_[7] = vertices_[13] = bottom_;
    }
    else if (&destination_->getLookAndFeel() == TextLookAndFeel::instance()) {
      float start = bottom_;
      float end = top_;
      float diff_percent = mod_percent_ - knob_percent_;

      if (diff_percent > 0.0)
        end = mopo::utils::interpolate(bottom_, top_, diff_percent);
      else
        start = mopo::utils::interpolate(top_, bottom_, -diff_percent);

      vertices_[7] = vertices_[13] = start;
      vertices_[1] = vertices_[19] = end;

      vertices_[0] = vertices_[6] = left_;
      vertices_[12] = vertices_[18] = right_;
    }
    else {
      float start = mopo::utils::interpolate(bottom_, top_, min_percent);
      vertices_[7] = vertices_[13] = start;

      float end = mopo::utils::interpolate(bottom_, top_, max_percent);
      vertices_[1] = vertices_[19] = end;

      vertices_[0] = vertices_[6] = left_;
      vertices_[12] = vertices_[18] = right_;
    }
  }
}
