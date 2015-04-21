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

#include "modulation_meter.h"
#include "mopo.h"

#define ANGLE 2.51327412f

ModulationMeter::ModulationMeter(const mopo::Processor::Output* mono_total,
                                 const mopo::Processor::Output* poly_total,
                                 const TwytchSlider* slider) :
        mono_total_(mono_total), poly_total_(poly_total),
        destination_(slider), current_knob_percent_(0.0), current_mod_percent_(0.0),
        knob_stroke_(0.0f, PathStrokeType::beveled, PathStrokeType::butt),
        full_radius_(0.0), outer_radius_(0.0), knob_percent_(0.0), mod_percent_(0.0) {
    setInterceptsMouseClicks(false, false);
    setPaintingIsUnclipped(true);
    setOpaque(false);
    update(0);
}

ModulationMeter::~ModulationMeter() {
}

void ModulationMeter::paint(Graphics& g) {
    if (destination_->getSliderStyle() == Slider::RotaryHorizontalVerticalDrag)
        drawKnob(g);
    else
        drawSlider(g);
}

void ModulationMeter::resized() {
    static const float stroke_percent = 0.12f;

    full_radius_ = std::min(getWidth() / 2.0f, getHeight() / 2.0f);
    float stroke_width = 2.0f * full_radius_ * stroke_percent;
    knob_stroke_ = PathStrokeType(stroke_width, PathStrokeType::beveled, PathStrokeType::butt);
    outer_radius_ = full_radius_ - stroke_width;
}

void ModulationMeter::update(int num_voices) {
    if (mono_total_) {
        float value = mono_total_->buffer[0];
        if (poly_total_ && num_voices)
            value += poly_total_->buffer[0] / num_voices;

        double range = destination_->getMaximum() - destination_->getMinimum();
        double mod_percent = CLAMP((value - destination_->getMinimum()) / range, 0.0, 1.0);
        double knob_percent = (destination_->getValue() - destination_->getMinimum()) / range;

        if (mod_percent != current_mod_percent_ || knob_percent != current_knob_percent_) {
            current_mod_percent_ = mod_percent;
            current_knob_percent_ = knob_percent;
            knob_percent_ = std::pow(current_knob_percent_, destination_->getSkewFactor());
            mod_percent_ = std::pow(current_mod_percent_, destination_->getSkewFactor());
            repaint();
        }
    }
}

void ModulationMeter::drawSlider(Graphics& g) {
    g.setColour(Colour(0x88ffffff));

    if (destination_->getSliderStyle() == Slider::LinearBar) {
        float knob_position = getWidth() * knob_percent_;
        float mod_position = getWidth() * mod_percent_;

        if (destination_->getInterval() == 1.0 && destination_->getMinimum() == 0.0) {
            int index = current_mod_percent_ * destination_->getMaximum() + 0.5;
            float width = getWidth() / (destination_->getMaximum() + 1.0);

            g.setColour(Colour(0xaaffffff));
            g.drawRect(index * width, 0.0f, width, float(getHeight()), 1.0f);
        }
        else {
            if (destination_->isBipolar())
                fillHorizontalRect(g, getWidth() / 2.0f, knob_position, getHeight());
            else
                fillHorizontalRect(g, 0, knob_position, getHeight());

            g.setColour(Colour(0xffffe57f));
            fillHorizontalRect(g, knob_position, mod_position, getHeight() / 2.0f);
        }
    }
    else {
        float mod_position = getHeight() * (1.0f - mod_percent_);
        float knob_position = getHeight() * (1.0f - knob_percent_);

        if (destination_->isBipolar())
            fillVerticalRect(g, getHeight() / 2.0f, knob_position, getWidth());
        else
            fillVerticalRect(g, 0, knob_position, getWidth());

        g.setColour(Colour(0xffffe57f));
        fillVerticalRect(g, mod_position, knob_position, getWidth() / 2.0f);
    }
}

void ModulationMeter::drawKnob(Graphics& g) {
    float current_mod_angle = INTERPOLATE(-ANGLE, ANGLE, mod_percent_);
    float current_knob_angle = INTERPOLATE(-ANGLE, ANGLE, knob_percent_);
    
    Path rail;
    rail.addCentredArc(full_radius_, full_radius_, outer_radius_, outer_radius_,
                       0.0f, -ANGLE, ANGLE, true);

    g.setColour(Colour(0xff4a4a4a));
    g.strokePath(rail, knob_stroke_);

    Path active_section;
    if (destination_->isBipolar()) {
        active_section.addCentredArc(full_radius_, full_radius_, outer_radius_, outer_radius_,
                                     0.0f, 0.0f, current_knob_angle, true);
    }
    else {
        active_section.addCentredArc(full_radius_, full_radius_, outer_radius_, outer_radius_,
                                     0.0f, -ANGLE, current_knob_angle, true);
    }

    g.setColour(Colour(0xffffab00));
    g.strokePath(active_section, knob_stroke_);

    if (current_mod_angle != current_knob_angle) {
        Path mod_section;
        mod_section.addCentredArc(full_radius_, full_radius_, outer_radius_, outer_radius_,
                                  0.0f, current_mod_angle, current_knob_angle, true);
        g.setColour(Colour(0xfff16504));
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