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

#include "contribute_section.h"
#include "browser_look_and_feel.h"
#include "colors.h"
#include "fonts.h"
#include "load_save.h"

#define INFO_HEIGHT 280
#define INFO_WIDTH 500

#define PADDING_X 25
#define PADDING_Y 10
#define AMOUNT_PADDING 5

class PayLookAndFeel : public DefaultLookAndFeel {
  public:
    void drawButtonBackground(Graphics& g, Button& button,
                              const Colour &backgroundColour,
                              bool hover,
                              bool is_down) override {
      g.setColour(button.findColour(TextButton::buttonColourId));
      if (hover)
        g.setColour(button.findColour(TextButton::buttonOnColourId));

      g.fillRoundedRectangle(0, 0, button.getWidth(), button.getHeight(), 5.0f);
    }

    void drawButtonText(Graphics& g, TextButton& button,
                        bool hover, bool is_down) override {
      Font font = Fonts::instance()->proportional_regular().withPointHeight(16.0f);

      g.setFont(font);
      g.setColour(button.findColour(TextButton::textColourOffId));
      g.drawFittedText(button.getName(), button.getLocalBounds(), Justification::centred, 1);
    }

    static PayLookAndFeel* instance() {
      static PayLookAndFeel instance;
      return &instance;
    }
};

class NeverAgainLookAndFeel : public DefaultLookAndFeel {
  public:
    void drawButtonBackground(Graphics& g, Button& button,
                              const Colour &backgroundColour,
                              bool hover,
                              bool is_down) override { }

    void drawButtonText(Graphics& g, TextButton& button,
                        bool hover, bool is_down) override {
      Font font = Fonts::instance()->proportional_regular().withPointHeight(11.0f);
      g.setFont(font);

      g.setColour(Colour(0xff666666));
      if (hover)
        g.setColour(Colour(0xff777777));
      g.drawFittedText(button.getName(), button.getLocalBounds(), Justification::centredRight, 1);
    }

    static NeverAgainLookAndFeel* instance() {
      static NeverAgainLookAndFeel instance;
      return &instance;
    }
};

ContributeSection::ContributeSection(String name) : Overlay(name) {
  give_100_button_ = new ToggleButton("$100");
  give_50_button_ = new ToggleButton("$50");
  give_25_button_ = new ToggleButton("$25");
  give_10_button_ = new ToggleButton("$10");

  give_buttons_.insert(give_100_button_);
  give_buttons_.insert(give_50_button_);
  give_buttons_.insert(give_25_button_);
  give_buttons_.insert(give_10_button_);

  for (Button* give_button : give_buttons_) {
    addAndMakeVisible(give_button);
    give_button->setLookAndFeel(BrowserLookAndFeel::instance());
    give_button->addListener(this);
    give_button->setMouseCursor(MouseCursor::PointingHandCursor);
  }

  give_25_button_->setToggleState(true, NotificationType::dontSendNotification);

  custom_amount_ = new CustomAmountEditor("custom amount");
  addAndMakeVisible(custom_amount_);
  custom_amount_->addFocusListener(this);

  custom_amount_->setTextToShowWhenEmpty(TRANS("$5"), Colour(0xff777777));
  custom_amount_->setInputRestrictions(6, "$0123456789.");
  custom_amount_->setFont(Fonts::instance()->proportional_light().withPointHeight(16.0f));
  custom_amount_->setIndents(4, 12);
  custom_amount_->setColour(CaretComponent::caretColourId, Colour(0xff888888));
  custom_amount_->setColour(TextEditor::textColourId, Colour(0xffcccccc));
  custom_amount_->setColour(TextEditor::highlightedTextColourId, Colour(0xffcccccc));
  custom_amount_->setColour(TextEditor::highlightColourId, Colour(0xff888888));
  custom_amount_->setColour(TextEditor::backgroundColourId, Colour(0xff323232));
  custom_amount_->setColour(TextEditor::outlineColourId, Colour(0xff888888));
  custom_amount_->setColour(TextEditor::focusedOutlineColourId, Colour(0xffffab00));

  pay_button_ = new TextButton(TRANS("Pay"));
  addAndMakeVisible(pay_button_);
  pay_button_->addListener(this);
  pay_button_->setMouseCursor(MouseCursor::PointingHandCursor);
  pay_button_->setLookAndFeel(PayLookAndFeel::instance());
  pay_button_->setColour(TextButton::buttonColourId, Colors::audio);
  pay_button_->setColour(TextButton::buttonOnColourId, Colour(0xff55bbff));
  pay_button_->setColour(TextButton::textColourOffId, Colours::white);

  remind_button_ = new TextButton(TRANS("Remind Me Later"));
  addAndMakeVisible(remind_button_);
  remind_button_->addListener(this);
  remind_button_->setMouseCursor(MouseCursor::PointingHandCursor);
  remind_button_->setLookAndFeel(PayLookAndFeel::instance());
  remind_button_->setColour(TextButton::buttonColourId, Colour(0xff484848));
  remind_button_->setColour(TextButton::buttonOnColourId, Colour(0xff555555));
  remind_button_->setColour(TextButton::textColourOffId, Colour(0xff999999));

  never_again_button_ = new TextButton(TRANS("Don't Ask Again"));
  never_again_button_->setLookAndFeel(NeverAgainLookAndFeel::instance());
  never_again_button_->setMouseCursor(MouseCursor::PointingHandCursor);
  addAndMakeVisible(never_again_button_);
  never_again_button_->addListener(this);
}

