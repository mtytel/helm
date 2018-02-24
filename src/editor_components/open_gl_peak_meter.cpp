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

#include "open_gl_peak_meter.h"

#include "colors.h"
#include "shaders.h"
#include "synth_gui_interface.h"
#include "utils.h"

#define MAX_GAIN 2.0

OpenGLPeakMeter::OpenGLPeakMeter(bool left) : left_(left) {
  peak_output_ = nullptr;
  position_vertices_ = new float[8] {
    -1.0f, 1.0f,
    -1.0f, -1.0f,
    1.0f, -1.0f,
    1.0f, 1.0f,
  };

  position_triangles_ = new int[6] {
    0, 1, 2,
    2, 3, 0
  };
}

OpenGLPeakMeter::~OpenGLPeakMeter() {
  delete[] position_vertices_;
  delete[] position_triangles_;
}

void OpenGLPeakMeter::resized() {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (peak_output_ == nullptr && parent)
    peak_output_ = parent->getSynth()->getModSource("peak_meter");

  OpenGLComponent::resized();
}

void OpenGLPeakMeter::init(OpenGLContext& open_gl_context) {
  open_gl_context.extensions.glGenBuffers(1, &vertex_buffer_);
  open_gl_context.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);

  GLsizeiptr vert_size = static_cast<GLsizeiptr>(static_cast<size_t>(8 * sizeof(float)));
  open_gl_context.extensions.glBufferData(GL_ARRAY_BUFFER, vert_size,
                                          position_vertices_, GL_STATIC_DRAW);

  open_gl_context.extensions.glGenBuffers(1, &triangle_buffer_);
  open_gl_context.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer_);

  GLsizeiptr tri_size = static_cast<GLsizeiptr>(static_cast<size_t>(6 * sizeof(float)));
  open_gl_context.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri_size,
                                          position_triangles_, GL_STATIC_DRAW);

  const char* vertex_shader = Shaders::getShader(Shaders::kGainMeterVertex);
  const char* fragment_shader = Shaders::getShader(Shaders::kGainMeterFragment);

  shader_ = new OpenGLShaderProgram(open_gl_context);

  if (shader_->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(vertex_shader)) &&
      shader_->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(fragment_shader)) &&
      shader_->link()) {
    shader_->use();
    position_ = new OpenGLShaderProgram::Attribute(*shader_, "position");
  }
}

void OpenGLPeakMeter::updateVertices() {
  if (peak_output_ == nullptr)
    return;

  float val = peak_output_->buffer[left_ ? 0 : 1];
  float t = val / MAX_GAIN;
  float position = mopo::utils::interpolate(-1.0f, 1.0f, sqrtf(t));
  position_vertices_[4] = position;
  position_vertices_[6] = position;
}

void OpenGLPeakMeter::render(OpenGLContext& open_gl_context, bool animate) {
  MOPO_ASSERT(glGetError() == GL_NO_ERROR);

  if (!animate || peak_output_ == nullptr)
    return;

  updateVertices();
  setViewPort(open_gl_context);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  shader_->use();

  open_gl_context.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  GLsizeiptr vert_size = static_cast<GLsizeiptr>(static_cast<size_t>(8 * sizeof(float)));
  open_gl_context.extensions.glBufferData(GL_ARRAY_BUFFER, vert_size,
                                          position_vertices_, GL_STATIC_DRAW);

  open_gl_context.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer_);

  open_gl_context.extensions.glVertexAttribPointer(position_->attributeID, 2, GL_FLOAT,
                                                   GL_FALSE, 2 * sizeof(float), 0);
  open_gl_context.extensions.glEnableVertexAttribArray(position_->attributeID);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  open_gl_context.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
  open_gl_context.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  MOPO_ASSERT(glGetError() == GL_NO_ERROR);
}

void OpenGLPeakMeter::destroy(OpenGLContext& open_gl_context) {
  shader_ = nullptr;
  position_ = nullptr;
  open_gl_context.extensions.glDeleteBuffers(1, &vertex_buffer_);
  open_gl_context.extensions.glDeleteBuffers(1, &triangle_buffer_);
}

void OpenGLPeakMeter::paintBackground(Graphics& g) {
  float x = getWidth() / sqrt(MAX_GAIN);
  g.setColour(Colour(0xff222222));
  g.fillRect(x, 0.0f, getWidth() - x, 1.0f * getHeight());

  g.setColour(Colour(0xff888888));
  g.fillRect(x - 1.0f, 0.0f, 2.0f, 1.0f * getHeight());
}
