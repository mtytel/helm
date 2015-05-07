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

#include "twytch_editor.h"

#include "default_look_and_feel.h"
#include "twytch_common.h"
#include "twytch_plugin.h"

#define WIDTH 754
#define HEIGHT 740

TwytchEditor::TwytchEditor(TwytchPlugin& twytch) : AudioProcessorEditor(&twytch), twytch_(twytch) {
  setSynth(twytch_.getSynth());
  setLookAndFeel(DefaultLookAndFeel::instance());

  gui_ = new FullInterface(twytch.getSynth()->getControls(),
                           twytch.getSynth()->getModulationSources(),
                           twytch.getSynth()->getMonoModulations(),
                           twytch.getSynth()->getPolyModulations());
  gui_->setOutputMemory(twytch.getOutputMemory());
  addAndMakeVisible(gui_);
  setSize(WIDTH, HEIGHT);
  repaint();
}

void TwytchEditor::paint(Graphics& g) {
  setSize(WIDTH, HEIGHT);
  g.fillAll(Colours::white);
}

void TwytchEditor::resized() {
  gui_->setBounds(0, 0, getWidth(), getHeight());
}

void TwytchEditor::updateFullGui() {
  gui_->setAllValues(controls_);
}

void TwytchEditor::updateGuiControl(std::string name, mopo::mopo_float value) {
  gui_->setValue(name, value);
}