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

#include "JuceHeader.h"
#include "border_bounds_constrainer.h"
#include "helm_editor.h"
#include "load_save.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_hints.h>
//#include <Python.h>

static int gamepadButtons[12] = {0,0,0,0, 0,0,0,0, 0,0,0,0};

class UpdateGamepad: private Timer { 
  public:
    HelmEditor* editor;
    SDL_Joystick *m_joystick;
    int gamepadHat = 0;
    int prevNote = -1;

    UpdateGamepad( HelmEditor* editor_, SDL_Joystick *joystick ) {
      this->editor = editor_;
      this->m_joystick = joystick;
      //startTimer(30);  // too fast? segfaults sometimes
      startTimer(60);
    }
    ~UpdateGamepad() {}
    void timerCallback() override {
      SDL_PumpEvents();  // poll event not required when calling pump
      int hat = SDL_JoystickGetHat(m_joystick,0);
      if (hat != this->gamepadHat) {
        std::cout << hat << std::endl;
        switch (hat) {
          case 1: // up
            this->editor->noteOn( 64 );
            break;
          case 2: // right
            this->editor->nextPatch();
            break;
          case 4: // down
            this->editor->noteOff( 64 );
            break;
          case 8: // left
            this->editor->prevPatch();
            break;
        }
        //if (hat==0)
        //  emit gamepadHatReleased();
        //else
        //  emit gamepadHatPressed(hat);
      }
      this->gamepadHat = hat;
      float x1 = ((double)SDL_JoystickGetAxis(m_joystick, 0)) / 32768.0;
      float y1 = ((double)SDL_JoystickGetAxis(m_joystick, 1)) / 32768.0;
      //double z1 = (((double)SDL_JoystickGetAxis(m_joystick, 2)) / 32768.0) + 1.0;
      float x2 = ((double)SDL_JoystickGetAxis(m_joystick, 2)) / 32768.0;
      float y2 = ((double)SDL_JoystickGetAxis(m_joystick, 3)) / 32768.0;
      //double z2 = (((double)SDL_JoystickGetAxis(m_joystick, 5)) / 32768.0) + 1.0;
      this->editor->updateGamepad( x1,y1, x2,y2 );
      //auto keyboard = this->editor->getComputerKeyboard();
      for (int i=0; i<12; i++) {
        if (SDL_JoystickGetButton(m_joystick, i)) {
          if (gamepadButtons[i]==0) {
            gamepadButtons[i] = 1;
            this->editor->noteOn( 64 );
          }
        } else {
          if (gamepadButtons[i]==1) {
            gamepadButtons[i] = 0;
            this->editor->noteOff( 64 );
          }
        }
      }
    }
};


