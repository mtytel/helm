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

#include "synth_section.h"

#include "synth_gui_interface.h"

#define TITLE_WIDTH 20
#define SHADOW_WIDTH 3

void SynthSection::paint(Graphics& g) {
    static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                             BinaryData::RobotoRegular_ttfSize));
    // Draw border.
    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 3.000f);

    // Draw shadow divider.
    float shadow_top = TITLE_WIDTH - SHADOW_WIDTH;
    float shadow_bottom = TITLE_WIDTH;
    g.setGradientFill(ColourGradient(Colour(0x00000000), 0.0f, shadow_top,
                                     Colour(0x77000000), 0.0f, shadow_bottom,
                                     false));
    g.fillRect(0, 0, getWidth(), TITLE_WIDTH);

    // Draw text title.
    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS(getName()), 0, 0, getWidth(), TITLE_WIDTH,
               Justification::centred, true);
}

void SynthSection::sliderValueChanged(Slider* sliderThatWasMoved) {
    std::string name = sliderThatWasMoved->getName().toStdString();
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    parent->valueChanged(name, sliderThatWasMoved->getValue());
}
