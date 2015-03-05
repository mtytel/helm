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
                                 const Slider* slider) :
        mono_total_(mono_total), poly_total_(poly_total),
        destination_(slider), current_percent_(0.0f) {
    setInterceptsMouseClicks(false, false);
    setOpaque(false);
    update(0);
}

ModulationMeter::~ModulationMeter() {
}

void ModulationMeter::update(int num_voices) {
    if (mono_total_) {
        float value = mono_total_->buffer[0];
        if (poly_total_ && num_voices)
            value += poly_total_->buffer[0] / num_voices;

        float range = destination_->getMaximum() - destination_->getMinimum();
        float percent = CLAMP((value - destination_->getMinimum()) / range, 0.0f, 1.0f);
        
        if (percent != current_percent_) {
            current_percent_ = percent;
            repaint();
        }
    }
}

void ModulationMeter::paint(Graphics& g) {
    if (destination_->getSliderStyle() == Slider::RotaryHorizontalVerticalDrag)
        drawKnob(g);
    else
        drawSlider(g);
}

void ModulationMeter::drawSlider(Graphics& g) {
    g.setColour(Colour(0x66ffffff));

    if (destination_->getSliderStyle() == Slider::LinearBar) {
        float middle = getWidth() / 2.0f;

        if (destination_->getMaximum() == -destination_->getMinimum())
            g.fillRect(middle, 1.0f, getWidth() * current_percent_ - middle, 1.0f * getHeight() - 2.0f);
        else
            g.fillRect(1.0f, 1.0f, getWidth() * current_percent_, 1.0f * getHeight() - 2.0f);
    }
    else {
        float middle = getHeight() / 2.0f;

        if (destination_->getMaximum() == -destination_->getMinimum())
            g.fillRect(1.0f, middle, getWidth() - 2.0f, -getHeight() * current_percent_ - middle);
        else
            g.fillRect(1.0f, getHeight() - 1.0f, getWidth() - 2.0f, -getHeight() * current_percent_);
    }
}

void ModulationMeter::drawKnob(Graphics& g) {
    static const float stroke_width = 4.0f;
    static const PathStrokeType stroke_type =
    PathStrokeType(stroke_width, PathStrokeType::beveled, PathStrokeType::butt);

    float current_angle = INTERPOLATE(-ANGLE, ANGLE, current_percent_);
    float full_radius = std::min(getWidth() / 2.0f, getHeight() / 2.0f);
    float outer_radius = full_radius - stroke_width;
    Path rail;
    rail.addCentredArc(full_radius, full_radius, outer_radius, outer_radius,
                       0.0f, -ANGLE, ANGLE, true);
    g.setColour(Colour(0xff000000));
    g.strokePath(rail, stroke_type);

    Path active_section;
    if (destination_->getMaximum() == -destination_->getMinimum()) {
        active_section.addCentredArc(full_radius, full_radius, outer_radius, outer_radius,
                                     0.0f, 0.0f, current_angle, true);
    }
    else {
        active_section.addCentredArc(full_radius, full_radius, outer_radius, outer_radius,
                                     0.0f, -ANGLE, current_angle, true);
    }
    g.setColour(Colour(0xffffffff));
    g.strokePath(active_section, stroke_type);
}