#ifndef TWYTCH_H
#define TWYTCH_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "cursynth_engine.h"

class Twytch : public AudioProcessor {
public:
  Twytch();
  virtual ~Twytch();

  void prepareToPlay(double sample_rate, int buffer_size) override;
  void releaseResources() override;

  void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  const String getName() const override;

  int getNumParameters() override;
  float getParameter(int index) override;
  void setParameter(int index, float new_value) override;

  const String getParameterName(int index) override;
  const String getParameterText(int index) override;

  const String getInputChannelName(int channel_index) const override;
  const String getOutputChannelName(int channel_index) const override;
  bool isInputChannelStereoPair(int index) const override;
  bool isOutputChannelStereoPair(int index) const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool silenceInProducesSilenceOut() const override;
  double getTailLengthSeconds() const override;

  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const String getProgramName(int index) override;
  void changeProgramName(int index, const String& new_name) override;

  void getStateInformation(MemoryBlock& destData) override;
  void setStateInformation(const void* data, int size_in_bytes) override;

  mopo::CursynthEngine* getSynth() { return &synth_; }

  double volume, phase;

private:

  mopo::CursynthEngine synth_;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Twytch)
};


#endif // TWYTCH_H