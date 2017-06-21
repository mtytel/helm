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
#include "colors.h"
#include "fonts.h"
#include "browser_look_and_feel.h"

#define INFO_HEIGHT 300
#define INFO_WIDTH 600

#define PADDING_X 25
#define PADDING_Y 25
#define AMOUNT_PADDING 5

ContributeSection::ContributeSection(String name) : Overlay(name) {
  give_100_button_ = new ToggleButton("$100");
  addAndMakeVisible(give_100_button_);
  give_100_button_->addListener(this);
  give_100_button_->setLookAndFeel(BrowserLookAndFeel::instance());

  give_50_button_ = new ToggleButton("$50");
  addAndMakeVisible(give_50_button_);
  give_50_button_->addListener(this);
  give_50_button_->setLookAndFeel(BrowserLookAndFeel::instance());

  give_25_button_ = new ToggleButton("$25");
  addAndMakeVisible(give_25_button_);
  give_25_button_->addListener(this);
  give_25_button_->setLookAndFeel(BrowserLookAndFeel::instance());

  give_10_button_ = new ToggleButton("$10");
  addAndMakeVisible(give_10_button_);
  give_10_button_->addListener(this);
  give_10_button_->setLookAndFeel(BrowserLookAndFeel::instance());
  give_10_button_->setToggleState(true, NotificationType::dontSendNotification);

  give_buttons_.insert(give_100_button_);
  give_buttons_.insert(give_50_button_);
  give_buttons_.insert(give_25_button_);
  give_buttons_.insert(give_10_button_);

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

  remind_button_ = new TextButton(TRANS("Remind Me Later"));
  addAndMakeVisible(remind_button_);
  remind_button_->addListener(this);

  never_again_button_ = new TextButton(TRANS("Don't Ask Again"));
  addAndMakeVisible(never_again_button_);
  never_again_button_->addListener(this);
}

void ContributeSection::paint(Graphics& g) {
  static const DropShadow shadow(Colour(0xff000000), 5, Point<int>(0, 0));

  g.setColour(Colors::overlay_screen);
  g.fillAll();

  Rectangle<int> info_rect = getInfoRect();
  shadow.drawForRectangle(g, info_rect);
  g.setColour(Colour(0xff303030));
  g.fillRect(info_rect);

  g.saveState();
  g.setOrigin(info_rect.getX() + PADDING_X, info_rect.getY() + PADDING_Y);



  g.restoreState();
}

void ContributeSection::resized() {
  static const int amount_height = 45;
  static const int pay_height = 60;
  static const int pay_width = 200;
  static const int remind_height = 45;
  Rectangle<int> info_rect = getInfoRect();
  int amount_width = (INFO_WIDTH - 2 * PADDING_X - 4 * AMOUNT_PADDING) / 5;
  int amount_y = info_rect.getY() + 160;

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
                         info_rect.getBottom() - PADDING_Y - pay_height,
                         pay_width, pay_height);
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
  }
}

void ContributeSection::textEditorFocusGained(FocusChangeType cause) {
  for (Button* give_button : give_buttons_)
    give_button->setToggleState(false, NotificationType::dontSendNotification);
}

Rectangle<int> ContributeSection::getInfoRect() {
  int x = (getWidth() - INFO_WIDTH) / 2;
  int y = (getHeight() - INFO_HEIGHT) / 2;
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
