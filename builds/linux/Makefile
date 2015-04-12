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

  CPPFLAGS := $(DEPFLAGS) -std=c++11 -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_6B3E762A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I "~/SDKs/VST3 SDK" -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../srcs/JUCE/modules -I ../../mopo/src -I ../../src -I ../../src/common -I ../../src/editor_components -I ../../src/look_and_feel -I ../../src/standalone -I ../../src/synthesis
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -fPIC -O0
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_6B3E762A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I "~/SDKs/VST3 SDK" -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../srcs/JUCE/modules -I ../../mopo/src -I ../../src -I ../../src/common -I ../../src/editor_components -I ../../src/look_and_feel -I ../../src/standalone -I ../../src/synthesis
  TARGET := twytch.so
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

  CPPFLAGS := $(DEPFLAGS) -std=c++11 -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_6B3E762A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I "~/SDKs/VST3 SDK" -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../srcs/JUCE/modules -I ../../mopo/src -I ../../src -I ../../src/common -I ../../src/editor_components -I ../../src/look_and_feel -I ../../src/standalone -I ../../src/synthesis
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -Os
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -fvisibility=hidden -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_6B3E762A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I "~/SDKs/VST3 SDK" -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../srcs/JUCE/modules -I ../../mopo/src -I ../../src -I ../../src/common -I ../../src/editor_components -I ../../src/look_and_feel -I ../../src/standalone -I ../../src/synthesis
  TARGET := twytch.so
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
  $(OBJDIR)/formant_416d86df.o \
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
  $(OBJDIR)/simple_delay_ba680dc4.o \
  $(OBJDIR)/smooth_filter_7e275f25.o \
  $(OBJDIR)/smooth_value_e19bb7ae.o \
  $(OBJDIR)/step_generator_6ff0a2ee.o \
  $(OBJDIR)/stutter_80df09d.o \
  $(OBJDIR)/trigger_operators_1e299f84.o \
  $(OBJDIR)/value_9037106d.o \
  $(OBJDIR)/voice_handler_b8888f39.o \
  $(OBJDIR)/wave_42d06f87.o \
  $(OBJDIR)/midi_manager_e9b5d29d.o \
  $(OBJDIR)/synth_gui_interface_bde2c2e.o \
  $(OBJDIR)/twytch_load_save_87fa95c8.o \
  $(OBJDIR)/audio_viewer_85409b82.o \
  $(OBJDIR)/filter_response_802ac2eb.o \
  $(OBJDIR)/filter_selector_d3a4a389.o \
  $(OBJDIR)/full_interface_ddba7cf0.o \
  $(OBJDIR)/global_tool_tip_6b9e5053.o \
  $(OBJDIR)/graphical_envelope_46db2063.o \
  $(OBJDIR)/graphical_step_sequencer_3cc8ce31.o \
  $(OBJDIR)/modulation_manager_329c97d1.o \
  $(OBJDIR)/modulation_meter_c566156d.o \
  $(OBJDIR)/modulation_slider_f0d81547.o \
  $(OBJDIR)/oscilloscope_75f26af6.o \
  $(OBJDIR)/synthesis_interface_c1af8a11.o \
  $(OBJDIR)/tempo_selector_71348ea.o \
  $(OBJDIR)/twytch_slider_4fa599a0.o \
  $(OBJDIR)/wave_selector_e1c74128.o \
  $(OBJDIR)/wave_viewer_feefa2db.o \
  $(OBJDIR)/xy_pad_a46b2bbc.o \
  $(OBJDIR)/modulation_look_and_feel_547d34cc.o \
  $(OBJDIR)/twytch_look_and_feel_7026b753.o \
  $(OBJDIR)/twytch_editor_cb41a52d.o \
  $(OBJDIR)/twytch_plugin_10f49ff3.o \
  $(OBJDIR)/switch_9aef122f.o \
  $(OBJDIR)/twytch_engine_ecbf26ed.o \
  $(OBJDIR)/twytch_lfo_95ce0b34.o \
  $(OBJDIR)/twytch_module_630cae17.o \
  $(OBJDIR)/twytch_oscillators_99cd5bd2.o \
  $(OBJDIR)/twytch_voice_handler_8b173dbc.o \
  $(OBJDIR)/juce_audio_basics_e9ef128d.o \
  $(OBJDIR)/juce_audio_devices_b01ff80b.o \
  $(OBJDIR)/juce_audio_formats_dea141ab.o \
  $(OBJDIR)/juce_audio_processors_d630f45.o \
  $(OBJDIR)/juce_core_4980556f.o \
  $(OBJDIR)/juce_cryptography_eb7415c9.o \
  $(OBJDIR)/juce_data_structures_b984fbeb.o \
  $(OBJDIR)/juce_events_5afc7263.o \
  $(OBJDIR)/juce_graphics_988b6847.o \
  $(OBJDIR)/juce_gui_basics_16035443.o \
  $(OBJDIR)/juce_gui_extra_c6a3ba8b.o \
  $(OBJDIR)/juce_opengl_1b449a8f.o \
  $(OBJDIR)/juce_PluginUtilities_fc38c517.o \
  $(OBJDIR)/juce_video_76a698ab.o \
  $(OBJDIR)/juce_VST_Wrapper_5b18a720.o \
  $(OBJDIR)/juce_VST3_Wrapper_125985e.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking Twytch
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning Twytch
	@$(CLEANCMD)

