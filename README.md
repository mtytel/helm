## Twytch
Twytch is a free, cross-platform, polyphonic synthesizer that runs on GNU/Linux, Mac, and Windows as a standalone program and as a VST/AU/AAX plugin. Release is in beta so there are outstanding bugs. Please send any bugs found to matthewtytel@gmail.com

### Installing:
Go to [tytel.org/twytch](http://tytel.org/twytch) for builds. If you're on Ubuntu you can download it through the command line:
```bash
sudo add-apt-repository ppa:tytel/twytch
sudo apt-get update
sudo apt-get install twytch
```

### Building:
If you want to build VST/AAX plugins You'll need their SDKs located in ~/srcs  
For AU on the Mac you'll want to put the CoreAudio SDK in /Applications/Xcode.app/Contents/Developer/Extras/CoreAudio

#### Linux
Currently only standalone is supported. LV2 plugin coming soon. 
```bash
make
```
The executable is located in standalone/builds/linux/build

#### OSX
Open /standalone/builds/osx/Twytch.xcodeproj for standalone version  
Open /builds/osx/Twytch.xcodeproj for plugin versions

#### Windows
Open /standalone/builds/vs12/Twytch.sln for standalone version  
Open /builds/vs12/Twytch.sln for plugin versions

### Features:
 - 32 voice polyphony
 - Interactive visual interface
 - Powerful modulation system with live visual feedback
 - Dual oscillators with cross modulation and up to 15 oscillators each
 - Unison and Harmony mode for oscillators
 - Oscillator feedback and saturation for waveshaping
 - 12 different waveforms
 - 7 filter types with keytracking
 - 2 monophonic and 1 polyphonic LFO
 - Step sequencer
 - Lots of modulation sources including polyphonic aftertouch
 - Simple arpeggiator
 - Effects: Formant filter, stutter, delay
 - Still more...

![alt tag](http://tytel.org/static/images/twytch_screenshot.png)
