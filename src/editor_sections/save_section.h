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
#ifndef SAVE_SECTION_H
#define SAVE_SECTION_H

#include "JuceHeader.h"
#include "file_list_box_model.h"
#include "overlay.h"

class SaveSection : public Overlay, public TextEditor::Listener,
                    public FileListBoxModel::Listener, public Button::Listener {
  public:
    class Listener {
      public:
        virtual ~Listener() { }

        virtual void fileSaved(File save_file) = 0;
    };

    SaveSection(String name);
    ~SaveSection() { }
    void paint(Graphics& g) override;
    void resized() override;
    void visibilityChanged() override;

    void textEditorReturnKeyPressed(TextEditor& editor) override;
    void selectedFilesChanged(FileListBoxModel* list_box) override;
    void buttonClicked(Button* clicked_button) override;
    void mouseUp(const MouseEvent& e) override;

    Rectangle<int> getSaveRect();
    void setSaveRect(Rectangle<int> rectangle) { active_rect_ = rectangle; }

    void setListener(Listener* listener) { listener_ = listener; }

  private:
    void save();
    void createNewBank();
    void createNewFolder();
    void rescanBanks();
    void rescanFolders();

    ScopedPointer<TextEditor> patch_name_;
    ScopedPointer<TextEditor> author_;
    ScopedPointer<TextEditor> add_bank_name_;
    ScopedPointer<TextEditor> add_folder_name_;

    ScopedPointer<ListBox> banks_view_;
    ScopedPointer<ListBox> folders_view_;
    ScopedPointer<FileListBoxModel> banks_model_;
    ScopedPointer<FileListBoxModel> folders_model_;

    ScopedPointer<TextButton> save_button_;
    ScopedPointer<TextButton> cancel_button_;
    ScopedPointer<TextButton> add_bank_button_;
    ScopedPointer<TextButton> add_folder_button_;

    Rectangle<int> active_rect_;

    Listener* listener_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SaveSection)
};

#endif // SAVE_SECTION_H
