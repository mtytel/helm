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

#include "settings_section.h"

#define INFO_WIDTH 350
#define INFO_HEIGHT 400
#define PADDING 10

SettingsSection::SettingsSection(String name) : Component(name) {
}

void SettingsSection::paint(Graphics& g) {
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  static Font roboto_light(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                             BinaryData::RobotoLight_ttfSize));
  static const DropShadow shadow(Colour(0xff000000), 5, Point<int>(0, 0));

  g.setColour(Colour(0xbb212121));
  g.fillAll();

  Rectangle<int> info_rect = getInfoRect();
  shadow.drawForRectangle(g, info_rect);
  g.setColour(Colour(0xff303030));
  g.fillRect(info_rect);

  g.saveState();
  g.setOrigin(info_rect.getX() + PADDING, info_rect.getY() + PADDING);

  g.restoreState();
}

void SettingsSection::resized() {
  Rectangle<int> info_rect = getInfoRect();
}

void SettingsSection::mouseUp(const MouseEvent &e) {
  if (!getInfoRect().contains(e.getPosition()))
    setVisible(false);
}

Rectangle<int> SettingsSection::getInfoRect() {
  int x = (getWidth() - INFO_WIDTH) / 2;
  int y = (getHeight() - INFO_HEIGHT) / 2;
  return Rectangle<int>(x, y, INFO_WIDTH, INFO_HEIGHT);
}
