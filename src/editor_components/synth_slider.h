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

#ifndef SYNTH_SLIDER_H
#define SYNTH_SLIDER_H

#include "JuceHeader.h"
#include "helm_common.h"

class FullInterface;

class SynthSlider : public Slider {
  public:
    static const float rotary_angle;
    static const float linear_rail_width;

    class SliderListener {
      public:
        virtual ~SliderListener() { }
        virtual void hoverStarted(const std::string& name) { }
        virtual void hoverEnded(const std::string& name) { }
        virtual void modulationsChanged(const std::string& name) { }
        virtual void guiChanged(SynthSlider* slider) { }
    };

    SynthSlider(String name);

    virtual void resized() override;
    virtual void mouseDown(const MouseEvent& e) override;
    virtual void mouseEnter(const MouseEvent& e) override;
    virtual void mouseExit(const MouseEvent& e) override;
    virtual void mouseUp(const MouseEvent& e) override;
    void valueChanged() override;
    String getTextFromValue(double value) override;

    virtual double snapValue(double attemptedValue, DragMode dragMode) override;

    void drawShadow(Graphics& g);
    void drawRotaryShadow(Graphics& g);
    void drawRectangularShadow(Graphics& g);
    void snapToValue(bool snap, float value = 0.0) {
      snap_to_value_ = snap;
      snap_value_ = value;
    }

    void setScalingType(mopo::ValueDetails::DisplaySkew scaling_type) {
      details_.display_skew = scaling_type;
    }

    mopo::ValueDetails::DisplaySkew getScalingType() const { return details_.display_skew; }

    void setStringLookup(const std::string* lookup) {
      string_lookup_ = lookup;
    }
    const std::string* getStringLookup() const { return string_lookup_; }

    void setPostMultiply(float post_multiply) { details_.display_multiply = post_multiply; }
    float getPostMultiply() const { return details_.display_multiply; }

    void setUnits(String units) { details_.display_units = units.toStdString(); }
    String getUnits() const { return details_.display_units; }
    String formatValue(float value);

    void flipColoring(bool flip_coloring = true);
    void setBipolar(bool bipolar = true);
    void setActive(bool active = true);

    void addSliderListener(SliderListener* listener);

    bool isBipolar() const { return bipolar_; }
    bool isFlippedColor() const { return flip_coloring_; }
    bool isActive() const { return active_; }
    void setPopupPlacement(int placement, int buffer = 0) {
      popup_placement_ = placement;
      popup_buffer_ = buffer;
    }
    int getPopupPlacement() { return popup_placement_; }
    int getPopupBuffer() { return popup_buffer_; }

    void notifyGuis();
    void handlePopupResult(int result);

  protected:
    void notifyTooltip();

    bool bipolar_;
    bool flip_coloring_;
    bool active_;
    bool snap_to_value_;
    float snap_value_;
    int popup_placement_;
    int popup_buffer_;

    mopo::ValueDetails details_;

    const std::string* string_lookup_;
    Point<float> click_position_;

    FullInterface* parent_;

    std::vector<SliderListener*> slider_listeners_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthSlider)
};

#endif // SYNTH_SLIDER_H
