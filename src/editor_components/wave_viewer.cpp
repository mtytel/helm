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

#include "wave_viewer.h"

#include "colors.h"
#include "synth_gui_interface.h"

#define GRID_CELL_WIDTH 8
#define FRAMES_PER_SECOND 30
#define PADDING 5.0f
#define MARKER_WIDTH 6.0f
#define NOISE_RESOLUTION 6

namespace {
  static const float random_values[NOISE_RESOLUTION] = {0.3f, 0.9f, -0.9f, -0.2f, -0.5f, 0.7f };
} // namespace

WaveViewer::WaveViewer(int resolution) {
  wave_slider_ = nullptr;
  amplitude_slider_ = nullptr;
  resolution_ = resolution;
  wave_phase_ = nullptr;
  wave_amp_ = nullptr;
  is_control_rate_ = false;
  phase_ = -1.0f;
  amp_ = 0.0;
  setOpaque(true);
}

WaveViewer::~WaveViewer() { }

void WaveViewer::paint(juce::Graphics &g) {
  g.drawImageWithin(background_,
                    0, 0, getWidth(), getHeight(), RectanglePlacement());

  if (wave_phase_) {
    if (phase_ >= 0.0 && phase_ < 1.0) {
      float x = phaseToX(phase_);
      g.setColour(Colour(0x33ffffff));
      g.fillRect(x - 0.5f, 0.0f, 1.0f, (float)getHeight());

      float y = PADDING + (getHeight() - 2 * PADDING) * (1.0f - amp_) / 2.0f;

      g.setColour(Colors::modulation);
      g.fillEllipse(x - MARKER_WIDTH / 2.0f, y - MARKER_WIDTH / 2.0f,
                    MARKER_WIDTH, MARKER_WIDTH);
      g.setColour(Colour(0xff000000));
      g.fillEllipse(x - MARKER_WIDTH / 4.0f, y - MARKER_WIDTH / 4.0f,
                    MARKER_WIDTH / 2.0f, MARKER_WIDTH / 2.0f);
    }
  }
}

void WaveViewer::paintBackground(Graphics& g) {
  static const DropShadow shadow(Colour(0xbb000000), 5, Point<int>(0, 0));

  g.fillAll(Colour(0xff424242));

  g.setColour(Colour(0xff4a4a4a));
  for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
    g.drawLine(x, 0, x, getHeight());
  for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
    g.drawLine(0, y, getWidth(), y);

  shadow.drawForPath(g, wave_path_);

  g.setColour(Colors::graph_fill);
  g.fillPath(wave_path_);

  if (is_control_rate_)
    g.setColour(Colors::modulation);
  else
    g.setColour(Colors::audio);

  float line_width = 1.5f * getRatio();
  PathStrokeType stroke(line_width, PathStrokeType::beveled, PathStrokeType::rounded);
  g.strokePath(wave_path_, stroke);
}

void WaveViewer::resized() {
  const Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  float scale = display.scale;
  background_ = Image(Image::RGB, scale * getWidth(), scale * getHeight(), true);
  resetWavePath();
}

void WaveViewer::mouseDown(const MouseEvent& e) {
  if (wave_slider_) {
    int current_value = wave_slider_->getValue();
    if (e.mods.isRightButtonDown())
      current_value = current_value + wave_slider_->getMaximum();
    else
      current_value = current_value + 1;
    wave_slider_->setValue(current_value % static_cast<int>(wave_slider_->getMaximum() + 1));

    resetWavePath();
  }
}

void WaveViewer::timerCallback() {
  if (wave_phase_) {
    float phase = wave_phase_->buffer[0];
    amp_ = wave_amp_->buffer[0];
    if (phase != phase_) {
      float last_x = phaseToX(phase_);
      float new_x = phaseToX(phase);
      phase_ = phase;
      repaint(last_x - MARKER_WIDTH / 2.0f - 1, 0.0, MARKER_WIDTH + 2, getHeight());
      repaint(new_x - MARKER_WIDTH / 2.0f - 1, 0.0, MARKER_WIDTH + 2, getHeight());
    }
  }
}

void WaveViewer::setWaveSlider(Slider* slider) {
  if (wave_slider_)
    wave_slider_->removeListener(this);
  wave_slider_ = slider;
  wave_slider_->addListener(this);
  resetWavePath();
}

void WaveViewer::setAmplitudeSlider(Slider* slider) {
  if (amplitude_slider_)
    amplitude_slider_->removeListener(this);
  amplitude_slider_ = slider;
  amplitude_slider_->addListener(this);
  resetWavePath();
}

