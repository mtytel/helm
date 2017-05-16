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

OpenGLModulationMeter::OpenGLModulationMeter(const mopo::Output* mono_total,
                                             const mopo::Output* poly_total,
                                             const SynthSlider* slider,
                                             float* vertices) :
        mono_total_(mono_total), poly_total_(poly_total), destination_(slider), vertices_(vertices),
        current_value_(0.0), knob_percent_(0.0), mod_percent_(0.0),
        full_radius_(0.0), outer_radius_(0.0) {
  static const float initial_vertices[24] {
    -1.0f, 1.0f, -1.0f, 1.0f, -2.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f, -2.0f, 1.0f,
    1.0f, -1.0f, 1.0f, -1.0f, -2.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f, -2.0f, 1.0f
  };

  memcpy(vertices_, initial_vertices, 24 * sizeof(float));

  setInterceptsMouseClicks(false, false);
  updateValue();
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
}

void OpenGLModulationMeter::setVertices() {
  Rectangle<int> top_level = getTopLevelComponent()->getScreenBounds();
  Rectangle<int> local = getScreenBounds();
  float left = local.getX() - top_level.getX();
  float right = local.getRight() - top_level.getX();
  float top = top_level.getHeight() - (local.getY() - top_level.getY());
  float bottom = top_level.getHeight() - (local.getBottom() - top_level.getY());
  vertices_[0] = vertices_[6] = 2.0f * left / top_level.getWidth() - 1.0f;
  vertices_[12] = vertices_[18] = 2.0f * right / top_level.getWidth() - 1.0f;
  vertices_[1] = vertices_[19] = 2.0f * top / top_level.getHeight() - 1.0f;
  vertices_[7] = vertices_[13] = 2.0f * bottom / top_level.getHeight() - 1.0f;
}

void OpenGLModulationMeter::collapseVertices() {
  vertices_[0] = vertices_[6] = 0.0f;
  vertices_[12] = vertices_[18] = 0.0f;
  vertices_[1] = vertices_[19] = 0.0f;
  vertices_[7] = vertices_[13] = 0.0f;
}

void OpenGLModulationMeter::updateValue() {
  if (mono_total_) {
    current_value_ = mono_total_->buffer[0];
    if (poly_total_)
      current_value_ += poly_total_->buffer[0];
  }
}

void OpenGLModulationMeter::updateDrawing() {
  double range = destination_->getMaximum() - destination_->getMinimum();
  double value = (current_value_ - destination_->getMinimum()) / range;
  double new_mod_percent = mopo::utils::clamp(value , 0.0, 1.0);
  double new_knob_percent = (destination_->getValue() - destination_->getMinimum()) / range;

  if (new_mod_percent != mod_percent_ || new_knob_percent != knob_percent_) {
    mod_percent_ = new_mod_percent;
    knob_percent_ = new_knob_percent;

    float min_percent = std::min(mod_percent_, knob_percent_);
    float max_percent = std::max(mod_percent_, knob_percent_);
    float angle = SynthSlider::rotary_angle;

    float min_radians = INTERPOLATE(-angle, angle, min_percent);
    float max_radians = INTERPOLATE(-angle, angle, max_percent);

    for (int i = 0; i < 4; ++i) {
      vertices_[4 + 6 * i] = min_radians;
      vertices_[5 + 6 * i] = max_radians;
    }
  }
}
