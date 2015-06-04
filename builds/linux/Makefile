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
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -std=c++11 -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_6B3E762A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ~/srcs/VST3\ SDK -I ~/srcs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../JUCE/modules -I ../../mopo/src -I ../../src -I ../../src/common -I ../../src/editor_components -I ../../src/look_and_feel -I ../../src/standalone -I ../../src/synthesis
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -fPIC -O0
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 

  TARGET := helm.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -std=c++11 -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_6B3E762A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ~/srcs/VST3\ SDK -I ~/srcs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../JUCE/modules -I ../../mopo/src -I ../../src -I ../../src/common -I ../../src/editor_components -I ../../src/look_and_feel -I ../../src/standalone -I ../../src/synthesis
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -Os
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -fvisibility=hidden -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 

  TARGET := helm.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/alias_1d388dcc.o \
  $(OBJDIR)/arpeggiator_9d4b0575.o \
  $(OBJDIR)/bit_crush_f4910885.o \
  $(OBJDIR)/bypass_router_af85fafc.o \
  $(OBJDIR)/delay_a1e4df7f.o \
  $(OBJDIR)/distortion_9a4df46b.o \
  $(OBJDIR)/envelope_265dd45e.o \
  $(OBJDIR)/feedback_1ba2cd93.o \
  $(OBJDIR)/filter_a2f71e46.o \
  $(OBJDIR)/formant_manager_a6e78c4d.o \
  $(OBJDIR)/linear_slope_aafebf9f.o \
  $(OBJDIR)/magnitude_lookup_2d96b997.o \
  $(OBJDIR)/memory_75db234f.o \
  $(OBJDIR)/midi_lookup_47a7ae94.o \
  $(OBJDIR)/mono_panner_e32a65f6.o \
  $(OBJDIR)/operators_17db10cb.o \
  $(OBJDIR)/oscillator_f8f582ee.o \
  $(OBJDIR)/phaser_edfdf1a5.o \
  $(OBJDIR)/processor_4dff978e.o \
  $(OBJDIR)/processor_router_23bde824.o \
  $(OBJDIR)/resonance_lookup_12e6d099.o \
  $(OBJDIR)/reverb_cff27fa0.o \
  $(OBJDIR)/reverb_all_pass_de190878.o \
  $(OBJDIR)/reverb_comb_4c5c288a.o \
  $(OBJDIR)/simple_delay_ba680dc4.o \
  $(OBJDIR)/smooth_filter_7e275f25.o \
  $(OBJDIR)/smooth_value_e19bb7ae.o \
  $(OBJDIR)/step_generator_6ff0a2ee.o \
  $(OBJDIR)/stutter_80df09d.o \
  $(OBJDIR)/trigger_operators_1e299f84.o \
  $(OBJDIR)/value_9037106d.o \
  $(OBJDIR)/voice_handler_b8888f39.o \
  $(OBJDIR)/wave_42d06f87.o \
  $(OBJDIR)/helm_common_5e4ffcc8.o \
  $(OBJDIR)/load_save_4069acb2.o \
  $(OBJDIR)/midi_manager_e9b5d29d.o \
  $(OBJDIR)/synth_gui_interface_bde2c2e.o \
  $(OBJDIR)/filter_response_802ac2eb.o \
  $(OBJDIR)/filter_selector_d3a4a389.o \
  $(OBJDIR)/global_tool_tip_6b9e5053.o \
  $(OBJDIR)/graphical_envelope_46db2063.o \
  $(OBJDIR)/graphical_step_sequencer_3cc8ce31.o \
  $(OBJDIR)/modulation_button_c4483318.o \
  $(OBJDIR)/modulation_meter_c566156d.o \
  $(OBJDIR)/modulation_slider_f0d81547.o \
  $(OBJDIR)/oscilloscope_75f26af6.o \
  $(OBJDIR)/patch_browser_9bc1caf4.o \
  $(OBJDIR)/synth_slider_37bec7ab.o \
  $(OBJDIR)/tempo_selector_71348ea.o \
  $(OBJDIR)/wave_selector_e1c74128.o \
  $(OBJDIR)/wave_viewer_feefa2db.o \
  $(OBJDIR)/xy_pad_a46b2bbc.o \
  $(OBJDIR)/arp_section_d08f420.o \
  $(OBJDIR)/articulation_section_a2117d16.o \
  $(OBJDIR)/delay_section_24197bc4.o \
  $(OBJDIR)/envelope_section_c4ca4205.o \
  $(OBJDIR)/extra_mod_section_f732c994.o \
  $(OBJDIR)/feedback_section_7831363a.o \
  $(OBJDIR)/filter_section_bda2302d.o \
  $(OBJDIR)/formant_section_c5fa5764.o \
  $(OBJDIR)/full_interface_56c3ed18.o \
  $(OBJDIR)/lfo_section_648e0af6.o \
  $(OBJDIR)/modulation_manager_2ba1b3f9.o \
  $(OBJDIR)/noise_section_4c383edb.o \
  $(OBJDIR)/oscillator_section_4f516455.o \
  $(OBJDIR)/reverb_section_cdbb9787.o \
  $(OBJDIR)/step_sequencer_section_7a594d3.o \
  $(OBJDIR)/stutter_section_8ec2322.o \
  $(OBJDIR)/sub_section_f8e56e81.o \
  $(OBJDIR)/synth_section_1dec2ffd.o \
  $(OBJDIR)/synthesis_interface_e94df2e9.o \
  $(OBJDIR)/volume_section_36dde8cf.o \
  $(OBJDIR)/browser_look_and_feel_1854ae20.o \
  $(OBJDIR)/default_look_and_feel_335824f9.o \
  $(OBJDIR)/modulation_look_and_feel_547d34cc.o \
  $(OBJDIR)/text_look_and_feel_6c688b3d.o \
  $(OBJDIR)/helm_editor_39961892.o \
  $(OBJDIR)/helm_plugin_7f491358.o \
  $(OBJDIR)/fixed_point_oscillator_88189c15.o \
  $(OBJDIR)/fixed_point_wave_cbeb31ee.o \
  $(OBJDIR)/helm_engine_d1a97912.o \
  $(OBJDIR)/helm_lfo_31e8732f.o \
  $(OBJDIR)/helm_module_47f7003c.o \
  $(OBJDIR)/helm_oscillators_63a998cd.o \
  $(OBJDIR)/helm_voice_handler_4ed827f7.o \
  $(OBJDIR)/noise_oscillator_3c84cde0.o \
  $(OBJDIR)/resonance_cancel_fe80786.o \
  $(OBJDIR)/switch_9aef122f.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/juce_audio_basics_5a4fbb1c.o \
  $(OBJDIR)/juce_audio_devices_8af8b8da.o \
  $(OBJDIR)/juce_audio_formats_b97a027a.o \
  $(OBJDIR)/juce_audio_processors_d8d5b8d4.o \
  $(OBJDIR)/juce_core_b709fbfe.o \
  $(OBJDIR)/juce_cryptography_5bd4be58.o \
  $(OBJDIR)/juce_data_structures_562bd3a.o \
  $(OBJDIR)/juce_events_7223972.o \
  $(OBJDIR)/juce_graphics_9f490fd6.o \
  $(OBJDIR)/juce_gui_basics_af749c52.o \
  $(OBJDIR)/juce_gui_extra_1495ba5a.o \
  $(OBJDIR)/juce_opengl_c76a619e.o \
  $(OBJDIR)/juce_PluginUtilities_c6ec07e6.o \
  $(OBJDIR)/juce_video_a860d77a.o \
  $(OBJDIR)/juce_VST_Wrapper_148728ef.o \
  $(OBJDIR)/juce_VST3_Wrapper_18fae26d.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
	@echo Linking HelmPlugin
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning HelmPlugin
	@$(CLEANCMD)

