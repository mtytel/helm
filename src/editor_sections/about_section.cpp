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

#include "about_section.h"
#include "colors.h"
#include "fonts.h"
#include "helm_common.h"
#include "load_save.h"
#include "synth_gui_interface.h"
#include "synth_section.h"
#include "text_look_and_feel.h"

#define LOGO_WIDTH 128
#define INFO_WIDTH 470
#define STANDALONE_INFO_HEIGHT 550
#define PLUGIN_INFO_HEIGHT 227
#define PADDING_X 25
#define PADDING_Y 15
#define BUTTON_WIDTH 16

#define MULT_SMALL 0.75f
#define MULT_LARGE 1.35f
#define MULT_EXTRA_LARGE 2.0f

AboutSection::AboutSection(String name) : Overlay(name) {
  developer_link_ = new HyperlinkButton("Matt Tytel", URL("http://tytel.org"));
  developer_link_->setFont(Fonts::instance()->proportional_light().withPointHeight(16.0f),
                           false, Justification::right);
  developer_link_->setColour(HyperlinkButton::textColourId, Colour(0xffffd740));
  addAndMakeVisible(developer_link_);

  free_software_link_ = new HyperlinkButton(TRANS("Read more about free software"),
                                            URL("http://www.gnu.org/philosophy/free-sw.html"));
  free_software_link_->setFont(Fonts::instance()->proportional_light().withPointHeight(12.0f),
                               false, Justification::right);
  free_software_link_->setColour(HyperlinkButton::textColourId, Colour(0xffffd740));
  addAndMakeVisible(free_software_link_);

  check_for_updates_ = new ToggleButton();
  check_for_updates_->setToggleState(LoadSave::shouldCheckForUpdates(),
                                     NotificationType::dontSendNotification);
  check_for_updates_->setLookAndFeel(TextLookAndFeel::instance());
  check_for_updates_->addListener(this);
  addAndMakeVisible(check_for_updates_);

  animate_ = new ToggleButton();
  animate_->setToggleState(LoadSave::shouldAnimateWidgets(),
                           NotificationType::dontSendNotification);
  animate_->setLookAndFeel(TextLookAndFeel::instance());
  animate_->addListener(this);
  addAndMakeVisible(animate_);

  size_button_small_ = new TextButton(String(100 * MULT_SMALL) + "%");
  addAndMakeVisible(size_button_small_);
  size_button_small_->addListener(this);

  size_button_normal_ = new TextButton(String("100") + "%");
  addAndMakeVisible(size_button_normal_);
  size_button_normal_->addListener(this);

  size_button_large_ = new TextButton(String(100 * MULT_LARGE) + "%");
  addAndMakeVisible(size_button_large_);
  size_button_large_->addListener(this);

  size_button_extra_large_ = new TextButton(String(100 * MULT_EXTRA_LARGE) + "%");
  addAndMakeVisible(size_button_extra_large_);
  size_button_extra_large_->addListener(this);

  size_button_extra_large_->setLookAndFeel(DefaultLookAndFeel::instance());
}

void AboutSection::paint(Graphics& g) {
  static const DropShadow shadow(Colour(0xff000000), 5, Point<int>(0, 0));

  g.setColour(Colors::overlay_screen);
  g.fillAll();

  Rectangle<int> info_rect = getInfoRect();
  shadow.drawForRectangle(g, info_rect);
  g.setColour(Colour(0xff303030));
  g.fillRect(info_rect);

  g.saveState();
  g.setOrigin(info_rect.getX() + PADDING_X, info_rect.getY() + PADDING_Y);
  Image helm_small = ImageCache::getFromMemory(BinaryData::helm_icon_128_1x_png,
                                               BinaryData::helm_icon_128_1x_pngSize);
  shadow.drawForImage(g, helm_small);

  const Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  if (display.scale > 1.5) {
    Image helm = ImageCache::getFromMemory(BinaryData::helm_icon_128_2x_png,
                                           BinaryData::helm_icon_128_2x_pngSize);
    g.drawImage(helm, 0, 0, 128, 128, 0, 0, 256, 256);
  }
  else
    g.drawImage(helm_small, 0, 0, 128, 128, 0, 0, 128, 128);

  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(32.0));
  g.setColour(Colour(0xff2196f3));
  g.drawText(TRANS("HELM"),
             0.0f, 0.0f,
             info_rect.getWidth() - 2 * PADDING_X, 32.0f, Justification::centredTop);

  g.setFont(Fonts::instance()->proportional_light().withPointHeight(12.0));
  g.setColour(Colour(0xff666666));
  g.drawText(TRANS("v") + " " + ProjectInfo::versionString,
             0.0f, 36.0f,
             info_rect.getWidth() - 2 * PADDING_X, 32.0f, Justification::centredTop);

  g.setFont(Fonts::instance()->proportional_light().withPointHeight(12.0));
  g.drawText(TRANS("Developed by"),
             0.0f, 4.0f,
             info_rect.getWidth() - 2 * PADDING_X, 20.0f, Justification::right);

  g.setColour(Colour(0xffaaaaaa));
  g.drawText(TRANS("Helm is free software and"),
             0.0f, 62.0,
             info_rect.getWidth() - 2 * PADDING_X, 20.0f, Justification::topRight);

  g.drawText(TRANS("comes with no warranty"),
             0.0f, 76.0f,
             info_rect.getWidth() - 2 * PADDING_X, 20.0f, Justification::topRight);

  g.setFont(Fonts::instance()->proportional_light().withPointHeight(12.0));
  g.drawText(TRANS("Check for updates"),
             0.0f, 141.0f,
             info_rect.getWidth() - 2 * PADDING_X - 1.5 * BUTTON_WIDTH,
             20.0f, Justification::topRight);
  g.drawText(TRANS("Animate graphics"),
             0.0f, 141.0f,
             273.0f - PADDING_X - 0.5 * BUTTON_WIDTH,
             20.0f, Justification::topRight);
  g.drawText(TRANS("Window size"),
             0.0f, 180.0f,
             155.0f,
             20.0f, Justification::topRight);

  g.restoreState();
}

