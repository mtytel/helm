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

#include "helm_editor.h"

#include "default_look_and_feel.h"
#include "helm_common.h"
#include "helm_plugin.h"

#define WIDTH 996
#define HEIGHT 670

HelmEditor::HelmEditor(HelmPlugin& helm) : AudioProcessorEditor(&helm), helm_(helm) {
  setSynth(helm_.getSynth());
  setLookAndFeel(DefaultLookAndFeel::instance());

  gui_ = new FullInterface(helm.getSynth()->getControls(),
                           helm.getSynth()->getModulationSources(),
                           helm.getSynth()->getMonoModulations(),
                           helm.getSynth()->getPolyModulations());
  gui_->setOutputMemory(helm.getOutputMemory());
  addAndMakeVisible(gui_);
  setSize(WIDTH, HEIGHT);
  repaint();
}

void HelmEditor::paint(Graphics& g) {
  setSize(WIDTH, HEIGHT);
  g.fillAll(Colours::white);
}

void HelmEditor::resized() {
  gui_->setBounds(0, 0, getWidth(), getHeight());
}

void HelmEditor::updateFullGui() {
  gui_->setAllValues(controls_);
}

void HelmEditor::updateGuiControl(std::string name, mopo::mopo_float value) {
  gui_->setValue(name, value, NotificationType::dontSendNotification);
}

void HelmEditor::beginChangeGesture(std::string name) {
  helm_.beginChangeGesture(name);
}

void HelmEditor::endChangeGesture(std::string name) {
  helm_.endChangeGesture(name);
}

void HelmEditor::setValueNotifyHost(std::string name, mopo::mopo_float value) {
  helm_.setValueNotifyHost(name, value);
}
