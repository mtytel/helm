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

  CPPFLAGS := $(DEPFLAGS) -std=c++11 -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I "~/SDKs/VST3 SDK" -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../srcs/JUCE-OSX/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -fPIC -O0
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I "~/SDKs/VST3 SDK" -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../srcs/JUCE-OSX/modules
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

  CPPFLAGS := $(DEPFLAGS) -std=c++11 -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I "~/SDKs/VST3 SDK" -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../srcs/JUCE-OSX/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -Os
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -fvisibility=hidden -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I "~/SDKs/VST3 SDK" -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../srcs/JUCE-OSX/modules
  TARGET := twytch.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/delay_a1e4df7f.o \
  $(OBJDIR)/envelope_265dd45e.o \
  $(OBJDIR)/feedback_1ba2cd93.o \
  $(OBJDIR)/filter_a2f71e46.o \
  $(OBJDIR)/linear_slope_aafebf9f.o \
  $(OBJDIR)/midi_lookup_47a7ae94.o \
  $(OBJDIR)/mono_panner_e32a65f6.o \
  $(OBJDIR)/operators_17db10cb.o \
  $(OBJDIR)/oscillator_f8f582ee.o \
  $(OBJDIR)/processor_4dff978e.o \
  $(OBJDIR)/processor_router_23bde824.o \
  $(OBJDIR)/send_receive_dbc9033a.o \
  $(OBJDIR)/smooth_filter_7e275f25.o \
  $(OBJDIR)/smooth_value_e19bb7ae.o \
  $(OBJDIR)/step_generator_6ff0a2ee.o \
  $(OBJDIR)/trigger_operators_1e299f84.o \
  $(OBJDIR)/value_9037106d.o \
  $(OBJDIR)/voice_handler_b8888f39.o \
  $(OBJDIR)/wave_42d06f87.o \
  $(OBJDIR)/cursynth_engine_27e324e6.o \
  $(OBJDIR)/cursynth_strings_18ca6928.o \
  $(OBJDIR)/glitch_look_and_feel_e090de07.o \
  $(OBJDIR)/glitch_bitch_45914711.o \
  $(OBJDIR)/glitch_bitch_editor_38ca2765.o \
  $(OBJDIR)/juce_audio_basics_36d50134.o \
  $(OBJDIR)/juce_audio_devices_5b44d8f2.o \
  $(OBJDIR)/juce_audio_formats_89c62292.o \
  $(OBJDIR)/juce_audio_processors_fd1f66ec.o \
  $(OBJDIR)/juce_core_bf4e7216.o \
  $(OBJDIR)/juce_cryptography_385a0470.o \
  $(OBJDIR)/juce_data_structures_93f11152.o \
  $(OBJDIR)/juce_events_cd8de38a.o \
  $(OBJDIR)/juce_graphics_9521edee.o \
  $(OBJDIR)/juce_gui_basics_3300ae6a.o \
  $(OBJDIR)/juce_gui_extra_f7af7272.o \
  $(OBJDIR)/juce_opengl_8dd60bb6.o \
  $(OBJDIR)/juce_PluginUtilities_4ae4f7fe.o \
  $(OBJDIR)/juce_video_b1602792.o \
  $(OBJDIR)/juce_VST_Wrapper_4d1db107.o \
  $(OBJDIR)/juce_VST3_Wrapper_860fc485.o \

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

$(OBJDIR)/delay_a1e4df7f.o: ../../mopo/src/delay.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling delay.cpp"
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

$(OBJDIR)/linear_slope_aafebf9f.o: ../../mopo/src/linear_slope.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling linear_slope.cpp"
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

$(OBJDIR)/processor_4dff978e.o: ../../mopo/src/processor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling processor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/processor_router_23bde824.o: ../../mopo/src/processor_router.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling processor_router.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/send_receive_dbc9033a.o: ../../mopo/src/send_receive.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling send_receive.cpp"
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

$(OBJDIR)/cursynth_engine_27e324e6.o: ../../Source/cursynth_engine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling cursynth_engine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/cursynth_strings_18ca6928.o: ../../Source/cursynth_strings.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling cursynth_strings.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/glitch_look_and_feel_e090de07.o: ../../Source/editor/glitch_look_and_feel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling glitch_look_and_feel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/glitch_bitch_45914711.o: ../../Source/glitch_bitch.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling glitch_bitch.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/glitch_bitch_editor_38ca2765.o: ../../Source/glitch_bitch_editor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling glitch_bitch_editor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_36d50134.o: ../../../srcs/JUCE-OSX/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_5b44d8f2.o: ../../../srcs/JUCE-OSX/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_89c62292.o: ../../../srcs/JUCE-OSX/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_fd1f66ec.o: ../../../srcs/JUCE-OSX/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_bf4e7216.o: ../../../srcs/JUCE-OSX/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_385a0470.o: ../../../srcs/JUCE-OSX/modules/juce_cryptography/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_93f11152.o: ../../../srcs/JUCE-OSX/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_cd8de38a.o: ../../../srcs/JUCE-OSX/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_9521edee.o: ../../../srcs/JUCE-OSX/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_3300ae6a.o: ../../../srcs/JUCE-OSX/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_f7af7272.o: ../../../srcs/JUCE-OSX/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_opengl_8dd60bb6.o: ../../../srcs/JUCE-OSX/modules/juce_opengl/juce_opengl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_PluginUtilities_4ae4f7fe.o: ../../../srcs/JUCE-OSX/modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_PluginUtilities.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_video_b1602792.o: ../../../srcs/JUCE-OSX/modules/juce_video/juce_video.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_video.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST_Wrapper_4d1db107.o: ../../../srcs/JUCE-OSX/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST3_Wrapper_860fc485.o: ../../../srcs/JUCE-OSX/modules/juce_audio_plugin_client/VST3/juce_VST3_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST3_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
