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

#include "formant_response.h"

#include "fonts.h"
#include "midi_lookup.h"
#include "utils.h"

#define MIN_GAIN_DB 0.0f
#define MAX_GAIN_DB 24.0f
#define MIN_RESONANCE 0.5
#define MAX_RESONANCE 16.0
#define GRID_CELL_WIDTH 8
#define DELTA_SLOPE_REDRAW_THRESHOLD 0.01
#define X_REDRAW_THRESHOLD 30

FormantResponse::FormantResponse(int resolution) : midi_(0.0f), frequency_(0.0f),
                                                   response_(0.0f), decibels_(0.0f) {
  resolution_ = resolution;

  formant_filter_.setSampleRate(44100);
  resetResponsePath();

  setOpaque(true);
  setBufferedToImage(true);
}

FormantResponse::~FormantResponse() { }

void FormantResponse::paintBackground(Graphics& g) {
  g.fillAll(Colour(0xff424242));

  g.setColour(Colour(0xff4a4a4a));
  for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
    g.drawLine(x, 0, x, getHeight());
  for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
    g.drawLine(0, y, getWidth(), y);
}

void FormantResponse::paint(Graphics& g) {
  static const PathStrokeType stroke(1.5f, PathStrokeType::beveled, PathStrokeType::rounded);

  g.drawImage(background_,
              0, 0, getWidth(), getHeight(),
              0, 0, background_.getWidth(), background_.getHeight());

  g.setColour(Colour(0xff03a9f4));
  g.strokePath(filter_response_path_, stroke);

  g.setFont(Fonts::getInstance()->proportional_regular().withPointHeight(16.0f));
  g.drawText("MIDI", 0, 0, 100, 20, Justification::left);
  g.drawText(String(midi_), 100, 0, 400, 20, Justification::left);
  g.drawText("Frequency", 0, 20, 100, 20, Justification::left);
  g.drawText(String(frequency_), 100, 20, 400, 20, Justification::left);
  g.drawText("Response", 0, 40, 100, 20, Justification::left);
  g.drawText(String(response_), 100, 40, 400, 20, Justification::left);
  g.drawText("Decibels", 0, 60, 100, 20, Justification::left);
  g.drawText(String(decibels_), 100, 60, 400, 20, Justification::left);
}

void FormantResponse::resized() {
  const Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  float scale = display.scale;
  background_ = Image(Image::ARGB, scale * getWidth(), scale * getHeight(), true);
  Graphics g(background_);
  g.addTransform(AffineTransform::scale(scale, scale));
  paintBackground(g);

  computeFilterCoefficients();
  resetResponsePath();
}

void FormantResponse::mouseMove(const MouseEvent& e) {
  if (cutoff_sliders_.empty())
    return;

  double percent = mopo::utils::clamp((1.0 * e.getPosition().x) / getWidth(), 0.0, 1.0);
  midi_ = cutoff_sliders_[0]->proportionOfLengthToValue(percent);
  frequency_ = mopo::utils::midiNoteToFrequency(midi_);
  response_ = fabs(formant_filter_.getAmplitudeResponse(frequency_));
  decibels_ = mopo::utils::gainToDb(response_);
  repaint();
}

float FormantResponse::getPercentForMidiNote(float midi_note) {
  float frequency = mopo::utils::midiNoteToFrequency(midi_note);
  float response = fabs(formant_filter_.getAmplitudeResponse(frequency));
  float gain_db = mopo::utils::gainToDb(response);
  return (gain_db - MIN_GAIN_DB) / (MAX_GAIN_DB - MIN_GAIN_DB);
}

void FormantResponse::resetResponsePath() {
  static const int wrap_size = 10;

  if (cutoff_sliders_.empty())
    return;

  filter_response_path_.clear();
  filter_response_path_.startNewSubPath(-wrap_size, getHeight() + wrap_size);
  float start_percent = getPercentForMidiNote(0.0);
  float last_y = getHeight() * (1.0f - start_percent);
  float last_slope = 0.0f;
  float last_x = 0.0f;

  filter_response_path_.lineTo(-wrap_size, last_y);

  for (int i = 0; i < resolution_; ++i) {
    float t = (1.0f * i) / (resolution_ - 1);
    float midi_note = cutoff_sliders_[0]->proportionOfLengthToValue(t);
    float percent = getPercentForMidiNote(midi_note);

    float new_x = getWidth() * t;
    float new_y = getHeight() * (1.0f - percent);
    float new_slope = (new_y - last_y) / (new_x - last_x);
    if (fabs(last_slope - new_slope) > DELTA_SLOPE_REDRAW_THRESHOLD ||
        new_x - last_x > X_REDRAW_THRESHOLD) {
      last_x = new_x;
      last_y = new_y;
      last_slope = new_slope;
      filter_response_path_.lineTo(new_x, new_y);
    }
  }

  float end_percent = getPercentForMidiNote(cutoff_sliders_[0]->getMaximum());

  filter_response_path_.lineTo(getWidth() + wrap_size, getHeight() * (1.0f - end_percent));
  filter_response_path_.lineTo(getWidth() + wrap_size, getHeight() + wrap_size);
}

void FormantResponse::computeFilterCoefficients() {
  if (cutoff_sliders_.empty() || resonance_sliders_.empty() || gain_sliders_.empty())
    return;

  for (int i = 0; i < formant_filter_.num_formants(); ++i) {
    double frequency = mopo::utils::midiNoteToFrequency(cutoff_sliders_[i]->getValue());
    double resonance = mopo::utils::magnitudeToQ(resonance_sliders_[i]->getValue());
    double decibels = INTERPOLATE(MIN_GAIN_DB, MAX_GAIN_DB, gain_sliders_[i]->getValue());
    double gain = mopo::utils::dbToGain(decibels);

    formant_filter_.getFormant(i)->computeCoefficients(mopo::Filter::kGainedBandPass,
                                                       frequency, resonance, gain);
  }
  resetResponsePath();
}

void FormantResponse::sliderValueChanged(Slider* moved_slider) {
  computeFilterCoefficients();
  repaint();
}

void FormantResponse::setResonanceSliders(std::vector<Slider*> sliders) {
  resonance_sliders_ = sliders;
  for (Slider* slider : sliders)
    slider->addListener(this);

  computeFilterCoefficients();
  repaint();
}

void FormantResponse::setCutoffSliders(std::vector<Slider*> sliders) {
  cutoff_sliders_ = sliders;
  for (Slider* slider : sliders)
    slider->addListener(this);

  computeFilterCoefficients();
  repaint();
}

void FormantResponse::setGainSliders(std::vector<Slider*> sliders) {
  gain_sliders_ = sliders;
  for (Slider* slider : sliders)
    slider->addListener(this);

  computeFilterCoefficients();
  repaint();
}
