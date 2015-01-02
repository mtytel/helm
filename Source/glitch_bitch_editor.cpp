#include "glitch_bitch.h"
#include "glitch_bitch_editor.h"

GlitchBitchEditor::GlitchBitchEditor(GlitchBitch& gb) : AudioProcessorEditor(&gb), glitch_bitch_(gb) {
  controls_ = glitch_bitch_.getSynth()->getControls();

  mopo::control_map::iterator iter = controls_.begin();
  for (; iter != controls_.end(); ++iter) {
    mopo::Control* control = iter->second;
    Slider* next_slider = new Slider(iter->first);
    next_slider->setRange(control->min(), control->max(), control->getIncrementSize());
    next_slider->setSliderStyle(Slider::LinearHorizontal);
    next_slider->setTextBoxStyle(Slider::TextBoxLeft, false, 80, 20);
    next_slider->setValue(control->current_value());
    next_slider->addListener(this);
    addAndMakeVisible(next_slider);

    sliders_[iter->first] = next_slider;
  }

  setSize(1000, 700);
}

GlitchBitchEditor::~GlitchBitchEditor() {
  std::map<std::string, Slider*>::iterator iter = sliders_.begin();
  for (; iter != sliders_.end(); ++iter)
    delete iter->second;
}

void GlitchBitchEditor::paint(Graphics& g) {
  g.fillAll(Colours::white);
  g.setColour(Colours::black);
  g.setFont(15.0f);

  int x = 20;
  int y = 20;
  std::map<std::string, Slider*>::iterator iter = sliders_.begin();
  for (; iter != sliders_.end(); ++iter) {
    g.drawText(iter->first, x, y, 100, 20, Justification::centredLeft);
    y+= 20;

    if (y > 600) {
      y = 20;
      x = 520;
    }
  }
}

void GlitchBitchEditor::resized() {
  int x = 120;
  int y = 20;
  std::map<std::string, Slider*>::iterator iter = sliders_.begin();
  for (; iter != sliders_.end(); ++iter) {
    iter->second->setBounds(x, y, 360, 20);
    y+= 20;

    if (y > 600) {
      y = 20;
      x = 620;
    }
  }
}

void GlitchBitchEditor::sliderValueChanged(Slider* edited_slider) {
  controls_[edited_slider->getName().toStdString()]->set(edited_slider->getValue());
}