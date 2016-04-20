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

#include "synth_slider.h"
#include "full_interface.h"
#include "helm_common.h"
#include "synth_gui_interface.h"
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

  mopo::mopo_float synthRound(mopo::mopo_float value) {
    static const mopo::mopo_float round_scale = 1000.0;
    int scaled_rounded = round_scale * value;
    return scaled_rounded / round_scale;
  }
} // namespace

SynthSlider::SynthSlider(String name) : Slider(name), bipolar_(false), flip_coloring_(false),
                                        active_(true),
                                        string_lookup_(nullptr), parent_(nullptr) {
  if (!mopo::Parameters::isParameter(name.toStdString()))
    return;

  mopo::ValueDetails details = mopo::Parameters::getDetails(name.toStdString());
  if (details.steps)
    setRange(details.min, details.max, (details.max - details.min) / (details.steps - 1));
  else
    setRange(details.min, details.max);

  post_multiply_ = details.display_multiply;
  scaling_type_ = details.display_skew;
  units_ = details.display_units;
  setDoubleClickReturnValue(true, details.default_value);
  setTextBoxStyle(Slider::NoTextBox, true, 0, 0);

  setBufferedToImage(true);
  setColour(Slider::backgroundColourId, Colour(0xff303030));
  setColour(Slider::textBoxOutlineColourId, Colour(0x00000000));
}

void SynthSlider::mouseDown(const MouseEvent& e) {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();

  if (e.mods.isPopupMenu()) {
    PopupMenu m;

    if (isDoubleClickReturnEnabled())
      m.addItem(kDefaultValue, "Set to Default Value");

    std::vector<mopo::ModulationConnection*> connections;
    if (parent) {
      m.addItem(kArmMidiLearn, "Learn MIDI Assignment");
      if (parent->isMidiMapped(getName().toStdString()))
        m.addItem(kClearMidiLearn, "Clear MIDI Assignment");

      connections = parent->getDestinationConnections(getName().toStdString());

      String disconnect("Disconnect from ");
      for (int i = 0; i < connections.size(); ++i)
        m.addItem(kModulationList + i, disconnect + connections[i]->source);

      if (connections.size() > 1)
        m.addItem(kClearModulations, "Disconnect all modulations");
    }

    int result = m.show();
    if (result == kArmMidiLearn) {
      parent->armMidiLearn(getName().toStdString(), getMinimum(), getMaximum());
    }
    else if (result == kClearMidiLearn) {
      parent->clearMidiLearn(getName().toStdString());
    }
    else if (result == kDefaultValue) {
      setValue(getDoubleClickReturnValue());
    }
    else if (result == kClearModulations) {
      for (mopo::ModulationConnection* connection : connections) {
        std::string source = connection->source;
        parent->disconnectModulation(connection);
      }
    }
    else if (result >= kModulationList) {
      int connection_index = result - kModulationList;
      std::string source = connections[connection_index]->source;
      parent->disconnectModulation(connections[connection_index]);
    }
  }
  else {
    Slider::mouseDown(e);

    if (parent)
      parent->beginChangeGesture(getName().toStdString());
    if (isRotary()) {
      click_position_ = e.getScreenPosition().toFloat();
      setMouseCursor(MouseCursor::NoCursor);
    }
  }
}

void SynthSlider::mouseUp(const MouseEvent& e) {
  Slider::mouseUp(e);

  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent)
    parent->endChangeGesture(getName().toStdString());

  if (isRotary() && !e.mods.isPopupMenu()) {
    setMouseCursor(MouseCursor::ParentCursor);
    Desktop::getInstance().getMainMouseSource().setScreenPosition(click_position_);
  }
}

void SynthSlider::mouseEnter(const MouseEvent &e) {
  Slider::mouseEnter(e);
  notifyTooltip();
}

void SynthSlider::valueChanged() {
  Slider::valueChanged();
  notifyTooltip();
}

String SynthSlider::getTextFromValue(double value) {
  if (string_lookup_) {
    int lookup = mopo::utils::iclamp(value, 0, getMaximum());
    return string_lookup_[lookup];
  }

  float display_value = value;
  switch (scaling_type_) {
    case mopo::ValueDetails::kQuadratic:
      display_value = powf(display_value, 2.0f);
      break;
    case mopo::ValueDetails::kExponential:
      display_value = powf(2.0f, display_value);
      break;
    default:
      break;
  }
  display_value *= post_multiply_;

  return String(synthRound(display_value)) + " " + units_;
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
    std::string name = getName().toStdString();
    if (mopo::Parameters::isParameter(name))
      name = mopo::Parameters::getDetails(name).display_name;

    parent_->setToolTipText(name, getTextFromValue(getValue()));
  }
}
