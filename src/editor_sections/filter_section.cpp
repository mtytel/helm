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

#include "filter_section.h"

#include "colors.h"
#include "filter_response.h"
#include "filter_selector.h"
#include "fonts.h"
#include "synth_button.h"
#include "synth_slider.h"
#include "text_selector.h"
#include "text_look_and_feel.h"

#define KNOB_SECTION_WIDTH 70
#define KNOB_WIDTH 40
#define SLIDER_WIDTH 10
#define FILTER_TYPE_WIDTH 16
#define STYLE_SLIDER_WIDTH 60
#define KEYTRACK_TOP_PADDING 5
#define KEYTRACK_HEIGHT 14
#define KEYTRACK_WIDTH 60
#define STYLE_SLIDER_PADDING 2
#define STYLE_LABEL_WIDTH 30
#define STYLE_LABEL_PADDING_X 5
#define STYLE_LABEL_PADDING_Y 3

FilterSection::FilterSection(String name) : SynthSection(name) {
  addSlider(filter_shelf_ = new FilterSelector("filter_shelf"));
  filter_shelf_->setSliderStyle(Slider::LinearBar);
  filter_shelf_->setStringLookup(mopo::strings::filter_shelves);
  filter_shelf_->setVisible(false);

  addSlider(cutoff_ = new SynthSlider("cutoff"));
  cutoff_->setSliderStyle(Slider::LinearBar);

  addSlider(blend_ = new SynthSlider("filter_blend"));
  blend_->snapToValue(true, 1.0);
  blend_->setBipolar(true);
  blend_->setSliderStyle(Slider::LinearBar);

  addSlider(resonance_ = new SynthSlider("resonance"));
  resonance_->setSliderStyle(Slider::LinearBarVertical);

  addAndMakeVisible(filter_response_ = new FilterResponse(300));
  filter_response_->setCutoffSlider(cutoff_);
  filter_response_->setResonanceSlider(resonance_);
  filter_response_->setFilterBlendSlider(blend_);
  filter_response_->setFilterShelfSlider(filter_shelf_);

  addSlider(drive_ = new SynthSlider("filter_drive"));
  drive_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(fil_env_depth_ = new SynthSlider("fil_env_depth"));
  fil_env_depth_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  fil_env_depth_->setBipolar();

  addSlider(keytrack_ = new SynthSlider("keytrack"));
  keytrack_->setSliderStyle(Slider::LinearBar);
  keytrack_->snapToValue(true, 0.0);
  keytrack_->setBipolar();

  TextSelector* style = new TextSelector("filter_style");
  addSlider(filter_style_ = style);
  filter_style_->setSliderStyle(Slider::LinearBar);
  filter_style_->setStringLookup(mopo::strings::filter_style);
  style->setShortStringLookup(mopo::strings::filter_style_short);

  addButton(filter_on_ = new SynthButton("filter_on"));
  setActivator(filter_on_);
}

FilterSection::~FilterSection() {
  filter_shelf_ = nullptr;
  filter_on_ = nullptr;
  filter_style_ = nullptr;
  cutoff_ = nullptr;
  resonance_ = nullptr;
  filter_response_ = nullptr;
  fil_env_depth_ = nullptr;
  keytrack_ = nullptr;
  drive_ = nullptr;
}

void FilterSection::paintBackground(Graphics& g) {
  static const PathStrokeType stroke(1.000f, PathStrokeType::curved, PathStrokeType::rounded);
  static const DropShadow component_shadow(Colour(0xbb000000), 4, Point<int>(0, 0));
  SynthSection::paintBackground(g);

  g.setColour(Colors::controlLabelText);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(10.0f));
  
  drawTextForComponent(g, TRANS("ENV DEPTH"), fil_env_depth_);
  drawTextForComponent(g, TRANS("KEY TRACK"), keytrack_);
  drawTextForComponent(g, TRANS("DRIVE"), drive_);

  g.setColour(Colour(0xffaaaaaa));
  g.strokePath(low_pass_, stroke);
  g.strokePath(high_pass_, stroke);

  component_shadow.drawForRectangle(g, filter_response_->getBounds());
}

