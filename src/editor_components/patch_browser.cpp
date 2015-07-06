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

#include "patch_browser.h"

#define PATCH_EXTENSION "helm"
#define TEXT_PADDING 4.0f
#define LINUX_SYSTEM_PATCH_DIRECTORY "/usr/share/helm/patches"
#define LINUX_USER_PATCH_DIRECTORY "/usr/local/share/helm/patches"

PatchBrowser::PatchBrowser() : SynthSection("patch_browser") {
  setLookAndFeel(BrowserLookAndFeel::instance());

  addButton(prev_folder_ = new TextButton("prev_folder"));
  prev_folder_->setButtonText(TRANS("<"));
  prev_folder_->setColour(TextButton::buttonColourId, Colour(0xff303030));
  prev_folder_->setColour(TextButton::textColourOffId, Colours::white);

  addButton(prev_patch_ = new TextButton("prev_patch"));
  prev_patch_->setButtonText(TRANS("<"));
  prev_patch_->setColour(TextButton::buttonColourId, Colour(0xff464646));
  prev_patch_->setColour(TextButton::textColourOffId, Colours::white);

  addButton(next_folder_ = new TextButton("next_folder"));
  next_folder_->setButtonText(TRANS(">"));
  next_folder_->setColour(TextButton::buttonColourId, Colour(0xff303030));
  next_folder_->setColour(TextButton::textColourOffId, Colours::white);

  addButton(next_patch_ = new TextButton("next_patch"));
  next_patch_->setButtonText(TRANS(">"));
  next_patch_->setColour(TextButton::buttonColourId, Colour(0xff464646));
  next_patch_->setColour(TextButton::textColourOffId, Colours::white);

  addButton(save_ = new TextButton("save"));
  save_->setButtonText(TRANS("SAVE"));
  save_->setColour(TextButton::buttonColourId, Colour(0xff303030));
  save_->setColour(TextButton::textColourOffId, Colours::white);

  folder_index_ = -1;
  patch_index_ = -1;
  folder_text_ = TRANS("Init Folder");
  patch_text_ = TRANS("Init Patch");
}

PatchBrowser::~PatchBrowser() {
  prev_folder_ = nullptr;
  prev_patch_ = nullptr;
  next_folder_ = nullptr;
  next_patch_ = nullptr;
  save_ = nullptr;
}

void PatchBrowser::paintBackground(Graphics& g) {
  static const DropShadow shadow(Colour(0xff000000), 4, Point<int>(0, 0));
  static Font browser_font(Typeface::createSystemTypefaceFor(BinaryData::DroidSansMono_ttf,
                                                             BinaryData::DroidSansMono_ttfSize));

  g.setColour(Colour(0xff303030));
  g.fillRect(0, 0, getWidth(), proportionOfHeight(0.5));

  g.setColour(Colour(0xff464646));
  g.fillRect(0, proportionOfHeight(0.5), getWidth(), proportionOfHeight(0.5));

  Rectangle<int> left(proportionOfWidth(0.2), 0,
                      proportionOfWidth(0.1), proportionOfHeight(1.0));
  Rectangle<int> right(proportionOfWidth(0.9), 0,
                       proportionOfWidth(0.1), proportionOfHeight(1.0));
  shadow.drawForRectangle(g, left);
  shadow.drawForRectangle(g, right);

  Rectangle<int> top(proportionOfWidth(0.3) + TEXT_PADDING, 0,
                     proportionOfWidth(0.6) - TEXT_PADDING, proportionOfHeight(0.5));
  Rectangle<int> bottom(proportionOfWidth(0.3) + TEXT_PADDING, proportionOfHeight(0.5),
                        proportionOfWidth(0.6) - TEXT_PADDING, proportionOfHeight(0.5));

  g.setFont(browser_font.withPointHeight(12.0f));
  g.setColour(Colour(0xffbbbbbb));
  g.drawFittedText(folder_text_, top, Justification::centredLeft, 1);
  g.setColour(Colour(0xffffffff));
  g.drawFittedText(patch_text_, bottom, Justification::centredLeft, 1);
}

