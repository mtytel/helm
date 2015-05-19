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

#include "wave_viewer.h"

#include "synth_gui_interface.h"

#define GRID_CELL_WIDTH 8
#define FRAMES_PER_SECOND 24
#define PADDING 5.0f
#define MARKER_WIDTH 4.0f

WaveViewer::WaveViewer(int resolution) {
    wave_slider_ = nullptr;
    amplitude_slider_ = nullptr;
    resolution_ = resolution;
    wave_state_ = nullptr;
    phase_ = -1.0f;
    setOpaque(true);
}

WaveViewer::~WaveViewer() { }

void WaveViewer::paint(Graphics& g) {
    static const DropShadow shadow(Colour(0xbb000000), 5, Point<int>(0, 0));

    g.fillAll(Colour(0xff424242));

    g.setColour(Colour(0xff4a4a4a));
    for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
        g.drawLine(x, 0, x, getHeight());
    for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
        g.drawLine(0, y, getWidth(), y);

    shadow.drawForPath(g, wave_path_);

    g.setColour(Colour(0xff565656));
    g.fillPath(wave_path_);
    g.setColour(Colour(0xff03a9f4));
    g.strokePath(wave_path_, PathStrokeType(1.5f, PathStrokeType::beveled, PathStrokeType::rounded));

    if (wave_state_) {
        float amplitude = amplitude_slider_ ? amplitude_slider_->getValue() : 1.0f;

        if (phase_ >= 0.0 && phase_ < 1.0) {
            float x = phaseToX(phase_);
            g.setColour(Colour(0x66ffffff));
            g.fillRect(x, 0.0f, 1.0f, (float)getHeight());

            mopo::Wave::Type type = static_cast<mopo::Wave::Type>(static_cast<int>(wave_slider_->getValue()));
            float value = amplitude * mopo::Wave::wave(type, phase_);
            float y = PADDING + (getHeight() - 2 * PADDING) * (1.0f - value) / 2.0f;
            g.setColour(Colour(0xffffffff));
            g.fillEllipse(x - MARKER_WIDTH / 2.0f, y - MARKER_WIDTH / 2.0f,
                          MARKER_WIDTH, MARKER_WIDTH);
        }
    }
}

void WaveViewer::resized() {
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
    if (wave_state_) {
        float phase = wave_state_->buffer[0];
        if (phase != phase_) {
            float last_x = phaseToX(phase_);
            float new_x = phaseToX(phase);
            phase_ = phase;
            repaint(last_x - MARKER_WIDTH / 2.0f, 0.0, MARKER_WIDTH, getHeight());
            repaint(new_x - MARKER_WIDTH / 2.0f, 0.0, MARKER_WIDTH, getHeight());
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

void WaveViewer::resetWavePath() {
    wave_path_.clear();

    if (wave_slider_ == nullptr)
        return;

    float amplitude = amplitude_slider_ ? amplitude_slider_->getValue() : 1.0f;
    float draw_width = getWidth() - 2.0f * PADDING;
    float draw_height = getHeight() - 2.0f * PADDING;

    wave_path_.startNewSubPath(PADDING, getHeight() / 2.0f);
    mopo::Wave::Type type = static_cast<mopo::Wave::Type>(static_cast<int>(wave_slider_->getValue()));
    for (int i = 1; i < resolution_ - 1; ++i) {
        float t = (1.0f * i) / resolution_;
        float val = amplitude * mopo::Wave::wave(type, t);
        wave_path_.lineTo(PADDING + t * draw_width, PADDING + draw_height * ((1.0f - val) / 2.0f));
    }

    wave_path_.lineTo(getWidth() - PADDING, getHeight() / 2.0f);
    repaint();
}

void WaveViewer::sliderValueChanged(Slider* sliderThatWasMoved) {
    resetWavePath();
}

void WaveViewer::showRealtimeFeedback() {
    if (wave_state_ == nullptr) {
        SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
        if (parent) {
            wave_state_ = parent->getModSource(getName().toStdString());
            // startTimerHz(FRAMES_PER_SECOND);
        }
    }
}

float WaveViewer::phaseToX(float phase) {
    return PADDING + phase * (getWidth() - 2 * PADDING);
}
