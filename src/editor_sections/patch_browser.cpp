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

#include "synth_gui_interface.h"
#include "browser_look_and_feel.h"
#include "helm_common.h"
#include "load_save.h"
#include "patch_browser.h"

#define WIDTH1_PERCENT 0.2
#define TEXT_PADDING 4.0f
#define LINUX_SYSTEM_PATCH_DIRECTORY "/usr/share/helm/patches"
#define LINUX_USER_PATCH_DIRECTORY "~/.helm/User Patches"
#define BROWSING_HEIGHT 422.0f
#define BROWSE_PADDING 8.0f


namespace {
  Array<File> getSelectedFolders(ListBox* view, FileListBoxModel* model) {
    SparseSet<int> selected_rows = view->getSelectedRows();

    Array<File> selected_folders;
    for (int i = 0; i < selected_rows.size(); ++i)
      selected_folders.add(model->getFileAtRow(selected_rows[i]));

    return selected_folders;
  }

  Array<File> getFoldersToScan(ListBox* view, FileListBoxModel* model) {
    if (view->getSelectedRows().size())
      return getSelectedFolders(view, model);

    return model->getAllFiles();
  }

  void setSelectedRows(ListBox* view, FileListBoxModel* model, Array<File> selected) {
    view->deselectAllRows();

    for (int sel = 0, row = 0; sel < selected.size() && row < model->getNumRows();) {
      String selected_path = selected[sel].getFullPathName();
      int compare = model->getFileAtRow(row).getFullPathName().compare(selected_path);

      if (compare < 0)
        ++row;
      else if (compare > 0)
        ++sel;
      else {
        view->selectRow(row, true, false);
        ++row;
        ++sel;
      }
    }
  }
} // namespace

PatchBrowser::PatchBrowser() : Component("patch_browser") {
  static Font search_font(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                            BinaryData::RobotoLight_ttfSize));
  static Font license_font(Typeface::createSystemTypefaceFor(BinaryData::DroidSansMono_ttf,
                                                             BinaryData::DroidSansMono_ttfSize));
  listener_ = nullptr;
  save_section_ = nullptr;
  delete_section_ = nullptr;

  banks_model_ = new FileListBoxModel();
  banks_model_->setListener(this);
  Array<File> bank_locations;
  File bank_dir = LoadSave::getBankDirectory();
  bank_locations.add(bank_dir);
  banks_model_->rescanFiles(bank_locations);

  banks_view_ = new ListBox("banks", banks_model_);
  banks_view_->setMultipleSelectionEnabled(true);
  banks_view_->setClickingTogglesRowSelection(true);
  banks_view_->updateContent();
  addAndMakeVisible(banks_view_);

  folders_model_ = new FileListBoxModel();
  folders_model_->setListener(this);

  folders_view_ = new ListBox("folders", folders_model_);
  folders_view_->setMultipleSelectionEnabled(true);
  folders_view_->setClickingTogglesRowSelection(true);
  folders_view_->updateContent();
  addAndMakeVisible(folders_view_);

  patches_model_ = new FileListBoxModel();
  patches_model_->setListener(this);

  patches_view_ = new ListBox("patches", patches_model_);
  patches_view_->updateContent();
  addAndMakeVisible(patches_view_);

  banks_view_->setColour(ListBox::backgroundColourId, Colour(0xff323232));
  folders_view_->setColour(ListBox::backgroundColourId, Colour(0xff323232));
  patches_view_->setColour(ListBox::backgroundColourId, Colour(0xff323232));

  search_box_ = new TextEditor("Search");
  search_box_->addListener(this);
  search_box_->setSelectAllWhenFocused(true);
  search_box_->setTextToShowWhenEmpty(TRANS("Search"), Colour(0xff777777));
  search_box_->setFont(search_font.withPointHeight(16.0f));
  search_box_->setColour(CaretComponent::caretColourId, Colour(0xff888888));
  search_box_->setColour(TextEditor::textColourId, Colour(0xffcccccc));
  search_box_->setColour(TextEditor::highlightedTextColourId, Colour(0xffcccccc));
  search_box_->setColour(TextEditor::highlightColourId, Colour(0xff888888));
  search_box_->setColour(TextEditor::backgroundColourId, Colour(0xff323232));
  search_box_->setColour(TextEditor::outlineColourId, Colour(0xff888888));
  search_box_->setColour(TextEditor::focusedOutlineColourId, Colour(0xff888888));
  addAndMakeVisible(search_box_);

  selectedFilesChanged(banks_model_);
  selectedFilesChanged(folders_model_);

  license_link_ = new HyperlinkButton("CC-BY", URL("https://creativecommons.org/licenses/by/4.0/"));
  license_link_->setFont(license_font.withPointHeight(12.0f), false, Justification::centredLeft);
  license_link_->setColour(HyperlinkButton::textColourId, Colour(0xffffd740));
  addAndMakeVisible(license_link_);

  save_as_button_ = new TextButton(TRANS("Save As"));
  save_as_button_->addListener(this);
  addAndMakeVisible(save_as_button_);

  delete_patch_button_ = new TextButton(TRANS("Delete Patch"));
  delete_patch_button_->addListener(this);
  addAndMakeVisible(delete_patch_button_);

  addKeyListener(this);
}

