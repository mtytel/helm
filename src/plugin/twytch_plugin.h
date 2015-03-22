/* Copyright 2013-2015 Matt Tytel
 *
 * twytch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * twytch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with twytch.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TWYTCH_PLUGIN_H
#define TWYTCH_PLUGIN_H

#include "JuceHeader.h"
#include "memory.h"
#include "save_load_manager.h"
#include "twytch_engine.h"

class TwytchPlugin : public AudioProcessor {
  public:
    TwytchPlugin();
    virtual ~TwytchPlugin();

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


    void processMidi(MidiBuffer&);
    mopo::TwytchEngine* getSynth() { return &synth_; }
    const mopo::Memory* getOutputMemory() { return output_memory_; }

  private:
    mopo::TwytchEngine synth_;
    mopo::Memory* output_memory_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TwytchPlugin)
};

#endif // TWYTCH_PLUGIN_H
