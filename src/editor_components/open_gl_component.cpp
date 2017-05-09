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

#include "open_gl_component.h"

#include "shaders.h"

OpenGlComponent::OpenGlComponent() {
  setOpaque(true);
  openGLContext.setRenderer(this);
  openGLContext.attachTo(*this);
  openGLContext.setContinuousRepainting(true);
  openGLContext.setSwapInterval(0);
}

OpenGlComponent::~OpenGlComponent() {
  openGLContext.extensions.glDeleteBuffers(1, &vertex_buffer_);
  openGLContext.extensions.glDeleteBuffers(1, &triangle_buffer_);

  openGLContext.detach();
}

void OpenGlComponent::resized() {
}

void OpenGlComponent::newOpenGLContextCreated() {
  static const float vertices[] = {
    -0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f
  };

  static const int triangles[] = {
    0, 1, 2,
    2, 3, 0
  };

  openGLContext.extensions.glGenBuffers(1, &vertex_buffer_);
  openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);

  GLsizeiptr vert_size = static_cast<GLsizeiptr>(static_cast<size_t>(sizeof(vertices)));
  openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, vert_size,
                                        vertices, GL_STATIC_DRAW);

  openGLContext.extensions.glGenBuffers(1, &triangle_buffer_);
  openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer_);

  GLsizeiptr tri_size = static_cast<GLsizeiptr>(static_cast<size_t>(sizeof(triangles)));
  openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri_size,
                                        triangles, GL_STATIC_DRAW);

  init();
}

void OpenGlComponent::renderOpenGL() {
  render();
}

void OpenGlComponent::openGLContextClosing() {
  destroy();
}

void OpenGlComponent::bind() {
  openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer_);
}

void OpenGlComponent::createAttributes(OpenGLShaderProgram& program) {
  position = new OpenGLShaderProgram::Attribute(program, "position");
}

void OpenGlComponent::enableAttributes() {
  if (position != nullptr) {
    openGLContext.extensions.glVertexAttribPointer(position->attributeID, 3, GL_FLOAT,
                                                   GL_FALSE, 3 * sizeof(float), 0);
    openGLContext.extensions.glEnableVertexAttribArray(position->attributeID);
  }
}

void OpenGlComponent::disableAttributes() {
  openGLContext.extensions.glDisableVertexAttribArray(position->attributeID);
}

void OpenGlComponent::draw() {
  bind();
  enableAttributes();
  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
  disableAttributes();
}
