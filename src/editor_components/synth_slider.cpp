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

#include "synth_slider.h"
#include "synth_gui_interface.h"
#include "full_interface.h"
#include "text_look_and_feel.h"

namespace {
    enum MenuIds {
        kCancel = 0,
        kArmMidiLearn,
        kClearMidiLearn,
        kDefaultValue,
        kClearModulations,
        kModulationList
    };

} // namespace

SynthSlider::SynthSlider(String name) :
        Slider(name), bipolar_(false), units_(""), scaling_type_(kLinear),
        post_multiply_(1.0), string_lookup_(nullptr), parent_(nullptr) {
    setBufferedToImage(true);
}


void SynthSlider::mouseDown(const MouseEvent& e) {
    if (e.mods.isPopupMenu()) {
        PopupMenu m;
        m.addItem(kArmMidiLearn, "Learn Controller Assignment");
        m.addItem(kClearMidiLearn, "Clear Controller Assignment");

        if (isDoubleClickReturnEnabled())
            m.addItem(kDefaultValue, "Set to Default Value");

        SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
        std::vector<mopo::ModulationConnection*> connections;
        if (parent) {
            connections = parent->getDestinationConnections(getName().toStdString());

            if (connections.size() > 1) {
                m.addItem(kClearModulations, "Clear All Modulations");
            }

            String disconnect("Disconnect From Source: ");
            for (int i = 0; i < connections.size(); ++i)
                m.addItem(kModulationList + i, disconnect + connections[i]->source);
        }

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
        else if (result == kClearModulations) {
            for (mopo::ModulationConnection* connection : connections) {
                std::string source = connection->source;
                parent->disconnectModulation(connection);

                FullInterface* parent = findParentComponentOfClass<FullInterface>();
                if (parent)
                    parent->modulationChanged(source);
            }
        }
        else if (result >= kModulationList) {
            int connection_index = result - kModulationList;
            std::string source = connections[connection_index]->source;
            parent->disconnectModulation(connections[connection_index]);

            FullInterface* parent = findParentComponentOfClass<FullInterface>();
            if (parent)
                parent->modulationChanged(source);
        }
    }
    else {
        Slider::mouseDown(e);
        if (isRotary()) {
            click_position_ = e.getScreenPosition().toFloat();
            setMouseCursor(MouseCursor::NoCursor);
        }
    }
}

void SynthSlider::mouseUp(const MouseEvent& e) {
    Slider::mouseUp(e);
    if (isRotary() && !e.mods.isPopupMenu()) {
        setMouseCursor(MouseCursor::ParentCursor);
        Desktop::getInstance().getMainMouseSource().setScreenPosition(click_position_);
    }
}

void SynthSlider::mouseEnter(const MouseEvent &e) {
    Slider::mouseEnter(e);
    // notifyTooltip();
}

void SynthSlider::valueChanged() {
    Slider::valueChanged();
    // notifyTooltip();
}

String SynthSlider::getTextFromValue(double value) {
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

void SynthSlider::drawShadow(Graphics &g) {
    if (&getLookAndFeel() == TextLookAndFeel::instance())
        drawRectangularShadow(g);
    else if (isRotary())
        drawRotaryShadow(g);
}

void SynthSlider::drawRotaryShadow(Graphics &g) {
    static const DropShadow shadow(Colour(0xbb000000), 3, Point<int>(0, 0));
    static const float shadow_angle = mopo::PI / 1.3f;

    g.saveState();
    g.setOrigin(getX(), getY());

    float full_radius = std::min(getWidth() / 2.0f, getHeight() / 2.0f);
    Path shadow_path;
    shadow_path.addCentredArc(full_radius, full_radius,
                              0.87f * full_radius, 0.85f * full_radius,
                              0, -shadow_angle, shadow_angle, true);
    shadow.drawForPath(g, shadow_path);
    g.restoreState();
}

void SynthSlider::drawRectangularShadow(Graphics &g) {
    static const DropShadow shadow(Colour(0xbb000000), 2, Point<int>(0, 0));

    g.saveState();
    g.setOrigin(getX(), getY());
    shadow.drawForRectangle(g, getLocalBounds());

    g.restoreState();
}

void SynthSlider::notifyTooltip() {
    if (!parent_)
        parent_ = findParentComponentOfClass<FullInterface>();
    if (parent_) {
        parent_->setToolTipText(getName(), getTextFromValue(getValue()));
    }
}
