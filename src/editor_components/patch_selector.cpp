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

#include "patch_selector.h"
#include "browser_look_and_feel.h"
#include "load_save.h"
#include "synth_gui_interface.h"

#define TEXT_PADDING 4.0f

PatchSelector::PatchSelector() : SynthSection("patch_selector"),
                                 browser_(nullptr), save_section_(nullptr) {
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

  addButton(browse_ = new TextButton("browse"));
  browse_->setButtonText(TRANS("BROWSE"));
  browse_->setColour(TextButton::buttonColourId, Colour(0xff303030));
  browse_->setColour(TextButton::textColourOffId, Colours::white);
}

PatchSelector::~PatchSelector() {
  prev_patch_ = nullptr;
  next_patch_ = nullptr;
  save_ = nullptr;
  browse_ = nullptr;
}

void PatchSelector::paintBackground(Graphics& g) {
  static const DropShadow shadow(Colour(0xff000000), 4, Point<int>(0, 0));
  static Font selector_font(Typeface::createSystemTypefaceFor(BinaryData::DroidSansMono_ttf,
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

  g.setFont(selector_font.withPointHeight(12.0f));
  g.setColour(Colour(0xffbbbbbb));
  g.drawFittedText(folder_text_, top, Justification::centredLeft, 1);
  g.setColour(Colour(0xffffffff));
  g.drawFittedText(patch_text_, bottom, Justification::centredLeft, 1);
}

void PatchSelector::resized() {
  prev_patch_->setBounds(proportionOfWidth(0.2f), 0,
                         proportionOfWidth(0.1f), proportionOfHeight (1.0f));
  next_patch_->setBounds(getWidth() - proportionOfWidth(0.1f), 0,
                         proportionOfWidth(0.1f), proportionOfHeight(1.0f));
  save_->setBounds(proportionOfWidth(0.0f), proportionOfHeight(0.0f),
                   proportionOfWidth(0.2f), proportionOfHeight (0.5f));
  browse_->setBounds(proportionOfWidth(0.0f), proportionOfHeight(0.5f),
                     proportionOfWidth(0.2f), proportionOfHeight (0.5f));

  SynthSection::resized();
}

void PatchSelector::buttonClicked(Button* clicked_button) {
  if (browser_ == nullptr)
    return;

  if (clicked_button == save_ && save_section_)
    save_section_->setVisible(true);
  else if (clicked_button == browse_)
    browser_->setVisible(!browser_->isVisible());
  else if (clicked_button == prev_patch_)
    browser_->loadPrevPatch();
  else if (clicked_button == next_patch_)
    browser_->loadNextPatch();
}

void PatchSelector::newPatchSelected(File patch) {
  File folder = patch.getParentDirectory();
  folder_text_ = folder.getFileNameWithoutExtension();
  patch_text_ = patch.getFileNameWithoutExtension();

  const Desktop::Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  float scale = display.scale;
  Graphics g(background_);
  g.addTransform(AffineTransform::scale(scale, scale));
  paintBackground(g);
  repaint();
}

void PatchSelector::loadFromFile(File& patch) {
  var parsed_json_state;
  if (JSON::parse(patch.loadFileAsString(), parsed_json_state).wasOk()) {
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    parent->loadFromVar(parsed_json_state);
  }
}
