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

#include "save_section.h"

#include "colors.h"
#include "fonts.h"
#include "helm_common.h"
#include "load_save.h"
#include "synth_gui_interface.h"
#include "text_look_and_feel.h"

#define SAVE_WIDTH 420
#define SAVE_HEIGHT 420
#define TEXT_EDITOR_HEIGHT 24
#define BUTTON_HEIGHT 30
#define ADD_FOLDER_HEIGHT 20
#define DIVISION 110
#define PADDING_X 25
#define PADDING_Y 15

SaveSection::SaveSection(String name) : Overlay(name) {
  listener_ = nullptr;
  banks_model_ = new FileListBoxModel();
  banks_model_->setListener(this);
  folders_model_ = new FileListBoxModel();
  banks_view_ = new ListBox("banks", banks_model_);
  folders_view_ = new ListBox("folders", folders_model_);
  rescanFolders();
  banks_view_->setColour(ListBox::backgroundColourId, Colour(0xff323232));
  folders_view_->setColour(ListBox::backgroundColourId, Colour(0xff323232));
  addAndMakeVisible(banks_view_);
  addAndMakeVisible(folders_view_);

  patch_name_ = new TextEditor("Patch Name");
  patch_name_->addListener(this);
  patch_name_->setTextToShowWhenEmpty(TRANS("Patch Name"), Colour(0xff777777));
  patch_name_->setFont(Fonts::instance()->monospace().withPointHeight(16.0f));
  patch_name_->setColour(CaretComponent::caretColourId, Colour(0xff888888));
  patch_name_->setColour(TextEditor::textColourId, Colors::audio);
  patch_name_->setColour(TextEditor::highlightedTextColourId, Colour(0xff03a9f4));
  patch_name_->setColour(TextEditor::highlightColourId, Colour(0xff888888));
  patch_name_->setColour(TextEditor::backgroundColourId, Colour(0xff323232));
  patch_name_->setColour(TextEditor::outlineColourId, Colour(0xff888888));
  patch_name_->setColour(TextEditor::focusedOutlineColourId, Colour(0xff888888));
  addAndMakeVisible(patch_name_);

  author_ = new TextEditor("Author");
  author_->addListener(this);
  author_->setTextToShowWhenEmpty(TRANS("Author"), Colour(0xff777777));
  author_->setFont(Fonts::instance()->monospace().withPointHeight(16.0f));
  author_->setColour(CaretComponent::caretColourId, Colour(0xff888888));
  author_->setColour(TextEditor::textColourId, Colour(0xffcccccc));
  author_->setColour(TextEditor::highlightedTextColourId, Colour(0xffcccccc));
  author_->setColour(TextEditor::highlightColourId, Colour(0xff888888));
  author_->setColour(TextEditor::backgroundColourId, Colour(0xff323232));
  author_->setColour(TextEditor::outlineColourId, Colour(0xff888888));
  author_->setColour(TextEditor::focusedOutlineColourId, Colour(0xff888888));
  addAndMakeVisible(author_);

  add_bank_name_ = new TextEditor("Add Bank");
  add_bank_name_->addListener(this);
  add_bank_name_->setTextToShowWhenEmpty(TRANS("New Bank"), Colour(0xff777777));
  add_bank_name_->setFont(Fonts::instance()->monospace().withPointHeight(12.0f));
  add_bank_name_->setColour(CaretComponent::caretColourId, Colour(0xff888888));
  add_bank_name_->setColour(TextEditor::textColourId, Colour(0xffcccccc));
  add_bank_name_->setColour(TextEditor::highlightedTextColourId, Colour(0xffcccccc));
  add_bank_name_->setColour(TextEditor::highlightColourId, Colour(0xff888888));
  add_bank_name_->setColour(TextEditor::backgroundColourId, Colour(0xff323232));
  add_bank_name_->setColour(TextEditor::outlineColourId, Colour(0xff888888));
  add_bank_name_->setColour(TextEditor::focusedOutlineColourId, Colour(0xff888888));
  addAndMakeVisible(add_bank_name_);

  add_folder_name_ = new TextEditor("Add Folder");
  add_folder_name_->addListener(this);
  add_folder_name_->setTextToShowWhenEmpty(TRANS("New Folder"), Colour(0xff777777));
  add_folder_name_->setFont(Fonts::instance()->monospace().withPointHeight(12.0f));
  add_folder_name_->setColour(CaretComponent::caretColourId, Colour(0xff888888));
  add_folder_name_->setColour(TextEditor::textColourId, Colour(0xffcccccc));
  add_folder_name_->setColour(TextEditor::highlightedTextColourId, Colour(0xffcccccc));
  add_folder_name_->setColour(TextEditor::highlightColourId, Colour(0xff888888));
  add_folder_name_->setColour(TextEditor::backgroundColourId, Colour(0xff323232));
  add_folder_name_->setColour(TextEditor::outlineColourId, Colour(0xff888888));
  add_folder_name_->setColour(TextEditor::focusedOutlineColourId, Colour(0xff888888));
  addAndMakeVisible(add_folder_name_);

  save_button_ = new TextButton(TRANS("Save"));
  save_button_->addListener(this);
  addAndMakeVisible(save_button_);

  cancel_button_ = new TextButton(TRANS("Cancel"));
  cancel_button_->addListener(this);
  addAndMakeVisible(cancel_button_);

  add_bank_button_ = new TextButton("+");
  add_bank_button_->addListener(this);
  addAndMakeVisible(add_bank_button_);

  add_folder_button_ = new TextButton("+");
  add_folder_button_->addListener(this);
  addAndMakeVisible(add_folder_button_);
}

