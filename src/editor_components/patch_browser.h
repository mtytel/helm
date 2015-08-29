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
#ifndef PATCH_BROWSER_H
#define PATCH_BROWSER_H

#include "JuceHeader.h"
#include "synth_section.h"

class FileListBoxModel : public ListBoxModel {
  public:
    class FileListBoxModelListener {
      public:
        virtual ~FileListBoxModelListener() { }

        virtual void selectedFilesChanged(FileListBoxModel* model) = 0;
    };

    FileListBoxModel() : listener_(nullptr), sort_ascending_(true) { }

    int getNumRows() override;
    void paintListBoxItem(int row_number, Graphics& g,
                          int width, int height, bool selected) override;
    void selectedRowsChanged(int last_selected_row) override;

    void rescanFiles(const Array<File>& folders, String search = "*", bool find_files = false);
    File getFileAtRow(int row) { return files_[row]; }
    void setListener(FileListBoxModelListener* listener) { listener_ = listener; }
    Array<File> getAllFiles() { return files_; }

  private:
    Array<File> files_;
    FileListBoxModelListener* listener_;
    bool sort_ascending_;
};

class PatchBrowser : public Component,
                     public FileListBoxModel::FileListBoxModelListener,
                     public TextEditor::Listener,
                     public KeyListener {
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
    bool keyStateChanged(bool isKeyDown, Component *origin) override;
    void visibilityChanged() override;

    void selectedFilesChanged(FileListBoxModel* model) override;
    void textEditorTextChanged(TextEditor& editor) override;

    bool isPatchSelected() { return patches_view_->getSelectedRows().size(); }
    File getSelectedPatch();
    void loadNextPatch();
    void loadPrevPatch();

    void setListener(PatchSelectedListener* listener) { listener_ = listener; }

  private:
    void loadFromFile(File& patch);
    void scanFolders();
    void scanPatches();

    ScopedPointer<ListBox> banks_view_;
    ScopedPointer<FileListBoxModel> banks_model_;

    ScopedPointer<ListBox> folders_view_;
    ScopedPointer<FileListBoxModel> folders_model_;

    ScopedPointer<ListBox> patches_view_;
    ScopedPointer<FileListBoxModel> patches_model_;

    ScopedPointer<TextEditor> search_box_;

    PatchSelectedListener* listener_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchBrowser)
};

#endif // PATCH_BROWSER_H
