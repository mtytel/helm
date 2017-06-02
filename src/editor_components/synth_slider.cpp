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

#include "synth_slider.h"

#include "default_look_and_feel.h"
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

const float SynthSlider::rotary_angle = 0.8f * static_cast<float>(mopo::PI);
const float SynthSlider::linear_rail_width = 2.0f;

SynthSlider::SynthSlider(String name) : Slider(name), bipolar_(false), flip_coloring_(false),
                                        active_(true), snap_to_value_(false), snap_value_(0.0),
                                        string_lookup_(nullptr), parent_(nullptr) {
  if (!mopo::Parameters::isParameter(name.toStdString()))
    return;

  setRotaryParameters(2.0f * mopo::PI - rotary_angle, 2.0f * mopo::PI + rotary_angle, true);
  mopo::ValueDetails details = mopo::Parameters::getDetails(name.toStdString());
  if (details.steps)
    setRange(details.min, details.max, (details.max - details.min) / (details.steps - 1));
  else
    setRange(details.min, details.max);

  post_offset_ = details.post_offset;
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
  if (parent == nullptr)
    return;
  SynthBase* synth = parent->getSynth();

  if (e.mods.isPopupMenu()) {
    PopupMenu m;
    m.setLookAndFeel(DefaultLookAndFeel::instance());

    if (isDoubleClickReturnEnabled())
      m.addItem(kDefaultValue, "Set to Default Value");

    std::vector<mopo::ModulationConnection*> connections;
    m.addItem(kArmMidiLearn, "Learn MIDI Assignment");
    if (parent->getSynth()->isMidiMapped(getName().toStdString()))
      m.addItem(kClearMidiLearn, "Clear MIDI Assignment");

    connections = parent->getSynth()->getDestinationConnections(getName().toStdString());

    String disconnect("Disconnect from ");
    for (int i = 0; i < connections.size(); ++i)
      m.addItem(kModulationList + i, disconnect + connections[i]->source);

    if (connections.size() > 1)
      m.addItem(kClearModulations, "Disconnect all modulations");

    int result = m.show();
    if (result == kArmMidiLearn)
      synth->armMidiLearn(getName().toStdString(), getMinimum(), getMaximum());
    else if (result == kClearMidiLearn)
      synth->clearMidiLearn(getName().toStdString());
    else if (result == kDefaultValue)
      setValue(getDoubleClickReturnValue());
    else if (result == kClearModulations) {
      for (mopo::ModulationConnection* connection : connections) {
        std::string source = connection->source;
        synth->disconnectModulation(connection);
      }
      for (SynthSlider::SliderListener* listener : slider_listeners_)
        listener->modulationsChanged(getName().toStdString());
    }
    else if (result >= kModulationList) {
      int connection_index = result - kModulationList;
      std::string source = connections[connection_index]->source;
      synth->disconnectModulation(connections[connection_index]);

      for (SynthSlider::SliderListener* listener : slider_listeners_)
        listener->modulationsChanged(getName().toStdString());
    }
  }
  else {
    Slider::mouseDown(e);

    if (parent)
      synth->beginChangeGesture(getName().toStdString());
    if (isRotary()) {
      click_position_ = e.getScreenPosition().toFloat();
      setMouseCursor(MouseCursor::NoCursor);
    }
  }
}

void SynthSlider::mouseUp(const MouseEvent& e) {
  Slider::mouseUp(e);

  if (isRotary() && !e.mods.isPopupMenu()) {
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    if (parent)
      parent->getSynth()->endChangeGesture(getName().toStdString());

    setMouseCursor(MouseCursor::ParentCursor);
    Desktop::getInstance().getMainMouseSource().setScreenPosition(click_position_);
  }
}

void SynthSlider::mouseEnter(const MouseEvent &e) {
  Slider::mouseEnter(e);
  notifyTooltip();
  for (SynthSlider::SliderListener* listener : slider_listeners_)
    listener->hoverStarted(getName().toStdString());
}

