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

#include "formant_tuner.h"

#include "fonts.h"
#include "formant_response.h"
#include "synth_slider.h"

FormantTuner::FormantTuner(String name) : SynthSection(name) {
  std::vector<Slider*> cutoffs;
  std::vector<Slider*> resonances;
  std::vector<Slider*> gains;

  for (int i = 0; i < kNumFormants; ++i) {
    cutoffs_[i] = new SynthSlider("cutoff");
    addAndMakeVisible(cutoffs_[i]);
    cutoffs_[i]->setSliderStyle(Slider::LinearBarVertical);
    cutoffs_[i]->setTextBoxStyle(Slider::TextBoxAbove, false, 150, 20);

    resonances_[i] = new SynthSlider("resonance");
    addAndMakeVisible(resonances_[i]);
    resonances_[i]->setSliderStyle(Slider::LinearBarVertical);
    resonances_[i]->setTextBoxStyle(Slider::TextBoxAbove, false, 150, 20);

    gains_[i] = new SynthSlider("reverb_dry_wet");
    addAndMakeVisible(gains_[i]);
    gains_[i]->setSliderStyle(Slider::LinearBarVertical);
    gains_[i]->setTextBoxStyle(Slider::TextBoxAbove, false, 150, 20);

    cutoffs.push_back(cutoffs_[i]);
    resonances.push_back(resonances_[i]);
    gains.push_back(gains_[i]);
  }

  addAndMakeVisible(formant_response_ = new FormantResponse(1400));
  formant_response_->setCutoffSliders(cutoffs);
  formant_response_->setResonanceSliders(resonances);
  formant_response_->setGainSliders(gains);
}

FormantTuner::~FormantTuner() {
  for (int i = 0; i < kNumFormants; ++i) {
    cutoffs_[i] = nullptr;
    resonances_[i] = nullptr;
    gains_[i] = nullptr;
  }
  formant_response_ = nullptr;
}

void FormantTuner::paintBackground(Graphics& g) {
  static const DropShadow component_shadow(Colour(0xbb000000), 4, Point<int>(0, 0));
  SynthSection::paintBackground(g);

  g.setColour(Colour(0xffbbbbbb));
  component_shadow.drawForRectangle(g, formant_response_->getBounds());
}

void FormantTuner::resized() {
  int slider_height = 100;
  int response_height = getHeight() - 20 - slider_height;
  int slider_y = getHeight() - slider_height;
  int slider_width = getWidth() / (3 * kNumFormants);

  for (int i = 0; i < kNumFormants; ++i) {
    int index = 3 * i;
    cutoffs_[i]->setBounds(slider_width * index, slider_y, slider_width, slider_height);
    resonances_[i]->setBounds(slider_width * (index + 1), slider_y, slider_width, slider_height);
    gains_[i]->setBounds(slider_width * (index + 2), slider_y, slider_width, slider_height);
  }

  formant_response_->setBounds(0, 20, getWidth(), response_height);
  SynthSection::resized();
}
