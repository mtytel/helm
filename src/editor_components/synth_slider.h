/* Copyright 2013-2016 Matt Tytel
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

#ifndef SYNTH_SLIDER_H
#define SYNTH_SLIDER_H

#include "JuceHeader.h"
#include "helm_common.h"

class FullInterface;

class SynthSlider : public Slider {
  public:
    class HoverListener {
      public:
        virtual ~HoverListener() { }
        virtual void hoverStarted(const std::string& name) = 0;
        virtual void hoverEnded(const std::string& name) = 0;
    };

    SynthSlider(String name);

    virtual void mouseDown(const MouseEvent& e) override;
    virtual void mouseEnter(const MouseEvent& e) override;
    virtual void mouseExit(const MouseEvent& e) override;
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

    void setUnits(String units) { units_ = units; }
    String getUnits() const { return units_; }

    void flipColoring(bool flip_coloring = true);
    void setBipolar(bool bipolar = true);
    void setActive(bool active = true);

    void addHoverListener(HoverListener*);

    bool isBipolar() const { return bipolar_; }
    bool isFlippedColor() const { return flip_coloring_; }
    bool isActive() const { return active_; }

  private:
    void notifyTooltip();

    bool bipolar_;
    bool flip_coloring_;
    bool active_;
    String units_;
    mopo::ValueDetails::DisplaySkew scaling_type_;
    float post_multiply_;
    const std::string* string_lookup_;
    Point<float> click_position_;

    FullInterface* parent_;

    std::vector<HoverListener*> hover_listeners_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthSlider)
};

#endif // SYNTH_SLIDER_H
