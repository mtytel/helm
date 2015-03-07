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

#ifndef SYNTH_GUI_INTERFACE_H
#define SYNTH_GUI_INTERFACE_H

#include "twytch_common.h"
#include "value.h"
#include <string>

class SynthGuiInterface {
  public:
    virtual ~SynthGuiInterface() { }

    virtual void valueChanged(std::string name, mopo::mopo_float value) = 0;
    virtual void connectModulation(mopo::ModulationConnection* connection) = 0;
    virtual void disconnectModulation(mopo::ModulationConnection* connection) = 0;
    virtual int getNumActiveVoices() = 0;
    virtual void enterCriticalSection() = 0;
    virtual void exitCriticalSection() = 0;
};

#endif // SYNTH_GUI_INTERFACE_H
