/* Copyright 2013-2015 Matt Tytel
 *
 * twytch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * twytch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with twytch.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WAVE_SELECTOR_H
#define WAVE_SELECTOR_H

#include "JuceHeader.h"
#include "twytch_slider.h"

class WaveSelector : public TwytchSlider {
public:
    WaveSelector(String name);

    void paint(Graphics& g) override;
    void resized() override;

private:
    void resizeSin(float x, float y, float width, float height);
    void resizeTriangle(float x, float y, float width, float height);
    void resizeSquare(float x, float y, float width, float height);
    void resizeDownSaw(float x, float y, float width, float height);
    void resizeUpSaw(float x, float y, float width, float height);
    void resizeNoise(float x, float y, float width, float height);

    Path sine_;
    Path triangle_;
    Path square_;
    Path down_saw_;
    Path up_saw_;
    Path three_step_;
    Path four_step_;
    Path eight_step_;
    Path three_pyramid_;
    Path five_pyramid_;
    Path nine_pyramid_;
    Path noise_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveSelector)
};

#endif // WAVE_SELECTOR_H
