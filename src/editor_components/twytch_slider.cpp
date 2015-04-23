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

#include "twytch_slider.h"
#include "synth_gui_interface.h"
#include "full_interface.h"

namespace {
    enum MenuIds {
        kCancel = 0,
        kArmMidiLearn,
        kClearMidiLearn,
        kDefaultValue
    };

} // namespace

TwytchSlider::TwytchSlider(String name) :
        Slider(name), bipolar_(false), units_(""), scaling_type_(kLinear),
        post_multiply_(1.0), string_lookup_(nullptr), parent_(nullptr) { }


void TwytchSlider::mouseDown(const MouseEvent& e) {
    if (e.mods.isPopupMenu()) {
        PopupMenu m;
        m.addItem(kArmMidiLearn, "Learn Controller Assignment");
        m.addItem(kClearMidiLearn, "Clear Controller Assignment");

        if (isDoubleClickReturnEnabled())
            m.addItem(kDefaultValue, "Set to Default Value");

        int result = m.show();
        if (result == kArmMidiLearn) {
            SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
            if (parent)
                parent->armMidiLearn(getName().toStdString(), getMinimum(), getMaximum());
        }
        else if (result == kClearMidiLearn) {
            SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
            if (parent)
                parent->clearMidiLearn(getName().toStdString());
        }
        else if (result == kDefaultValue) {
            setValue(getDoubleClickReturnValue());
        }
    }
    else
        Slider::mouseDown(e);
}

void TwytchSlider::mouseEnter(const juce::MouseEvent &e) {
    // notifyTooltip();
}

void TwytchSlider::valueChanged() {
    Slider::valueChanged();
    // notifyTooltip();
}

String TwytchSlider::getTextFromValue(double value) {
    if (scaling_type_ == kStringLookup)
        return string_lookup_[(int)value];

    float display_value = value;
    switch (scaling_type_) {
        case kPolynomial:
            display_value = powf(display_value, 2.0f);
            break;
        case kExponential:
            display_value = powf(2.0f, display_value);
            break;
        default:
            break;
    }
    display_value *= post_multiply_;

    return String(display_value) + " " + units_;
}

void TwytchSlider::drawShadow(Graphics &g) {
    static const DropShadow shadow(Colour(0xbb000000), 3, Point<int>(0, 0));
    static const float shadow_angle = mopo::PI / 1.3f;

    g.saveState();
    g.setOrigin(getX(), getY());

    if (isRotary()) {
        float full_radius = std::min(getWidth() / 2.0f, getHeight() / 2.0f);
        Path shadow_path;
        shadow_path.addCentredArc(full_radius, full_radius,
                                  0.87f * full_radius, 0.85f * full_radius,
                                  0, -shadow_angle, shadow_angle, true);
        shadow.drawForPath(g, shadow_path);
    }
    g.restoreState();
}

void TwytchSlider::notifyTooltip() {
    if (!parent_)
        parent_ = findParentComponentOfClass<FullInterface>();
    if (parent_) {
        parent_->setToolTipText(getName(), getTextFromValue(getValue()));
    }
}