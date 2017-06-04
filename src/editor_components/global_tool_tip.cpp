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

#include "global_tool_tip.h"
#include "fonts.h"

#define FRAMES_PER_SECOND 20
#define TIME_TO_STAY_VISIBLE 2000.0

GlobalToolTip::GlobalToolTip() {
  startTimerHz(FRAMES_PER_SECOND);
  time_updated_ = 0;
  setInterceptsMouseClicks(false, false);
}

GlobalToolTip::~GlobalToolTip() { }

void GlobalToolTip::paint(Graphics& g) {
  g.setColour(Colour(0xff383838));
  g.fillRect(0, 0, getWidth(), getHeight() / 2);

  g.setColour(Colour(0xff444444));
  g.fillRect(0, getHeight() / 2, getWidth(), getHeight() / 2);

  g.setColour(Colour(0xffffffff));
  g.setFont(Fonts::instance()->monospace().withPointHeight(proportionOfHeight(0.3f)));
  g.drawText(parameter_text_, 0.0, 0.0,
             getWidth(), proportionOfHeight(0.5), Justification::centred, false);
  g.drawText(value_text_, 0.0, proportionOfHeight(0.5),
             getWidth(), proportionOfHeight(0.5), Justification::centred, false);
}

void GlobalToolTip::setText(String parameter, String value) {
  parameter_text_ = parameter;
  value_text_ = value;
  time_updated_ = Time::currentTimeMillis();
  setVisible(true);
}

void GlobalToolTip::timerCallback() {
  if (shown_parameter_text_ != parameter_text_ || shown_value_text_ != value_text_) {
    shown_value_text_ = value_text_;
    shown_parameter_text_ = parameter_text_;
    repaint();
  }
  else if (isVisible() && Time::currentTimeMillis() - time_updated_ > TIME_TO_STAY_VISIBLE)
    setVisible(false);
}
