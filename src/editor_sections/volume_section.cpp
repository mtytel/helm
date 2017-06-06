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

#include "volume_section.h"
#include "synth_slider.h"

VolumeSection::VolumeSection(String name) : SynthSection(name) {
  addSlider(volume_ = new SynthSlider("volume"));
  addOpenGLComponent(peak_meter_left_ = new OpenGLPeakMeter(true));
  addOpenGLComponent(peak_meter_right_ = new OpenGLPeakMeter(false));
  volume_->setSliderStyle(Slider::LinearBar);
  volume_->setPopupPlacement(BubbleComponent::below, 0);
}

VolumeSection::~VolumeSection() {
  volume_ = nullptr;
}

void VolumeSection::resized() {
  int title_width = getTitleWidth();
  int height = getHeight() - title_width;
  int meter_height = height / 4;
  int volume_height = height - 2 * meter_height;
  peak_meter_left_->setBounds(0, title_width, getWidth(), meter_height);
  peak_meter_right_->setBounds(0, title_width + meter_height, getWidth(), meter_height);
  volume_->setBounds(0, title_width + 2 * meter_height, getWidth(), volume_height);

  SynthSection::resized();
}
