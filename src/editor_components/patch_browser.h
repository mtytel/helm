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

#pragma once
#ifndef PATCH_BROWSER_H
#define PATCH_BROWSER_H

#include "JuceHeader.h"
#include "synth_section.h"

class PatchBrowser : public SynthSection {
  public:
    PatchBrowser();
    ~PatchBrowser();

    void paintBackground(Graphics& g) override;
    void resized() override;
    void buttonClicked(Button* buttonThatWasClicked) override;

  private:
    void refreshPatches();
    File getSystemPatchDirectory();
    File getUserPatchDirectory();
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchBrowser)
};

#endif // PATCH_BROWSER_H