class HelmApplication : public JUCEApplication {
  public:
    class MainWindow : public DocumentWindow,
                       public ApplicationCommandTarget,
                       private AsyncUpdater {
    public:
      enum PatchCommands {
        kSave = 0x5001,
        kSaveAs,
        kOpen,
      };

      MainWindow(String name, bool visible = true) :
          DocumentWindow(name, Colour(0x11000000)/*Colours::lightgrey*/, DocumentWindow::allButtons, visible) {
        editor_ = new HelmEditor(visible);
        if (visible) {
          editor_->animate(LoadSave::shouldAnimateWidgets());

          setUsingNativeTitleBar(true);
          setContentOwned(editor_, true);
          setResizable(true, true);
          /*
          constrainer_.setMinimumSize(2 * mopo::DEFAULT_WINDOW_WIDTH / 3,
                                      2 * mopo::DEFAULT_WINDOW_HEIGHT / 3);
          constrainer_.setBorder(getPeer()->getFrameSize());
          double ratio = (1.0 * mopo::DEFAULT_WINDOW_WIDTH) / mopo::DEFAULT_WINDOW_HEIGHT;

          constrainer_.setFixedAspectRatio(ratio);
          setConstrainer(&constrainer_);
          */
          //centreWithSize(getWidth(), getHeight());
          setVisible(visible);
          triggerAsyncUpdate();

          //SDL_Init(SDL_INIT_JOYSTICK);
          SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS,"1");  // required in SDL2 when initialized before display
          SDL_InitSubSystem(SDL_INIT_JOYSTICK);
          // Check for joystick
          if (SDL_NumJoysticks() > 0) {
            // Open joystick
            auto m_joystick = SDL_JoystickOpen(0);
            if (m_joystick) {
              printf("Opened Gamepad 0\n");
              printf("Name: %s\n", SDL_JoystickNameForIndex(0));  // SDL2
              printf("Number of Axes: %d\n", SDL_JoystickNumAxes(m_joystick));
              printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(m_joystick));
              printf("Number of Balls: %d\n", SDL_JoystickNumBalls(m_joystick));
              printf("Number of Hats: %d\n", SDL_JoystickNumHats(m_joystick));
              auto gamepad = new UpdateGamepad(editor_, m_joystick);
            } else {
              printf("Couldn't open gamepad 0\n");
            }
          } else {
            printf("No gamepads are attached\n"); 
          }
        }
        else
          editor_->animate(false);
      }

      void closeButtonPressed() override {
        JUCEApplication::getInstance()->systemRequestedQuit();
      }

      bool loadFile(File file) {
        bool success = editor_->loadFromFile(file);
        if (success)
          editor_->externalPatchLoaded(file);
        return success;
      }

      ApplicationCommandTarget* getNextCommandTarget() override {
        return findFirstTargetParentComponent();
      }

      void getAllCommands(Array<CommandID>& commands) override {
        commands.add(kSave);
        commands.add(kSaveAs);
        commands.add(kOpen);
      }

      void getCommandInfo(const CommandID commandID, ApplicationCommandInfo& result) override {
        if (commandID == kSave) {
          result.setInfo(TRANS("Save"), TRANS("Saves the current patch"), "Application", 0);
          result.defaultKeypresses.add(KeyPress('s', ModifierKeys::commandModifier, 0));
        }
        else if (commandID == kSaveAs) {
          result.setInfo(TRANS("Save As"), TRANS("Saves patch to a new file"), "Application", 0);
          ModifierKeys modifier = ModifierKeys::commandModifier | ModifierKeys::shiftModifier;
          result.defaultKeypresses.add(KeyPress('s', modifier, 0));
        }
        else if (commandID == kOpen) {
          result.setInfo(TRANS("Open"), TRANS("Opens a patch"), "Application", 0);
          result.defaultKeypresses.add(KeyPress('o', ModifierKeys::commandModifier, 0));
        }
      }

      void open() {
        File active_file = editor_->getActiveFile();
        FileChooser open_box("Open Patch", File(),
                             String("*.") + mopo::PATCH_EXTENSION);
        if (open_box.browseForFileToOpen())
          loadFile(open_box.getResult());
      }

      bool perform(const InvocationInfo& info) override {
        if (info.commandID == kSave) {
          if (!editor_->saveToActiveFile())
            editor_->exportToFile();
          grabKeyboardFocus();
          editor_->setFocus();
          return true;
        }
        if (info.commandID == kSaveAs) {
          editor_->exportToFile();
          grabKeyboardFocus();
          editor_->setFocus();
          return true;
        }
        if (info.commandID == kOpen) {
          open();
          grabKeyboardFocus();
          editor_->setFocus();
          return true;
        }

        return false;
      }

      void handleAsyncUpdate() override {
        command_manager_ = new ApplicationCommandManager();
        command_manager_->registerAllCommandsForTarget(JUCEApplication::getInstance());
        command_manager_->registerAllCommandsForTarget(this);
        addKeyListener(command_manager_->getKeyMappings());
        editor_->setFocus();
      }

      void activeWindowStatusChanged() override {
        if (editor_)
          editor_->animate(LoadSave::shouldAnimateWidgets() && isActiveWindow());

        if (isActiveWindow())
          editor_->grabKeyboardFocus();
      }

    private:
      JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
      HelmEditor* editor_;
      ScopedPointer<ApplicationCommandManager> command_manager_;
      //BorderBoundsConstrainer constrainer_;
    };

    HelmApplication() { }

    const String getApplicationName() override { return ProjectInfo::projectName; }
    const String getApplicationVersion() override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const String& command_line) override {
      String command = " " + command_line + " ";
      if (command.contains(" --version ") || command.contains(" -v ")) {
        std::cout << getApplicationName() << " " << getApplicationVersion() << newLine;
        quit();
      }
      else if (command.contains(" --help ") || command.contains(" -h ")) {
        std::cout << "Usage:" << newLine;
        std::cout << "  " << getApplicationName().toLowerCase() << " [OPTION...]" << newLine << newLine;
        std::cout << getApplicationName() << " polyphonic, semi-modular synthesizer." << newLine << newLine;
        std::cout << "Help Options:" << newLine;
        std::cout << "  -h, --help                          Show help options" << newLine << newLine;
        std::cout << "Application Options:" << newLine;
        std::cout << "  -v, --version                       Show version information and exit" << newLine;
        std::cout << "  --headless                          Run without graphical interface." << newLine << newLine;
        quit();
      }
      else {
        bool visible = !command.contains(" --headless ");
        main_window_ = new MainWindow(getApplicationName(), visible);

        StringArray args = getCommandLineParameterArray();
        File file;

        for (int i = 0; i < args.size(); ++i) {
          if (args[i] != "" && args[i][0] != '-' && loadFromCommandLine(args[i]))
            return;
        }
      }
    }

    bool loadFromCommandLine(const String& command_line) {
      String file_path = command_line;
      if (file_path[0] == '"' && file_path[file_path.length() - 1] == '"')
        file_path = command_line.substring(1, command_line.length() - 1);
      File file = File::getCurrentWorkingDirectory().getChildFile(file_path);
      if (file.exists())
        return main_window_->loadFile(file);
      return false;
    }

    void shutdown() override {
      main_window_ = nullptr;
    }

    void systemRequestedQuit() override {
      quit();
    }

    void anotherInstanceStarted(const String& command_line) override {
      loadFromCommandLine(command_line);
    }

  private:
    ScopedPointer<MainWindow> main_window_;
};

START_JUCE_APPLICATION(HelmApplication)