void FilterSection::resized() {
  filter_on_->setBounds(2, 0, 20, 20);
  int response_width = getWidth() - KNOB_SECTION_WIDTH - SLIDER_WIDTH;
  int response_height = getHeight() - SLIDER_WIDTH - FILTER_TYPE_WIDTH - 20;
  int knob_center_x = getWidth() - KNOB_SECTION_WIDTH / 2;

  int style_slider_remove = STYLE_SLIDER_WIDTH + STYLE_SLIDER_PADDING;
  blend_->setBounds(STYLE_LABEL_WIDTH, 22,
                    getWidth() - KNOB_SECTION_WIDTH - 2 * STYLE_LABEL_WIDTH - style_slider_remove,
                    FILTER_TYPE_WIDTH - 4);
  filter_shelf_->setBounds(0, 20, getWidth() - KNOB_SECTION_WIDTH - style_slider_remove,
                           FILTER_TYPE_WIDTH);

  cutoff_->setBounds(0, getHeight() - SLIDER_WIDTH, response_width, SLIDER_WIDTH);
  resonance_->setBounds(response_width, 20 + FILTER_TYPE_WIDTH, SLIDER_WIDTH, response_height);
  filter_response_->setBounds(0, 20 + FILTER_TYPE_WIDTH, response_width, response_height);

  int knob_vertical = KNOB_WIDTH + 14;
  int keytrack_vertical =  KEYTRACK_TOP_PADDING + KEYTRACK_HEIGHT + 14;
  int space = (getHeight() - 20 - 2 * knob_vertical - keytrack_vertical) / 4;

  drive_->setBounds(knob_center_x - KNOB_WIDTH / 2, 20 + space,
                    KNOB_WIDTH, KNOB_WIDTH);
  fil_env_depth_->setBounds(knob_center_x - KNOB_WIDTH / 2,
                            20 + knob_vertical + 2 * space,
                            KNOB_WIDTH, KNOB_WIDTH);
  keytrack_->setBounds(knob_center_x - KEYTRACK_WIDTH / 2,
                       20 + 2 * knob_vertical + 3 * space + KEYTRACK_TOP_PADDING,
                       KEYTRACK_WIDTH, KEYTRACK_HEIGHT);
  filter_style_->setBounds(filter_shelf_->getRight() + STYLE_SLIDER_PADDING, 20,
                           STYLE_SLIDER_WIDTH, FILTER_TYPE_WIDTH);

  resizeLowPass(STYLE_LABEL_PADDING_X, 20 + STYLE_LABEL_PADDING_Y,
                STYLE_LABEL_WIDTH - 2 * STYLE_LABEL_PADDING_X,
                FILTER_TYPE_WIDTH - 2 * STYLE_LABEL_PADDING_Y);
  resizeHighPass(blend_->getRight() + STYLE_LABEL_PADDING_X, 20 + STYLE_LABEL_PADDING_Y,
                 STYLE_LABEL_WIDTH - 2 * STYLE_LABEL_PADDING_X,
                 FILTER_TYPE_WIDTH - 2 * STYLE_LABEL_PADDING_Y);

  SynthSection::resized();
}

void FilterSection::reset() {
  filter_response_->computeFilterCoefficients();
  filter_response_->repaint();
  SynthSection::reset();
}

void FilterSection::sliderValueChanged(Slider* changed_slider) {
  SynthSection::sliderValueChanged(changed_slider);
  if (changed_slider == filter_style_) {
    float value = changed_slider->getValue();
    mopo::StateVariableFilter::Styles style =
        static_cast<mopo::StateVariableFilter::Styles>(static_cast<int>(value));
    filter_response_->setStyle(style);

    bool shelf = style == mopo::StateVariableFilter::kShelf;
    blend_->setVisible(!shelf);
    filter_shelf_->setVisible(shelf);
  }
}

void FilterSection::setActive(bool active) {
  SynthSection::setActive(active);
  filter_response_->setActive(active);
}

void FilterSection::resizeLowPass(float x, float y, float width, float height) {
  low_pass_.clear();
  low_pass_.startNewSubPath(x, y + height / 2.0f);
  low_pass_.lineTo(x + width / 2.0f, y + height / 2.0f);
  low_pass_.lineTo(x + 4.0f * width / 6.0f, y + height / 4.0f);
  low_pass_.lineTo(x + width, y + height);
}

void FilterSection::resizeHighPass(float x, float y, float width, float height) {
  high_pass_.clear();
  high_pass_.startNewSubPath(x + width, y + height / 2.0f);
  high_pass_.lineTo(x + width / 2.0f, y + height / 2.0f);
  high_pass_.lineTo(x + 2.0f * width / 6.0f, y + height / 4.0f);
  high_pass_.lineTo(x, y + height);
}
