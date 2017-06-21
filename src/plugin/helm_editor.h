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

#ifndef HELM_EDITOR_H
#define HELM_EDITOR_H

#include "JuceHeader.h"
#include "helm_plugin.h"
#include "full_interface.h"
#include "synth_gui_interface.h"

class HelmEditor : public AudioProcessorEditor, public SynthGuiInterface {
  public:
    HelmEditor(HelmPlugin&);

    // AudioProcessorEditor
    void paint(Graphics&) override;
    void resized() override;
    void visibilityChanged() override;
    void focusGained(FocusChangeType cause) override;
    void focusLost(FocusChangeType cause) override;
    void focusOfChildComponentChanged(FocusChangeType cause) override;
    void parentHierarchyChanged() override;

    // SynthGuiInterface
    void updateFullGui() override;

  private:
    void checkAnimate();

    HelmPlugin& helm_;
    bool was_animating_;
    ComponentBoundsConstrainer constrainer_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HelmEditor)
};

#endif // HELM_EDITOR_H
