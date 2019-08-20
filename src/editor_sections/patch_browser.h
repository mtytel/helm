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
#ifndef PATCH_BROWSER_H
#define PATCH_BROWSER_H

#include "JuceHeader.h"
#include "delete_section.h"
#include "file_list_box_model.h"
#include "overlay.h"
#include "save_section.h"

class PatchBrowser : public Overlay,
                     public FileListBoxModel::Listener,
                     public TextEditor::Listener,
                     public KeyListener,
                     public Button::Listener,
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
    void textEditorEscapeKeyPressed(TextEditor& editor) override;

    void fileSaved(File saved_file) override;
    void fileDeleted(File deleted_file) override;

    void buttonClicked(Button* clicked_button) override;

    bool isPatchSelected();
    File getSelectedPatch();
    void jumpToPatch(int indices);
    void loadNextPatch();
    void loadPrevPatch();
    void externalPatchLoaded(File file);

    void setListener(PatchSelectedListener* listener) { listener_ = listener; }
    void setSaveSection(SaveSection* save_section);
    void setDeleteSection(DeleteSection* delete_section);

  private:
    bool loadFromFile(File& patch);
    void setPatchInfo(File& patch);
    void scanBanks();
    void scanFolders();
    void scanPatches();
    void scanAll();
    float getBanksWidth();
    float getFoldersWidth();
    float getPatchesWidth();
    float getPatchInfoWidth();

    ScopedPointer<ListBox> banks_view_;
    ScopedPointer<FileListBoxModel> banks_model_;

    ScopedPointer<ListBox> folders_view_;
    ScopedPointer<FileListBoxModel> folders_model_;

    ScopedPointer<ListBox> patches_view_;
    ScopedPointer<FileListBoxModel> patches_model_;

    ScopedPointer<TextEditor> search_box_;

    PatchSelectedListener* listener_;
    ScopedPointer<HyperlinkButton> cc_license_link_;
    ScopedPointer<HyperlinkButton> gpl_license_link_;

    SaveSection* save_section_;
    DeleteSection* delete_section_;
    ScopedPointer<TextButton> save_as_button_;
    ScopedPointer<TextButton> delete_patch_button_;
    ScopedPointer<TextButton> import_bank_button_;
    ScopedPointer<TextButton> export_bank_button_;

    ScopedPointer<TextButton> hide_button_;
    ScopedPointer<TextButton> done_button_;

    File external_patch_;
    String author_;
    String license_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchBrowser)
};

#endif // PATCH_BROWSER_H
