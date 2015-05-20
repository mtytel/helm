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

#include "filter_response.h"
#include "midi_lookup.h"

#define MAG_TO_DB_CONSTANT 20.0f
#define MIN_GAIN_DB -30.0f
#define MAX_GAIN_DB 24.0f
#define MIN_RESONANCE 0.5
#define MAX_RESONANCE 16.0
#define GRID_CELL_WIDTH 8

FilterResponse::FilterResponse(int resolution) {
  resolution_ = resolution;
  cutoff_slider_ = nullptr;
  resonance_slider_ = nullptr;
  filter_type_slider_ = nullptr;

  filter_.setSampleRate(44100);
  resetResponsePath();

  setOpaque(true);
  setBufferedToImage(true);
}

FilterResponse::~FilterResponse() { }

void FilterResponse::paint(Graphics& g) {
  static const PathStrokeType stroke(1.5f, PathStrokeType::beveled, PathStrokeType::rounded);
  static const DropShadow shadow(Colour(0xbb000000), 5, Point<int>(0, 0));
  
  g.fillAll (Colour (0xff424242));
  g.setColour(Colour(0xff4a4a4a));
  for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
    g.drawLine(x, 0, x, getHeight());
  for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
    g.drawLine(0, y, getWidth(), y);

  shadow.drawForPath(g, filter_response_path_);

  g.setColour(Colour(0xff565656));
  g.fillPath(filter_response_path_);

  g.setColour(Colour(0xff03a9f4));
  g.strokePath(filter_response_path_, stroke);
}

void FilterResponse::resized() {
  computeFilterCoefficients();
  resetResponsePath();
}

void FilterResponse::mouseDown(const MouseEvent& e) {
  if (e.mods.isRightButtonDown() && filter_type_slider_) {
    int max = filter_type_slider_->getMaximum();
    int current_value = filter_type_slider_->getValue();
    filter_type_slider_->setValue((current_value + 1) % (max + 1));

    computeFilterCoefficients();
  }
  else
    setFilterSettingsFromPosition(e.getPosition());
  repaint();
}

void FilterResponse::mouseDrag(const MouseEvent& e) {
  if (!e.mods.isRightButtonDown()) {
    setFilterSettingsFromPosition(e.getPosition());
    repaint();
  }
}

float FilterResponse::getPercentForMidiNote(float midi_note) {
  float frequency = mopo::utils::midiNoteToFrequency(midi_note);
  float response = fabs(filter_.getAmplitudeResponse(frequency));
  float gain_db = MAG_TO_DB_CONSTANT * log10(response);
  return (gain_db - MIN_GAIN_DB) / (MAX_GAIN_DB - MIN_GAIN_DB);
}

void FilterResponse::resetResponsePath() {
  static const int wrap_size = 10;

  if (cutoff_slider_ == nullptr)
    return;

  filter_response_path_.clear();
  filter_response_path_.startNewSubPath(-wrap_size, getHeight() + wrap_size);
  float start_percent = getPercentForMidiNote(0.0);
  filter_response_path_.lineTo(-wrap_size, getHeight() * (1.0f - start_percent));

  for (int i = 0; i < resolution_; ++i) {
    float t = (1.0f * i) / (resolution_ - 1);
    float midi_note = cutoff_slider_->proportionOfLengthToValue(t);
    float percent = getPercentForMidiNote(midi_note);

    filter_response_path_.lineTo(getWidth() * t, getHeight() * (1.0f - percent));
  }

  float end_percent = getPercentForMidiNote(cutoff_slider_->getMaximum());

  filter_response_path_.lineTo(getWidth() + wrap_size, getHeight() * (1.0f - end_percent));
  filter_response_path_.lineTo(getWidth() + wrap_size, getHeight() + wrap_size);
}

void FilterResponse::computeFilterCoefficients() {
  if (cutoff_slider_ == nullptr || resonance_slider_ == nullptr || filter_type_slider_ == nullptr)
    return;

  mopo::Filter::Type type = static_cast<mopo::Filter::Type>(
                            static_cast<int>(filter_type_slider_->getValue()));
  double frequency = mopo::utils::midiNoteToFrequency(cutoff_slider_->getValue());
  double resonance = mopo::utils::magnitudeToQ(resonance_slider_->getValue());
  double decibals = INTERPOLATE(MIN_GAIN_DB, MAX_GAIN_DB, resonance_slider_->getValue());
  double gain = mopo::utils::dbToGain(decibals);
  filter_.computeCoefficients(type, frequency, resonance, gain);
  resetResponsePath();
}

void FilterResponse::setFilterSettingsFromPosition(Point<int> position) {
  if (cutoff_slider_) {
    double percent = CLAMP((1.0 * position.x) / getWidth(), 0.0, 1.0);
    double frequency = cutoff_slider_->proportionOfLengthToValue(percent);
    cutoff_slider_->setValue(frequency);
  }
  if (resonance_slider_) {
    double percent = CLAMP(1.0 - (1.0 * position.y) / getHeight(), 0.0, 1.0);
    resonance_slider_->setValue(resonance_slider_->proportionOfLengthToValue(percent));
  }

  computeFilterCoefficients();
}

void FilterResponse::sliderValueChanged(Slider* moved_slider) {
  computeFilterCoefficients();
  repaint();
}

void FilterResponse::setResonanceSlider(Slider* slider) {
  if (resonance_slider_)
    resonance_slider_->removeListener(this);
  resonance_slider_ = slider;
  resonance_slider_->addListener(this);
  computeFilterCoefficients();
  repaint();
}

void FilterResponse::setCutoffSlider(Slider* slider) {
  if (cutoff_slider_)
    cutoff_slider_->removeListener(this);
  cutoff_slider_ = slider;
  cutoff_slider_->addListener(this);
  computeFilterCoefficients();
  repaint();
}

void FilterResponse::setFilterTypeSlider(Slider* slider) {
  if (filter_type_slider_)
    filter_type_slider_->removeListener(this);
  filter_type_slider_ = slider;
  filter_type_slider_->addListener(this);
  computeFilterCoefficients();
  repaint();
}
