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

#ifndef SYNTH_SLIDER_H
#define SYNTH_SLIDER_H

#include "JuceHeader.h"
#include "twytch_common.h"

class FullInterface;

class SynthSlider : public Slider {
  public:
    SynthSlider(String name);

    virtual void mouseDown(const MouseEvent& e) override;
    virtual void mouseEnter(const MouseEvent& e) override;
    virtual void mouseUp(const MouseEvent& e) override;
    void valueChanged() override;
    String getTextFromValue(double value) override;

    void drawShadow(Graphics& g);
    void drawRotaryShadow(Graphics& g);
    void drawRectangularShadow(Graphics& g);

    void setScalingType(mopo::ValueDetails::DisplaySkew scaling_type) {
      scaling_type_ = scaling_type;
    }

    mopo::ValueDetails::DisplaySkew getScalingType() const { return scaling_type_; }

    void setStringLookup(const std::string* lookup) {
      string_lookup_ = lookup;
    }
    const std::string* getStringLookup() const { return string_lookup_; }

    void setPostMultiply(float post_multiply) { post_multiply_ = post_multiply; }
    float getPostMultiply() const { return post_multiply_; }

    void setBipolar(bool bipolar = true) { bipolar_ = bipolar; }

    void setUnits(String units) { units_ = units; }
    String getUnits() const { return units_; }

    bool isBipolar() const { return bipolar_; }

  private:
    void notifyTooltip();

    bool bipolar_;
    String units_;
    mopo::ValueDetails::DisplaySkew scaling_type_;
    float post_multiply_;
    const std::string* string_lookup_;
    Point<float> click_position_;

    FullInterface* parent_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthSlider)
};

#endif // SYNTH_SLIDER_H
