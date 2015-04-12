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

#ifndef FILTER_SELECTOR_H
#define FILTER_SELECTOR_H

#include "JuceHeader.h"
#include "twytch_slider.h"

class FilterSelector : public TwytchSlider {
public:
    FilterSelector(String name);

    void paint(Graphics& g) override;
    void resized() override;

private:
    void resizeLowPass(float x, float y, float width, float height);
    void resizeHighPass(float x, float y, float width, float height);
    void resizeBandPass(float x, float y, float width, float height);
    void resizeNotch(float x, float y, float width, float height);
    void resizeLowShelf(float x, float y, float width, float height);
    void resizeHighShelf(float x, float y, float width, float height);
    void resizeAllPass(float x, float y, float width, float height);

    Path low_pass_;
    Path high_pass_;
    Path band_pass_;
    Path notch_;
    Path low_shelf_;
    Path high_shelf_;
    Path all_pass_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterSelector)
};

#endif // FILTER_SELECTOR_H
