# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D JUCE_JACK_CLIENT_NAME=\"Helm\" -D "JUCER_LINUX_MAKE_6B3E762A=1" -D "JUCE_APP_VERSION=0.5.0" -D "JUCE_APP_VERSION_HEX=0x500" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../JUCE/modules -I ../../../mopo/src -I ../../../src -I ../../../src/common -I ../../../src/editor_components -I ../../../src/editor_sections -I ../../../src/look_and_feel -I ../../../src/standalone -I ../../../src/synthesis
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 -ffast-math
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt -ffast-math

  TARGET := helm
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D JUCE_JACK_CLIENT_NAME=\"Helm\" -D "JUCER_LINUX_MAKE_6B3E762A=1" -D "JUCE_APP_VERSION=0.5.0" -D "JUCE_APP_VERSION_HEX=0x500" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../JUCE/modules -I ../../../mopo/src -I ../../../src -I ../../../src/common -I ../../../src/editor_components -I ../../../src/editor_sections -I ../../../src/look_and_feel -I ../../../src/standalone -I ../../../src/synthesis
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O2 -ffast-math
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt -ffast-math

  TARGET := helm
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/alias_3b4a33b.o \
  $(OBJDIR)/arpeggiator_89770ba4.o \
  $(OBJDIR)/bit_crush_6b16ce74.o \
  $(OBJDIR)/bypass_router_40c9316b.o \
  $(OBJDIR)/delay_8860f4ee.o \
  $(OBJDIR)/distortion_f480ec5c.o \
  $(OBJDIR)/envelope_e820148f.o \
  $(OBJDIR)/feedback_dd650dc4.o \
  $(OBJDIR)/filter_8bfdb6b7.o \
  $(OBJDIR)/formant_manager_f436e2fc.o \
  $(OBJDIR)/linear_slope_44537f50.o \
  $(OBJDIR)/magnitude_lookup_8a3238c8.o \
  $(OBJDIR)/memory_5ee1bbc0.o \
  $(OBJDIR)/midi_lookup_33d3b4c3.o \
  $(OBJDIR)/mono_panner_cf566c25.o \
  $(OBJDIR)/operators_8e60d6ba.o \
  $(OBJDIR)/oscillator_53287adf.o \
  $(OBJDIR)/phaser_d7048a16.o \
  $(OBJDIR)/portamento_slope_c638d2fc.o \
  $(OBJDIR)/processor_c4855d7d.o \
  $(OBJDIR)/processor_router_80596755.o \
  $(OBJDIR)/resonance_lookup_6f824fca.o \
  $(OBJDIR)/reverb_b8f91811.o \
  $(OBJDIR)/reverb_all_pass_2b685f27.o \
  $(OBJDIR)/reverb_comb_38882eb9.o \
  $(OBJDIR)/simple_delay_53bccd75.o \
  $(OBJDIR)/smooth_filter_f6a9594.o \
  $(OBJDIR)/smooth_value_7af0775f.o \
  $(OBJDIR)/step_generator_7143a5f.o \
  $(OBJDIR)/stutter_3fda664c.o \
  $(OBJDIR)/trigger_operators_54fe0673.o \
  $(OBJDIR)/value_76b325dc.o \
  $(OBJDIR)/voice_handler_49cbc5a8.o \
  $(OBJDIR)/wave_9cd47038.o \
  $(OBJDIR)/file_list_box_model_85bc4022.o \
  $(OBJDIR)/helm_common_ef933337.o \
  $(OBJDIR)/load_save_2c95b2e1.o \
  $(OBJDIR)/midi_manager_80d96a0e.o \
  $(OBJDIR)/startup_52cb2a28.o \
  $(OBJDIR)/synth_gui_interface_6337839d.o \
  $(OBJDIR)/filter_response_7394009c.o \
  $(OBJDIR)/filter_selector_c70de13a.o \
  $(OBJDIR)/global_tool_tip_5f078e04.o \
  $(OBJDIR)/graphical_envelope_4add3912.o \
  $(OBJDIR)/graphical_step_sequencer_763b67a0.o \
  $(OBJDIR)/modulation_button_8258c889.o \
  $(OBJDIR)/modulation_meter_3f248ddc.o \
  $(OBJDIR)/modulation_slider_aee8aab8.o \
  $(OBJDIR)/oscilloscope_48e472e5.o \
  $(OBJDIR)/patch_selector_fe7c390c.o \
  $(OBJDIR)/retrigger_selector_9cbab7cf.o \
  $(OBJDIR)/synth_slider_ab0cf9a.o \
  $(OBJDIR)/tempo_selector_e5a31119.o \
  $(OBJDIR)/wave_selector_6d163719.o \
  $(OBJDIR)/wave_viewer_e4b5610c.o \
  $(OBJDIR)/xy_pad_77c192eb.o \
  $(OBJDIR)/about_section_7d82e4ff.o \
  $(OBJDIR)/arp_section_a0da8a91.o \
  $(OBJDIR)/articulation_section_a61395c5.o \
  $(OBJDIR)/delay_section_9df39f5.o \
  $(OBJDIR)/delete_section_a0453dcf.o \
  $(OBJDIR)/envelope_section_a35a0a34.o \
  $(OBJDIR)/extra_mod_section_ea9c0745.o \
  $(OBJDIR)/feedback_section_56c0fe69.o \
  $(OBJDIR)/filter_section_9094381c.o \
  $(OBJDIR)/formant_section_51494d55.o \
  $(OBJDIR)/full_interface_29b5f507.o \
  $(OBJDIR)/lfo_section_f85fa167.o \
  $(OBJDIR)/modulation_manager_a5602c68.o \
  $(OBJDIR)/noise_section_31fdfd0c.o \
  $(OBJDIR)/oscillator_section_c90fdcc4.o \
  $(OBJDIR)/patch_browser_b6fb73fd.o \
  $(OBJDIR)/reverb_section_a0ad9f76.o \
  $(OBJDIR)/save_section_1a7d8ce1.o \
  $(OBJDIR)/step_sequencer_section_13843dc2.o \
  $(OBJDIR)/stutter_section_943b1913.o \
  $(OBJDIR)/sub_section_8cb704f2.o \
  $(OBJDIR)/synth_section_3b1ee2e.o \
  $(OBJDIR)/synthesis_interface_a75e885a.o \
  $(OBJDIR)/update_check_section_c57e00f6.o \
  $(OBJDIR)/volume_section_9cff0be.o \
  $(OBJDIR)/browser_look_and_feel_d6654391.o \
  $(OBJDIR)/default_look_and_feel_f168ba6a.o \
  $(OBJDIR)/modulation_look_and_feel_605bddbb.o \
  $(OBJDIR)/text_look_and_feel_4af8536c.o \
  $(OBJDIR)/helm_computer_keyboard_15a10faf.o \
  $(OBJDIR)/helm_standalone_editor_c8ee03cb.o \
  $(OBJDIR)/main_b7ad981e.o \
  $(OBJDIR)/dc_filter_3d140d58.o \
  $(OBJDIR)/fixed_point_oscillator_66a86444.o \
  $(OBJDIR)/fixed_point_wave_2344895d.o \
  $(OBJDIR)/helm_engine_2e44f843.o \
  $(OBJDIR)/helm_lfo_c32ba99e.o \
  $(OBJDIR)/helm_module_a4927f6d.o \
  $(OBJDIR)/helm_oscillators_bb02f03c.o \
  $(OBJDIR)/helm_voice_handler_35395fa6.o \
  $(OBJDIR)/noise_oscillator_93de254f.o \
  $(OBJDIR)/resonance_cancel_67415ef5.o \
  $(OBJDIR)/switch_871b185e.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/juce_audio_basics_181b4cb.o \
  $(OBJDIR)/juce_audio_devices_2d9302c9.o \
  $(OBJDIR)/juce_audio_formats_5c144c69.o \
  $(OBJDIR)/juce_audio_processors_a9c69383.o \
  $(OBJDIR)/juce_audio_utils_f6fe6709.o \
  $(OBJDIR)/juce_core_9d6b33ad.o \
  $(OBJDIR)/juce_cryptography_306b807.o \
  $(OBJDIR)/juce_data_structures_dcd877a9.o \
  $(OBJDIR)/juce_events_e5b201a1.o \
  $(OBJDIR)/juce_graphics_a34b2885.o \
  $(OBJDIR)/juce_gui_basics_3e4cc581.o \
  $(OBJDIR)/juce_gui_extra_20746349.o \
  $(OBJDIR)/juce_opengl_a5fa29cd.o \
  $(OBJDIR)/juce_video_7b52df69.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
	@echo Linking Helm
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning Helm
	@$(CLEANCMD)

