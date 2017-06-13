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

#pragma once
#ifndef MODULATION_BUTTON_H
#define MODULATION_BUTTON_H

#include "JuceHeader.h"

namespace mopo {
  struct ModulationConnection;
} // namespace mopo

class ModulationButton : public ToggleButton {
  public:
    class ModulationDisconnectListener {
      public:
        virtual ~ModulationDisconnectListener() { }

        virtual void modulationDisconnected(mopo::ModulationConnection* connection, bool last) = 0;
    };
  
    ModulationButton(String name);

    void mouseDown(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void addDisconnectListener(ModulationDisconnectListener* listener);
    void disconnectIndex(int index);

  private:
    void disconnectModulation(mopo::ModulationConnection* connection);

    std::vector<ModulationDisconnectListener*> listeners_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationButton)
};

#endif // MODULATION_BUTTON_H
