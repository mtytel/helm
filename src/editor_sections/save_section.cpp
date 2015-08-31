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

#include "save_section.h"
#include "helm_common.h"
#include "load_save.h"
#include "text_look_and_feel.h"

#define SAVE_WIDTH 450
#define SAVE_HEIGHT 230
#define PADDING_X 25
#define PADDING_Y 15

SaveSection::SaveSection(String name) : Component(name) {
  folders_model_ = new FileListBoxModel();
  Array<File> folder_locations;
  File bank_dir = LoadSave::getUserBankDirectory();
  folder_locations.add(bank_dir);
  folders_model_->rescanFiles(folder_locations);

  folders_view_ = new ListBox("folders", folders_model_);
  folders_view_->updateContent();
  folders_view_->setColour(ListBox::backgroundColourId, Colour(0xff323232));
  addAndMakeVisible(folders_view_);
}

void SaveSection::paint(Graphics& g) {
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  static Font roboto_light(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                             BinaryData::RobotoLight_ttfSize));
  static const DropShadow shadow(Colour(0xff000000), 5, Point<int>(0, 0));

  g.setColour(Colour(0xbb111111));
  g.fillAll();

  Rectangle<int> save_rect = getSaveRect();
  shadow.drawForRectangle(g, save_rect);
  g.setColour(Colour(0xff212121));
  g.fillRect(save_rect);

  g.saveState();
  g.setOrigin(save_rect.getX() + PADDING_X, save_rect.getY() + PADDING_Y);

  g.restoreState();
}

void SaveSection::resized() {
  Rectangle<int> save_rect = getSaveRect();
  folders_view_->setBounds(save_rect.getX() + PADDING_X, save_rect.getY() + PADDING_Y,
                           140.0f, 200.0f);
}

void SaveSection::mouseUp(const MouseEvent &e) {
  if (!getSaveRect().contains(e.getPosition()))
    setVisible(false);
}

Rectangle<int> SaveSection::getSaveRect() {
  int x = (getWidth() - SAVE_WIDTH) / 2;
  int y = (getHeight() - SAVE_HEIGHT) / 2;
  return Rectangle<int>(x, y, SAVE_WIDTH, SAVE_HEIGHT);
}