void SynthSlider::mouseExit(const MouseEvent &e) {
  Slider::mouseExit(e);
  for (SynthSlider::SliderListener* listener : slider_listeners_)
    listener->hoverEnded(getName().toStdString());
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
    case mopo::ValueDetails::kSquareRoot:
      display_value = sqrt(display_value);
      break;
    default:
      break;
  }
  display_value += post_offset_;
  display_value *= post_multiply_;

  return String(synthRound(display_value)) + " " + units_;
}

double SynthSlider::snapValue(double attempted_value, DragMode drag_mode) {
  const double percent = 0.05;
  if (!snap_to_value_ || drag_mode != DragMode::absoluteDrag)
    return attempted_value;

  double range = getMaximum() - getMinimum();
  double radius = percent * range;
  if (attempted_value - snap_value_ <= radius && attempted_value - snap_value_ >= -radius)
    return snap_value_;
  return attempted_value;
}

void SynthSlider::drawShadow(Graphics &g) {
  if (&getLookAndFeel() == TextLookAndFeel::instance())
    drawRectangularShadow(g);
  else if (isRotary())
    drawRotaryShadow(g);
  else {
    g.setColour(Colour(0xff222222));
    g.fillRect(getBounds());
  }
}

void SynthSlider::drawRotaryShadow(Graphics &g) {
  static const DropShadow shadow(Colour(0xee000000), 3, Point<int>(0, 0));
  static const float stroke_percent = 0.12f;

  g.saveState();
  g.setOrigin(getX(), getY());

  float full_radius = std::min(getWidth() / 2.0f, getHeight() / 2.0f);
  float stroke_width = 2.0f * full_radius * stroke_percent;
  Path shadow_path;
  float outer_radius = full_radius - stroke_width;
  shadow_path.addCentredArc(full_radius, full_radius,
                            0.89f * full_radius, 0.87f * full_radius,
                            0, -rotary_angle, rotary_angle, true);
  shadow.drawForPath(g, shadow_path);

  Path rail_outer;
  rail_outer.addCentredArc(full_radius, full_radius, outer_radius, outer_radius,
                           0.0f, -rotary_angle, rotary_angle, true);

  g.setColour(Colour(0xff333333));

  PathStrokeType outer_stroke =
      PathStrokeType(stroke_width, PathStrokeType::beveled, PathStrokeType::butt);
  g.strokePath(rail_outer, outer_stroke);

  g.restoreState();
}

void SynthSlider::drawRectangularShadow(Graphics &g) {
  static const DropShadow shadow(Colour(0xbb000000), 2, Point<int>(0, 0));

  g.saveState();
  g.setOrigin(getX(), getY());
  shadow.drawForRectangle(g, getLocalBounds());

  g.setColour(Colours::white);
  if (isHorizontal()) {
    float y1 = (getHeight() - linear_rail_width) / 2.0f;
    int y2 = y1 + linear_rail_width;
    g.fillRect(0, (int)y1, getWidth(), 1);
    g.fillRect(0, y2, getWidth(), 1);
  }
  else {
    float x1 = (getWidth() - linear_rail_width) / 2.0f;
    int x2 = x1 + linear_rail_width;
    g.fillRect((int)x1, 0, 1, getHeight());
    g.fillRect(x2, 0, 1, getHeight());
  }

  g.restoreState();
}

void SynthSlider::flipColoring(bool flip_coloring) {
  flip_coloring_ = flip_coloring;
  repaint();
}

void SynthSlider::setBipolar(bool bipolar) {
  bipolar_ = bipolar;
  repaint();
}

void SynthSlider::setActive(bool active) {
  active_ = active;
  repaint();
}

void SynthSlider::addSliderListener(SynthSlider::SliderListener* listener) {
  slider_listeners_.push_back(listener);
}

void SynthSlider::notifyTooltip() {
  if (parent_ == nullptr)
    parent_ = findParentComponentOfClass<FullInterface>();
  if (parent_) {
    std::string name = getName().toStdString();
    if (mopo::Parameters::isParameter(name))
      name = mopo::Parameters::getDetails(name).display_name;

    parent_->setToolTipText(name, getTextFromValue(getValue()));
  }
}
