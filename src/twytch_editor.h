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
#include "twytch.h"
#include "twytch_look_and_feel.h"
#include "full_interface.h"
#include "save_load_manager.h"
#include "value_change_manager.h"

class TwytchEditor : public AudioProcessorEditor,
                     public ValueChangeManager,
                     public SaveLoadManager {
  public:
    TwytchEditor(Twytch&);
    ~TwytchEditor();

    // AudioProcessorEditor
    void paint(Graphics&) override;
    void resized() override;

    // ValueChangeManager
    void valueChanged(std::string name, mopo::mopo_float value) override;
    void connectModulation(mopo::ModulationConnection* connection) override;
    void disconnectModulation(mopo::ModulationConnection* connection) override;
    const mopo::Processor::Output* getModulationSourceOutput(std::string name) override;
    const mopo::Processor::Output* getModulationTotal(std::string name) override;
    void enterCriticalSection() { twytch_.getCallbackLock().enter(); }
    void exitCriticalSection() { twytch_.getCallbackLock().exit(); }

    // SaveLoadManager
    var stateToVar() override;
    void varToState(var state) override;

  private:
    Twytch& twytch_;
    TwytchLookAndFeel look_and_feel_;
    mopo::control_map controls_;

    FullInterface* gui_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TwytchEditor)
};

#endif // TWYTCH_EDITOR_H
