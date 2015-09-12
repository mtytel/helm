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

#include "update_check_section.h"
#include "helm_common.h"
#include "load_save.h"
#include "JuceHeader.h"
#include "text_look_and_feel.h"

#define UPDATE_CHECK_WIDTH 340
#define UPDATE_CHECK_HEIGHT 160
#define PADDING_X 25
#define PADDING_Y 20
#define BUTTON_HEIGHT 30


UpdateCheckSection::UpdateCheckSection(String name) : Component(name) {
  download_button_ = new TextButton(TRANS("Download"));
  download_button_->addListener(this);
  addAndMakeVisible(download_button_);

  nope_button_ = new TextButton(TRANS("Nope"));
  nope_button_->addListener(this);
  addAndMakeVisible(nope_button_);

  checkUpdate();
}

void UpdateCheckSection::paint(Graphics& g) {
  static Font patch_font(Typeface::createSystemTypefaceFor(BinaryData::DroidSansMono_ttf,
                                                           BinaryData::DroidSansMono_ttfSize));
  static Font roboto_light(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                             BinaryData::RobotoLight_ttfSize));
  static const DropShadow shadow(Colour(0xff000000), 5, Point<int>(0, 0));

  g.setColour(Colour(0xbb212121));
  g.fillAll();

  Rectangle<int> delete_rect = getUpdateCheckRect();
  shadow.drawForRectangle(g, delete_rect);
  g.setColour(Colour(0xff303030));
  g.fillRect(delete_rect);

  g.saveState();
  g.setOrigin(delete_rect.getX() + PADDING_X, delete_rect.getY() + PADDING_Y);

  g.setFont(roboto_light.withPointHeight(14.0f));
  g.setColour(Colour(0xffaaaaaa));

  g.drawText(TRANS("There is a new version of Helm!"),
             0, 0.0f, delete_rect.getWidth() - 2 * PADDING_X, 22.0f,
             Justification::centred, false);
  g.drawText(TRANS("Version: ") + version_,
             0, 22.0f, delete_rect.getWidth() - 2 * PADDING_X, 22.0f,
             Justification::centred, false);
  g.drawText(TRANS("Would you like to download it?"),
             0, 54.0f, delete_rect.getWidth() - 2 * PADDING_X, 22.0f,
             Justification::centred, false);

  g.restoreState();
}

void UpdateCheckSection::resized() {
  Rectangle<int> update_rect = getUpdateCheckRect();

  float button_width = (update_rect.getWidth() - 3 * PADDING_X) / 2.0f;
  nope_button_->setBounds(update_rect.getX() + PADDING_X,
                          update_rect.getBottom() - PADDING_Y - BUTTON_HEIGHT,
                          button_width, BUTTON_HEIGHT);
  download_button_->setBounds(update_rect.getX() + button_width + 2 * PADDING_X,
                              update_rect.getBottom() - PADDING_Y - BUTTON_HEIGHT,
                              button_width, BUTTON_HEIGHT);
}

void UpdateCheckSection::buttonClicked(Button *clicked_button) {
  if (clicked_button == download_button_)
    URL("http://tytel.org/helm").launchInDefaultBrowser();
  setVisible(false);
}

void UpdateCheckSection::mouseUp(const MouseEvent &e) {
  if (!getUpdateCheckRect().contains(e.getPosition()))
    setVisible(false);
}

void UpdateCheckSection::checkUpdate() {
  URL version_url("http://tytel.org/static/dist/helm_version.txt");
  version_ = version_url.readEntireTextStream().upToFirstOccurrenceOf("\n", false, false);

  if (!version_.isEmpty() &&
      LoadSave::compareVersionStrings(ProjectInfo::versionString, version_) < 0) {
    repaint();
    setVisible(true);
  }
}

Rectangle<int> UpdateCheckSection::getUpdateCheckRect() {
  int x = (getWidth() - UPDATE_CHECK_WIDTH) / 2;
  int y = (getHeight() - UPDATE_CHECK_HEIGHT) / 2;
  return Rectangle<int>(x, y, UPDATE_CHECK_WIDTH, UPDATE_CHECK_HEIGHT);
}
