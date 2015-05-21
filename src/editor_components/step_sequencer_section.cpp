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

#include "step_sequencer_section.h"

#include "synth_slider.h"
#include "tempo_selector.h"
#include "text_look_and_feel.h"

#define KNOB_SECTION_WIDTH 40
#define KNOB_WIDTH 32
#define TEXT_HEIGHT 16
#define TEXT_WIDTH 42

StepSequencerSection::StepSequencerSection(String name) : SynthSection(name) {
  static const int TEMPO_DRAG_SENSITIVITY = 150;

  addAndMakeVisible(step_sequencer_ = new GraphicalStepSequencer());

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

  addSlider(smoothing_ = new SynthSlider("step_smoothing"));
  smoothing_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(sync_ = new TempoSelector("step_sequencer_sync"));
  sync_->setSliderStyle(Slider::LinearBar);
  sync_->setTempoSlider(tempo_);
  sync_->setFreeSlider(frequency_);
  sync_->setStringLookup(mopo::strings::freq_sync_styles);

  createStepSequencerSliders();
}

StepSequencerSection::~StepSequencerSection() {
  step_sequencer_ = nullptr;
  num_steps_ = nullptr;
  frequency_ = nullptr;
  tempo_ = nullptr;
  sync_ = nullptr;

  for (Slider* slider : sequencer_sliders_)
    delete slider;
}

void StepSequencerSection::paint(Graphics& g) {
  static const DropShadow component_shadow(Colour(0x88000000), 2, Point<int>(0, 1));
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  SynthSection::paint(g);
  step_sequencer_->showRealtimeFeedback();

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  drawTextForSlider(g, TRANS("STEPS"), num_steps_);
  g.drawText(TRANS("FREQUENCY"),
             frequency_->getBounds().getX(), frequency_->getBounds().getY() + TEXT_HEIGHT + 6,
             frequency_->getBounds().getWidth() + TEXT_HEIGHT, 10, Justification::centred, false);

  component_shadow.drawForRectangle(g, step_sequencer_->getBounds());
}

void StepSequencerSection::resized() {
  step_sequencer_->setBounds(0, 20, getWidth(), getHeight() - 20 - KNOB_SECTION_WIDTH);
  int y = getHeight() - 5 * KNOB_SECTION_WIDTH / 6;

  num_steps_->setBounds(proportionOfWidth(0.25f), y, TEXT_WIDTH, TEXT_HEIGHT);
  frequency_->setBounds(proportionOfWidth(0.51f), y, TEXT_WIDTH, TEXT_HEIGHT);
  sync_->setBounds(frequency_->getBounds().getX() + TEXT_WIDTH, frequency_->getBounds().getY(),
                   TEXT_HEIGHT, TEXT_HEIGHT);

  smoothing_->setBounds(proportionOfWidth(0.8f), y, KNOB_WIDTH, KNOB_WIDTH);
  tempo_->setBounds(frequency_->getBounds());
}

void StepSequencerSection::createStepSequencerSliders() {
  for (int i = 0; i < num_steps_->getMaximum(); ++i) {
    String num(i);
    if (num.length() == 1)
      num = String("0") + num;

    SynthSlider* step = new SynthSlider(String("step_seq_") + num);
    step->setRange(-1.0, 1.0);
    step->addListener(this);
    sequencer_sliders_.push_back(step);
    slider_lookup_[step->getName().toStdString()] = step;
  }
  step_sequencer_->setStepSliders(sequencer_sliders_);
  step_sequencer_->setNumStepsSlider(num_steps_);
  step_sequencer_->setName("step_sequencer_step");
}
