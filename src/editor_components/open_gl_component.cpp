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

#define GRID_CELL_WIDTH 8

#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256

OpenGlComponent::OpenGlComponent() {
  setOpaque(true);
  openGLContext.setRenderer(this);
  openGLContext.attachTo(*this);
  openGLContext.setContinuousRepainting(true);
}

OpenGlComponent::~OpenGlComponent() {
  openGLContext.extensions.glDeleteBuffers(1, &vertex_buffer_);
  openGLContext.extensions.glDeleteBuffers(1, &triangle_buffer_);

  openGLContext.detach();
}

void OpenGlComponent::resized() {
  const Desktop::Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  float scale = display.scale;
  float image_width = scale * IMAGE_WIDTH;
  float image_height = scale * IMAGE_HEIGHT;
  Image background = Image(Image::RGB, scale * IMAGE_WIDTH, scale * IMAGE_HEIGHT, true);
  Graphics g(background);
  g.addTransform(AffineTransform::scale(image_width / getWidth(), image_height / getHeight()));

  g.fillAll(Colour(0xff424242));

  g.setColour(Colour(0xff4a4a4a));
  for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
    g.drawLine(x, 0, x, getHeight());
  for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
    g.drawLine(0, y, getWidth(), y);

  updateBackgroundImage(background);
}

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

  shader_ = new OpenGLShaderProgram(openGLContext);

  if (shader_->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(vertex_shader)) &&
      shader_->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(fragment_shader)) &&
      shader_->link()) {
    shader_->use();
    position_ = createAttribute(*shader_, "position");
    texture_coordinates_ = createAttribute(*shader_, "tex_coord_in");
    texture_ = createUniform(*shader_, "texture");
  }
  
  init();
}

void OpenGlComponent::renderOpenGL() {
  render();
}

void OpenGlComponent::openGLContextClosing() {
  if (background_.getWidth())
    background_.release();
  destroy();
}

void OpenGlComponent::bind() {
  openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer_);
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
  if (background_.getWidth() != background_image_.getWidth() ||
      background_.getHeight() != background_image_.getHeight())
    background_.loadImage(background_image_);

  bind();
  openGLContext.extensions.glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH);

  background_.bind();
  if (texture_ != nullptr)
    texture_->set(0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  shader_->use();
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
}
