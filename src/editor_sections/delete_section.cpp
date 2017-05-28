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

#include "delete_section.h"

#include "colors.h"
#include "fonts.h"
#include "helm_common.h"
#include "text_look_and_feel.h"

#define DELETE_WIDTH 340
#define DELETE_HEIGHT 140
#define PADDING_X 25
#define PADDING_Y 20
#define BUTTON_HEIGHT 30

DeleteSection::DeleteSection(String name) : Overlay(name) {
  delete_button_ = new TextButton(TRANS("Delete"));
  delete_button_->addListener(this);
  addAndMakeVisible(delete_button_);

  cancel_button_ = new TextButton(TRANS("Cancel"));
  cancel_button_->addListener(this);
  addAndMakeVisible(cancel_button_);
}

void DeleteSection::paint(Graphics& g) {
  static const DropShadow shadow(Colour(0xff000000), 5, Point<int>(0, 0));

  g.setColour(Colors::overlay_screen);
  g.fillAll();

  Rectangle<int> delete_rect = getDeleteRect();
  shadow.drawForRectangle(g, delete_rect);
  g.setColour(Colour(0xff303030));
  g.fillRect(delete_rect);

  g.saveState();
  g.setOrigin(delete_rect.getX() + PADDING_X, delete_rect.getY() + PADDING_Y);

  g.setFont(Fonts::instance()->proportional_light().withPointHeight(14.0f));
  g.setColour(Colour(0xffaaaaaa));

  String text;
  if (file_.isDirectory())
    text = TRANS("Are you sure you want to delte this folder?");
  else
  text = TRANS("Are you sure you want to delte this patch?");
  g.drawText(text,
             0, 0.0f, delete_rect.getWidth() - 2 * PADDING_X, 22.0f,
             Justification::centred, false);

  g.setFont(Fonts::instance()->monospace().withPointHeight(16.0f));
  g.setColour(Colors::audio);
  g.drawText(file_.getFileNameWithoutExtension(),
             0, 20.0f, delete_rect.getWidth() - 2 * PADDING_X, 22.0f,
             Justification::centred, false);

  g.restoreState();
}

void DeleteSection::resized() {
  Rectangle<int> delete_rect = getDeleteRect();

  float button_width = (delete_rect.getWidth() - 3 * PADDING_X) / 2.0f;
  delete_button_->setBounds(delete_rect.getX() + PADDING_X,
                            delete_rect.getBottom() - PADDING_Y - BUTTON_HEIGHT,
                            button_width, BUTTON_HEIGHT);
  cancel_button_->setBounds(delete_rect.getX() + button_width + 2 * PADDING_X,
                            delete_rect.getBottom() - PADDING_Y - BUTTON_HEIGHT,
                            button_width, BUTTON_HEIGHT);
}

void DeleteSection::mouseUp(const MouseEvent &e) {
  if (!getDeleteRect().contains(e.getPosition()))
    setVisible(false);
}

void DeleteSection::buttonClicked(Button* clicked_button) {
  if (clicked_button == delete_button_) {
    file_.deleteRecursively();
    setVisible(false);
    for (int i = 0; i < listeners_.size(); ++i)
      listeners_[i]->fileDeleted(file_);
  }
  else if (clicked_button == cancel_button_)
    setVisible(false);
}

Rectangle<int> DeleteSection::getDeleteRect() {
  int x = (getWidth() - DELETE_WIDTH) / 2;
  int y = (getHeight() - DELETE_HEIGHT) / 2;
  return Rectangle<int>(x, y, DELETE_WIDTH, DELETE_HEIGHT);
}