strip:
	@echo Stripping Twytch
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

$(OBJDIR)/formant_416d86df.o: ../../mopo/src/formant.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling formant.cpp"
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

$(OBJDIR)/midi_manager_e9b5d29d.o: ../../src/common/midi_manager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling midi_manager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/synth_gui_interface_bde2c2e.o: ../../src/common/synth_gui_interface.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling synth_gui_interface.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_load_save_87fa95c8.o: ../../src/common/twytch_load_save.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_load_save.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/audio_viewer_85409b82.o: ../../src/editor_components/audio_viewer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling audio_viewer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/filter_response_802ac2eb.o: ../../src/editor_components/filter_response.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling filter_response.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/filter_selector_d3a4a389.o: ../../src/editor_components/filter_selector.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling filter_selector.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/full_interface_ddba7cf0.o: ../../src/editor_components/full_interface.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling full_interface.cpp"
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

$(OBJDIR)/modulation_manager_329c97d1.o: ../../src/editor_components/modulation_manager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_manager.cpp"
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

$(OBJDIR)/synthesis_interface_c1af8a11.o: ../../src/editor_components/synthesis_interface.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling synthesis_interface.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/tempo_selector_71348ea.o: ../../src/editor_components/tempo_selector.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling tempo_selector.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_slider_4fa599a0.o: ../../src/editor_components/twytch_slider.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_slider.cpp"
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

$(OBJDIR)/modulation_look_and_feel_547d34cc.o: ../../src/look_and_feel/modulation_look_and_feel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_look_and_feel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_look_and_feel_7026b753.o: ../../src/look_and_feel/twytch_look_and_feel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_look_and_feel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_editor_cb41a52d.o: ../../src/plugin/twytch_editor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_editor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_plugin_10f49ff3.o: ../../src/plugin/twytch_plugin.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_plugin.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/switch_9aef122f.o: ../../src/synthesis/switch.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling switch.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_engine_ecbf26ed.o: ../../src/synthesis/twytch_engine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_engine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_lfo_95ce0b34.o: ../../src/synthesis/twytch_lfo.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_lfo.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_module_630cae17.o: ../../src/synthesis/twytch_module.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_module.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_oscillators_99cd5bd2.o: ../../src/synthesis/twytch_oscillators.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_oscillators.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_voice_handler_8b173dbc.o: ../../src/synthesis/twytch_voice_handler.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_voice_handler.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_e9ef128d.o: ../../../srcs/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_b01ff80b.o: ../../../srcs/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_dea141ab.o: ../../../srcs/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_d630f45.o: ../../../srcs/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_4980556f.o: ../../../srcs/JUCE/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_eb7415c9.o: ../../../srcs/JUCE/modules/juce_cryptography/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_b984fbeb.o: ../../../srcs/JUCE/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_5afc7263.o: ../../../srcs/JUCE/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_988b6847.o: ../../../srcs/JUCE/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_16035443.o: ../../../srcs/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_c6a3ba8b.o: ../../../srcs/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_opengl_1b449a8f.o: ../../../srcs/JUCE/modules/juce_opengl/juce_opengl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_PluginUtilities_fc38c517.o: ../../../srcs/JUCE/modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_PluginUtilities.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_video_76a698ab.o: ../../../srcs/JUCE/modules/juce_video/juce_video.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_video.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST_Wrapper_5b18a720.o: ../../../srcs/JUCE/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST3_Wrapper_125985e.o: ../../../srcs/JUCE/modules/juce_audio_plugin_client/VST3/juce_VST3_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST3_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