void SaveSection::paint(Graphics& g) {
  static const DropShadow shadow(Colour(0xff000000), 5, Point<int>(0, 0));

  g.setColour(Colors::overlay_screen);
  g.fillAll();

  Rectangle<int> save_rect = getSaveRect();
  shadow.drawForRectangle(g, save_rect);
  g.setColour(Colour(0xff303030));
  g.fillRect(save_rect);

  g.saveState();
  g.setOrigin(save_rect.getX() + PADDING_X, save_rect.getY() + PADDING_Y);

  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(14.0f));
  g.setColour(Colour(0xff888888));

  g.drawText(TRANS("PATCH NAME"),
             0, PADDING_Y, DIVISION - 10, TEXT_EDITOR_HEIGHT,
             Justification::centredRight, false);
  g.drawText(TRANS("AUTHOR"),
             0, 2 * PADDING_Y + TEXT_EDITOR_HEIGHT, DIVISION - 10, TEXT_EDITOR_HEIGHT,
             Justification::centredRight, false);
  g.drawText(TRANS("BANK"),
             0, banks_view_->getY() - save_rect.getY() - PADDING_Y,
             DIVISION - 10, TEXT_EDITOR_HEIGHT,
             Justification::centredRight, false);
  g.drawText(TRANS("FOLDER"),
             0, folders_view_->getY() - save_rect.getY() - PADDING_Y,
             DIVISION - 10, TEXT_EDITOR_HEIGHT,
             Justification::centredRight, false);

  g.restoreState();
}

void SaveSection::resized() {
  Rectangle<int> save_rect = getSaveRect();
  patch_name_->setBounds(save_rect.getX() + PADDING_X + DIVISION,
                         save_rect.getY() + 2 * PADDING_Y,
                         save_rect.getWidth() - 2 * PADDING_X - DIVISION,
                         TEXT_EDITOR_HEIGHT);
  author_->setBounds(save_rect.getX() + PADDING_X + DIVISION,
                     save_rect.getY() + 3 * PADDING_Y + TEXT_EDITOR_HEIGHT,
                     save_rect.getWidth() - 2 * PADDING_X - DIVISION,
                     TEXT_EDITOR_HEIGHT);

  float button_width = (save_rect.getWidth() - 3 * PADDING_X) / 2.0f;
  save_button_->setBounds(save_rect.getX() + button_width + 2 * PADDING_X,
                          save_rect.getBottom() - PADDING_Y - BUTTON_HEIGHT,
                          button_width, BUTTON_HEIGHT);
  cancel_button_->setBounds(save_rect.getX() + PADDING_X,
                            save_rect.getBottom() - PADDING_Y - BUTTON_HEIGHT,
                            button_width, BUTTON_HEIGHT);

  float folder_x = save_rect.getX() + PADDING_X + DIVISION;
  float folder_width = save_rect.getWidth() - 2 * PADDING_X - DIVISION;
  float add_folder_y = save_button_->getY() - PADDING_Y - ADD_FOLDER_HEIGHT;
  add_folder_button_->setBounds(folder_x, add_folder_y, ADD_FOLDER_HEIGHT, ADD_FOLDER_HEIGHT);
  add_folder_name_->setBounds(folder_x + ADD_FOLDER_HEIGHT, add_folder_y,
                              folder_width - ADD_FOLDER_HEIGHT, ADD_FOLDER_HEIGHT);

  float bank_y = save_rect.getY() + 4 * PADDING_Y + 2 * TEXT_EDITOR_HEIGHT;
  float bank_folder_height = (add_folder_y - bank_y - PADDING_Y - ADD_FOLDER_HEIGHT) / 2.0f;
  float folder_y = bank_y + bank_folder_height + PADDING_Y + ADD_FOLDER_HEIGHT;
  banks_view_->setBounds(folder_x, bank_y, folder_width, bank_folder_height);
  add_bank_button_->setBounds(folder_x, bank_y + bank_folder_height, ADD_FOLDER_HEIGHT, ADD_FOLDER_HEIGHT);
  add_bank_name_->setBounds(folder_x + ADD_FOLDER_HEIGHT, bank_y + bank_folder_height,
                            folder_width - ADD_FOLDER_HEIGHT, ADD_FOLDER_HEIGHT);
  folders_view_->setBounds(folder_x, folder_y, folder_width, bank_folder_height);
}

