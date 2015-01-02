#ifndef GLITCH_BITCH_EDITOR_H
#define GLITCH_BITCH_EDITOR_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "glitch_bitch.h"

class GlitchBitchEditor  : public AudioProcessorEditor, juce::Slider::Listener {
public:
  GlitchBitchEditor(GlitchBitch&);
  ~GlitchBitchEditor();

  void paint(Graphics&) override;
  void resized() override;
  void sliderValueChanged(Slider* edited_slider) override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  GlitchBitch& glitch_bitch_;
  Slider* volume_;
  mopo::control_map controls_;
  std::map<std::string, Slider*> sliders_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlitchBitchEditor)
};


#endif  // GLITCH_BITCH_EDITOR_H
