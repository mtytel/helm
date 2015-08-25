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
        virtual void selectedFilesChanged(FileListBoxModel* model) = 0;
    };

    FileListBoxModel() : listener_(nullptr), sort_ascending_(true) { }

    int getNumRows() override;
    void paintListBoxItem(int row_number, Graphics& g,
                          int width, int height, bool selected) override;
    void selectedRowsChanged(int last_selected_row) override;

    File getFileAtRow(int row) { return files_[row]; }
    void rescanFiles(const Array<File>& folders, bool find_files = false);
    void setListener(FileListBoxModelListener* listener) { listener_ = listener; }

  private:
    Array<File> files_;
    FileListBoxModelListener* listener_;
    bool sort_ascending_;
};

class PatchBrowser : public Component, public FileListBoxModel::FileListBoxModelListener {
  public:
    PatchBrowser();
    ~PatchBrowser();

    void paint(Graphics& g) override;
    void resized() override;
    void mouseUp(const MouseEvent& e) override;

    void selectedFilesChanged(FileListBoxModel* model) override;

  private:
    void loadFromFile(File& patch);

    ScopedPointer<ListBox> banks_view_;
    ScopedPointer<FileListBoxModel> banks_model_;

    ScopedPointer<ListBox> folders_view_;
    ScopedPointer<FileListBoxModel> folders_model_;

    ScopedPointer<ListBox> patches_view_;
    ScopedPointer<FileListBoxModel> patches_model_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchBrowser)
};

#endif // PATCH_BROWSER_H
