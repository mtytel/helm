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

#include "about_section.h"
#include "synth_gui_interface.h"

#define LOGO_WIDTH 128
#define INFO_WIDTH 350
#define INFO_HEIGHT 160

AboutSection::AboutSection(String name) : Component(name) {
  static Font roboto_light(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                             BinaryData::RobotoLight_ttfSize));

  developer_link_ = new HyperlinkButton("Matt Tytel", URL("http://tytel.org"));
  developer_link_->setFont(roboto_light.withPointHeight(12.0f), false);
  developer_link_->setColour(HyperlinkButton::textColourId, Colour(0xffffd740));
  addAndMakeVisible(developer_link_);

  free_software_link_ = new HyperlinkButton(TRANS("Read more about free software."),
                                            URL("http://www.gnu.org/philosophy/free-sw.html"));
  free_software_link_->setFont(roboto_light.withPointHeight(12.0f), false);
  free_software_link_->setColour(HyperlinkButton::textColourId, Colour(0xffffd740));
  addAndMakeVisible(free_software_link_);

#ifndef JucePlugin_Version
  settings_button_ = new ImageButton("settings_button");
  const Desktop::Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  if (display.scale > 1.5) {
    Image icon = ImageCache::getFromMemory(BinaryData::ic_settings_white_24dp_1x_png,
                                           BinaryData::ic_settings_white_24dp_1x_pngSize);
    settings_button_->setImages(false, true, false,
                                icon, 1.0, Colours::transparentBlack,
                                icon, 1.0, Colour(0x77ffffff),
                                icon, 1.0, Colour(0x55000000));
  }
  else {
    Image icon_small = ImageCache::getFromMemory(BinaryData::ic_settings_white_24dp_2x_png,
                                                 BinaryData::ic_settings_white_24dp_2x_pngSize);
    settings_button_->setImages(false, true, false,
                                icon_small, 1.0, Colours::transparentBlack,
                                icon_small, 1.0, Colour(0x77ffffff),
                                icon_small, 1.0, Colour(0x55000000));
  }
  addAndMakeVisible(settings_button_);
  settings_button_->addListener(this);
#endif
}

void AboutSection::paint(Graphics& g) {
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
  int logo_padding = (INFO_HEIGHT - LOGO_WIDTH) / 2;
  g.setOrigin(info_rect.getX() + logo_padding, info_rect.getY() + logo_padding);
  Image helm_small = ImageCache::getFromMemory(BinaryData::helm_icon_128_1x_png,
                                               BinaryData::helm_icon_128_1x_pngSize);
  shadow.drawForImage(g, helm_small);

  const Desktop::Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  if (display.scale > 1.5) {
    Image helm = ImageCache::getFromMemory(BinaryData::helm_icon_128_2x_png,
                                           BinaryData::helm_icon_128_2x_pngSize);
    g.drawImage(helm, 0, 0, 128, 128, 0, 0, 256, 256);
  }
  else
    g.drawImage(helm_small, 0, 0, 128, 128, 0, 0, 128, 128);

  g.setFont(roboto_reg.withPointHeight(32.0));
  g.setColour(Colour(0xff2196f3));
  g.drawText(TRANS("HELM"),
             LOGO_WIDTH + logo_padding - 1.0f, 8.0f,
             info_rect.getWidth(), 32.0f, Justification::left);

  g.setFont(roboto_light.withPointHeight(12.0));
  g.setColour(Colour(0xff666666));
  g.drawText(TRANS("v") + " " + ProjectInfo::versionString,
             LOGO_WIDTH + logo_padding + 90.0f, 14.0f,
             info_rect.getWidth(), 32.0f, Justification::left);

  g.setFont(roboto_light.withPointHeight(12.0));
  g.drawText(TRANS("Developer") + ":",
             LOGO_WIDTH + logo_padding, 56.0f - logo_padding,
             info_rect.getWidth(), 20.0f, Justification::left);

  g.setColour(Colour(0xffaaaaaa));
  g.drawText(TRANS("Helm is free software and"),
             LOGO_WIDTH + logo_padding, 88.0f - logo_padding,
             160.0f, 20.0f, Justification::topLeft);

  g.drawText(TRANS("comes with no warranty."),
             LOGO_WIDTH + logo_padding, 102.0f - logo_padding,
             160.0f, 20.0f, Justification::topLeft);

  g.restoreState();
}

void AboutSection::resized() {
  Rectangle<int> info_rect = getInfoRect();
  developer_link_->setBounds(info_rect.getX() + INFO_HEIGHT + 48.0f, info_rect.getY() + 56.0f,
                             98.0f, 20.0f);
  free_software_link_->setBounds(info_rect.getX() + INFO_HEIGHT - 2.0f, info_rect.getY() + 120.0f,
                                 180.0f, 20.0f);
  if (settings_button_)
    settings_button_->setBounds(info_rect.getRight() - 36.0f, info_rect.getY() + 12.0f, 24.0f, 24.0f);
}

void AboutSection::mouseUp(const MouseEvent &e) {
  if (!getInfoRect().contains(e.getPosition()))
    setVisible(false);
}

void AboutSection::buttonClicked(Button* clicked_button) {
  DialogWindow::LaunchOptions options;

  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  AudioDeviceManager* device_manager =  parent->getAudioDeviceManager();
  if (device_manager == nullptr)
    return;

  options.content.setOwned(new AudioDeviceSelectorComponent(*device_manager,
                                                            0, 0, 1, 2,
                                                            true, false, true, false));
  options.content->setSize (500, 350);
  options.dialogTitle = TRANS("Audio Settings");
  options.dialogBackgroundColour = Colours::darkgrey;
  options.escapeKeyTriggersCloseButton = true;
  options.useNativeTitleBar = true;
  options.resizable = false;
  options.launchAsync();
}

Rectangle<int> AboutSection::getInfoRect() {
  int x = (getWidth() - INFO_WIDTH) / 2;
  int y = (getHeight() - INFO_HEIGHT) / 2;
  return Rectangle<int>(x, y, INFO_WIDTH, INFO_HEIGHT);
}
