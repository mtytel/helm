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

#include "open_gl_oscilloscope.h"

#include "helm_common.h"
#include "shaders.h"

#define RESOLUTION 256

OpenGlOscilloscope::OpenGlOscilloscope() : output_memory_(nullptr) {
  line_data_ = new float[2 * RESOLUTION];
  line_indices_ = new int[2 * RESOLUTION];

  for (int i = 0; i < RESOLUTION; ++i) {
    float t = i / (RESOLUTION - 1.0f);
    line_data_[2 * i] = 2.0f * t - 1.0f;
    line_data_[2 * i + 1] = 0.0f;

    line_indices_[2 * i] = i;
    line_indices_[2 * i + 1] = i + 1;
  }

  line_indices_[2 * RESOLUTION - 1] = RESOLUTION - 1;
}

OpenGlOscilloscope::~OpenGlOscilloscope() {
  openGLContext.extensions.glDeleteBuffers(1, &line_buffer_);
  openGLContext.extensions.glDeleteBuffers(1, &line_indices_buffer_);
}

void OpenGlOscilloscope::init() {
  openGLContext.extensions.glGenBuffers(1, &line_buffer_);
  openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, line_buffer_);

  GLsizeiptr vert_size = static_cast<GLsizeiptr>(2 * RESOLUTION * sizeof(float));
  openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, vert_size,
                                        line_data_, GL_STATIC_DRAW);

  openGLContext.extensions.glGenBuffers(1, &line_indices_buffer_);
  openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, line_indices_buffer_);

  GLsizeiptr line_size = static_cast<GLsizeiptr>(2 * RESOLUTION * sizeof(int));
  openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, line_size,
                                        line_indices_, GL_STATIC_DRAW);

  const char* vertex_shader = Shaders::getShader(Shaders::kOscilloscopeVertex);
  const char* fragment_shader = Shaders::getShader(Shaders::kOscilloscopeFragment);

  shader_ = new OpenGLShaderProgram(openGLContext);

  if (shader_->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(vertex_shader)) &&
      shader_->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(fragment_shader)) &&
      shader_->link()) {
    shader_->use();
    position_ = createAttribute(*shader_, "position");
  }
}

void OpenGlOscilloscope::drawLines() {
  glEnable(GL_LINE_SMOOTH);
  glDisable(GL_DEPTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glLineWidth(3);

  if (output_memory_) {
    for (int i = 0; i < RESOLUTION; ++i) {
      float memory_spot = (1.0f * i * mopo::MEMORY_RESOLUTION) / RESOLUTION;
      int memory_index = memory_spot;
      float remainder = memory_spot - memory_index;
      line_data_[2 * i + 1] = INTERPOLATE(output_memory_[memory_index],
                                          output_memory_[memory_index + 1],
                                          remainder);
    }

    openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, line_buffer_);

    GLsizeiptr vert_size = static_cast<GLsizeiptr>(2 * RESOLUTION * sizeof(float));
    openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, vert_size,
                                          line_data_, GL_STATIC_DRAW);
  }

  shader_->use();
  openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, line_buffer_);
  openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, line_indices_buffer_);

  openGLContext.extensions.glVertexAttribPointer(position_->attributeID, 2, GL_FLOAT,
                                                 GL_FALSE, 2 * sizeof(float), 0);
  openGLContext.extensions.glEnableVertexAttribArray(position_->attributeID);
  glDrawElements(GL_LINES, 2 * RESOLUTION, GL_UNSIGNED_INT, 0);
  openGLContext.extensions.glDisableVertexAttribArray(position_->attributeID);

  openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
  openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGlOscilloscope::render() {
  const float desktopScale = openGLContext.getRenderingScale();
  OpenGLHelpers::clear(Colours::red);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glViewport(0, 0, roundToInt(desktopScale * getWidth()), roundToInt(desktopScale * getHeight()));

  drawBackground();
  drawLines();
}

void OpenGlOscilloscope::destroy() {

}
