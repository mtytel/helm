/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_BFBAB84DA31A01CD__
#define __JUCE_HEADER_BFBAB84DA31A01CD__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "twytch_engine.h"
#include "twytch_filter_selector.h"
#include "twytch_slider.h"
#include "twytch_tempo_selector.h"
#include "twytch_wave_selector.h"
//[/Headers]

#include "graphical_step_sequencer.h"
#include "graphical_envelope.h"
#include "wave_form_selector.h"
#include "filter_response.h"
#include "xy_pad.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SynthesisInterface  : public Component,
                            public SliderListener,
                            public ButtonListener
{
public:
    //==============================================================================
    SynthesisInterface (mopo::control_map controls);
    ~SynthesisInterface();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setSliderUnits();
    void createTempoSliders();
    void markBipolarSliders();
    void setDefaultDoubleClickValues();
    void createStepSequencerSliders();

    void setAllValues(mopo::control_map& controls);
    void setValue(std::string name, mopo::mopo_float value,
                  NotificationType notification = sendNotificationAsync);

    TwytchSlider* getSlider(std::string name);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::map<std::string, TwytchSlider*> slider_lookup_;
    std::map<std::string, Button*> button_lookup_;
    std::vector<Slider*> step_sequencer_sliders_;
    ScopedPointer<TwytchSlider> step_sequencer_tempo_;
    ScopedPointer<TwytchSlider> mono_lfo_1_tempo_;
    ScopedPointer<TwytchSlider> mono_lfo_2_tempo_;
    ScopedPointer<TwytchSlider> poly_lfo_tempo_;
    ScopedPointer<TwytchSlider> delay_tempo_;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GraphicalStepSequencer> step_sequencer_;
    ScopedPointer<GraphicalEnvelope> amplitude_envelope_;
    ScopedPointer<GraphicalEnvelope> filter_envelope_;
    ScopedPointer<WaveFormSelector> osc_1_wave_display_;
    ScopedPointer<WaveFormSelector> osc_2_wave_display_;
    ScopedPointer<TwytchSlider> polyphony_;
    ScopedPointer<TwytchSlider> portamento_;
    ScopedPointer<TwytchSlider> pitch_bend_range_;
    ScopedPointer<TwytchSlider> cross_modulation_;
    ScopedPointer<FilterResponse> filter_response_;
    ScopedPointer<TwytchSlider> portamento_type_;
    ScopedPointer<TwytchSlider> osc_mix_;
    ScopedPointer<TwytchSlider> osc_2_transpose_;
    ScopedPointer<TwytchSlider> osc_2_tune_;
    ScopedPointer<TwytchSlider> volume_;
    ScopedPointer<TwytchSlider> delay_feedback_;
    ScopedPointer<TwytchSlider> delay_dry_wet_;
    ScopedPointer<TwytchSlider> velocity_track_;
    ScopedPointer<TwytchSlider> amp_attack_;
    ScopedPointer<TwytchSlider> amp_decay_;
    ScopedPointer<TwytchSlider> amp_release_;
    ScopedPointer<TwytchSlider> amp_sustain_;
    ScopedPointer<TwytchSlider> fil_attack_;
    ScopedPointer<TwytchSlider> fil_decay_;
    ScopedPointer<TwytchSlider> fil_release_;
    ScopedPointer<TwytchSlider> fil_sustain_;
    ScopedPointer<TwytchSlider> resonance_;
    ScopedPointer<TwytchWaveSelector> osc_1_waveform_;
    ScopedPointer<TwytchWaveSelector> osc_2_waveform_;
    ScopedPointer<TwytchSlider> cutoff_;
    ScopedPointer<TwytchSlider> fil_env_depth_;
    ScopedPointer<TwytchSlider> keytrack_;
    ScopedPointer<TwytchSlider> osc_feedback_transpose_;
    ScopedPointer<TwytchSlider> osc_feedback_amount_;
    ScopedPointer<TwytchSlider> osc_feedback_tune_;
    ScopedPointer<WaveFormSelector> mono_lfo_1_wave_display_;
    ScopedPointer<TwytchWaveSelector> mono_lfo_1_waveform_;
    ScopedPointer<TwytchSlider> num_steps_;
    ScopedPointer<TwytchSlider> step_frequency_;
    ScopedPointer<TwytchSlider> mono_lfo_1_frequency_;
    ScopedPointer<TwytchSlider> filter_saturation_;
    ScopedPointer<ToggleButton> formant_on_;
    ScopedPointer<TwytchSlider> legato_;
    ScopedPointer<XYPad> formant_xy_pad_;
    ScopedPointer<TwytchSlider> formant_x_;
    ScopedPointer<TwytchSlider> formant_y_;
    ScopedPointer<TwytchFilterSelector> filter_type_;
    ScopedPointer<WaveFormSelector> poly_lfo_wave_display_;
    ScopedPointer<TwytchWaveSelector> poly_lfo_waveform_;
    ScopedPointer<TextButton> filter_envelope_mod_;
    ScopedPointer<TextButton> amplitude_envelope_mod_;
    ScopedPointer<TextButton> step_sequencer_mod_;
    ScopedPointer<TextButton> mono_lfo_1_mod_;
    ScopedPointer<TextButton> pitch_wheel_mod_;
    ScopedPointer<TextButton> mod_wheel_mod_;
    ScopedPointer<TextButton> note_mod_;
    ScopedPointer<TextButton> velocity_mod_;
    ScopedPointer<TextButton> aftertouch_mod_;
    ScopedPointer<TwytchSlider> osc_1_tune_;
    ScopedPointer<TwytchSlider> delay_frequency_;
    ScopedPointer<WaveFormSelector> mono_lfo_2_wave_display_;
    ScopedPointer<TwytchWaveSelector> mono_lfo_2_waveform_;
    ScopedPointer<TwytchSlider> osc_1_transpose_;
    ScopedPointer<TwytchSlider> mono_lfo_1_amplitude_;
    ScopedPointer<TwytchSlider> mono_lfo_2_frequency_;
    ScopedPointer<TextButton> mono_lfo_2_mod_;
    ScopedPointer<TwytchSlider> mono_lfo_2_amplitude_;
    ScopedPointer<TwytchSlider> poly_lfo_frequency_;
    ScopedPointer<TextButton> poly_lfo_mod_;
    ScopedPointer<TwytchSlider> poly_lfo_amplitude_;
    ScopedPointer<TwytchSlider> stutter_frequency_;
    ScopedPointer<ToggleButton> stutter_on_;
    ScopedPointer<TwytchSlider> stutter_resample_frequency_;
    ScopedPointer<TwytchSlider> step_smoothing_;
    ScopedPointer<TwytchTempoSelector> mono_lfo_1_sync_;
    ScopedPointer<TwytchTempoSelector> mono_lfo_2_sync_;
    ScopedPointer<TwytchTempoSelector> poly_lfo_sync_;
    ScopedPointer<TwytchTempoSelector> delay_sync_;
    ScopedPointer<TwytchTempoSelector> step_sequencer_sync_;
    Path internalPath1;
    Path internalPath2;
    Path internalPath3;
    Path internalPath4;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthesisInterface)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_BFBAB84DA31A01CD__
