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
#ifndef OPEN_GL_ENVELOPE_H
#define OPEN_GL_ENVELOPE_H

#include "JuceHeader.h"
#include "helm_common.h"
#include "open_gl_background.h"
#include "open_gl_component.h"
#include "synth_slider.h"

class OpenGLEnvelope : public OpenGLComponent, public SynthSlider::SliderListener {
  public:
    OpenGLEnvelope();
    ~OpenGLEnvelope();

    void resetEnvelopeLine();
    void guiChanged(SynthSlider* slider) override;

    void setAttackSlider(SynthSlider* attack_slider);
    void setDecaySlider(SynthSlider* decay_slider);
    void setSustainSlider(SynthSlider* sustain_slider);
    void setReleaseSlider(SynthSlider* release_slider);

    void resized() override;
    void mouseMove(const MouseEvent& e) override;
    void mouseExit(const MouseEvent& e) override;
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;

    void init(OpenGLContext& open_gl_context) override;
    void render(OpenGLContext& open_gl_context, bool animate = true) override;
    void destroy(OpenGLContext& open_gl_context) override;
    void paintBackground(Graphics& g) override { }

  private:
    void drawPosition(OpenGLContext& open_gl_context);
    void paintPositionImage();
    void paintBackground();
    Point<float> valuesToPosition(float phase, float amp);

    float getAttackX();
    float getDecayX();
    float getSustainY();
    float getReleaseX();

    void setAttackX(double x);
    void setDecayX(double x);
    void setSustainY(double y);
    void setReleaseX(double x);

    bool attack_hover_;
    bool decay_hover_;
    bool sustain_hover_;
    bool release_hover_;
    bool mouse_down_;
    Path envelope_line_;

    mopo::Output* envelope_phase_;
    mopo::Output* envelope_amp_;

    SynthSlider* attack_slider_;
    SynthSlider* decay_slider_;
    SynthSlider* sustain_slider_;
    SynthSlider* release_slider_;

    OpenGLBackground background_;

    Image position_image_;
    Image background_image_;
    OpenGLTexture position_texture_;
    ScopedPointer<OpenGLShaderProgram::Uniform> texture_;

    float* position_vertices_;
    int* position_triangles_;
    GLuint vertex_buffer_;
    GLuint triangle_buffer_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLEnvelope)
};

#endif // OPEN_GL_ENVELOPE_H
