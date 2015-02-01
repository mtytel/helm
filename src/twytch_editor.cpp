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

#include "twytch.h"
#include "twytch_common.h"

#define WIDTH 1000
#define HEIGHT 800

TwytchEditor::TwytchEditor(Twytch& twytch) : AudioProcessorEditor(&twytch), twytch_(twytch) {
  controls_ = twytch.getSynth()->getControls();
  setLookAndFeel(&look_and_feel_);

  gui_ = new FullInterface(controls_);
  gui_->setOutputMemory(twytch.getOutputMemory());
  addAndMakeVisible(gui_);
  setSize(WIDTH, HEIGHT);
  repaint();
}

TwytchEditor::~TwytchEditor() {
  delete gui_;
}

void TwytchEditor::paint(Graphics& g) {
  setSize(WIDTH, HEIGHT);
  g.fillAll(Colours::white);
}

void TwytchEditor::resized() {
  gui_->setBounds(0, 0, getWidth(), getHeight());
}

void TwytchEditor::valueChanged(std::string name, mopo::mopo_float value) {
  MOPO_ASSERT(controls_.count(name));
  controls_[name]->set(value);
}

void TwytchEditor::connectModulation(mopo::ModulationConnection* connection) {
  ScopedLock(twytch_.getCallbackLock());
  twytch_.getSynth()->connectModulation(connection);
}

void TwytchEditor::disconnectModulation(mopo::ModulationConnection* connection) {
  ScopedLock(twytch_.getCallbackLock());
  twytch_.getSynth()->disconnectModulation(connection);
}
