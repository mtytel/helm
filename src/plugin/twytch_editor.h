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

#include "JuceHeader.h"
#include "twytch_plugin.h"
#include "twytch_look_and_feel.h"
#include "full_interface.h"
#include "save_load_manager.h"
#include "synth_gui_interface.h"

class TwytchEditor : public AudioProcessorEditor, public SynthGuiInterface {
  public:
    TwytchEditor(TwytchPlugin&);

    // AudioProcessorEditor
    void paint(Graphics&) override;
    void resized() override;

    // SynthGuiInterface
    mopo::TwytchEngine* getSynth() override { return twytch_.getSynth(); }
    const CriticalSection& getCriticalSection() override { return twytch_.getCallbackLock(); }
    MidiManager* getMidiManager() override { return twytch_.getMidiManager(); }
    void updateFullGui() override;

  private:
    TwytchPlugin& twytch_;
    TwytchLookAndFeel look_and_feel_;

    ScopedPointer<FullInterface> gui_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TwytchEditor)
};

#endif // TWYTCH_EDITOR_H
