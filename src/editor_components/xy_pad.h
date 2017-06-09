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
#ifndef XY_PAD_H
#define XY_PAD_H

#include "JuceHeader.h"
#include "synth_slider.h"

class XYPad : public Component, public SynthSlider::SliderListener {
  public:
    XYPad();
    ~XYPad();

    void guiChanged(SynthSlider* moved_slider) override;
    void setSlidersFromPosition(Point<int> position);

    void setXSlider(SynthSlider* slider);
    void setYSlider(SynthSlider* slider);

    void paint(Graphics& g) override;
    void paintBackground(Graphics& g);
    void resized() override;
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;

    void setActive(bool active = true);

  private:
    SynthSlider* x_slider_;
    SynthSlider* y_slider_;
    bool mouse_down_;
    bool active_;

    Image background_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XYPad)
};

#endif // XY_PAD_H
