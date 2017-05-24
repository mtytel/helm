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
#ifndef OPEN_GL_PEAK_METER_H
#define OPEN_GL_PEAK_METER_H

#include "JuceHeader.h"

#include "helm_common.h"
#include "open_gl_component.h"

class OpenGLPeakMeter : public OpenGLComponent {
  public:
    OpenGLPeakMeter(bool left);
    virtual ~OpenGLPeakMeter();

    void resized() override;

    void init(OpenGLContext& open_gl_context) override;
    void render(OpenGLContext& open_gl_context, bool animate = true) override;
    void destroy(OpenGLContext& open_gl_context) override;
    void paintBackground(Graphics& g) override;

  private:
    void updateVertices();

    mopo::Output* peak_output_;

    ScopedPointer<OpenGLShaderProgram> shader_;
    ScopedPointer<OpenGLShaderProgram::Attribute> position_;

    float* position_vertices_;
    int* position_triangles_;
    GLuint vertex_buffer_;
    GLuint triangle_buffer_;
    bool left_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGLPeakMeter)
};

#endif // OPEN_GL_PEAK_METER_H