PatchBrowser::~PatchBrowser() {
}

void PatchBrowser::paint(Graphics& g) {
  static Font info_font(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                          BinaryData::RobotoLight_ttfSize));
  static Font data_font(Typeface::createSystemTypefaceFor(BinaryData::DroidSansMono_ttf,
                                                          BinaryData::DroidSansMono_ttfSize));
  g.fillAll(Colour(0xbb212121));
  g.setColour(Colour(0xff111111));
  g.fillRect(0.0f, 0.0f, 1.0f * getWidth(), BROWSING_HEIGHT);

  g.setColour(Colour(0xff212121));
  float info_width = getWideWidth();
  Rectangle<int> data_rect(getWidth() - info_width - BROWSE_PADDING, BROWSE_PADDING,
                           info_width, BROWSING_HEIGHT - 2.0f * BROWSE_PADDING);
  g.fillRect(data_rect);

  if (isPatchSelected()) {
    float data_x = BROWSE_PADDING + 2.0f * getNarrowWidth() + getWideWidth() + 3.0f * BROWSE_PADDING;
    float division = 90.0f;
    float buffer = 20.0f;

    g.setFont(info_font.withPointHeight(14.0f));
    g.setColour(Colour(0xff888888));

    g.fillRect(data_x + division + buffer / 2.0f, BROWSE_PADDING + 70.0f,
               1.0f, 120.0f);

    g.drawText(TRANS("AUTHOR"),
               data_x, BROWSE_PADDING + 80.0f, division, 20.0f,
               Justification::centredRight, false);
    g.drawText(TRANS("BANK"),
               data_x, BROWSE_PADDING + 120.0f, division, 20.0f,
               Justification::centredRight, false);
    g.drawText(TRANS("LICENSE"),
               data_x, BROWSE_PADDING + 160.0f, division, 20.0f,
               Justification::centredRight, false);
    
    g.setFont(data_font.withPointHeight(16.0f));
    g.setColour(Colour(0xff03a9f4));

    File selected_patch = getSelectedPatch();
    g.drawFittedText(selected_patch.getFileNameWithoutExtension(),
                     data_x + 2.0f * BROWSE_PADDING, 2.0f * BROWSE_PADDING,
                     info_width - 2.0f * BROWSE_PADDING, 20.0f,
                     Justification::centred, true);

    g.setFont(data_font.withPointHeight(12.0f));
    g.setColour(Colour(0xffbbbbbb));

    float data_width = info_width - division - buffer - 2.0f * BROWSE_PADDING;
    g.drawText(selected_patch.getParentDirectory().getParentDirectory().getFileName(),
               data_x + division + buffer, BROWSE_PADDING + 80.0f, data_width, 20.0f,
               Justification::centredLeft, true);
    g.drawText(selected_patch.getParentDirectory().getParentDirectory().getFileName(),
               data_x + division + buffer, BROWSE_PADDING + 120.0f, data_width, 20.0f,
               Justification::centredLeft, true);
  }
}

void PatchBrowser::resized() {
  float start_x = BROWSE_PADDING;
  float width1 = getNarrowWidth();
  float width2 = getWideWidth();
  float height = BROWSING_HEIGHT - 2.0f * BROWSE_PADDING;
  float search_box_height = 28.0f;

  banks_view_->setBounds(start_x, BROWSE_PADDING, width1, height);
  folders_view_->setBounds(start_x + width1 + BROWSE_PADDING, BROWSE_PADDING, width1, height);

  float patches_x = start_x + 2.0f * (width1 + BROWSE_PADDING);
  search_box_->setBounds(patches_x, BROWSE_PADDING, width2, search_box_height);
  patches_view_->setBounds(patches_x, search_box_height + BROWSE_PADDING,
                           width2, height - search_box_height);

  float data_x = start_x + 2.0f * width1 + width2 + 3.0f * BROWSE_PADDING;
  float data_widget_buffer_x = 12.0f;
  license_link_->setBounds(data_x + 108.0f, BROWSE_PADDING + 160.0f,
                           200.0f, 20.0f);

  float button_width = (width2 - 3.0f * data_widget_buffer_x) / 2.0f;
  save_as_button_->setBounds(data_x + data_widget_buffer_x, height - 30.0f,
                             button_width, 30.0f);
  delete_patch_button_->setBounds(data_x + button_width + 2.0f * data_widget_buffer_x,
                                  height - 30.0f,
                                  button_width, 30.0f);
}

