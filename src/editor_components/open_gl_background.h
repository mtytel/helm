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

#pragma once
#ifndef OPEN_GL_BACKGROUND_H
#define OPEN_GL_BACKGROUND_H

#include "JuceHeader.h"

class OpenGLBackground {
  public:
    OpenGLBackground();
    virtual ~OpenGLBackground();

    void updateBackgroundImage(Image background);
    virtual void init(OpenGLContext& open_gl_context);
    virtual void render(OpenGLContext& open_gl_context);
    virtual void destroy(OpenGLContext& open_gl_context);

    OpenGLShaderProgram* shader() { return image_shader_; }
    OpenGLShaderProgram::Uniform* texture_uniform() { return texture_uniform_; }

    void bind(OpenGLContext& open_gl_context);
    void enableAttributes(OpenGLContext& open_gl_context);
    void disableAttributes(OpenGLContext& open_gl_context);

  private:
    ScopedPointer<OpenGLShaderProgram> image_shader_;
    ScopedPointer<OpenGLShaderProgram::Uniform> texture_uniform_;
    ScopedPointer<OpenGLShaderProgram::Attribute> position_;
    ScopedPointer<OpenGLShaderProgram::Attribute> texture_coordinates_;

    float vertices_[16];

    OpenGLTexture background_;
    bool new_background_;
    Image background_image_;

    GLuint vertex_buffer_;
    GLuint triangle_buffer_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGLBackground)
};

#endif // OPEN_GL_BACKGROUND_H
