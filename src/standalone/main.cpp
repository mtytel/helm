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
static int gamepad_offset = 0;

class UpdateGamepad: private Timer { 
  public:
    HelmEditor* editor;
    SDL_Joystick *primary_joystick;
    SDL_Joystick *secondary_joystick;
    SDL_Joystick *third_joystick;
    //SDL_Joystick *fourth_joystick;
    int gamepadHat = 0;
    int prevNote = -1;
    bool analogKeyboard = false;
    int analogKeyboardOffset = 0;

    UpdateGamepad( HelmEditor* editor_, SDL_Joystick* pad1, SDL_Joystick* pad2, SDL_Joystick* pad3) {
      this->editor = editor_;
      this->primary_joystick = pad1;
      this->secondary_joystick = pad2;
      this->third_joystick = pad3;
      if (this->secondary_joystick) {
        this->analogKeyboard = true;
        this->editor->linkGamepadAxis(std::string("osc_feedback_amount"), 1);
        this->editor->linkGamepadAxis(std::string("cutoff"), 2);
        this->editor->linkGamepadAxis(std::string("portamento"), 3);
        this->editor->linkGamepadAxis(std::string("resonance"), 3);

        this->editor->linkGamepadButton(std::string("formant_on"), 0);
        this->editor->linkGamepadButton(std::string("arp_on"), 1);
        this->editor->linkGamepadButton(std::string("distortion_on"), 2);
        this->editor->linkGamepadButton(std::string("reverb_on"), 3);
        this->editor->linkGamepadButton(std::string("delay_on"), 3);
        this->editor->linkGamepadButton(std::string("filter_on"), 4);
        this->editor->linkGamepadButton(std::string("stutter_on"), 5);

        this->editor->linkGamepadAxis(std::string("arp_gate"), 4);
        this->editor->linkGamepadAxis(std::string("sub_shuffle"), 4);
        this->editor->linkGamepadAxis(std::string("beats_per_minute"), 5);
        this->editor->linkGamepadAxis(std::string("formant_x"), 6);
        this->editor->linkGamepadAxis(std::string("formant_y"), 7);
      }
      if (this->third_joystick) {
        this->editor->linkGamepadAxis(std::string("osc_1_tune"), 8);
        //this->editor->linkGamepadAxis(std::string("osc_1_transpose"), 9);
        //this->editor->linkGamepadAxis(std::string("osc_1_volume"), 9);
        this->editor->linkGamepadAxis(std::string("delay_feedback"), 9);

        this->editor->linkGamepadAxis(std::string("osc_2_tune"), 10);
        //this->editor->linkGamepadAxis(std::string("osc_2_transpose"), 11);
        //this->editor->linkGamepadAxis(std::string("osc_2_volume"), 11);
        this->editor->linkGamepadAxis(std::string("cross_modulation"), 11);
      }
      //startTimer(30);  // too fast? segfaults sometimes
      startTimer(60);
    }
    ~UpdateGamepad() {}
    void timerCallback() override {
      // secondary gamepad
      float x3 = 0.0;
      float y3 = 0.0;
      float x4 = 0.0;
      float y4 = 0.0;
      // 3rd gamepad
      float x5 = 0.0;
      float y5 = 0.0;
      float x6 = 0.0;
      float y6 = 0.0;

      int btns[12];
      SDL_PumpEvents();  // poll event not required when calling pump
      int hat = SDL_JoystickGetHat(this->primary_joystick, 0);
      bool button_lock = hat == 1;  // if hat is up
      if (hat != this->gamepadHat) {
        std::cout << hat << std::endl;
        switch (hat) {
          case 1: // up
            //this->editor->noteOn( 64 );
            this->analogKeyboard = true;
            break;
          case 2: // right
            this->editor->nextPatch();
            break;
          case 4: // down
            this->editor->noteOff( this->prevNote );
            this->analogKeyboard = false;
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
      float x1 = ((double)SDL_JoystickGetAxis(this->primary_joystick, 0)) / 32768.0;
      float y1 = ((double)SDL_JoystickGetAxis(this->primary_joystick, 1)) / 32768.0;
      //double z1 = (((double)SDL_JoystickGetAxis(m_joystick, 2)) / 32768.0) + 1.0;  // xbox gamepads have 6 axes
      float x2 = ((double)SDL_JoystickGetAxis(this->primary_joystick, 2)) / 32768.0;
      float y2 = ((double)SDL_JoystickGetAxis(this->primary_joystick, 3)) / 32768.0;
      //double z2 = (((double)SDL_JoystickGetAxis(m_joystick, 5)) / 32768.0) + 1.0;

      if (this->secondary_joystick) {
        x3 = ((double)SDL_JoystickGetAxis(this->secondary_joystick, 0)) / 32768.0;
        y3 = ((double)SDL_JoystickGetAxis(this->secondary_joystick, 1)) / 32768.0;
        x4 = ((double)SDL_JoystickGetAxis(this->secondary_joystick, 2)) / 32768.0;
        y4 = ((double)SDL_JoystickGetAxis(this->secondary_joystick, 3)) / 32768.0;
      }

      if (this->third_joystick) {
        x5 = ((double)SDL_JoystickGetAxis(this->third_joystick, 0)) / 32768.0;
        y5 = ((double)SDL_JoystickGetAxis(this->third_joystick, 1)) / 32768.0;
        x6 = ((double)SDL_JoystickGetAxis(this->third_joystick, 2)) / 32768.0;
        y6 = ((double)SDL_JoystickGetAxis(this->third_joystick, 3)) / 32768.0;
      }


      for (int i=0; i<12; i++) {
        btns[i] = 0;
        if (SDL_JoystickGetButton(this->primary_joystick, i)) {
          if (gamepadButtons[i]==0) {
            gamepadButtons[i] = 1;
            btns[i] = 1;
            //if (this->analogKeyboard)
            //  this->editor->noteOn( this->prevNote + i - 6 );
          } else {
            gamepadButtons[i] += 1;
            btns[i] = gamepadButtons[i];
          }
        } else {
          if (gamepadButtons[i] >= 1) {
            gamepadButtons[i] = 0;
            btns[i] = -1;
            //if (this->analogKeyboard)
            //  this->editor->noteOff( this->prevNote + i - 6 );
          }
        }
      }

      if (this->analogKeyboard) {
        float x2mult = 8.0;

        if (btns[4] >= 1){
          this->analogKeyboardOffset = static_cast<int>( (x2+0.5) * 40);
        }
        else if (btns[5] >= 1) {
              x2mult *= 0.3;
        }
        if (btns[6] >= 1)
              x2mult *= 2;
        if (btns[7] >= 1)
              x2mult *= 3;

        /*
        else if (btns[8]==1)
              this->analogKeyboardOffset -= 1;
        else if (btns[9]==1)
              this->analogKeyboardOffset += 1;
        */

        //int keyindex = static_cast<int>( (x1 + x2) * 64.0 );
        //int keyindex = static_cast<int>( ((x1+0.5)*64.0) + (x2*32.0) );
        int keyindex = static_cast<int>( ((x1+1.0)*64.0) + (x2*x2mult) );
        keyindex += this->analogKeyboardOffset;
        if (keyindex != this->prevNote) {
          if (this->prevNote != -1)
            this->editor->noteOff( this->prevNote );
          this->editor->noteOn( keyindex, -y2+1.1 );
          this->prevNote = keyindex;
        }
      }


      // updates slider buttons set by user from UI
      this->editor->updateGamepad(
        x1,y1, x2,y2, // primary gamepad
        x3,y3, x4,y4, // optional secondary gamepad
        x5,y5, x6,y6, // optional 3rd gamepad
        btns[0], 
        btns[1], 
        btns[2], 
        btns[3], 
        btns[4], 
        btns[5], 
        btns[6], 
        btns[7], 
        btns[8], 
        btns[9], 
        btns[10], 
        btns[11],
        button_lock
      );
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
            auto joystick1 = SDL_JoystickOpen(gamepad_offset);
            auto joystick2 = SDL_JoystickOpen(gamepad_offset + 1);
            auto joystick3 = SDL_JoystickOpen(gamepad_offset + 2);
            if (joystick1) {
              printf("Primary Gamepad %i\n", gamepad_offset);
              printf("Primary Name: %s\n", SDL_JoystickNameForIndex(gamepad_offset));  // SDL2
              printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joystick1));
              printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joystick1));
              printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joystick1));
              printf("Number of Hats: %d\n", SDL_JoystickNumHats(joystick1));
              if (joystick2){
                printf("Secondary Gamepad %i\n", gamepad_offset+1);
                printf("Secondary Name: %s\n", SDL_JoystickNameForIndex(gamepad_offset + 1));
              }
              if (joystick3){
                printf("3rd Gamepad %i\n", gamepad_offset+2);
                printf("3rd Name: %s\n", SDL_JoystickNameForIndex(gamepad_offset + 2));
              }
              auto gamepad = new UpdateGamepad(editor_, joystick1, joystick2, joystick3);

            } else {
              printf("Couldn't open gamepad %i\n", gamepad_offset);
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
        std::cout << "  --gamepad=n                         For use with multiple gamepads, the offset index." << newLine << newLine;
        quit();
      }
      else if (command.contains(" --gamepad=2 ")) {
        gamepad_offset = 1;
      }
      else if (command.contains(" --gamepad=3 ")) {
        gamepad_offset = 2;
      }
      else if (command.contains(" --gamepad=4 ")) {
        gamepad_offset = 3;
      }
      else if (command.contains(" --gamepad=5 ")) {
        gamepad_offset = 4;
      }
      else if (command.contains(" --gamepad=6 ")) {
        gamepad_offset = 5;
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
