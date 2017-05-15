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

class OpenGLComponent : public Component {
  public:
    OpenGLComponent() { }
    virtual ~OpenGLComponent() { }

    void paint(Graphics& g) override { }

    virtual void init(OpenGLContext& open_gl_context) = 0;
    virtual void render(OpenGLContext& open_gl_context, bool animate = true) = 0;
    virtual void destroy(OpenGLContext& open_gl_context) = 0;
    virtual void paintBackground(Graphics& g) = 0;

  protected:
    void setViewPort(OpenGLContext& open_gl_context);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGLComponent)
};

#endif // OPEN_GL_COMPONENT_H
