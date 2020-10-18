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

#include "filter_response.h"

#include "colors.h"
#include "midi_lookup.h"
#include "utils.h"

#define MIN_GAIN_DB -30.0
#define MAX_GAIN_DB 24.0
#define MIN_RESONANCE 0.5
#define MAX_RESONANCE 16.0
#define GRID_CELL_WIDTH 8
#define DELTA_SLOPE_REDRAW_THRESHOLD 0.01
#define X_REDRAW_THRESHOLD 30

FilterResponse::FilterResponse(int resolution) {
  resolution_ = resolution;
  cutoff_slider_ = nullptr;
  resonance_slider_ = nullptr;
  filter_blend_slider_ = nullptr;
  filter_shelf_slider_ = nullptr;

  filter_low_.setSampleRate(44100);
  filter_band_.setSampleRate(44100);
  filter_high_.setSampleRate(44100);
  filter_shelf_.setSampleRate(44100);
  style_ = mopo::StateVariableFilter::kNumStyles;
  active_ = false;
  resetResponsePath();

  setOpaque(true);
  setBufferedToImage(true);
}

FilterResponse::~FilterResponse() { }

void FilterResponse::paintBackground(Graphics& g) {
  g.fillAll(Colour(0xff424242));

  g.setColour(Colour(0xff4a4a4a));
  for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
    g.drawLine(x, 0, x, getHeight());
  for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
    g.drawLine(0, y, getWidth(), y);
}

void FilterResponse::paint(Graphics& g) {
  static const DropShadow shadow(Colour(0xbb000000), 5, Point<int>(0, 0));

  g.drawImage(background_,
              0, 0, getWidth(), getHeight(),
              0, 0, background_.getWidth(), background_.getHeight());

  shadow.drawForPath(g, filter_response_path_);

  g.setColour(Colors::graph_fill);
  g.fillPath(filter_response_path_);

  if (active_)
    g.setColour(Colors::audio);
  else
    g.setColour(Colors::graph_disable);

  float line_width = 1.5f * getHeight() / 150.0f;
  PathStrokeType stroke(line_width, PathStrokeType::beveled, PathStrokeType::rounded);
  g.strokePath(filter_response_path_, stroke);
}

void FilterResponse::resized() {
  const Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  float scale = display.scale;
  background_ = Image(Image::RGB, scale * getWidth(), scale * getHeight(), true);
  Graphics g(background_);
  g.addTransform(AffineTransform::scale(scale, scale));
  paintBackground(g);

  computeFilterCoefficients();
  resetResponsePath();
}

void FilterResponse::mouseDown(const MouseEvent& e) {
  setFilterSettingsFromPosition(e.getPosition());
  repaint();
}

void FilterResponse::mouseDrag(const MouseEvent& e) {
  setFilterSettingsFromPosition(e.getPosition());
  repaint();
}

float FilterResponse::getPercentForMidiNote(float midi_note) {
  float frequency = mopo::utils::midiNoteToFrequency(midi_note);

  float response = 0.0f;
  if (style_ == mopo::StateVariableFilter::kShelf)
    response = fabs(filter_shelf_.getAmplitudeResponse(frequency));
  else {
    float blend = filter_blend_slider_->getValue();
    float low_pass_amount = mopo::utils::clamp(1.0 - blend, 0.0, 1.0);
    float band_pass_amount = mopo::utils::clamp(1.0 - fabs(blend - 1.0), 0.0, 1.0);
    float high_pass_amount = mopo::utils::clamp(blend - 1.0, 0.0, 1.0);

    response = low_pass_amount * filter_low_.getAmplitudeResponse(frequency) +
               band_pass_amount * filter_band_.getAmplitudeResponse(frequency) +
               high_pass_amount * filter_high_.getAmplitudeResponse(frequency);

    response = fabs(response);
    if (style_ == mopo::StateVariableFilter::k24dB)
      response *= response;
  }

  float gain_db = mopo::utils::gainToDb(response);
  return (gain_db - MIN_GAIN_DB) / (MAX_GAIN_DB - MIN_GAIN_DB);
}

