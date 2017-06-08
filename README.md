## Helm
Helm is a free, cross-platform, polyphonic synthesizer that runs on GNU/Linux, Mac, and Windows as a standalone program and as a LV2/VST/AU/AAX plugin. Release is in beta so there are outstanding bugs. Please send any bugs found to matthewtytel@gmail.com

### Installing:
Go to [tytel.org/helm](http://tytel.org/helm) for builds.

### Building:
If you want to build AAX plugins You'll need its SDK located in ~/srcs  
For AU on the Mac you'll want to put the CoreAudio SDK in /Applications/Xcode.app/Contents/Developer/Extras/CoreAudio  
Nothing extra is needed for building VST/LV2 plugins

#### Linux
To build and install the standalone, lv2 and vst plugin:
```bash
make
sudo make install
```

Other make commands:
```bash
# Build just the Linux standalone executable:
make standalone

# Build just the Linux LV2 plugin:
make lv2

# Build just the Linux VST plugin
make vst

# Install just the Linux standalone executable:
sudo make install_standalone

# Install just the Linux LV2 plugin:
sudo make install_lv2

# Install just the Linux VST plugin
sudo make install_vst
```

The standalone executable is built to standalone/builds/linux/build and installed to /usr/bin
The LV2 plugin is built to builds/linux/LV2 and installed to /usr/lib/lv2
The VST plugin is built to builds/linux/VST and installed to /usr/lib/lxvst

#### OSX
Open /standalone/builds/osx/Helm.xcodeproj for standalone version  
Open /builds/osx/Helm.xcodeproj for plugin versions

#### Windows
Open /standalone/builds/vs15/Helm.sln for standalone version  
Open /builds/vs15/Helm.sln for plugin versions

### Features:
 - 32 voice polyphony
 - Interactive visual interface
 - Powerful modulation system with live visual feedback
 - Dual oscillators with cross modulation and up to 15 unison oscillators each
 - Sub oscillator with shuffle waveshaping
 - Oscillator feedback and saturation for waveshaping
 - 12 different waveforms
 - Blending between 12 or 24dB low/band/high pass filter
 - Low/Band/High Shelf filters
 - 2 monophonic and 1 polyphonic LFO
 - Step sequencer
 - Lots of modulation sources including polyphonic aftertouch
 - Simple arpeggiator
 - Effects: Formant filter, stutter, delay, distortion, reverb

![alt tag](http://tytel.org/static/images/helm_screenshot.png)
