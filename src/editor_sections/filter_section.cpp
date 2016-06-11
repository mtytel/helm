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

#include "filter_section.h"

#include "filter_response.h"
#include "filter_selector.h"
#include "fonts.h"
#include "synth_slider.h"

#define KNOB_SECTION_WIDTH 70
#define KNOB_WIDTH 40
#define SLIDER_WIDTH 10
#define FILTER_TYPE_WIDTH 16

FilterSection::FilterSection(String name) : SynthSection(name) {
  addSlider(filter_type_ = new FilterSelector("filter_type"));
  filter_type_->setSliderStyle(Slider::LinearBar);
  filter_type_->setStringLookup(mopo::strings::filter_types);

  addSlider(cutoff_ = new SynthSlider("cutoff"));
  cutoff_->setSliderStyle(Slider::LinearBar);

  addSlider(resonance_ = new SynthSlider("resonance"));
  resonance_->setSliderStyle(Slider::LinearBarVertical);

  addAndMakeVisible(filter_response_ = new FilterResponse(300));
  filter_response_->setCutoffSlider(cutoff_);
  filter_response_->setResonanceSlider(resonance_);
  filter_response_->setFilterTypeSlider(filter_type_);

  addSlider(fil_env_depth_ = new SynthSlider("fil_env_depth"));
  fil_env_depth_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  fil_env_depth_->setBipolar();

  addSlider(keytrack_ = new SynthSlider("keytrack"));
  keytrack_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  keytrack_->setBipolar();
}

FilterSection::~FilterSection() {
  filter_type_ = nullptr;
  cutoff_ = nullptr;
  resonance_ = nullptr;
  filter_response_ = nullptr;
  fil_env_depth_ = nullptr;
  keytrack_ = nullptr;
}

void FilterSection::paintBackground(Graphics& g) {
  static const DropShadow component_shadow(Colour(0xbb000000), 4, Point<int>(0, 0));
  SynthSection::paintBackground(g);

  g.setColour(Colour(0xffbbbbbb));

  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(10.0f));
  drawTextForComponent(g, TRANS("ENV DEPTH"), fil_env_depth_);
  drawTextForComponent(g, TRANS("KEY TRACK"), keytrack_);

  component_shadow.drawForRectangle(g, filter_response_->getBounds());
}

void FilterSection::resized() {
  int response_width = getWidth() - KNOB_SECTION_WIDTH - SLIDER_WIDTH;
  int response_height = getHeight() - SLIDER_WIDTH - FILTER_TYPE_WIDTH - 20;
  int knob_center_x = getWidth() - KNOB_SECTION_WIDTH / 2;

  filter_type_->setBounds(0, 20, getWidth() - KNOB_SECTION_WIDTH, FILTER_TYPE_WIDTH);
  cutoff_->setBounds(0, getHeight() - SLIDER_WIDTH, response_width, SLIDER_WIDTH);
  resonance_->setBounds(response_width, 20 + FILTER_TYPE_WIDTH, SLIDER_WIDTH, response_height);
  filter_response_->setBounds(0, 20 + FILTER_TYPE_WIDTH, response_width, response_height);
  fil_env_depth_->setBounds(knob_center_x - KNOB_WIDTH / 2, getHeight() / 5,
                            KNOB_WIDTH, KNOB_WIDTH);
  keytrack_->setBounds(knob_center_x - KNOB_WIDTH / 2, 3 * getHeight() / 5,
                       KNOB_WIDTH, KNOB_WIDTH);

  SynthSection::resized();
}
