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
#ifndef UPDATE_CHECK_SECTION_H
#define UPDATE_CHECK_SECTION_H

#include "JuceHeader.h"

class UpdateMemory : public DeletedAtShutdown {
  public:
    UpdateMemory();
    virtual ~UpdateMemory();

    bool shouldCheck() const { return needs_check_; }
    void check() { needs_check_ = false; }

    JUCE_DECLARE_SINGLETON(UpdateMemory, false)

  private:
    bool needs_check_;
};

class UpdateCheckSection : public Component, public Button::Listener {
  public:
    UpdateCheckSection(String name);
    ~UpdateCheckSection() { }
    void paint(Graphics& g) override;
    void resized() override;

    void buttonClicked(Button* clicked_button) override;
    void mouseUp(const MouseEvent& e) override;

    void checkUpdate();

    Rectangle<int> getUpdateCheckRect();

  private:
    ScopedPointer<TextButton> download_button_;
    ScopedPointer<TextButton> nope_button_;

    String version_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UpdateCheckSection)
};

#endif // UPDATE_CHECK_SECTION_H