void FilterResponse::resetResponsePath() {
  static const int wrap_size = 10;

  if (cutoff_slider_ == nullptr)
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
    float midi_note = cutoff_slider_->proportionOfLengthToValue(t);
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

  float end_percent = getPercentForMidiNote(cutoff_slider_->getMaximum());

  filter_response_path_.lineTo(getWidth() + wrap_size, getHeight() * (1.0f - end_percent));
  filter_response_path_.lineTo(getWidth() + wrap_size, getHeight() + wrap_size);
}

void FilterResponse::computeFilterCoefficients() {
  if (cutoff_slider_ == nullptr || resonance_slider_ == nullptr ||
      filter_blend_slider_ == nullptr || filter_shelf_slider_ == nullptr) {
    return;
  }

  mopo::StateVariableFilter::Shelves shelf = static_cast<mopo::StateVariableFilter::Shelves>(
      static_cast<int>(filter_shelf_slider_->getValue()));
  double frequency = mopo::utils::midiNoteToFrequency(cutoff_slider_->getValue());
  double resonance = mopo::utils::magnitudeToQ(resonance_slider_->getValue());
  double decibels = mopo::utils::interpolate(MIN_GAIN_DB, MAX_GAIN_DB,
                                             resonance_slider_->getValue());
  double gain = mopo::utils::dbToGain(decibels);

  if (style_ == mopo::StateVariableFilter::k24dB) {
    resonance = sqrt(resonance);
    gain = sqrt(gain);
  }

  if (style_ == mopo::StateVariableFilter::kShelf) {
    mopo::BiquadFilter::Type type = mopo::BiquadFilter::kLowShelf;
    if (shelf == mopo::StateVariableFilter::kBandShelf)
      type = mopo::BiquadFilter::kBandShelf;
    else if (shelf == mopo::StateVariableFilter::kHighShelf)
      type = mopo::BiquadFilter::kHighShelf;
    filter_shelf_.computeCoefficients(type, frequency, 1.0, gain);
  }
  else {
    filter_low_.computeCoefficients(mopo::BiquadFilter::kLowPass, frequency, resonance, 1.0);
    filter_band_.computeCoefficients(mopo::BiquadFilter::kBandPass, frequency, resonance, 1.0);
    filter_high_.computeCoefficients(mopo::BiquadFilter::kHighPass, frequency, resonance, 1.0);
  }
  resetResponsePath();
}

void FilterResponse::setFilterSettingsFromPosition(Point<int> position) {
  if (cutoff_slider_) {
    double percent = mopo::utils::clamp((1.0 * position.x) / getWidth(), 0.0, 1.0);
    double frequency = cutoff_slider_->proportionOfLengthToValue(percent);
    cutoff_slider_->setValue(frequency);
  }
  if (resonance_slider_) {
    double percent = mopo::utils::clamp(1.0 - (1.0 * position.y) / getHeight(), 0.0, 1.0);
    resonance_slider_->setValue(resonance_slider_->proportionOfLengthToValue(percent));
  }

  computeFilterCoefficients();
}

void FilterResponse::guiChanged(SynthSlider* slider) {
  computeFilterCoefficients();
  repaint();
}

void FilterResponse::setResonanceSlider(SynthSlider* slider) {
  resonance_slider_ = slider;
  resonance_slider_->addSliderListener(this);
  computeFilterCoefficients();
  repaint();
}

void FilterResponse::setCutoffSlider(SynthSlider* slider) {
  cutoff_slider_ = slider;
  cutoff_slider_->addSliderListener(this);
  computeFilterCoefficients();
  repaint();
}

void FilterResponse::setFilterBlendSlider(SynthSlider* slider) {
  filter_blend_slider_ = slider;
  filter_blend_slider_->addSliderListener(this);
  computeFilterCoefficients();
  repaint();
}

void FilterResponse::setFilterShelfSlider(SynthSlider* slider) {
  filter_shelf_slider_ = slider;
  filter_shelf_slider_->addSliderListener(this);
  computeFilterCoefficients();
  repaint();
}

void FilterResponse::setStyle(mopo::StateVariableFilter::Styles style) {
  if (style_ == style)
    return;

  style_ = style;
  computeFilterCoefficients();
  repaint();
}

void FilterResponse::setActive(bool active) {
  active_ = active;
  repaint();
}
