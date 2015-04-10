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
        destination_(slider), current_knob_percent_(0.0), current_mod_percent_(0.0) {
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

        double range = destination_->getMaximum() - destination_->getMinimum();
        double mod_percent = CLAMP((value - destination_->getMinimum()) / range, 0.0, 1.0);
        double knob_percent = (destination_->getValue() - destination_->getMinimum()) / range;

        if (mod_percent != current_mod_percent_ || knob_percent != current_knob_percent_) {
            current_mod_percent_ = mod_percent;
            current_knob_percent_ = knob_percent;
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
    g.setColour(Colour(0x88ffffff));
    float knob_skew = std::pow(current_knob_percent_, destination_->getSkewFactor());
    float mod_skew = std::pow(current_mod_percent_, destination_->getSkewFactor());

    if (destination_->getSliderStyle() == Slider::LinearBar) {
        float middle = getWidth() / 2.0f;
        float mod_position = getWidth() * mod_skew;
        float knob_position = getWidth() * knob_skew;

        if (destination_->getInterval() == 1.0 && destination_->getMinimum() == 0.0) {
            g.setColour(Colour(0xaaffffff));
            int index = current_mod_percent_ * destination_->getMaximum() + 0.5;
            float width = getWidth() / (destination_->getMaximum() + 1.0);
            g.drawRect(index * width, 0.0f, width, float(getHeight()), 1.0f);
        }
        else {
            if (destination_->isBipolar()) {
                if (knob_position >= middle) {
                    g.fillRect(middle, 1.0f, knob_position - middle,
                               1.0f * getHeight() - 2.0f);
                }
                else {
                    g.fillRect(knob_position, 1.0f, middle - knob_position,
                               1.0f * getHeight() - 2.0f);
                }
            }
            else
                g.fillRect(1.0f, 1.0f, getWidth() * knob_skew, 1.0f * getHeight() - 2.0f);

            if (knob_position >= mod_position) {
                g.fillRect(mod_position, 1.0f, knob_position - mod_position,
                           1.0f * getHeight() - 2.0f);
            }
            else {
                g.fillRect(knob_position, 1.0f, mod_position - knob_position,
                           1.0f * getHeight() - 2.0f);
            }
        }
    }
    else {
        float middle = getHeight() / 2.0f;
        float mod_position = getHeight() * (1.0 - mod_skew);
        float knob_position = getHeight() * (1.0 - knob_skew);

        if (destination_->isBipolar())
            if (knob_position >= middle)
                g.fillRect(1.0f, middle, getWidth() - 2.0f, knob_position - middle);
            else
                g.fillRect(1.0f, knob_position, getWidth() - 2.0f, middle - knob_position);
        else
            g.fillRect(1.0f, knob_position, getWidth() - 2.0f, getHeight() - 1.0f - knob_position);

        if (knob_position >= mod_position) {
            g.fillRect(1.0f, mod_position, 1.0f * getHeight() - 2.0f,
                       knob_position - mod_position);
        }
        else {
            g.fillRect(1.0f, knob_position, 1.0f * getHeight() - 2.0f,
                       mod_position - knob_position);
        }
    }
}

void ModulationMeter::drawKnob(Graphics& g) {
    static const float stroke_percent = 0.12f;

    float mod_skew = std::pow(current_mod_percent_, destination_->getSkewFactor());
    float knob_skew = std::pow(current_knob_percent_, destination_->getSkewFactor());

    float current_mod_angle = INTERPOLATE(-ANGLE, ANGLE, mod_skew);
    float current_knob_angle = INTERPOLATE(-ANGLE, ANGLE, knob_skew);

    float full_radius = std::min(getWidth() / 2.0f, getHeight() / 2.0f);
    float stroke_width = 2.0f * full_radius * stroke_percent;
    PathStrokeType stroke_type =
        PathStrokeType(stroke_width, PathStrokeType::beveled, PathStrokeType::butt);
    float outer_radius = full_radius - stroke_width;
    Path rail;
    rail.addCentredArc(full_radius, full_radius, outer_radius, outer_radius,
                       0.0f, -ANGLE, ANGLE, true);
    g.setColour(Colour(0xff333333));
    g.strokePath(rail, stroke_type);

    Path active_section;
    if (destination_->isBipolar()) {
        active_section.addCentredArc(full_radius, full_radius, outer_radius, outer_radius,
                                     0.0f, 0.0f, current_knob_angle, true);
    }
    else {
        active_section.addCentredArc(full_radius, full_radius, outer_radius, outer_radius,
                                     0.0f, -ANGLE, current_knob_angle, true);
    }

    g.setColour(Colour(0xffffae07));
    g.strokePath(active_section, stroke_type);

    Path mod_section;
    mod_section.addCentredArc(full_radius, full_radius, outer_radius, outer_radius,
                              0.0f, current_mod_angle, current_knob_angle, true);
    g.setColour(Colour(0xfff16504));
    g.strokePath(mod_section, stroke_type);

}