strip:
	@echo Stripping Helm
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/alias_3b4a33b.o: ../../../mopo/src/alias.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling alias.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/arpeggiator_89770ba4.o: ../../../mopo/src/arpeggiator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling arpeggiator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/bit_crush_6b16ce74.o: ../../../mopo/src/bit_crush.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling bit_crush.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/bypass_router_40c9316b.o: ../../../mopo/src/bypass_router.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling bypass_router.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/delay_8860f4ee.o: ../../../mopo/src/delay.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling delay.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/distortion_f480ec5c.o: ../../../mopo/src/distortion.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling distortion.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/envelope_e820148f.o: ../../../mopo/src/envelope.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling envelope.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/feedback_dd650dc4.o: ../../../mopo/src/feedback.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling feedback.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/filter_8bfdb6b7.o: ../../../mopo/src/filter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling filter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/formant_manager_f436e2fc.o: ../../../mopo/src/formant_manager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling formant_manager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/linear_slope_44537f50.o: ../../../mopo/src/linear_slope.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling linear_slope.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/magnitude_lookup_8a3238c8.o: ../../../mopo/src/magnitude_lookup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling magnitude_lookup.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/memory_5ee1bbc0.o: ../../../mopo/src/memory.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling memory.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/midi_lookup_33d3b4c3.o: ../../../mopo/src/midi_lookup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling midi_lookup.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/mono_panner_cf566c25.o: ../../../mopo/src/mono_panner.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling mono_panner.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/operators_8e60d6ba.o: ../../../mopo/src/operators.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling operators.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/oscillator_53287adf.o: ../../../mopo/src/oscillator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling oscillator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/phaser_d7048a16.o: ../../../mopo/src/phaser.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling phaser.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/portamento_slope_c638d2fc.o: ../../../mopo/src/portamento_slope.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling portamento_slope.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/processor_c4855d7d.o: ../../../mopo/src/processor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling processor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/processor_router_80596755.o: ../../../mopo/src/processor_router.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling processor_router.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/resonance_lookup_6f824fca.o: ../../../mopo/src/resonance_lookup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling resonance_lookup.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/reverb_b8f91811.o: ../../../mopo/src/reverb.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling reverb.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/reverb_all_pass_2b685f27.o: ../../../mopo/src/reverb_all_pass.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling reverb_all_pass.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/reverb_comb_38882eb9.o: ../../../mopo/src/reverb_comb.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling reverb_comb.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/simple_delay_53bccd75.o: ../../../mopo/src/simple_delay.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling simple_delay.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/smooth_filter_f6a9594.o: ../../../mopo/src/smooth_filter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling smooth_filter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/smooth_value_7af0775f.o: ../../../mopo/src/smooth_value.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling smooth_value.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/step_generator_7143a5f.o: ../../../mopo/src/step_generator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling step_generator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/stutter_3fda664c.o: ../../../mopo/src/stutter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling stutter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/trigger_operators_54fe0673.o: ../../../mopo/src/trigger_operators.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling trigger_operators.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/value_76b325dc.o: ../../../mopo/src/value.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling value.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/voice_handler_49cbc5a8.o: ../../../mopo/src/voice_handler.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling voice_handler.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/wave_9cd47038.o: ../../../mopo/src/wave.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling wave.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/file_list_box_model_85bc4022.o: ../../../src/common/file_list_box_model.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling file_list_box_model.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_common_ef933337.o: ../../../src/common/helm_common.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_common.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/load_save_2c95b2e1.o: ../../../src/common/load_save.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling load_save.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/midi_manager_80d96a0e.o: ../../../src/common/midi_manager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling midi_manager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/startup_52cb2a28.o: ../../../src/common/startup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling startup.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/synth_gui_interface_6337839d.o: ../../../src/common/synth_gui_interface.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling synth_gui_interface.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/filter_response_7394009c.o: ../../../src/editor_components/filter_response.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling filter_response.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/filter_selector_c70de13a.o: ../../../src/editor_components/filter_selector.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling filter_selector.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/global_tool_tip_5f078e04.o: ../../../src/editor_components/global_tool_tip.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling global_tool_tip.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/graphical_envelope_4add3912.o: ../../../src/editor_components/graphical_envelope.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling graphical_envelope.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/graphical_step_sequencer_763b67a0.o: ../../../src/editor_components/graphical_step_sequencer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling graphical_step_sequencer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modulation_button_8258c889.o: ../../../src/editor_components/modulation_button.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_button.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modulation_meter_3f248ddc.o: ../../../src/editor_components/modulation_meter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_meter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modulation_slider_aee8aab8.o: ../../../src/editor_components/modulation_slider.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_slider.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/oscilloscope_48e472e5.o: ../../../src/editor_components/oscilloscope.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling oscilloscope.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/patch_selector_fe7c390c.o: ../../../src/editor_components/patch_selector.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling patch_selector.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/retrigger_selector_9cbab7cf.o: ../../../src/editor_components/retrigger_selector.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling retrigger_selector.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/synth_slider_ab0cf9a.o: ../../../src/editor_components/synth_slider.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling synth_slider.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/tempo_selector_e5a31119.o: ../../../src/editor_components/tempo_selector.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling tempo_selector.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/wave_selector_6d163719.o: ../../../src/editor_components/wave_selector.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling wave_selector.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/wave_viewer_e4b5610c.o: ../../../src/editor_components/wave_viewer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling wave_viewer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/xy_pad_77c192eb.o: ../../../src/editor_components/xy_pad.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling xy_pad.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/about_section_7d82e4ff.o: ../../../src/editor_sections/about_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling about_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/arp_section_a0da8a91.o: ../../../src/editor_sections/arp_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling arp_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/articulation_section_a61395c5.o: ../../../src/editor_sections/articulation_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling articulation_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/delay_section_9df39f5.o: ../../../src/editor_sections/delay_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling delay_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/delete_section_a0453dcf.o: ../../../src/editor_sections/delete_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling delete_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/envelope_section_a35a0a34.o: ../../../src/editor_sections/envelope_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling envelope_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/extra_mod_section_ea9c0745.o: ../../../src/editor_sections/extra_mod_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling extra_mod_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/feedback_section_56c0fe69.o: ../../../src/editor_sections/feedback_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling feedback_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/filter_section_9094381c.o: ../../../src/editor_sections/filter_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling filter_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/formant_section_51494d55.o: ../../../src/editor_sections/formant_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling formant_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/full_interface_29b5f507.o: ../../../src/editor_sections/full_interface.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling full_interface.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/lfo_section_f85fa167.o: ../../../src/editor_sections/lfo_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling lfo_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modulation_manager_a5602c68.o: ../../../src/editor_sections/modulation_manager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_manager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/noise_section_31fdfd0c.o: ../../../src/editor_sections/noise_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling noise_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/oscillator_section_c90fdcc4.o: ../../../src/editor_sections/oscillator_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling oscillator_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/patch_browser_b6fb73fd.o: ../../../src/editor_sections/patch_browser.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling patch_browser.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/reverb_section_a0ad9f76.o: ../../../src/editor_sections/reverb_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling reverb_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/save_section_1a7d8ce1.o: ../../../src/editor_sections/save_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling save_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/step_sequencer_section_13843dc2.o: ../../../src/editor_sections/step_sequencer_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling step_sequencer_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/stutter_section_943b1913.o: ../../../src/editor_sections/stutter_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling stutter_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/sub_section_8cb704f2.o: ../../../src/editor_sections/sub_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling sub_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/synth_section_3b1ee2e.o: ../../../src/editor_sections/synth_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling synth_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/synthesis_interface_a75e885a.o: ../../../src/editor_sections/synthesis_interface.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling synthesis_interface.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/update_check_section_c57e00f6.o: ../../../src/editor_sections/update_check_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling update_check_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/volume_section_9cff0be.o: ../../../src/editor_sections/volume_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling volume_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/browser_look_and_feel_d6654391.o: ../../../src/look_and_feel/browser_look_and_feel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling browser_look_and_feel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/default_look_and_feel_f168ba6a.o: ../../../src/look_and_feel/default_look_and_feel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling default_look_and_feel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modulation_look_and_feel_605bddbb.o: ../../../src/look_and_feel/modulation_look_and_feel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_look_and_feel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/text_look_and_feel_4af8536c.o: ../../../src/look_and_feel/text_look_and_feel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling text_look_and_feel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_computer_keyboard_15a10faf.o: ../../../src/standalone/helm_computer_keyboard.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_computer_keyboard.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_standalone_editor_c8ee03cb.o: ../../../src/standalone/helm_standalone_editor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_standalone_editor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/main_b7ad981e.o: ../../../src/standalone/main.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling main.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/dc_filter_3d140d58.o: ../../../src/synthesis/dc_filter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling dc_filter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/fixed_point_oscillator_66a86444.o: ../../../src/synthesis/fixed_point_oscillator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling fixed_point_oscillator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/fixed_point_wave_2344895d.o: ../../../src/synthesis/fixed_point_wave.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling fixed_point_wave.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_engine_2e44f843.o: ../../../src/synthesis/helm_engine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_engine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_lfo_c32ba99e.o: ../../../src/synthesis/helm_lfo.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_lfo.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_module_a4927f6d.o: ../../../src/synthesis/helm_module.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_module.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_oscillators_bb02f03c.o: ../../../src/synthesis/helm_oscillators.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_oscillators.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_voice_handler_35395fa6.o: ../../../src/synthesis/helm_voice_handler.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_voice_handler.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/noise_oscillator_93de254f.o: ../../../src/synthesis/noise_oscillator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling noise_oscillator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/resonance_cancel_67415ef5.o: ../../../src/synthesis/resonance_cancel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling resonance_cancel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/switch_871b185e.o: ../../../src/synthesis/switch.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling switch.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_181b4cb.o: ../../../JUCE/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_2d9302c9.o: ../../../JUCE/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_5c144c69.o: ../../../JUCE/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_a9c69383.o: ../../../JUCE/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_f6fe6709.o: ../../../JUCE/modules/juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_9d6b33ad.o: ../../../JUCE/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_306b807.o: ../../../JUCE/modules/juce_cryptography/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_dcd877a9.o: ../../../JUCE/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_e5b201a1.o: ../../../JUCE/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_a34b2885.o: ../../../JUCE/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_3e4cc581.o: ../../../JUCE/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_20746349.o: ../../../JUCE/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_opengl_a5fa29cd.o: ../../../JUCE/modules/juce_opengl/juce_opengl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_video_7b52df69.o: ../../../JUCE/modules/juce_video/juce_video.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_video.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
