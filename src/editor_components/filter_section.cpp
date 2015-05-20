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

#include "filter_section.h"

#include "filter_response.h"
#include "filter_selector.h"
#include "synth_slider.h"

FilterSection::FilterSection(String name) : SynthSection(name) {
  addSlider(filter_type_ = new FilterSelector("filter_type"));
  filter_type_->setRange(0, 6, 1);
  filter_type_->setSliderStyle(Slider::LinearBar);
  filter_type_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
  filter_type_->addListener(this);
  filter_type_->setStringLookup(mopo::strings::filter_types);

  addSlider(cutoff_ = new SynthSlider("cutoff"));
  cutoff_->setRange(28, 127, 0);
  cutoff_->setSliderStyle(Slider::LinearBar);
  cutoff_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
  cutoff_->addListener(this);
  cutoff_->setDoubleClickReturnValue(true, 80);

  addSlider(resonance_ = new SynthSlider("resonance"));
  resonance_->setRange(0, 1, 0);
  resonance_->setSliderStyle(Slider::LinearBarVertical);
  resonance_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
  resonance_->addListener(this);
  resonance_->setDoubleClickReturnValue(true, 0.5f);

  addAndMakeVisible(filter_response_ = new FilterResponse(300));
  filter_response_->setCutoffSlider(cutoff_);
  filter_response_->setResonanceSlider(resonance_);
  filter_response_->setFilterTypeSlider(filter_type_);

  addSlider(fil_env_depth_ = new SynthSlider("fil_env_depth"));
  fil_env_depth_->setRange(-128, 128, 0);
  fil_env_depth_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  fil_env_depth_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
  fil_env_depth_->addListener(this);
  fil_env_depth_->setUnits("semitones");
  fil_env_depth_->setBipolar();
  fil_env_depth_->setDoubleClickReturnValue(true, 0.0f);

  addSlider(keytrack_ = new SynthSlider("keytrack"));
  keytrack_->setRange(-1, 1, 0);
  keytrack_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  keytrack_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
  keytrack_->addListener(this);
  keytrack_->setUnits("%");
  keytrack_->setPostMultiply(100.0);
  keytrack_->setBipolar();
  keytrack_->setDoubleClickReturnValue(true, 0.0f);
}

FilterSection::~FilterSection() {
  filter_type_ = nullptr;
  cutoff_ = nullptr;
  resonance_ = nullptr;
  filter_response_ = nullptr;
  fil_env_depth_ = nullptr;
  keytrack_ = nullptr;
}

void FilterSection::paint(Graphics& g) {
  static const DropShadow component_shadow(Colour(0xbb000000), 4, Point<int>(0, 0));
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  SynthSection::paint(g);

  g.setColour(Colour(0xffbbbbbb));

  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("ENV DEPTH"), 257, 78, 56, 10, Justification::centred, true);

  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("KEY TRACK"), 255, 142, 60, 10, Justification::centred, true);

  component_shadow.drawForRectangle(g, filter_response_->getBounds());
}

void FilterSection::resized() {
  filter_type_->setBounds(0, 20, 250, 16);
  cutoff_->setBounds(0, 148, 240, 10);
  resonance_->setBounds(240, 36, 10, 112);
  filter_response_->setBounds(0, 36, 240, 112);
  fil_env_depth_->setBounds(265, 32, 40, 40);
  keytrack_->setBounds(265, 96, 40, 40);
}
