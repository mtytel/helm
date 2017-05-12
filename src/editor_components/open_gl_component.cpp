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

#include "common.h"
#include "shaders.h"

OpenGlComponent::OpenGlComponent() {
  static OpenGLPixelFormat pixelFormat;
  pixelFormat.multisamplingLevel = 4;
  openGLContext.setPixelFormat(pixelFormat);

  setOpaque(true);
  openGLContext.setContinuousRepainting(true);
  openGLContext.setRenderer(this);
  openGLContext.attachTo(*this);
  new_background_ = false;
  animating_ = true;
}

OpenGlComponent::~OpenGlComponent() {
  openGLContext.detach();
}

void OpenGlComponent::paint(Graphics& g) { }

void OpenGlComponent::newOpenGLContextCreated() {
  static const float vertices[] = {
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f
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

  const char* vertex_shader = Shaders::getShader(Shaders::kBackgroundImageVertex);
  const char* fragment_shader = Shaders::getShader(Shaders::kBackgroundImageFragment);

  image_shader_ = new OpenGLShaderProgram(openGLContext);

  if (image_shader_->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(vertex_shader)) &&
      image_shader_->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(fragment_shader)) &&
      image_shader_->link()) {
    image_shader_->use();
    position_ = createAttribute(*image_shader_, "position");
    texture_coordinates_ = createAttribute(*image_shader_, "tex_coord_in");
    texture_uniform_ = createUniform(*image_shader_, "texture");
  }
  
  init();
}

void OpenGlComponent::renderOpenGL() {
  MOPO_ASSERT(OpenGLHelpers::isContextActive());

  render();
}

void OpenGlComponent::openGLContextClosing() {
  if (background_.getWidth())
    background_.release();

  image_shader_ = nullptr;
  position_ = nullptr;
  texture_coordinates_ = nullptr;
  texture_uniform_ = nullptr;
  openGLContext.extensions.glDeleteBuffers(1, &vertex_buffer_);
  openGLContext.extensions.glDeleteBuffers(1, &triangle_buffer_);

  destroy();
}

void OpenGlComponent::showRealtimeFeedback(bool show_feedback) {
  openGLContext.setContinuousRepainting(show_feedback);
  animating_ = show_feedback;
}

void OpenGlComponent::bind() {
  openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer_);
  background_.bind();
}

OpenGLShaderProgram::Uniform* OpenGlComponent::createUniform(OpenGLShaderProgram& program,
                                                             const char* name) {
  if (openGLContext.extensions.glGetUniformLocation(program.getProgramID(), name) < 0)
    return nullptr;

  return new OpenGLShaderProgram::Uniform(program, name);
}

OpenGLShaderProgram::Attribute* OpenGlComponent::createAttribute(OpenGLShaderProgram& program,
                                                                 const char* name) {
  if (openGLContext.extensions.glGetAttribLocation(program.getProgramID(), name) < 0)
    return nullptr;

  return new OpenGLShaderProgram::Attribute(program, name);
}

void OpenGlComponent::enableAttributes() {
  if (position_ != nullptr) {
    openGLContext.extensions.glVertexAttribPointer(position_->attributeID, 2, GL_FLOAT,
                                                   GL_FALSE, 4 * sizeof(float), 0);
    openGLContext.extensions.glEnableVertexAttribArray(position_->attributeID);
  }
  if (texture_coordinates_ != nullptr) {
    openGLContext.extensions.glVertexAttribPointer(texture_coordinates_->attributeID, 2, GL_FLOAT,
                                                   GL_FALSE, 4 * sizeof(float),
                                                   (GLvoid*)(2 * sizeof(float)));
    openGLContext.extensions.glEnableVertexAttribArray(texture_coordinates_->attributeID);
  }
}

void OpenGlComponent::disableAttributes() {
  if (position_ != nullptr)
    openGLContext.extensions.glDisableVertexAttribArray(position_->attributeID);
  if (texture_coordinates_ != nullptr)
    openGLContext.extensions.glDisableVertexAttribArray(texture_coordinates_->attributeID);
}

void OpenGlComponent::drawBackground() {
  OpenGLHelpers::clear(Colours::grey);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  if (new_background_ || background_.getWidth() != background_image_.getWidth()) {
    new_background_ = false;
    background_.loadImage(background_image_);
  }

  bind();

  openGLContext.extensions.glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH);

  if (texture_uniform_ != nullptr)
    texture_uniform_->set(0);

  image_shader_->use();
  enableAttributes();
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  disableAttributes();

  background_.unbind();

  glDisable(GL_TEXTURE_2D);

  openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
  openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGlComponent::updateBackgroundImage(Image background) {
  background_image_ = background;
  new_background_ = true;
}
