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

#ifndef SYNTH_GUI_INTERFACE_H
#define SYNTH_GUI_INTERFACE_H

#include "full_interface.h"
#include "synth_base.h"

class SynthGuiInterface {
  public:
    SynthGuiInterface(SynthBase* synth, bool use_gui = true);
    virtual ~SynthGuiInterface() { }

    virtual AudioDeviceManager* getAudioDeviceManager() { return nullptr; }

    SynthBase* getSynth() { return synth_; }
    virtual void updateFullGui();
    virtual void updateGuiControl(const std::string& name, mopo::mopo_float value);
    mopo::mopo_float getControlValue(const std::string& name);
    void setFocus();
    void notifyChange();
    void notifyFresh();
    void externalPatchLoaded(File patch);
    void setGuiSize(int width, int height);

  protected:
    SynthBase* synth_;
    ScopedPointer<FullInterface> gui_;
};

#endif // SYNTH_GUI_INTERFACE_H
