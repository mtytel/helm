/* Copyright 2013-2015 Matt Tytel
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
#ifndef PATCH_SELECTOR_H
#define PATCH_SELECTOR_H

#include "JuceHeader.h"
#include "patch_browser.h"
#include "synth_section.h"

class PatchSelector : public SynthSection {
  public:
    PatchSelector();
    ~PatchSelector();

    void paintBackground(Graphics& g) override;
    void resized() override;
    void buttonClicked(Button* buttonThatWasClicked) override;
    void setBrowser(PatchBrowser* browser) { browser_ = browser; }

  private:
    void refreshPatches();
    File getCurrentPatch();
    File getCurrentFolder();
    void loadFromFile(File& patch);

    int folder_index_;
    int patch_index_;

    String folder_text_;
    String patch_text_;

    ScopedPointer<TextButton> prev_folder_;
    ScopedPointer<TextButton> prev_patch_;
    ScopedPointer<TextButton> next_folder_;
    ScopedPointer<TextButton> next_patch_;
    ScopedPointer<TextButton> save_;
    ScopedPointer<TextButton> browse_;
    PatchBrowser* browser_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchSelector)
};

#endif // PATCH_SELECTOR_H