void WaveViewer::drawRandom() {
  float amplitude = amplitude_slider_ ? amplitude_slider_->getValue() : 1.0f;
  float draw_width = getWidth();
  float padding = getRatio() * PADDING;
  float draw_height = getHeight() - 2.0f * padding;

  wave_path_.startNewSubPath(0, getHeight() / 2.0f);
  for (int i = 0; i < NOISE_RESOLUTION; ++i) {
    float t1 = (1.0f * i) / NOISE_RESOLUTION;
    float t2 = (1.0f + i) / NOISE_RESOLUTION;
    float val = amplitude * random_values[i];
    wave_path_.lineTo(t1 * draw_width, padding + draw_height * ((1.0f - val) / 2.0f));
    wave_path_.lineTo(t2 * draw_width, padding + draw_height * ((1.0f - val) / 2.0f));
  }

  wave_path_.lineTo(getWidth(), getHeight() / 2.0f);
}

void WaveViewer::drawSmoothRandom() {
  float amplitude = amplitude_slider_ ? amplitude_slider_->getValue() : 1.0f;
  float draw_width = getWidth();
  float padding = getRatio() * PADDING;
  float draw_height = getHeight() - 2.0f * padding;

  float start_val = amplitude * random_values[0];
  wave_path_.startNewSubPath(-50, getHeight() / 2.0f);
  wave_path_.lineTo(0, padding + draw_height * ((1.0f - start_val) / 2.0f));
  for (int i = 1; i < resolution_ - 1; ++i) {
    float t = (1.0f * i) / resolution_;
    float phase = t * (NOISE_RESOLUTION - 1);
    int index = (int)phase;
    phase = mopo::PI * (phase - index);
    float val = amplitude * mopo::utils::interpolate(random_values[index],
                                                     random_values[index + 1],
                                                     0.5f - cosf(phase) / 2.0f);
    wave_path_.lineTo(t * draw_width, padding + draw_height * ((1.0f - val) / 2.0f));
  }

  float end_val = amplitude * random_values[NOISE_RESOLUTION - 1];
  wave_path_.lineTo(getWidth(), padding + draw_height * ((1.0f - end_val) / 2.0f));
  wave_path_.lineTo(getWidth() + 50, getHeight() / 2.0f);

}

void WaveViewer::resetWavePath() {
  if (!background_.isValid())
    return;

  wave_path_.clear();

  if (wave_slider_ == nullptr)
    return;

  float amplitude = amplitude_slider_ ? amplitude_slider_->getValue() : 1.0f;
  float draw_width = getWidth();
  float padding = getRatio() * PADDING;
  float draw_height = getHeight() - 2.0f * padding;

  mopo::Wave::Type type = static_cast<mopo::Wave::Type>(static_cast<int>(wave_slider_->getValue()));

  if (type < mopo::Wave::kWhiteNoise) {
    wave_path_.startNewSubPath(0, getHeight() / 2.0f);
    for (int i = 1; i < resolution_ - 1; ++i) {
      float t = (1.0f * i) / resolution_;
      float val = amplitude * mopo::Wave::wave(type, t);
      wave_path_.lineTo(t * draw_width, padding + draw_height * ((1.0f - val) / 2.0f));
    }

    wave_path_.lineTo(getWidth(), getHeight() / 2.0f);
  }
  else if (type == mopo::Wave::kWhiteNoise)
    drawRandom();
  else
    drawSmoothRandom();

  const Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  float scale = display.scale;
  Graphics g(background_);
  g.addTransform(AffineTransform::scale(scale, scale));
  paintBackground(g);

  repaint();
}

void WaveViewer::sliderValueChanged(Slider* sliderThatWasMoved) {
  resetWavePath();
}

void WaveViewer::showRealtimeFeedback(bool show_feedback) {
  if (show_feedback) {
    if (wave_phase_ == nullptr) {
      SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
      if (parent) {
        wave_amp_ = parent->getSynth()->getModSource(getName().toStdString());
        wave_phase_ = parent->getSynth()->getModSource(getName().toStdString() + "_phase");
        startTimerHz(FRAMES_PER_SECOND);
      }
    }
  }
  else {
    wave_phase_ = nullptr;
    stopTimer();
    repaint();
  }
}

float WaveViewer::phaseToX(float phase) {
  return phase * getWidth();
}

float WaveViewer::getRatio() {
  return getHeight() / 80.0f;
}
