/* Copyright 2013-2015 Matt Tytel
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

#include "oscillator_section.h"

#include "synth_slider.h"
#include "tempo_selector.h"
#include "text_look_and_feel.h"

#define WAVE_RESOLUTION 256
#define CROSS_MOD_WIDTH_PERCENT 0.2
#define KNOB_SECTION_HEIGHT 75
#define KNOB_WIDTH 40
#define TEXT_WIDTH 56
#define TEXT_HEIGHT 16
#define TRANS_WIDTH 42
#define TUNE_WIDTH 32
#define OSC_MIX_HEIGHT 15
#define WAVE_SELECTOR_HEIGHT 10

OscillatorSection::OscillatorSection(String name) : SynthSection(name) {
  static const int UNISON_DRAG_SENSITIVITY = 100;

  addSlider(wave_selector_1_ = new WaveSelector("osc_1_waveform"));
  wave_selector_1_->setSliderStyle(Slider::LinearBar);
  wave_selector_1_->setStringLookup(mopo::strings::waveforms);

  addSlider(wave_selector_2_ = new WaveSelector("osc_2_waveform"));
  wave_selector_2_->setSliderStyle(Slider::LinearBar);
  wave_selector_2_->setStringLookup(mopo::strings::waveforms);

  addAndMakeVisible(wave_viewer_1_ = new WaveViewer(WAVE_RESOLUTION));
  wave_viewer_1_->setWaveSlider(wave_selector_1_);
  addAndMakeVisible(wave_viewer_2_ = new WaveViewer(WAVE_RESOLUTION));
  wave_viewer_2_->setWaveSlider(wave_selector_2_);

  addSlider(cross_modulation_ = new SynthSlider("cross_modulation"));
  cross_modulation_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(osc_mix_ = new SynthSlider("osc_mix"));
  osc_mix_->setSliderStyle(Slider::LinearBar);
  osc_mix_->setBipolar();

  addSlider(transpose_1_ = new SynthSlider("osc_1_transpose"));
  transpose_1_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  transpose_1_->setBipolar();

  addSlider(transpose_2_ = new SynthSlider("osc_2_transpose"));
  transpose_2_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  transpose_2_->setBipolar();

  addSlider(tune_1_ = new SynthSlider("osc_1_tune"));
  tune_1_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  tune_1_->setBipolar();

  addSlider(tune_2_ = new SynthSlider("osc_2_tune"));
  tune_2_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  tune_2_->setBipolar();

  addSlider(unison_detune_1_ = new SynthSlider("osc_1_unison_detune"));
  unison_detune_1_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  unison_detune_1_->setLookAndFeel(TextLookAndFeel::instance());

  addSlider(unison_detune_2_ = new SynthSlider("osc_2_unison_detune"));
  unison_detune_2_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  unison_detune_2_->setLookAndFeel(TextLookAndFeel::instance());

  addSlider(unison_voices_1_ = new SynthSlider("osc_1_unison_voices"));
  unison_voices_1_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  unison_voices_1_->setLookAndFeel(TextLookAndFeel::instance());
  unison_voices_1_->setMouseDragSensitivity(UNISON_DRAG_SENSITIVITY);

  addSlider(unison_voices_2_ = new SynthSlider("osc_2_unison_voices"));
  unison_voices_2_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  unison_voices_2_->setLookAndFeel(TextLookAndFeel::instance());
  unison_voices_2_->setMouseDragSensitivity(UNISON_DRAG_SENSITIVITY);

  addButton(unison_harmonize_1_ = new ToggleButton("unison_1_harmonize"));
  unison_harmonize_1_->setLookAndFeel(TextLookAndFeel::instance());

  addButton(unison_harmonize_2_ = new ToggleButton("unison_2_harmonize"));
  unison_harmonize_2_->setLookAndFeel(TextLookAndFeel::instance());
}

OscillatorSection::~OscillatorSection() {
  wave_viewer_1_ = nullptr;
  wave_viewer_2_ = nullptr;
  wave_selector_1_ = nullptr;
  wave_selector_2_ = nullptr;
  transpose_1_ = nullptr;
  transpose_2_ = nullptr;
  tune_1_ = nullptr;
  tune_2_ = nullptr;
  unison_voices_1_ = nullptr;
  unison_voices_2_ = nullptr;
  unison_detune_1_ = nullptr;
  unison_detune_2_ = nullptr;
  unison_harmonize_1_ = nullptr;
  unison_harmonize_2_ = nullptr;
  osc_mix_ = nullptr;
  cross_modulation_ = nullptr;
}

void OscillatorSection::paintBackground(Graphics& g) {
  static const float extra_knob_padding = 4.0f;
  static const DropShadow component_shadow(Colour(0x99000000), 3, Point<int>(0, 1));
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));

  SynthSection::paintBackground(g);

  g.setColour(Colour(0xff212121));
  g.fillEllipse(transpose_1_->getBounds().toFloat().expanded(extra_knob_padding));
  g.fillEllipse(tune_1_->getBounds().toFloat().expanded(extra_knob_padding));
  g.fillEllipse(transpose_2_->getBounds().toFloat().expanded(extra_knob_padding));
  g.fillEllipse(tune_2_->getBounds().toFloat().expanded(extra_knob_padding));

  g.setColour(Colour(0xff303030));
  g.fillRect(0, tune_1_->getBottom() + 2, getWidth(), 5);

  g.setColour(Colour(0xff4fc3f7));
  g.strokePath(top_left_cross_path_, PathStrokeType(1.0f));

  g.setColour(Colour(0xff4fc3f7));
  g.strokePath(top_right_cross_path_, PathStrokeType(1.0f));

  g.setColour(Colour(0xff4fc3f7));
  g.strokePath(bottom_left_cross_path_, PathStrokeType(1.0f));

  g.setColour(Colour(0xff4fc3f7));
  g.strokePath(bottom_right_cross_path_, PathStrokeType(1.0f));

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  drawTextForComponent(g, TRANS("MOD"), cross_modulation_);
  drawTextForComponent(g, TRANS("MIX"), osc_mix_);
  drawTextForComponent(g, TRANS("TRANS"), transpose_1_);
  drawTextForComponent(g, TRANS("TRANS"), transpose_2_);
  drawTextForComponent(g, TRANS("TUNE"), tune_1_);
  drawTextForComponent(g, TRANS("TUNE"), tune_2_);
  drawTextForComponent(g, TRANS("UNISON"), unison_detune_1_);
  drawTextForComponent(g, TRANS("UNISON"), unison_detune_2_);

  component_shadow.drawForRectangle(g, wave_viewer_1_->getBounds());
  component_shadow.drawForRectangle(g, wave_viewer_2_->getBounds());
  component_shadow.drawForRectangle(g, osc_mix_->getBounds());

  g.setColour(Colour(0xff424242));
  g.fillRect(osc_mix_->getBounds());
  paintKnobShadows(g);
}

void OscillatorSection::resized() {
  float cross_mod_width = CROSS_MOD_WIDTH_PERCENT * getWidth();
  float osc_width = (getWidth() - cross_mod_width) / 2.0f;
  float osc_height = getHeight() - 20 - WAVE_SELECTOR_HEIGHT - OSC_MIX_HEIGHT - KNOB_SECTION_HEIGHT;
  float osc_y = 20 + WAVE_SELECTOR_HEIGHT;

  wave_selector_1_->setBounds(0.0f, 20.0f, osc_width, WAVE_SELECTOR_HEIGHT);
  wave_selector_2_->setBounds(getWidth() - osc_width, 20.0f, osc_width, WAVE_SELECTOR_HEIGHT);
  wave_viewer_1_->setBounds(0.0f, osc_y, osc_width, osc_height);
  wave_viewer_2_->setBounds(getWidth() - osc_width, osc_y, osc_width, osc_height);
  osc_mix_->setBounds(0, osc_y + osc_height + 1, getWidth(), OSC_MIX_HEIGHT - 1);
  cross_modulation_->setBounds((getWidth() - KNOB_WIDTH) / 2.0f,
                               osc_y + (osc_height - KNOB_WIDTH) / 2.0f,
                               KNOB_WIDTH, KNOB_WIDTH);

  float space = (getWidth() - (2.0f * TEXT_WIDTH + 2.0f * TRANS_WIDTH + 2.0f * TUNE_WIDTH)) / 5.0f;
  int knob_bottom = getHeight() - 22.0f;

  tune_1_->setBounds(space, knob_bottom - TUNE_WIDTH, TUNE_WIDTH, TUNE_WIDTH);
  transpose_1_->setBounds(TUNE_WIDTH + space, knob_bottom - TRANS_WIDTH,
                          TRANS_WIDTH, TRANS_WIDTH);

  unison_detune_1_->setBounds(TRANS_WIDTH + TUNE_WIDTH + 2 * space,
                              knob_bottom - TEXT_HEIGHT, TEXT_WIDTH, TEXT_HEIGHT);
  unison_voices_1_->setBounds(TRANS_WIDTH + TUNE_WIDTH + 2 * space,
                              knob_bottom - 2 * TEXT_HEIGHT, TEXT_WIDTH - TEXT_HEIGHT, TEXT_HEIGHT);
  unison_harmonize_1_->setBounds(unison_voices_1_->getX() + unison_voices_1_->getWidth(),
                                 unison_voices_1_->getY(), TEXT_HEIGHT, TEXT_HEIGHT);
  unison_detune_2_->setBounds(TRANS_WIDTH + TEXT_WIDTH + TUNE_WIDTH + 3 * space,
                              knob_bottom - TEXT_HEIGHT, TEXT_WIDTH, TEXT_HEIGHT);
  unison_voices_2_->setBounds(TRANS_WIDTH + TEXT_WIDTH + TUNE_WIDTH + 3 * space,
                              knob_bottom - 2 * TEXT_HEIGHT, TEXT_WIDTH - TEXT_HEIGHT, TEXT_HEIGHT);
  unison_harmonize_2_->setBounds(unison_voices_2_->getX() + unison_voices_2_->getWidth(),
                                 unison_voices_2_->getY(), TEXT_HEIGHT, TEXT_HEIGHT);

  transpose_2_->setBounds(TRANS_WIDTH + 2 * TEXT_WIDTH + TUNE_WIDTH + 4 * space,
                          knob_bottom - TRANS_WIDTH, TRANS_WIDTH, TRANS_WIDTH);
  tune_2_->setBounds(2 * TRANS_WIDTH + 2 * TEXT_WIDTH + TUNE_WIDTH + 4 * space,
                     knob_bottom - TUNE_WIDTH, TUNE_WIDTH, TUNE_WIDTH);

  float cross_x_padding = 8.0f;
  float cross_y_padding = 8.0f;
  float cross_width = cross_mod_width - 2.0f * cross_x_padding;
  float cross_height = osc_height - 2.0f * cross_y_padding;
  float cross_percent = 0.2f;

  top_left_cross_path_.clear();
  top_left_cross_path_.startNewSubPath(osc_width, osc_y + cross_y_padding);
  top_left_cross_path_.lineTo(osc_width + cross_x_padding, osc_y + cross_y_padding);
  top_left_cross_path_.lineTo(osc_width + cross_x_padding + cross_percent * cross_width,
                              osc_y + cross_y_padding + cross_percent * cross_height);

  top_right_cross_path_.clear();
  top_right_cross_path_.startNewSubPath(getWidth() - osc_width, osc_y + cross_y_padding);
  top_right_cross_path_.lineTo(getWidth() - osc_width - cross_x_padding, osc_y + cross_y_padding);
  top_right_cross_path_.lineTo(getWidth() - osc_width - cross_x_padding -
                               cross_percent * cross_width,
                               osc_y + cross_y_padding + cross_percent * cross_height);

  bottom_left_cross_path_.clear();
  bottom_left_cross_path_.startNewSubPath(osc_width, osc_y + osc_height - cross_y_padding);
  bottom_left_cross_path_.lineTo(osc_width + cross_x_padding, osc_y + osc_height - cross_y_padding);
  bottom_left_cross_path_.lineTo(osc_width + cross_x_padding + cross_percent * cross_width,
                                 osc_y + osc_height - cross_y_padding -
                                 cross_percent * cross_height);

  bottom_right_cross_path_.clear();
  bottom_right_cross_path_.startNewSubPath(getWidth() - osc_width,
                                           osc_y + osc_height - cross_y_padding);
  bottom_right_cross_path_.lineTo(getWidth() - osc_width - cross_x_padding,
                                  osc_y + osc_height - cross_y_padding);
  bottom_right_cross_path_.lineTo(getWidth() - osc_width - cross_x_padding -
                                  cross_percent * cross_width,
                                  osc_y + osc_height - cross_y_padding -
                                  cross_percent * cross_height);

  SynthSection::resized();
}