void PatchBrowser::resized() {
  prev_folder_->setBounds(proportionOfWidth(0.2f), 0,
                          proportionOfWidth(0.1f), proportionOfHeight(0.5f));
  prev_patch_->setBounds(proportionOfWidth(0.2f), proportionOfHeight(0.5f),
                         proportionOfWidth(0.1f), proportionOfHeight (0.5f));
  next_folder_->setBounds(getWidth() - proportionOfWidth(0.1f), 0,
                          proportionOfWidth(0.1f), proportionOfHeight(0.5f));
  next_patch_->setBounds(getWidth() - proportionOfWidth(0.1f), proportionOfHeight(0.5f),
                         proportionOfWidth(0.1f), proportionOfHeight(0.5f));
  save_->setBounds(proportionOfWidth(0.0f), proportionOfHeight(0.0f),
                   proportionOfWidth(0.2f), proportionOfHeight (1.0f));

  SynthSection::resized();
}

void PatchBrowser::buttonClicked(Button* buttonThatWasClicked) {
  if (buttonThatWasClicked == save_) {
    int flags = FileBrowserComponent::canSelectFiles | FileBrowserComponent::saveMode;
    FileBrowserComponent browser(flags, getUserPatchDirectory(), nullptr, nullptr);
    FileChooserDialogBox save_dialog("save patch", "save", browser, true, Colours::white);
    if (save_dialog.show()) {
      SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
      File save_file = browser.getSelectedFile(0);
      if (save_file.getFileExtension() != PATCH_EXTENSION)
        save_file = save_file.withFileExtension(PATCH_EXTENSION);
      save_file.replaceWithText(JSON::toString(parent->saveToVar()));
    }
  }
  else {
    if (buttonThatWasClicked == prev_folder_) {
      folder_index_--;
      patch_index_ = 0;
    }
    else if (buttonThatWasClicked == next_folder_) {
      folder_index_++;
      patch_index_ = 0;
    }
    else if (buttonThatWasClicked == prev_patch_)
      patch_index_--;
    else if (buttonThatWasClicked == next_patch_)
      patch_index_++;

    File folder = getCurrentFolder();
    File patch = getCurrentPatch();
    folder_text_ = folder.getFileNameWithoutExtension();
    patch_text_ = patch.getFileNameWithoutExtension();
    loadFromFile(patch);

    const Desktop::Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
    float scale = display.scale;
    Graphics g(background_);
    g.addTransform(AffineTransform::scale(scale, scale));
    paintBackground(g);
    repaint();
  }
}

File PatchBrowser::getSystemPatchDirectory() {
  File patch_dir = File("");
#ifdef LINUX
  patch_dir = File(LINUX_SYSTEM_PATCH_DIRECTORY);
#elif defined(__APPLE__)
  File data_dir = File::getSpecialLocation(File::commonApplicationDataDirectory);
  patch_dir = data_dir.getChildFile(String("Audio/Presets/") + "helm");
#elif defined(_WIN32)
  patch_dir = File("C:");
#endif

  if (!patch_dir.exists())
    patch_dir.createDirectory();
  return patch_dir;
}

File PatchBrowser::getUserPatchDirectory() {
  File patch_dir = File("");
#ifdef LINUX
  patch_dir = File(LINUX_USER_PATCH_DIRECTORY);
#elif defined(__APPLE__)
  File data_dir = File::getSpecialLocation(File::userApplicationDataDirectory);
  patch_dir = data_dir.getChildFile(String("Audio/Presets/") + "helm");
#elif defined(_WIN32)
  patch_dir = File("C:");
#endif

  if (!patch_dir.exists())
    patch_dir.createDirectory();
  return patch_dir;
}

File PatchBrowser::getCurrentPatch() {
  Array<File> patches;
  File patch_folder = getCurrentFolder();

  patch_folder.findChildFiles(patches, File::findFiles, false, String("*.") + PATCH_EXTENSION);
  if (patch_index_ >= patches.size())
    patch_index_ = 0;
  else if (patch_index_ < 0)
    patch_index_ = patches.size() - 1;

  return patches[patch_index_];
}

File PatchBrowser::getCurrentFolder() {
  Array<File> folders;
  File patch_dir = getSystemPatchDirectory();

  patch_dir.findChildFiles(folders, File::findDirectories, false);
  folders.add(getUserPatchDirectory());
  if (folder_index_ >= folders.size())
    folder_index_ = 0;
  else if (folder_index_ < 0)
    folder_index_ = folders.size() - 1;

  return folders[folder_index_];
}

void PatchBrowser::loadFromFile(File &patch) {
  var parsed_json_state;
  if (JSON::parse(patch.loadFileAsString(), parsed_json_state).wasOk()) {
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    parent->loadFromVar(parsed_json_state);
  }
}
