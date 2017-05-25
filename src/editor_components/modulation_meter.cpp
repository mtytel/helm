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

#include "modulation_meter.h"

#include "colors.h"
#include "mopo.h"
#include "synth_gui_interface.h"
#include "text_look_and_feel.h"
#include "utils.h"

#define ANGLE 2.51327412f
#define SLIDER_MOD_COLOR 0xff69f0ae

ModulationMeter::ModulationMeter(const mopo::Output* mono_total,
                                 const mopo::Output* poly_total,
                                 const SynthSlider* slider) :
        mono_total_(mono_total), poly_total_(poly_total),
        destination_(slider), current_value_(0.0), knob_percent_(0.0), mod_percent_(0.0),
        knob_stroke_(0.0f, PathStrokeType::beveled, PathStrokeType::butt),
        full_radius_(0.0), outer_radius_(0.0) {
  setInterceptsMouseClicks(false, false);
  updateValue();
  updateDrawing();
}

ModulationMeter::~ModulationMeter() {
}

void ModulationMeter::paint(Graphics& g) {
  if (destination_->getSliderStyle() == Slider::RotaryHorizontalVerticalDrag) {
    if (&destination_->getLookAndFeel() == TextLookAndFeel::instance())
      drawTextSlider(g);
    else
      drawKnob(g);
  }
  else
    drawSlider(g);
}

void ModulationMeter::resized() {
  static const float stroke_percent = 0.12f;

  full_radius_ = std::min(getWidth() / 2.0f, getHeight() / 2.0f);
  float stroke_width = 2.0f * full_radius_ * stroke_percent;
  knob_stroke_ = PathStrokeType(stroke_width, PathStrokeType::beveled, PathStrokeType::butt);
  outer_radius_ = full_radius_ - stroke_width;

  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent) {
    std::vector<mopo::ModulationConnection*> connections;
    connections = parent->getSynth()->getSourceConnections(getName().toStdString());
    setModulated(connections.size());
  }
}

void ModulationMeter::updateValue() {
  if (mono_total_) {
    current_value_ = mono_total_->buffer[0];
    if (poly_total_)
      current_value_ += poly_total_->buffer[0];
  }
}

void ModulationMeter::updateDrawing() {
  double range = destination_->getMaximum() - destination_->getMinimum();
  double value = (current_value_ - destination_->getMinimum()) / range;
  double new_mod_percent = mopo::utils::clamp(value , 0.0, 1.0);
  double new_knob_percent = (destination_->getValue() - destination_->getMinimum()) / range;

  if (new_mod_percent != mod_percent_ || new_knob_percent != knob_percent_) {
    mod_percent_ = new_mod_percent;
    knob_percent_ = new_knob_percent;
    repaint();
  }
}

void ModulationMeter::drawTextSlider(Graphics& g) {
  g.setColour(Colour(SLIDER_MOD_COLOR));

  float diff_percent = mod_percent_ - knob_percent_;

  if (diff_percent < 0.0f) {
    float mod_position = -getHeight() * diff_percent;
    fillVerticalRect(g, 0.0f, mod_position, getWidth() / 2.0f);
  }
  else if (diff_percent > 0.0f) {
    float mod_position = getHeight() * (1.0f - diff_percent);
    fillVerticalRect(g, mod_position, getHeight(), getWidth() / 2.0f);
  }
}

void ModulationMeter::drawSlider(Graphics& g) {
  g.setColour(Colour(SLIDER_MOD_COLOR));

  if (destination_->getSliderStyle() == Slider::LinearBar) {
    float knob_position = getWidth() * knob_percent_;
    float mod_position = getWidth() * mod_percent_;

    if (destination_->getInterval() == 1.0 && destination_->getMinimum() == 0.0) {
      int index = mod_percent_ * destination_->getMaximum() + 0.5;
      float width = getWidth() / (destination_->getMaximum() + 1.0);

      g.setColour(Colour(0xaaffffff));
      g.drawRect(index * width, 0.0f, width, float(getHeight()), 1.0f);
    }
    else
      fillHorizontalRect(g, knob_position, mod_position, getHeight() / 2.0f);
  }
  else {
    float mod_position = getHeight() * (1.0f - mod_percent_);
    float knob_position = getHeight() * (1.0f - knob_percent_);

    fillVerticalRect(g, mod_position, knob_position, getWidth() / 2.0f);
  }
}

void ModulationMeter::drawKnob(Graphics& g) {
  float current_mod_angle = mopo::utils::interpolate(-ANGLE, ANGLE, mod_percent_);
  float current_knob_angle = mopo::utils::interpolate(-ANGLE, ANGLE, knob_percent_);

  if (current_mod_angle != current_knob_angle) {
    Path mod_section;
    mod_section.addCentredArc(full_radius_, full_radius_, outer_radius_, outer_radius_,
                              0.0f, current_mod_angle, current_knob_angle, true);
    g.setColour(Colors::modulation);
    g.strokePath(mod_section, knob_stroke_);
  }
}

void ModulationMeter::fillHorizontalRect(Graphics& g, float x1, float x2, float height) {
  float x = std::min(x1, x2);
  float width = fabsf(x1 - x2);
  float padding = (getHeight() - height) / 2.0f;
  g.fillRect(x, padding, width, height);
}

void ModulationMeter::fillVerticalRect(Graphics& g, float y1, float y2, float width) {
  float y = std::min(y1, y2);
  float height = fabsf(y1 - y2);
  float padding = (getWidth() - width) / 2.0f;
  g.fillRect(padding, y, width, height);
}