void PatchBrowser::visibilityChanged() {
  Component::visibilityChanged();
  if (isVisible()) {
    search_box_->setText("");
    search_box_->grabKeyboardFocus();
    license_link_->setVisible(isPatchSelected());
  }
}

void PatchBrowser::selectedFilesChanged(FileListBoxModel* model) {
  if (model == banks_model_)
    scanFolders();
  if (model == banks_model_ || model == folders_model_)
    scanPatches();
  else if (model == patches_model_) {
    SparseSet<int> selected_rows = patches_view_->getSelectedRows();
    if (selected_rows.size()) {
      File patch = patches_model_->getFileAtRow(selected_rows[0]);
      loadFromFile(patch);

      if (listener_)
        listener_->newPatchSelected(patch);
    }
    else
      license_link_->setVisible(false);
    repaint();
  }
}

void PatchBrowser::textEditorTextChanged(TextEditor& editor) {
  scanPatches();
}

void PatchBrowser::fileSaved(File save_file) {
  scanPatches();
  int index = patches_model_->getIndexOfFile(save_file);
  patches_view_->selectRow(index);
}

void PatchBrowser::buttonClicked(Button* clicked_button) {
  if (clicked_button == save_as_button_ && save_section_)
    save_section_->setVisible(true);
  else if (clicked_button == delete_patch_button_ && delete_section_)
    delete_section_->setVisible(true);
}

bool PatchBrowser::keyPressed(const KeyPress &key, Component *origin) {
  return search_box_->hasKeyboardFocus(true);
}

bool PatchBrowser::keyStateChanged(bool is_key_down, Component *origin) {
  if (is_key_down)
    return search_box_->hasKeyboardFocus(true);
  return false;
}

void PatchBrowser::mouseUp(const MouseEvent& e) {
  if (e.getPosition().y > BROWSING_HEIGHT)
    setVisible(false);
}

File PatchBrowser::getSelectedPatch() {
  SparseSet<int> selected_rows = patches_view_->getSelectedRows();
  if (selected_rows.size())
    return patches_model_->getFileAtRow(selected_rows[0]);
  return File();
}

void PatchBrowser::loadPrevPatch() {
  SparseSet<int> selected_rows = patches_view_->getSelectedRows();
  if (selected_rows.size()) {
    int row = selected_rows[0] - 1;
    if (row < 0)
      row += patches_model_->getNumRows();
    patches_view_->selectRow(row);
  }
  else
    patches_view_->selectRow(0);
}

void PatchBrowser::loadNextPatch() {
  SparseSet<int> selected_rows = patches_view_->getSelectedRows();
  if (selected_rows.size()) {
    int row = selected_rows[0] + 1;
    if (row >= patches_model_->getNumRows())
      row -= patches_model_->getNumRows();
    patches_view_->selectRow(row);
  }
  else
    patches_view_->selectRow(0);
}

void PatchBrowser::loadFromFile(File& patch) {
  var parsed_json_state;
  if (JSON::parse(patch.loadFileAsString(), parsed_json_state).wasOk()) {
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    parent->loadFromVar(parsed_json_state);
    license_link_->setVisible(true);
  }
}

void PatchBrowser::setSaveSection(SaveSection* save_section) {
  save_section_ = save_section;
  save_section_->setListener(this);
}

void PatchBrowser::scanFolders() {
  Array<File> banks = getFoldersToScan(banks_view_, banks_model_);
  Array<File> folders_selected = getSelectedFolders(folders_view_, folders_model_);

  folders_model_->rescanFiles(banks);
  folders_view_->updateContent();
  setSelectedRows(folders_view_, folders_model_, folders_selected);
}

void PatchBrowser::scanPatches() {
  Array<File> folders = getFoldersToScan(folders_view_, folders_model_);
  Array<File> patches_selected = getSelectedFolders(patches_view_, patches_model_);

  String search = "*" + search_box_->getText() + "*." + mopo::PATCH_EXTENSION;
  patches_model_->rescanFiles(folders, search, true);
  patches_view_->updateContent();
  setSelectedRows(patches_view_, patches_model_, patches_selected);
}

float PatchBrowser::getNarrowWidth() {
  return (getWidth() - 5.0f * BROWSE_PADDING) * WIDTH1_PERCENT;
}

float PatchBrowser::getWideWidth() {
  return (getWidth() - 5.0f * BROWSE_PADDING) * (0.5f - WIDTH1_PERCENT);
}
