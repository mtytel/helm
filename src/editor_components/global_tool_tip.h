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

#pragma once
#ifndef GLOBAL_TOOL_TIP_H
#define GLOBAL_TOOL_TIP_H

#include "JuceHeader.h"

class GlobalToolTip  : public Component, public Timer {
  public:
    GlobalToolTip();
    ~GlobalToolTip();

    void setText(String parameter, String value);
    void timerCallback() override;
    void paint(Graphics& g) override;

  private:
    String shown_parameter_text_;
    String shown_value_text_;
    String parameter_text_;
    String value_text_;
    int64 time_updated_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlobalToolTip)
};

#endif // GLOBAL_TOOL_TIP_H
