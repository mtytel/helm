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

  CPPFLAGS := $(DEPFLAGS) -std=c++11 -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_6B3E762A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../srcs/JUCE-OSX/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_6B3E762A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../srcs/JUCE-OSX/modules
  TARGET := Twytch
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

  CPPFLAGS := $(DEPFLAGS) -std=c++11 -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_6B3E762A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../srcs/JUCE-OSX/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_6B3E762A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../srcs/JUCE-OSX/modules
  TARGET := Twytch
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
  $(OBJDIR)/formant_7939fc8e.o \
  $(OBJDIR)/formant_manager_f436e2fc.o \
  $(OBJDIR)/linear_slope_44537f50.o \
  $(OBJDIR)/magnitude_lookup_8a3238c8.o \
  $(OBJDIR)/memory_5ee1bbc0.o \
  $(OBJDIR)/midi_lookup_33d3b4c3.o \
  $(OBJDIR)/mono_panner_cf566c25.o \
  $(OBJDIR)/operators_8e60d6ba.o \
  $(OBJDIR)/oscillator_53287adf.o \
  $(OBJDIR)/phaser_d7048a16.o \
  $(OBJDIR)/processor_c4855d7d.o \
  $(OBJDIR)/processor_router_80596755.o \
  $(OBJDIR)/resonance_lookup_6f824fca.o \
  $(OBJDIR)/smooth_filter_f6a9594.o \
  $(OBJDIR)/smooth_value_7af0775f.o \
  $(OBJDIR)/step_generator_7143a5f.o \
  $(OBJDIR)/trigger_operators_54fe0673.o \
  $(OBJDIR)/value_76b325dc.o \
  $(OBJDIR)/voice_handler_49cbc5a8.o \
  $(OBJDIR)/wave_9cd47038.o \
  $(OBJDIR)/audio_viewer_5832a371.o \
  $(OBJDIR)/filter_response_7394009c.o \
  $(OBJDIR)/full_interface_bc4a451f.o \
  $(OBJDIR)/graphical_envelope_4add3912.o \
  $(OBJDIR)/graphical_step_sequencer_763b67a0.o \
  $(OBJDIR)/modulation_manager_369eb080.o \
  $(OBJDIR)/modulation_meter_3f248ddc.o \
  $(OBJDIR)/modulation_slider_aee8aab8.o \
  $(OBJDIR)/oscilloscope_48e472e5.o \
  $(OBJDIR)/synthesis_interface_3df08742.o \
  $(OBJDIR)/wave_form_selector_9d91edca.o \
  $(OBJDIR)/xy_pad_77c192eb.o \
  $(OBJDIR)/main_d7bc4a08.o \
  $(OBJDIR)/modulation_look_and_feel_ec5f597a.o \
  $(OBJDIR)/twytch_engine_92e4ab19.o \
  $(OBJDIR)/twytch_look_and_feel_1a426301.o \
  $(OBJDIR)/twytch_oscillators_8d8f9326.o \
  $(OBJDIR)/twytch_standalone_editor_626e757a.o \
  $(OBJDIR)/twytch_voice_handler_9728f010.o \
  $(OBJDIR)/juce_audio_basics_196aec63.o \
  $(OBJDIR)/juce_audio_devices_f00cb461.o \
  $(OBJDIR)/juce_audio_formats_1e8dfe01.o \
  $(OBJDIR)/juce_audio_processors_852ab31b.o \
  $(OBJDIR)/juce_audio_utils_e680a4a1.o \
  $(OBJDIR)/juce_core_4e269b45.o \
  $(OBJDIR)/juce_cryptography_1aefef9f.o \
  $(OBJDIR)/juce_data_structures_816f9d41.o \
  $(OBJDIR)/juce_events_74bfdd39.o \
  $(OBJDIR)/juce_graphics_fa51781d.o \
  $(OBJDIR)/juce_gui_basics_3f18919.o \
  $(OBJDIR)/juce_gui_extra_cf252ce1.o \
  $(OBJDIR)/juce_opengl_35080565.o \
  $(OBJDIR)/juce_video_ead2c101.o \

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

$(OBJDIR)/formant_7939fc8e.o: ../../../mopo/src/formant.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling formant.cpp"
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

$(OBJDIR)/audio_viewer_5832a371.o: ../../../src/editor_components/audio_viewer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling audio_viewer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/filter_response_7394009c.o: ../../../src/editor_components/filter_response.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling filter_response.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/full_interface_bc4a451f.o: ../../../src/editor_components/full_interface.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling full_interface.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/graphical_envelope_4add3912.o: ../../../src/editor_components/graphical_envelope.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling graphical_envelope.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/graphical_step_sequencer_763b67a0.o: ../../../src/editor_components/graphical_step_sequencer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling graphical_step_sequencer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modulation_manager_369eb080.o: ../../../src/editor_components/modulation_manager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_manager.cpp"
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

$(OBJDIR)/synthesis_interface_3df08742.o: ../../../src/editor_components/synthesis_interface.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling synthesis_interface.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/wave_form_selector_9d91edca.o: ../../../src/editor_components/wave_form_selector.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling wave_form_selector.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/xy_pad_77c192eb.o: ../../../src/editor_components/xy_pad.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling xy_pad.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/main_d7bc4a08.o: ../../../src/main.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling main.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modulation_look_and_feel_ec5f597a.o: ../../../src/modulation_look_and_feel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_look_and_feel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_engine_92e4ab19.o: ../../../src/twytch_engine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_engine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_look_and_feel_1a426301.o: ../../../src/twytch_look_and_feel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_look_and_feel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_oscillators_8d8f9326.o: ../../../src/twytch_oscillators.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_oscillators.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_standalone_editor_626e757a.o: ../../../src/twytch_standalone_editor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_standalone_editor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_voice_handler_9728f010.o: ../../../src/twytch_voice_handler.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_voice_handler.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_196aec63.o: ../../../../srcs/JUCE-OSX/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_f00cb461.o: ../../../../srcs/JUCE-OSX/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_1e8dfe01.o: ../../../../srcs/JUCE-OSX/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_852ab31b.o: ../../../../srcs/JUCE-OSX/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_e680a4a1.o: ../../../../srcs/JUCE-OSX/modules/juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_4e269b45.o: ../../../../srcs/JUCE-OSX/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_1aefef9f.o: ../../../../srcs/JUCE-OSX/modules/juce_cryptography/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_816f9d41.o: ../../../../srcs/JUCE-OSX/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_74bfdd39.o: ../../../../srcs/JUCE-OSX/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_fa51781d.o: ../../../../srcs/JUCE-OSX/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_3f18919.o: ../../../../srcs/JUCE-OSX/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_cf252ce1.o: ../../../../srcs/JUCE-OSX/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_opengl_35080565.o: ../../../../srcs/JUCE-OSX/modules/juce_opengl/juce_opengl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_video_ead2c101.o: ../../../../srcs/JUCE-OSX/modules/juce_video/juce_video.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_video.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
