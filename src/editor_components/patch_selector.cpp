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

#include "patch_selector.h"

#include "browser_look_and_feel.h"
#include "colors.h"
#include "default_look_and_feel.h"
#include "fonts.h"
#include "load_save.h"
#include "synth_gui_interface.h"

#define TEXT_PADDING 4.0f
#define BROWSE_PERCENT 0.35f

namespace {
  static void initPatchCallback(int result, PatchSelector* patch_selector) {
    if (patch_selector != nullptr && result != 0)
      patch_selector->initPatch();
  }
}

PatchSelector::PatchSelector() : SynthSection("patch_selector"),
                                 browser_(nullptr), save_section_(nullptr), modified_(false) {
  setLookAndFeel(BrowserLookAndFeel::instance());
  addButton(prev_patch_ = new TextButton("prev_patch"));
  prev_patch_->setButtonText(TRANS("<"));
  prev_patch_->setColour(TextButton::buttonColourId, Colour(0xff464646));
  prev_patch_->setColour(TextButton::textColourOffId, Colours::white);

  addButton(next_patch_ = new TextButton("next_patch"));
  next_patch_->setButtonText(TRANS(">"));
  next_patch_->setColour(TextButton::buttonColourId, Colour(0xff464646));
  next_patch_->setColour(TextButton::textColourOffId, Colours::white);

  addButton(save_ = new TextButton("save"));
  save_->setButtonText(TRANS("SAVE"));
  save_->setColour(TextButton::buttonColourId, Colour(0xff303030));
  save_->setColour(TextButton::textColourOffId, Colours::white);

  addButton(export_ = new TextButton("export"));
  export_->setButtonText(TRANS("EXPORT"));
  export_->setColour(TextButton::buttonColourId, Colour(0xff303030));
  export_->setColour(TextButton::textColourOffId, Colours::white);

  addButton(browse_ = new TextButton("browse"));
  browse_->setButtonText(TRANS("BROWSE"));
  browse_->setColour(TextButton::buttonColourId, Colour(0xff303030));
  browse_->setColour(TextButton::textColourOffId, Colours::white);
}

PatchSelector::~PatchSelector() {
  prev_patch_ = nullptr;
  next_patch_ = nullptr;
  save_ = nullptr;
  export_ = nullptr;
  browse_ = nullptr;
}

void PatchSelector::paint(Graphics& g) {
  SynthSection::paint(g);

  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  patch_text_ = parent->getSynth()->getPatchName();
  if (patch_text_ == "")
    patch_text_ = TRANS("init");

  if (modified_)
    patch_text_ = "*" + patch_text_;
  folder_text_ = parent->getSynth()->getFolderName();

  int browse_height = proportionOfHeight(BROWSE_PERCENT);
  Rectangle<int> top(proportionOfWidth(0.1f) + TEXT_PADDING, 0,
                     proportionOfWidth(0.8f) - TEXT_PADDING, browse_height);
  Rectangle<int> bottom(proportionOfWidth(0.1f) + TEXT_PADDING, browse_height,
                        proportionOfWidth(0.8f) - TEXT_PADDING, browse_height);

  g.setFont(Fonts::instance()->monospace().withPointHeight(size_ratio_ * 12.0f));
  g.setColour(Colors::control_label_text);
  g.drawFittedText(folder_text_, top, Justification::centredLeft, 1);
  g.setColour(Colour(0xffffffff));
  g.drawFittedText(patch_text_, bottom, Justification::centredLeft, 1);
}


void PatchSelector::paintBackground(Graphics& g) {
  static const DropShadow shadow(Colour(0xff000000), 4, Point<int>(0, 0));

  g.setColour(Colour(0xff383838));
  g.fillRect(0, 0, getWidth(), proportionOfHeight(BROWSE_PERCENT));

  g.setColour(Colour(0xff444444));
  g.fillRect(0, proportionOfHeight(BROWSE_PERCENT), getWidth(), proportionOfHeight(BROWSE_PERCENT));

  int browse_height = proportionOfHeight(BROWSE_PERCENT);

  Rectangle<int> left(0, 0, proportionOfWidth(0.1f), 2 * browse_height);
  Rectangle<int> right(proportionOfWidth(0.9f), 0, proportionOfWidth(0.1f), 2 * browse_height);
  shadow.drawForRectangle(g, left);
  shadow.drawForRectangle(g, right);
}

void PatchSelector::resized() {
  int full_browse_height = 2 * proportionOfHeight(BROWSE_PERCENT);
  prev_patch_->setBounds(0, 0, proportionOfWidth(0.1f), full_browse_height);
  next_patch_->setBounds(getWidth() - proportionOfWidth(0.1f), 0,
                         proportionOfWidth(0.1f), full_browse_height);

  int button_width = (getWidth() - 2.0) / 3.0 - 1;
  int button_height = getHeight() - full_browse_height;
  int last_button_width = getWidth() - 2 * button_width - 2;
  save_->setBounds(0, full_browse_height, button_width, button_height);
  export_->setBounds(button_width + 1, full_browse_height, button_width, button_height);
  browse_->setBounds(2 * button_width + 2, full_browse_height, last_button_width, button_height);
  SynthSection::resized();
}

void PatchSelector::mouseUp(const MouseEvent& event) {
  if (event.mods.isPopupMenu()) {
    PopupMenu m;
    m.setLookAndFeel(DefaultLookAndFeel::instance());

    m.addItem(1, "Load Init Patch");
    m.showMenuAsync(PopupMenu::Options(),
                    ModalCallbackFunction::forComponent(initPatchCallback, this));
  }
  else if (browser_)
    browser_->setVisible(!browser_->isVisible());
}

void PatchSelector::buttonClicked(Button* clicked_button) {
  if (browser_ == nullptr)
    return;

  if (clicked_button == save_ && save_section_)
    save_section_->setVisible(true);
  else if (clicked_button == browse_)
    browser_->setVisible(!browser_->isVisible());
  else if (clicked_button == export_) {
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    if (parent == nullptr)
      return;

    SynthBase* synth = parent->getSynth();
    synth->exportToFile();
    parent->externalPatchLoaded(synth->getActiveFile());
  }
  else if (clicked_button == prev_patch_)
    browser_->loadPrevPatch();
  else if (clicked_button == next_patch_)
    browser_->loadNextPatch();
}

void PatchSelector::newPatchSelected(File patch) {
  repaint();
}

void PatchSelector::setModified(bool modified) {
  if (modified_ == modified)
    return;

  modified_ = modified;
  repaint();
}

void PatchSelector::loadFromFile(File& patch) {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  parent->getSynth()->loadFromFile(patch);
}

int PatchSelector::getBrowseHeight() {
  return 2 * proportionOfHeight(BROWSE_PERCENT);
}

void PatchSelector::initPatch() {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  parent->getSynth()->loadInitPatch();
  browser_->externalPatchLoaded(File());
  parent->updateFullGui();
  parent->notifyFresh();
}