void AboutSection::resized() {
  static const float software_link_width = 200.0f;
  static const float developer_link_width = 120.0f;

  Rectangle<int> info_rect = getInfoRect();
  developer_link_->setBounds(info_rect.getRight() - PADDING_X - developer_link_width,
                             info_rect.getY() + PADDING_Y + 24.0f, developer_link_width, 20.0f);

  free_software_link_->setBounds(info_rect.getRight() - PADDING_X - software_link_width,
                                 info_rect.getY() + PADDING_Y + 105.0f, software_link_width, 20.0f);

  check_for_updates_->setBounds(info_rect.getRight() - PADDING_X - BUTTON_WIDTH,
                                info_rect.getY() + PADDING_Y + 140.0f, BUTTON_WIDTH, BUTTON_WIDTH);

  animate_->setBounds(info_rect.getX() + 273.0f,
                      info_rect.getY() + PADDING_Y + 140.0f, BUTTON_WIDTH, BUTTON_WIDTH);

  int size_y = animate_->getBottom() + PADDING_Y;
  int size_height = 2 * BUTTON_WIDTH;
  int size_width = 60;
  int size_padding = 5;
  size_button_extra_large_->setBounds(info_rect.getRight() - PADDING_X - size_width, size_y,
                                      size_width, size_height);
  size_button_large_->setBounds(size_button_extra_large_->getX() - size_padding - size_width,
                                size_y, size_width, size_height);
  size_button_normal_->setBounds(size_button_large_->getX() - size_padding - size_width, size_y,
                                 size_width, size_height);
  size_button_small_->setBounds(size_button_normal_->getX() - size_padding - size_width, size_y,
                                size_width, size_height);

  if (device_selector_) {
    int y = size_button_extra_large_->getBottom() + PADDING_Y;
    device_selector_->setBounds(info_rect.getX(), y,
                                info_rect.getWidth(), info_rect.getBottom() - y);
  }
}

void AboutSection::mouseUp(const MouseEvent &e) {
  if (!getInfoRect().contains(e.getPosition()))
    setVisible(false);
}

void AboutSection::setVisible(bool should_be_visible) {
  if (should_be_visible && device_selector_.get() == nullptr) {
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    AudioDeviceManager* device_manager = parent->getAudioDeviceManager();
    if (device_manager) {
      device_selector_ = new AudioDeviceSelectorComponent(*device_manager, 0, 0,
                                                          mopo::NUM_CHANNELS, mopo::NUM_CHANNELS,
                                                          true, false, false, false);
      device_selector_->setLookAndFeel(TextLookAndFeel::instance());
      addAndMakeVisible(device_selector_);
      Rectangle<int> info_rect = getInfoRect();
      int y = info_rect.getY() + LOGO_WIDTH + 2 * PADDING_Y;
      device_selector_->setBounds(info_rect.getX(), y,
                                  info_rect.getWidth(), info_rect.getBottom() - y);
      resized();
    }
  }

  Overlay::setVisible(should_be_visible);
}

void AboutSection::buttonClicked(Button* clicked_button) {
  if (clicked_button == check_for_updates_)
    LoadSave::saveUpdateCheckConfig(check_for_updates_->getToggleState());
  else if (clicked_button == animate_) {
    LoadSave::saveAnimateWidgets(animate_->getToggleState());

    SynthSection* parent = findParentComponentOfClass<SynthSection>();
    for (SynthSection* s = parent; s; s = parent->findParentComponentOfClass<SynthSection>())
      parent = s;

    parent->animate(animate_->getToggleState());
  }
  else if (clicked_button == size_button_small_)
    setGuiSize(MULT_SMALL);
  else if (clicked_button == size_button_normal_)
    setGuiSize(1.0);
  else if (clicked_button == size_button_large_)
    setGuiSize(MULT_LARGE);
  else if (clicked_button == size_button_extra_large_)
    setGuiSize(MULT_EXTRA_LARGE);
}

Rectangle<int> AboutSection::getInfoRect() {
  int info_height = device_selector_ ? STANDALONE_INFO_HEIGHT : PLUGIN_INFO_HEIGHT;
  int x = (getWidth() - INFO_WIDTH) / 2;
  int y = (getHeight() - info_height) / 2;
  return Rectangle<int>(x, y, INFO_WIDTH, info_height);
}

void AboutSection::setGuiSize(float multiplier) {
  float percent = sqrtf(multiplier);
  LoadSave::saveWindowSize(percent);

  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent) {
    parent->setGuiSize(percent * mopo::DEFAULT_WINDOW_WIDTH,
                       percent * mopo::DEFAULT_WINDOW_HEIGHT);
  }
}
