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

#define DEFAULT_POPUP_BUFFER 10

namespace {
  enum MenuIds {
    kCancel = 0,
    kArmMidiLearn,
    kClearMidiLearn,
    kDefaultValue,
    kClearModulations,
    kModulationList
  };

  static void sliderPopupCallback(int result, SynthSlider* slider) {
    if (slider != nullptr && result != kCancel)
      slider->handlePopupResult(result);
  }
} // namespace

const float SynthSlider::rotary_angle = 0.8f * static_cast<float>(mopo::PI);
const float SynthSlider::linear_rail_width = 2.0f;

SynthSlider::SynthSlider(String name) : Slider(name), bipolar_(false), flip_coloring_(false),
                                        active_(true), snap_to_value_(false), snap_value_(0.0),
                                        string_lookup_(nullptr), parent_(nullptr) {
  popup_placement_ = BubbleComponent::below;
  popup_buffer_ = DEFAULT_POPUP_BUFFER;

  if (!mopo::Parameters::isParameter(name.toStdString()))
    return;

  setRotaryParameters(2.0f * mopo::PI - rotary_angle, 2.0f * mopo::PI + rotary_angle, true);
  details_ = mopo::Parameters::getDetails(name.toStdString());
  if (details_.steps)
    setRange(details_.min, details_.max, (details_.max - details_.min) / (details_.steps - 1));
  else
    setRange(details_.min, details_.max);

  setDoubleClickReturnValue(true, details_.default_value);
  setTextBoxStyle(Slider::NoTextBox, true, 0, 0);

  setBufferedToImage(true);
  setColour(Slider::backgroundColourId, Colour(0xff303030));
  setColour(Slider::textBoxOutlineColourId, Colour(0x00000000));
}

void SynthSlider::resized() {
  if (parent_ == nullptr)
    parent_ = findParentComponentOfClass<FullInterface>();

  setPopupDisplayEnabled(true, false, parent_);
  Slider::resized();
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

    m.showMenuAsync(PopupMenu::Options(),
                    ModalCallbackFunction::forComponent(sliderPopupCallback, this));
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
  if (!e.mods.isPopupMenu()) {
    Slider::mouseUp(e);

    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    if (parent)
      parent->getSynth()->endChangeGesture(getName().toStdString());

    if (isRotary()) {
      setMouseCursor(MouseCursor::ParentCursor);
      Desktop::getInstance().getMainMouseSource().setScreenPosition(click_position_);
    }
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
  notifyGuis();

  if (popup_placement_ == BubbleComponent::below && popup_buffer_) {
    Component* popup = getCurrentPopupDisplay();
    if (popup) {
      Rectangle<int> bounds = popup->getBounds();
      Rectangle<int> local_bounds = getLocalArea(popup, popup->getLocalBounds());

      int y_diff = getHeight() + popup_buffer_ - local_bounds.getY();
      bounds.setY(bounds.getY() + y_diff);
      popup->setBounds(bounds);
    }
  }
}

String SynthSlider::getTextFromValue(double value) {
  if (string_lookup_) {
    int lookup = mopo::utils::iclamp(value, 0, getMaximum());
    return string_lookup_[lookup];
  }

  float display_value = value;
  switch (details_.display_skew) {
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
  display_value += details_.post_offset;
  if (details_.display_invert)
    display_value = 1.0 / display_value;
  display_value *= details_.display_multiply;

  return formatValue(display_value);
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
  g.setColour(Colour(0xff333333));
  g.fillRect(getLocalBounds());

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

String SynthSlider::formatValue(float value) {
  static const int number_length = 5;
  static const int max_decimals = 3;

  if (details_.steps)
    return String(value) + " " + details_.display_units;

  String format = String(value, max_decimals);
  format = format.substring(0, number_length);
  int spaces = number_length - format.length();
  
  for (int i = 0; i < spaces; ++i)
    format = " " + format;
  
  return format + " " + details_.display_units;
}

void SynthSlider::notifyGuis() {
  for (SynthSlider::SliderListener* listener : slider_listeners_)
    listener->guiChanged(this);
}

void SynthSlider::handlePopupResult(int result) {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent == nullptr)
    return;

  SynthBase* synth = parent->getSynth();
  std::vector<mopo::ModulationConnection*> connections =
      parent->getSynth()->getDestinationConnections(getName().toStdString());

  if (result == kArmMidiLearn)
    synth->armMidiLearn(getName().toStdString());
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
