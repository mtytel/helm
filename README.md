## Helm
Helm is a free, cross-platform, polyphonic synthesizer that runs on GNU/Linux, Mac, and Windows as a standalone program and as a LV2/VST/AU/AAX plugin. Release is in beta so there are outstanding bugs. Please send any bugs found to matthewtytel@gmail.com

### Installing:
Go to [tytel.org/helm](http://tytel.org/helm) for builds. If you're on Ubuntu you can download it through the command line:
```bash
sudo add-apt-repository ppa:tytel/helm
sudo apt-get update
sudo apt-get install helm
```

### Building:
If you want to build VST/AAX plugins You'll need their SDKs located in ~/srcs  
For AU on the Mac you'll want to put the CoreAudio SDK in /Applications/Xcode.app/Contents/Developer/Extras/CoreAudio  
Nothing extra is needed for building LV2 plugins

#### GNU/Linux
```bash
make
```
The standalone executable is located in standalone/builds/linux/build  
The LV2 plugin is located in builds/linux/LV2  

To build a GNU/Linux VST plugin:
```bash
make vst
```
The VST plugin is located in builds/linux/VST as an .so file.

#### OSX
Open /standalone/builds/osx/Helm.xcodeproj for standalone version  
Open /builds/osx/HelmPlugin.xcodeproj for plugin versions

#### Windows
Open /standalone/builds/vs12/Helm.sln for standalone version  
Open /builds/vs12/HelmPlugin.sln for plugin versions

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

![alt tag](http://tytel.org/static/images/helm_screenshot.png)
