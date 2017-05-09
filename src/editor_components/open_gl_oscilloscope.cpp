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

#include "shaders.h"

OpenGlOscilloscope::OpenGlOscilloscope() : output_memory_(nullptr) { }

OpenGlOscilloscope::~OpenGlOscilloscope() { }

void OpenGlOscilloscope::init() {
  vertex_shader_ = Shaders::getShader(Shaders::kVertexPassthrough);
  fragment_shader_ = Shaders::getShader(Shaders::kGreenFragment);

  shader_ = new OpenGLShaderProgram(openGLContext);

  if (shader_->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(vertex_shader_)) &&
      shader_->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(fragment_shader_)) &&
      shader_->link()) {
    shader_->use();
    createAttributes(*shader_);
  }
}

void OpenGlOscilloscope::render() {
  const float desktopScale = openGLContext.getRenderingScale();
  OpenGLHelpers::clear(Colours::red);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glViewport(0, 0, roundToInt(desktopScale * getWidth()), roundToInt(desktopScale * getHeight()));

  shader_->use();
  draw();

  openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
  openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGlOscilloscope::destroy() {

}
