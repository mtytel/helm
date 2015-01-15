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

TwytchEditor::TwytchEditor(Twytch& twytch) : AudioProcessorEditor(&twytch), twytch_(twytch) {
  mopo::control_map controls = twytch.getSynth()->getControls();
  setLookAndFeel(&look_and_feel_);

  synth_editor_ = new SynthesisEditor();
  synth_editor_->addControls(controls);
  addAndMakeVisible(synth_editor_);
  setSize(1000, 700);
}

TwytchEditor::~TwytchEditor() {
  delete synth_editor_;
}

void TwytchEditor::paint(Graphics& g) {
  setSize(1000, 700);
  g.fillAll(Colours::white);
}

void TwytchEditor::resized() {
  synth_editor_->setBounds(0, 0, getWidth(), getHeight());
}