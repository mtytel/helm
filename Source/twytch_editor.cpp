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

#include "twytch.h"
#include "twytch_editor.h"

#define WIDTH 1000
#define HEIGHT 800

TwytchEditor::TwytchEditor(Twytch& twytch) : AudioProcessorEditor(&twytch), twytch_(twytch) {
  mopo::control_map controls = twytch.getSynth()->getControls();
  setLookAndFeel(&look_and_feel_);

  gui_ = new FullInterface();
  gui_->addControls(controls);
  gui_->setSynth(twytch.getSynth());
  gui_->setOutputMemory(twytch.getOutputMemory());
  addAndMakeVisible(gui_);
  setSize(WIDTH, HEIGHT);
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