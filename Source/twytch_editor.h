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

#ifndef TWYTCH_EDITOR_H
#define TWYTCH_EDITOR_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "twytch.h"
#include "twytch_look_and_feel.h"
#include "synthesis_editor.h"

class TwytchEditor : public AudioProcessorEditor {
public:
  TwytchEditor(Twytch&);
  ~TwytchEditor();

  void paint(Graphics&) override;
  void resized() override;

private:
  Twytch& twytch_;
  TwytchLookAndFeel look_and_feel_;

  SynthesisEditor* synth_editor_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TwytchEditor)
};

#endif // TWYTCH_EDITOR_H