void ContributeSection::paint(Graphics& g) {
  static const DropShadow shadow(Colour(0xff000000), 5, Point<int>(0, 0));
  static const int logo_padding = 0;
  static const int y_shift = 10;
  static const int logo_width = 256;

  g.setColour(Colors::overlay_screen);
  g.fillAll();

  Rectangle<int> info_rect = getInfoRect();

  int icon_x = info_rect.getX() + (info_rect.getWidth() - logo_width) / 2;
  int icon_y = info_rect.getY() - 0.8 * logo_width;
  int circle_width = logo_width + 2 * logo_padding;
  Rectangle<float> icon_border_rect = Rectangle<float>(icon_x - logo_padding,
                                                       icon_y - logo_padding + y_shift,
                                                       circle_width - 1, circle_width);

  shadow.drawForRectangle(g, info_rect);
  Path shadow_path;
  shadow_path.addEllipse(icon_border_rect);
  shadow.drawForPath(g, shadow_path);

  g.setColour(Colour(0xff303030));
  g.fillRect(info_rect);
  g.fillEllipse(icon_border_rect);

  g.saveState();
  g.setOrigin(icon_x, icon_y);

  Image helm_small = ImageCache::getFromMemory(BinaryData::helm_icon_256_1x_png,
                                               BinaryData::helm_icon_256_1x_pngSize);
  shadow.drawForImage(g, helm_small);

  const Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  if (display.scale > 1.5) {
    Image helm = ImageCache::getFromMemory(BinaryData::helm_icon_256_2x_png,
                                           BinaryData::helm_icon_256_2x_pngSize);
    g.drawImage(helm, 0, 0, logo_width, logo_width, 0, 0, 2 * logo_width, 2 * logo_width);
  }
  else
    g.drawImage(helm_small, 0, 0, logo_width, logo_width, 0, 0, logo_width, logo_width);

  g.restoreState();

  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(24.0));
  g.setColour(Colour(0xff2196f3));
  g.drawText(TRANS("Hope you're enjoying Helm!"),
             info_rect.getX(), info_rect.getY() + 40,
             info_rect.getWidth(), 32.0f, Justification::centred);

  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(16.0));
  g.setColour(Colour(0xff888888));
  g.drawText(TRANS("Pay what you can to keep the project going"),
             info_rect.getX(), info_rect.getY() + 80,
             info_rect.getWidth(), 16.0f, Justification::centred);
}

void ContributeSection::resized() {
  static const int amount_height = 45;
  static const int pay_height = 50;
  static const int pay_padding = 50;
  static const int pay_width = (INFO_WIDTH - 2 * PADDING_X - pay_padding) / 2;
  static const int never_again_height = 25;
  static const int never_again_width = 100;
  static const int remind_width = INFO_WIDTH - 2 * PADDING_X - pay_width - pay_padding;

  Rectangle<int> info_rect = getInfoRect();
  int amount_width = (INFO_WIDTH - 2 * PADDING_X - 4 * AMOUNT_PADDING) / 5;
  int amount_y = info_rect.getY() + 130;

  give_100_button_->setBounds(info_rect.getX() + PADDING_X, amount_y, amount_width, amount_height);
  give_50_button_->setBounds(give_100_button_->getRight() + AMOUNT_PADDING, amount_y,
                             amount_width, amount_height);
  give_25_button_->setBounds(give_50_button_->getRight() + AMOUNT_PADDING, amount_y,
                             amount_width, amount_height);
  give_10_button_->setBounds(give_25_button_->getRight() + AMOUNT_PADDING, amount_y,
                             amount_width, amount_height);
  custom_amount_->setBounds(give_10_button_->getRight() + AMOUNT_PADDING, amount_y,
                            amount_width, amount_height);

  pay_button_->setBounds(info_rect.getX() + PADDING_X,
                         info_rect.getBottom() - PADDING_Y - pay_height - never_again_height,
                         pay_width, pay_height);

  remind_button_->setBounds(info_rect.getRight() - PADDING_X - remind_width, pay_button_->getY(),
                            remind_width, pay_height);

  never_again_button_->setBounds(remind_button_->getRight() - never_again_width,
                                 remind_button_->getBottom(),
                                 never_again_width, never_again_height);
}

void ContributeSection::mouseUp(const MouseEvent &e) {
  if (!getInfoRect().contains(e.getPosition()))
    setVisible(false);
}

void ContributeSection::buttonClicked(Button* clicked_button) {
  if (give_buttons_.count(clicked_button)) {
    for (Button* give_button : give_buttons_) {
      if (give_button != clicked_button)
        give_button->setToggleState(false, NotificationType::dontSendNotification);
    }
  }
  else if (clicked_button == pay_button_) {
    URL url = getUrl();
    url.launchInDefaultBrowser();
    LoadSave::saveShouldAskForMoney(false);
  }
  else if (clicked_button == remind_button_) {
    setVisible(false);
    LoadSave::saveLastAskedForMoney();
  }
  else if (clicked_button == never_again_button_) {
    setVisible(false);
    LoadSave::saveShouldAskForMoney(false);
  }
}

void ContributeSection::textEditorFocusGained(FocusChangeType cause) {
  for (Button* give_button : give_buttons_)
    give_button->setToggleState(false, NotificationType::dontSendNotification);
}

Rectangle<int> ContributeSection::getInfoRect() {
  int x = (getWidth() - INFO_WIDTH) / 2;
  int y = (getHeight() - INFO_HEIGHT) / 2 + 50;
  return Rectangle<int>(x, y, INFO_WIDTH, INFO_HEIGHT);
}

URL ContributeSection::getUrl() {
  String amount;
  for (Button* give_button : give_buttons_) {
    if (give_button->getToggleState())
      amount = give_button->getName();
  }
  if (amount == "")
    amount = custom_amount_->getText();

  amount = URL::addEscapeChars(amount.replace("$", ""), true);
  return URL(String("http://tytel.org/helm/paylater/?amount=") + amount);
}
