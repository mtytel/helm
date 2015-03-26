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

#ifndef TWYTCH_SLIDER_H
#define TWYTCH_SLIDER_H

#include "JuceHeader.h"

class FullInterface;

class TwytchSlider : public Slider {
public:
    TwytchSlider(String name) : Slider(name), bipolar_(false), parent_(nullptr) { }

    void mouseDown(const MouseEvent& e) override;
    void mouseEnter(const MouseEvent& e) override;
    void valueChanged() override;

    void setBipolar(bool bipolar = true) { bipolar_ = bipolar; }
    bool isBipolar() const { return bipolar_; }

private:
    void notifyTooltip();
    bool bipolar_;
    FullInterface* parent_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TwytchSlider)
};

#endif // TWYTCH_SLIDER_H
