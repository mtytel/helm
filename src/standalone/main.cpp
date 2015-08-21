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

#include "JuceHeader.h"
#include "helm_standalone_editor.h"

class HelmApplication  : public JUCEApplication {
public:
  class MainWindow : public DocumentWindow {
  public:
    MainWindow(String name) : DocumentWindow(name, Colours::lightgrey,
                                             DocumentWindow::closeButton) {
      setUsingNativeTitleBar(true);
      setContentOwned(new HelmStandaloneEditor(), true);
      setResizable(true, true);

      centreWithSize(getWidth(), getHeight());
      setVisible(true);
    }

    void closeButtonPressed() override {
      JUCEApplication::getInstance()->systemRequestedQuit();
    }

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
  };

  HelmApplication() { }

  const String getApplicationName() override { return ProjectInfo::projectName; }
  const String getApplicationVersion() override { return ProjectInfo::versionString; }
  bool moreThanOneInstanceAllowed() override { return true; }

  void initialise(const String& commandLine) override {
    mainWindow = new MainWindow (getApplicationName());
  }

  void shutdown() override {
    mainWindow = nullptr;
  }

  void systemRequestedQuit() override {
    quit();
  }

  void anotherInstanceStarted(const String& commandLine) override {
  }

private:
  ScopedPointer<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(HelmApplication)
