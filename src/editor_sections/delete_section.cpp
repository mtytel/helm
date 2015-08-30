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

#include "delete_section.h"
#include "helm_common.h"
#include "text_look_and_feel.h"

#define DELETE_WIDTH 450
#define DELETE_HEIGHT 160
#define PADDING_X 25
#define PADDING_Y 15

DeleteSection::DeleteSection(String name) : Component(name) {
}

void DeleteSection::paint(Graphics& g) {
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  static Font roboto_light(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                             BinaryData::RobotoLight_ttfSize));
  static const DropShadow shadow(Colour(0xff000000), 5, Point<int>(0, 0));

  g.setColour(Colour(0xbb212121));
  g.fillAll();

  Rectangle<int> save_rect = getDeleteRect();
  shadow.drawForRectangle(g, save_rect);
  g.setColour(Colour(0xff303030));
  g.fillRect(save_rect);

  g.saveState();
  g.setOrigin(save_rect.getX() + PADDING_X, save_rect.getY() + PADDING_Y);
}

void DeleteSection::resized() {
}

void DeleteSection::mouseUp(const MouseEvent &e) {
  if (!getDeleteRect().contains(e.getPosition()))
    setVisible(false);
}

Rectangle<int> DeleteSection::getDeleteRect() {
  int x = (getWidth() - DELETE_WIDTH) / 2;
  int y = (getHeight() - DELETE_HEIGHT) / 2;
  return Rectangle<int>(x, y, DELETE_WIDTH, DELETE_HEIGHT);
}
