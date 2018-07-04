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

#include "step_sequencer_section.h"

#include "colors.h"
#include "fonts.h"
#include "modulation_look_and_feel.h"
#include "tempo_selector.h"
#include "text_look_and_feel.h"

#define CONTROL_SECTION_WIDTH 45
#define TEXT_HEIGHT 16
#define TEXT_WIDTH 42
#define SLIDER_WIDTH 36

StepSequencerSection::StepSequencerSection(String name) : SynthSection(name) {
  static const int TEMPO_DRAG_SENSITIVITY = 150;

  addAndMakeVisible(step_sequencer_ = new GraphicalStepSequencer());

  addSlider(retrigger_ = new RetriggerSelector("step_sequencer_retrigger"));
  retrigger_->setSliderStyle(Slider::LinearBar);
  retrigger_->setStringLookup(mopo::strings::freq_retrigger_styles);

  addSlider(num_steps_ = new SynthSlider("num_steps"));
  num_steps_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  num_steps_->setLookAndFeel(TextLookAndFeel::instance());

  addSlider(frequency_ = new SynthSlider("step_frequency"));
  frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  frequency_->setLookAndFeel(TextLookAndFeel::instance());

  addSlider(tempo_ = new SynthSlider("step_sequencer_tempo"));
  tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  tempo_->setStringLookup(mopo::strings::synced_frequencies);
  tempo_->setLookAndFeel(TextLookAndFeel::instance());
  tempo_->setMouseDragSensitivity(TEMPO_DRAG_SENSITIVITY);

  addSlider(sync_ = new TempoSelector("step_sequencer_sync"));
  sync_->setSliderStyle(Slider::LinearBar);
  sync_->setTempoSlider(tempo_);
  sync_->setFreeSlider(frequency_);
  sync_->setStringLookup(mopo::strings::freq_sync_styles);

  addSlider(smoothing_ = new SynthSlider("step_smoothing"));
  smoothing_->setSliderStyle(Slider::LinearBar);

  addModulationButton(modulation_button_ = new ModulationButton("step_sequencer"));
  modulation_button_->setLookAndFeel(ModulationLookAndFeel::instance());

  createStepSequencerSliders();
}

StepSequencerSection::~StepSequencerSection() {
  retrigger_ = nullptr;
  step_sequencer_ = nullptr;
  num_steps_ = nullptr;
  frequency_ = nullptr;
  tempo_ = nullptr;
  sync_ = nullptr;

  for (Slider* slider : sequencer_sliders_)
    delete slider;
}

void StepSequencerSection::paintBackground(Graphics& g) {
  static const DropShadow component_shadow(Colour(0x88000000), 2, Point<int>(0, 1));
  int text_height = size_ratio_ * TEXT_HEIGHT;
  float font_size = size_ratio_ * 10.0f;
  int text_buffer = 6;

  SynthSection::paintBackground(g);
  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(font_size));
  drawTextForComponent(g, TRANS("STEPS"), num_steps_, text_buffer);
  g.drawText(TRANS("FREQUENCY"),
             retrigger_->getBounds().getX(), frequency_->getBounds().getBottom() + text_buffer,
             frequency_->getBounds().getWidth() + 2 * text_height, font_size,
             Justification::centred, false);

  drawTextForComponent(g, TRANS("SLIDE"), smoothing_, text_buffer);

  component_shadow.drawForRectangle(g, step_sequencer_->getBounds());
}

void StepSequencerSection::resized() {
  int mod_button_width = getModButtonWidth();
  int title_width = getTitleWidth();
  int text_width = size_ratio_ * TEXT_WIDTH;
  int text_height = size_ratio_ * TEXT_HEIGHT;
  int control_section_width = size_ratio_ * CONTROL_SECTION_WIDTH;
  int slider_width = size_ratio_* SLIDER_WIDTH;

  step_sequencer_->setBounds(0, title_width, getWidth(),
                             getHeight() - title_width - control_section_width);
  int y = getHeight() - control_section_width + size_ratio_ * 6.0f;

  modulation_button_->setBounds(size_ratio_ * 10.0f, y, mod_button_width, mod_button_width);
  num_steps_->setBounds(proportionOfWidth(0.22f), y, text_width, text_height);
  retrigger_->setBounds(proportionOfWidth(0.51f) - text_height, y, text_height, text_height);
  frequency_->setBounds(proportionOfWidth(0.51f), y, text_width, text_height);
  sync_->setBounds(frequency_->getBounds().getX() + text_width, frequency_->getBounds().getY(),
                   text_height, text_height);

  smoothing_->setBounds(proportionOfWidth(0.81f), y, slider_width, text_height);
  tempo_->setBounds(frequency_->getBounds());

  SynthSection::resized();

  frequency_->setPopupDisplayEnabled(false, false, nullptr);
  tempo_->setPopupDisplayEnabled(false, false, nullptr);
}

void StepSequencerSection::createStepSequencerSliders() {
  for (int i = 0; i < num_steps_->getMaximum(); ++i) {
    String num(i);
    if (num.length() == 1)
      num = String("0") + num;

    SynthSlider* step = new SynthSlider(String("step_seq_") + num);
    step->snapToValue(true, 0.0);
    addSlider(step);
    sequencer_sliders_.push_back(step);
  }
  step_sequencer_->setStepSliders(sequencer_sliders_);
  step_sequencer_->setNumStepsSlider(num_steps_);
  step_sequencer_->setName("step_sequencer_step");
}

void StepSequencerSection::animate(bool animate) {
  SynthSection::animate(animate);
  step_sequencer_->showRealtimeFeedback(animate);
}

void StepSequencerSection::reset() {
  step_sequencer_->resetBackground();
  SynthSection::reset();
}
