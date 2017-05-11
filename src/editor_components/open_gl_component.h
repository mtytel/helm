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
#ifndef OPEN_GL_COMPONENT_H
#define OPEN_GL_COMPONENT_H

#include "JuceHeader.h"
#include "memory.h"

class OpenGlComponent : public Component, public OpenGLRenderer {
  public:
    OpenGlComponent();
    ~OpenGlComponent();

    void paint(Graphics& g) override { }
    void resized() override;

    void newOpenGLContextCreated() override;
    void renderOpenGL() override;
    void openGLContextClosing() override;

  protected:
    OpenGLShaderProgram::Uniform* createUniform(OpenGLShaderProgram& program, const char* name);
    OpenGLShaderProgram::Attribute* createAttribute(OpenGLShaderProgram& program, const char* name);

    void drawBackground();

    void bind();
    void enableAttributes();
    void disableAttributes();
    void updateBackgroundImage(Image background);

    virtual void init() { }
    virtual void render() { }
    virtual void destroy() { }

    OpenGLContext openGLContext;

  private:
    ScopedPointer<OpenGLShaderProgram> shader_;

    ScopedPointer<OpenGLShaderProgram::Attribute> position_;
    ScopedPointer<OpenGLShaderProgram::Attribute> texture_coordinates_;

    ScopedPointer<OpenGLShaderProgram::Uniform> texture_;

    OpenGLTexture background_;
    Image background_image_;

    GLuint vertex_buffer_;
    GLuint triangle_buffer_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGlComponent)
};

#endif // OPEN_GL_COMPONENT_H