void SaveSection::visibilityChanged() {
  if (isVisible()) {
    SparseSet<int> selected_banks = banks_view_->getSelectedRows();
    if (selected_banks.size() == 0)
      banks_view_->selectRow(0);

    SparseSet<int> selected_rows = folders_view_->getSelectedRows();
    if (selected_rows.size() == 0)
      folders_view_->selectRow(0);

    rescanFolders();
  }
}

void SaveSection::mouseUp(const MouseEvent &e) {
  if (!getSaveRect().contains(e.getPosition()))
    setVisible(false);
}

void SaveSection::textEditorReturnKeyPressed(TextEditor& editor) {
  if (&editor == add_folder_name_)
    createNewFolder();
  else
    save();
}

void SaveSection::selectedFilesChanged(FileListBoxModel* list_box) {
  if (list_box == banks_model_)
    rescanFolders();
}

void SaveSection::buttonClicked(Button* clicked_button) {
  if (clicked_button == save_button_)
    save();
  else if (clicked_button == cancel_button_)
    setVisible(false);
  else if (clicked_button == add_bank_button_)
    createNewBank();
  else if (clicked_button == add_folder_button_)
    createNewFolder();
}

Rectangle<int> SaveSection::getSaveRect() {
  if (active_rect_.getWidth() > 0.0f)
    return active_rect_;

  int x = (getWidth() - SAVE_WIDTH) / 2;
  int y = (getHeight() - SAVE_HEIGHT) / 2;
  return Rectangle<int>(x, y, SAVE_WIDTH, SAVE_HEIGHT);
}

void SaveSection::save() {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  SparseSet<int> selected_rows = folders_view_->getSelectedRows();
  if (selected_rows.size() == 0)
    return;

  File folder = folders_model_->getFileAtRow(selected_rows[0]);
  String patch_name = patch_name_->getText();
  if (patch_name.length() == 0)
    return;

  File save_file = folder.getChildFile(patch_name);
  parent->getSynth()->setAuthor(author_->getText());
  parent->getSynth()->saveToFile(save_file);

  patch_name_->clear();
  setVisible(false);
  if (listener_)
    listener_->fileSaved(save_file);
}

void SaveSection::createNewBank() {
  String bank_name = add_bank_name_->getText();
  if (bank_name.length() == 0)
    return;

  File banks_dir = LoadSave::getBankDirectory();
  File new_bank = banks_dir.getChildFile(bank_name);
  if (!new_bank.exists())
    new_bank.createDirectory();

  add_bank_name_->clear();

  rescanFolders();
  int row = banks_model_->getIndexOfFile(new_bank);
  banks_view_->selectRow(row);
  banks_view_->updateContent();
}

void SaveSection::createNewFolder() {
  String folder_name = add_folder_name_->getText();
  if (folder_name.length() == 0)
    return;

  SparseSet<int> selected_rows = banks_view_->getSelectedRows();
  if (selected_rows.size() == 0)
    return;
  File bank_dir = banks_model_->getFileAtRow(selected_rows[0]);

  File new_folder = bank_dir.getChildFile(folder_name);
  if (!new_folder.exists())
    new_folder.createDirectory();

  add_folder_name_->clear();

  rescanFolders();
  int row = folders_model_->getIndexOfFile(new_folder);
  folders_view_->selectRow(row);
  folders_view_->updateContent();
}

void SaveSection::rescanBanks() {
  Array<File> bank_locations;
  File banks_dir = LoadSave::getBankDirectory();
  bank_locations.add(banks_dir);
  banks_model_->rescanFiles(bank_locations);
  banks_view_->updateContent();
}

void SaveSection::rescanFolders() {
  rescanBanks();
  SparseSet<int> selected_rows = banks_view_->getSelectedRows();
  if (selected_rows.size() == 0)
    return;
  File bank_dir = banks_model_->getFileAtRow(selected_rows[0]);

  Array<File> folder_locations;
  folder_locations.add(bank_dir);
  folders_model_->rescanFiles(folder_locations);
  folders_view_->updateContent();
}
