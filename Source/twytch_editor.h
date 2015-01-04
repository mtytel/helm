#ifndef TWYTCH_EDITOR_H
#define TWYTCH_EDITOR_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "twytch.h"
#include "editor/twytch_look_and_feel.h"
#include "editor/graphical_envelope.h"

class TwytchEditor : public AudioProcessorEditor, juce::Slider::Listener {
public:
  TwytchEditor(Twytch&);
  ~TwytchEditor();

  void paint(Graphics&) override;
  void resized() override;
  void sliderValueChanged(Slider* edited_slider) override;

private:
  Twytch& glitch_bitch_;
  TwytchLookAndFeel look_and_feel_;

  mopo::control_map controls_;
  std::map<std::string, Slider*> sliders_;

  GraphicalEnvelope* envelope_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TwytchEditor)
};

#endif // TWYTCH_EDITOR_H
