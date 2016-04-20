/* Copyright 2013-2016 Matt Tytel
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
#ifndef PATCH_BROWSER_H
#define PATCH_BROWSER_H

#include "JuceHeader.h"
#include "delete_section.h"
#include "file_list_box_model.h"
#include "save_section.h"

class PatchBrowser : public Component,
                     public FileListBoxModel::Listener,
                     public TextEditor::Listener,
                     public KeyListener,
                     public ButtonListener,
                     public SaveSection::Listener,
                     public DeleteSection::Listener {
  public:
    class PatchSelectedListener {
      public:
        virtual ~PatchSelectedListener() { }

        virtual void newPatchSelected(File patch) = 0;
    };

    PatchBrowser();
    ~PatchBrowser();

    void paint(Graphics& g) override;
    void resized() override;
    void mouseUp(const MouseEvent& e) override;
    bool keyPressed(const KeyPress &key, Component *origin) override;
    bool keyStateChanged(bool is_key_down, Component *origin) override;
    void visibilityChanged() override;

    void selectedFilesChanged(FileListBoxModel* model) override;
    void textEditorTextChanged(TextEditor& editor) override;

    void fileSaved(File saved_file) override;
    void fileDeleted(File deleted_file) override;

    void buttonClicked(Button* clicked_button) override;

    bool isPatchSelected() { return patches_view_->getSelectedRows().size(); }
    File getSelectedPatch();
    void loadNextPatch();
    void loadPrevPatch();

    void setListener(PatchSelectedListener* listener) { listener_ = listener; }
    void setSaveSection(SaveSection* save_section);
    void setDeleteSection(DeleteSection* delete_section);

  private:
    void loadFromFile(File& patch);
    void scanFolders();
    void scanPatches();
    float getNarrowWidth();
    float getWideWidth();

    ScopedPointer<ListBox> banks_view_;
    ScopedPointer<FileListBoxModel> banks_model_;

    ScopedPointer<ListBox> folders_view_;
    ScopedPointer<FileListBoxModel> folders_model_;

    ScopedPointer<ListBox> patches_view_;
    ScopedPointer<FileListBoxModel> patches_model_;

    ScopedPointer<TextEditor> search_box_;

    PatchSelectedListener* listener_;
    ScopedPointer<HyperlinkButton> license_link_;

    SaveSection* save_section_;
    DeleteSection* delete_section_;
    ScopedPointer<TextButton> save_as_button_;
    ScopedPointer<TextButton> delete_patch_button_;

    ScopedPointer<TextButton> hide_button_;

    String author_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchBrowser)
};

#endif // PATCH_BROWSER_H