strip:
	@echo Stripping HelmPlugin
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/alias_1d388dcc.o: ../../mopo/src/alias.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling alias.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/arpeggiator_9d4b0575.o: ../../mopo/src/arpeggiator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling arpeggiator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/bit_crush_f4910885.o: ../../mopo/src/bit_crush.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling bit_crush.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/bypass_router_af85fafc.o: ../../mopo/src/bypass_router.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling bypass_router.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/delay_a1e4df7f.o: ../../mopo/src/delay.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling delay.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/distortion_9a4df46b.o: ../../mopo/src/distortion.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling distortion.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/envelope_265dd45e.o: ../../mopo/src/envelope.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling envelope.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/feedback_1ba2cd93.o: ../../mopo/src/feedback.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling feedback.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/filter_a2f71e46.o: ../../mopo/src/filter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling filter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/formant_manager_a6e78c4d.o: ../../mopo/src/formant_manager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling formant_manager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/linear_slope_aafebf9f.o: ../../mopo/src/linear_slope.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling linear_slope.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/magnitude_lookup_2d96b997.o: ../../mopo/src/magnitude_lookup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling magnitude_lookup.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/memory_75db234f.o: ../../mopo/src/memory.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling memory.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/midi_lookup_47a7ae94.o: ../../mopo/src/midi_lookup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling midi_lookup.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/mono_panner_e32a65f6.o: ../../mopo/src/mono_panner.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling mono_panner.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/operators_17db10cb.o: ../../mopo/src/operators.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling operators.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/oscillator_f8f582ee.o: ../../mopo/src/oscillator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling oscillator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/phaser_edfdf1a5.o: ../../mopo/src/phaser.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling phaser.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/processor_4dff978e.o: ../../mopo/src/processor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling processor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/processor_router_23bde824.o: ../../mopo/src/processor_router.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling processor_router.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/resonance_lookup_12e6d099.o: ../../mopo/src/resonance_lookup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling resonance_lookup.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/reverb_cff27fa0.o: ../../mopo/src/reverb.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling reverb.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/reverb_all_pass_de190878.o: ../../mopo/src/reverb_all_pass.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling reverb_all_pass.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/reverb_comb_4c5c288a.o: ../../mopo/src/reverb_comb.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling reverb_comb.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/simple_delay_ba680dc4.o: ../../mopo/src/simple_delay.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling simple_delay.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/smooth_filter_7e275f25.o: ../../mopo/src/smooth_filter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling smooth_filter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/smooth_value_e19bb7ae.o: ../../mopo/src/smooth_value.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling smooth_value.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/step_generator_6ff0a2ee.o: ../../mopo/src/step_generator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling step_generator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/stutter_80df09d.o: ../../mopo/src/stutter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling stutter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/trigger_operators_1e299f84.o: ../../mopo/src/trigger_operators.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling trigger_operators.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/value_9037106d.o: ../../mopo/src/value.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling value.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/voice_handler_b8888f39.o: ../../mopo/src/voice_handler.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling voice_handler.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/wave_42d06f87.o: ../../mopo/src/wave.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling wave.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_common_5e4ffcc8.o: ../../src/common/helm_common.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_common.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/load_save_4069acb2.o: ../../src/common/load_save.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling load_save.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/midi_manager_e9b5d29d.o: ../../src/common/midi_manager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling midi_manager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/synth_gui_interface_bde2c2e.o: ../../src/common/synth_gui_interface.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling synth_gui_interface.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/filter_response_802ac2eb.o: ../../src/editor_components/filter_response.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling filter_response.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/filter_selector_d3a4a389.o: ../../src/editor_components/filter_selector.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling filter_selector.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/global_tool_tip_6b9e5053.o: ../../src/editor_components/global_tool_tip.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling global_tool_tip.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/graphical_envelope_46db2063.o: ../../src/editor_components/graphical_envelope.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling graphical_envelope.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/graphical_step_sequencer_3cc8ce31.o: ../../src/editor_components/graphical_step_sequencer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling graphical_step_sequencer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modulation_button_c4483318.o: ../../src/editor_components/modulation_button.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_button.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modulation_meter_c566156d.o: ../../src/editor_components/modulation_meter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_meter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modulation_slider_f0d81547.o: ../../src/editor_components/modulation_slider.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_slider.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/oscilloscope_75f26af6.o: ../../src/editor_components/oscilloscope.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling oscilloscope.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/patch_browser_9bc1caf4.o: ../../src/editor_components/patch_browser.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling patch_browser.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/synth_slider_37bec7ab.o: ../../src/editor_components/synth_slider.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling synth_slider.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/tempo_selector_71348ea.o: ../../src/editor_components/tempo_selector.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling tempo_selector.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/wave_selector_e1c74128.o: ../../src/editor_components/wave_selector.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling wave_selector.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/wave_viewer_feefa2db.o: ../../src/editor_components/wave_viewer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling wave_viewer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/xy_pad_a46b2bbc.o: ../../src/editor_components/xy_pad.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling xy_pad.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/arp_section_d08f420.o: ../../src/editor_sections/arp_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling arp_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/articulation_section_a2117d16.o: ../../src/editor_sections/articulation_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling articulation_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/delay_section_24197bc4.o: ../../src/editor_sections/delay_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling delay_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/envelope_section_c4ca4205.o: ../../src/editor_sections/envelope_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling envelope_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/extra_mod_section_f732c994.o: ../../src/editor_sections/extra_mod_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling extra_mod_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/feedback_section_7831363a.o: ../../src/editor_sections/feedback_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling feedback_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/filter_section_bda2302d.o: ../../src/editor_sections/filter_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling filter_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/formant_section_c5fa5764.o: ../../src/editor_sections/formant_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling formant_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/full_interface_56c3ed18.o: ../../src/editor_sections/full_interface.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling full_interface.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/lfo_section_648e0af6.o: ../../src/editor_sections/lfo_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling lfo_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modulation_manager_2ba1b3f9.o: ../../src/editor_sections/modulation_manager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_manager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/noise_section_4c383edb.o: ../../src/editor_sections/noise_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling noise_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/oscillator_section_4f516455.o: ../../src/editor_sections/oscillator_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling oscillator_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/reverb_section_cdbb9787.o: ../../src/editor_sections/reverb_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling reverb_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/step_sequencer_section_7a594d3.o: ../../src/editor_sections/step_sequencer_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling step_sequencer_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/stutter_section_8ec2322.o: ../../src/editor_sections/stutter_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling stutter_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/sub_section_f8e56e81.o: ../../src/editor_sections/sub_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling sub_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/synth_section_1dec2ffd.o: ../../src/editor_sections/synth_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling synth_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/synthesis_interface_e94df2e9.o: ../../src/editor_sections/synthesis_interface.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling synthesis_interface.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/volume_section_36dde8cf.o: ../../src/editor_sections/volume_section.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling volume_section.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/browser_look_and_feel_1854ae20.o: ../../src/look_and_feel/browser_look_and_feel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling browser_look_and_feel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/default_look_and_feel_335824f9.o: ../../src/look_and_feel/default_look_and_feel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling default_look_and_feel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modulation_look_and_feel_547d34cc.o: ../../src/look_and_feel/modulation_look_and_feel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_look_and_feel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/text_look_and_feel_6c688b3d.o: ../../src/look_and_feel/text_look_and_feel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling text_look_and_feel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_editor_39961892.o: ../../src/plugin/helm_editor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_editor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_plugin_7f491358.o: ../../src/plugin/helm_plugin.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_plugin.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/fixed_point_oscillator_88189c15.o: ../../src/synthesis/fixed_point_oscillator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling fixed_point_oscillator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/fixed_point_wave_cbeb31ee.o: ../../src/synthesis/fixed_point_wave.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling fixed_point_wave.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_engine_d1a97912.o: ../../src/synthesis/helm_engine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_engine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_lfo_31e8732f.o: ../../src/synthesis/helm_lfo.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_lfo.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_module_47f7003c.o: ../../src/synthesis/helm_module.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_module.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_oscillators_63a998cd.o: ../../src/synthesis/helm_oscillators.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_oscillators.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/helm_voice_handler_4ed827f7.o: ../../src/synthesis/helm_voice_handler.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling helm_voice_handler.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/noise_oscillator_3c84cde0.o: ../../src/synthesis/noise_oscillator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling noise_oscillator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/resonance_cancel_fe80786.o: ../../src/synthesis/resonance_cancel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling resonance_cancel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/switch_9aef122f.o: ../../src/synthesis/switch.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling switch.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_5a4fbb1c.o: ../../JUCE/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_8af8b8da.o: ../../JUCE/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_b97a027a.o: ../../JUCE/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_d8d5b8d4.o: ../../JUCE/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_b709fbfe.o: ../../JUCE/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_5bd4be58.o: ../../JUCE/modules/juce_cryptography/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_562bd3a.o: ../../JUCE/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_7223972.o: ../../JUCE/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_9f490fd6.o: ../../JUCE/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_af749c52.o: ../../JUCE/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_1495ba5a.o: ../../JUCE/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_opengl_c76a619e.o: ../../JUCE/modules/juce_opengl/juce_opengl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_PluginUtilities_c6ec07e6.o: ../../JUCE/modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_PluginUtilities.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_video_a860d77a.o: ../../JUCE/modules/juce_video/juce_video.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_video.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST_Wrapper_148728ef.o: ../../JUCE/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST3_Wrapper_18fae26d.o: ../../JUCE/modules/juce_audio_plugin_client/VST3/juce_VST3_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST3_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
