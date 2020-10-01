# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# build with "V=1" for verbose builds
ifeq ($(V), 1)
V_AT =
else
V_AT = @
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

JUCE_ARCH_LABEL := $(shell uname -m)

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DDEBUG=1" "-D_DEBUG=1" "-DJUCE_JACK_CLIENT_NAME=\"Helm\"" "-DJUCE_ALSA_MIDI_INPUT_NAME=\"Helm\"" "-DJUCE_ALSA_MIDI_OUTPUT_NAME=\"Helm\"" "-DJUCE_USE_XRANDR=0" "-DJUCER_LINUX_MAKE_1D79FBD2=1" "-DJUCE_APP_VERSION=0.9.0" "-DJUCE_APP_VERSION_HEX=0x900" $(shell pkg-config --cflags alsa libcurl x11 xinerama xext freetype2) -pthread -I../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK -I../../../JuceLibraryCode -I../../../JUCE/modules -I../../../mopo/src -I../../../concurrentqueue -I../../../src -I../../../src/common -I../../../src/editor_components -I../../../src/editor_sections -I../../../src/look_and_feel -I../../../src/standalone -I../../../src/synthesis $(CPPFLAGS)

  JUCE_CPPFLAGS_VST :=  "-DJucePlugin_Build_VST=1" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0"
  JUCE_CFLAGS_VST := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST := -shared -Wl,--no-undefined
  JUCE_TARGET_VST := helm.so
  JUCE_VSTDESTDIR := $(HOME)/.vst
  JUCE_COPYCMD_VST := $(JUCE_OUTDIR)/helm.so $(JUCE_VSTDESTDIR)

  JUCE_CPPFLAGS_VST3 :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=1" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0"
  JUCE_CFLAGS_VST3 := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST3 := -shared -Wl,--no-undefined
  JUCE_VST3DIR := helm.vst3
  JUCE_VST3SUBDIR := Contents/x86_64-linux
  JUCE_TARGET_VST3 := $(JUCE_VST3DIR)/$(JUCE_VST3SUBDIR)/helm.so
  JUCE_VST3DESTDIR := $(HOME)/.vst3
  JUCE_COPYCMD_VST3 := $(JUCE_OUTDIR)/$(JUCE_VST3DIR) $(JUCE_VST3DESTDIR)

  JUCE_CPPFLAGS_SHARED_CODE :=  "-DJucePlugin_Build_VST=1" "-DJucePlugin_Build_VST3=1" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0" "-DJUCE_SHARED_CODE=1"
  JUCE_TARGET_SHARED_CODE := helm.a

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -fPIC -g -ggdb -O0 $(DEBCXXFLAGS) -ffast-math $(SIMDFLAGS) -ftree-vectorize -ftree-slp-vectorize $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++14 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ $(shell pkg-config --libs alsa libcurl x11 xinerama xext freetype2) -fvisibility=hidden $(DEBLDFLAGS) -ffast-math $(SIMDFLAGS) -ftree-vectorize -ftree-slp-vectorize -lrt -ldl -lpthread -lGL $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DNDEBUG=1" "-DJUCE_JACK_CLIENT_NAME=\"Helm\"" "-DJUCE_ALSA_MIDI_INPUT_NAME=\"Helm\"" "-DJUCE_ALSA_MIDI_OUTPUT_NAME=\"Helm\"" "-DJUCE_USE_XRANDR=0" "-DJUCER_LINUX_MAKE_1D79FBD2=1" "-DJUCE_APP_VERSION=0.9.0" "-DJUCE_APP_VERSION_HEX=0x900" $(shell pkg-config --cflags alsa libcurl x11 xinerama xext freetype2) -pthread -I../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK -I../../../JuceLibraryCode -I../../../JUCE/modules -I../../../mopo/src -I../../../concurrentqueue -I../../../src -I../../../src/common -I../../../src/editor_components -I../../../src/editor_sections -I../../../src/look_and_feel -I../../../src/standalone -I../../../src/synthesis $(CPPFLAGS)

  JUCE_CPPFLAGS_VST :=  "-DJucePlugin_Build_VST=1" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0"
  JUCE_CFLAGS_VST := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST := -shared -Wl,--no-undefined
  JUCE_TARGET_VST := helm.so
  JUCE_VSTDESTDIR := $(HOME)/.vst
  JUCE_COPYCMD_VST := $(JUCE_OUTDIR)/helm.so $(JUCE_VSTDESTDIR)

  JUCE_CPPFLAGS_VST3 :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=1" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0"
  JUCE_CFLAGS_VST3 := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST3 := -shared -Wl,--no-undefined
  JUCE_VST3DIR := helm.vst3
  JUCE_VST3SUBDIR := Contents/x86_64-linux
  JUCE_TARGET_VST3 := $(JUCE_VST3DIR)/$(JUCE_VST3SUBDIR)/helm.so
  JUCE_VST3DESTDIR := $(HOME)/.vst3
  JUCE_COPYCMD_VST3 := $(JUCE_OUTDIR)/$(JUCE_VST3DIR) $(JUCE_VST3DESTDIR)

  JUCE_CPPFLAGS_SHARED_CODE :=  "-DJucePlugin_Build_VST=1" "-DJucePlugin_Build_VST3=1" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0" "-DJUCE_SHARED_CODE=1"
  JUCE_TARGET_SHARED_CODE := helm.a

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -fPIC -O3 $(DEBCXXFLAGS) -ffast-math $(SIMDFLAGS) -ftree-vectorize -ftree-slp-vectorize $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++14 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ $(shell pkg-config --libs alsa libcurl x11 xinerama xext freetype2) -fvisibility=hidden $(DEBLDFLAGS) -ffast-math $(SIMDFLAGS) -ftree-vectorize -ftree-slp-vectorize -lrt -ldl -lpthread -lGL $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS_ALL := \

OBJECTS_VST := \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_VST2_1e16d9b9.o \

OBJECTS_VST3 := \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_VST3_1e24f13a.o \

OBJECTS_SHARED_CODE := \
  $(JUCE_OBJDIR)/alias_3b4a33b.o \
  $(JUCE_OBJDIR)/arpeggiator_89770ba4.o \
  $(JUCE_OBJDIR)/biquad_filter_5a44dd34.o \
  $(JUCE_OBJDIR)/bit_crush_6b16ce74.o \
  $(JUCE_OBJDIR)/bypass_router_40c9316b.o \
  $(JUCE_OBJDIR)/delay_8860f4ee.o \
  $(JUCE_OBJDIR)/distortion_f480ec5c.o \
  $(JUCE_OBJDIR)/envelope_e820148f.o \
  $(JUCE_OBJDIR)/feedback_dd650dc4.o \
  $(JUCE_OBJDIR)/formant_manager_f436e2fc.o \
  $(JUCE_OBJDIR)/ladder_filter_a3cf6a0.o \
  $(JUCE_OBJDIR)/linear_slope_44537f50.o \
  $(JUCE_OBJDIR)/magnitude_lookup_8a3238c8.o \
  $(JUCE_OBJDIR)/memory_5ee1bbc0.o \
  $(JUCE_OBJDIR)/midi_lookup_33d3b4c3.o \
  $(JUCE_OBJDIR)/mono_panner_cf566c25.o \
  $(JUCE_OBJDIR)/operators_8e60d6ba.o \
  $(JUCE_OBJDIR)/oscillator_53287adf.o \
  $(JUCE_OBJDIR)/portamento_slope_c638d2fc.o \
  $(JUCE_OBJDIR)/processor_c4855d7d.o \
  $(JUCE_OBJDIR)/processor_router_80596755.o \
  $(JUCE_OBJDIR)/resonance_lookup_6f824fca.o \
  $(JUCE_OBJDIR)/reverb_b8f91811.o \
  $(JUCE_OBJDIR)/reverb_all_pass_2b685f27.o \
  $(JUCE_OBJDIR)/reverb_comb_38882eb9.o \
  $(JUCE_OBJDIR)/sample_decay_lookup_eafa367f.o \
  $(JUCE_OBJDIR)/simple_delay_53bccd75.o \
  $(JUCE_OBJDIR)/smooth_filter_f6a9594.o \
  $(JUCE_OBJDIR)/smooth_value_7af0775f.o \
  $(JUCE_OBJDIR)/state_variable_filter_4b869558.o \
  $(JUCE_OBJDIR)/step_generator_7143a5f.o \
  $(JUCE_OBJDIR)/stutter_3fda664c.o \
  $(JUCE_OBJDIR)/trigger_operators_54fe0673.o \
  $(JUCE_OBJDIR)/value_76b325dc.o \
  $(JUCE_OBJDIR)/voice_handler_49cbc5a8.o \
  $(JUCE_OBJDIR)/border_bounds_constrainer_b5a34af8.o \
  $(JUCE_OBJDIR)/file_list_box_model_85bc4022.o \
  $(JUCE_OBJDIR)/helm_common_ef933337.o \
  $(JUCE_OBJDIR)/load_save_2c95b2e1.o \
  $(JUCE_OBJDIR)/midi_manager_80d96a0e.o \
  $(JUCE_OBJDIR)/startup_52cb2a28.o \
  $(JUCE_OBJDIR)/synth_base_c3ad3b73.o \
  $(JUCE_OBJDIR)/synth_gui_interface_6337839d.o \
  $(JUCE_OBJDIR)/bpm_slider_64fb0d57.o \
  $(JUCE_OBJDIR)/filter_response_7394009c.o \
  $(JUCE_OBJDIR)/filter_selector_c70de13a.o \
  $(JUCE_OBJDIR)/global_tool_tip_5f078e04.o \
  $(JUCE_OBJDIR)/graphical_step_sequencer_763b67a0.o \
  $(JUCE_OBJDIR)/midi_keyboard_ec3d63f9.o \
  $(JUCE_OBJDIR)/modulation_button_8258c889.o \
  $(JUCE_OBJDIR)/modulation_highlight_dd34a287.o \
  $(JUCE_OBJDIR)/modulation_meter_3f248ddc.o \
  $(JUCE_OBJDIR)/modulation_slider_aee8aab8.o \
  $(JUCE_OBJDIR)/open_gl_background_fe08efa9.o \
  $(JUCE_OBJDIR)/open_gl_component_d67359ac.o \
  $(JUCE_OBJDIR)/open_gl_envelope_96eb82cb.o \
  $(JUCE_OBJDIR)/open_gl_modulation_meter_7446c901.o \
  $(JUCE_OBJDIR)/open_gl_oscilloscope_48c4088a.o \
  $(JUCE_OBJDIR)/open_gl_peak_meter_28e8c124.o \
  $(JUCE_OBJDIR)/open_gl_wave_viewer_bb6a02c7.o \
  $(JUCE_OBJDIR)/oscilloscope_48e472e5.o \
  $(JUCE_OBJDIR)/patch_selector_fe7c390c.o \
  $(JUCE_OBJDIR)/retrigger_selector_9cbab7cf.o \
  $(JUCE_OBJDIR)/synth_button_de20ed6b.o \
  $(JUCE_OBJDIR)/synth_slider_ab0cf9a.o \
  $(JUCE_OBJDIR)/tempo_selector_e5a31119.o \
  $(JUCE_OBJDIR)/text_selector_f115b965.o \
  $(JUCE_OBJDIR)/text_slider_41684da7.o \
  $(JUCE_OBJDIR)/wave_selector_6d163719.o \
  $(JUCE_OBJDIR)/wave_viewer_e4b5610c.o \
  $(JUCE_OBJDIR)/xy_pad_77c192eb.o \
  $(JUCE_OBJDIR)/about_section_7d82e4ff.o \
  $(JUCE_OBJDIR)/arp_section_a0da8a91.o \
  $(JUCE_OBJDIR)/bpm_section_6b714e11.o \
  $(JUCE_OBJDIR)/contribute_section_44364711.o \
  $(JUCE_OBJDIR)/delay_section_9df39f5.o \
  $(JUCE_OBJDIR)/delete_section_a0453dcf.o \
  $(JUCE_OBJDIR)/distortion_section_2cd18141.o \
  $(JUCE_OBJDIR)/dynamic_section_846a1cf1.o \
  $(JUCE_OBJDIR)/envelope_section_a35a0a34.o \
  $(JUCE_OBJDIR)/extra_mod_section_ea9c0745.o \
  $(JUCE_OBJDIR)/feedback_section_56c0fe69.o \
  $(JUCE_OBJDIR)/filter_section_9094381c.o \
  $(JUCE_OBJDIR)/formant_section_51494d55.o \
  $(JUCE_OBJDIR)/full_interface_29b5f507.o \
  $(JUCE_OBJDIR)/lfo_section_f85fa167.o \
  $(JUCE_OBJDIR)/mixer_section_9e4a737b.o \
  $(JUCE_OBJDIR)/noise_section_31fdfd0c.o \
  $(JUCE_OBJDIR)/open_gl_modulation_manager_939ca4d.o \
  $(JUCE_OBJDIR)/oscillator_section_c90fdcc4.o \
  $(JUCE_OBJDIR)/patch_browser_b6fb73fd.o \
  $(JUCE_OBJDIR)/reverb_section_a0ad9f76.o \
  $(JUCE_OBJDIR)/save_section_1a7d8ce1.o \
  $(JUCE_OBJDIR)/step_sequencer_section_13843dc2.o \
  $(JUCE_OBJDIR)/stutter_section_943b1913.o \
  $(JUCE_OBJDIR)/sub_section_8cb704f2.o \
  $(JUCE_OBJDIR)/synth_section_3b1ee2e.o \
  $(JUCE_OBJDIR)/synthesis_interface_a75e885a.o \
  $(JUCE_OBJDIR)/update_check_section_c57e00f6.o \
  $(JUCE_OBJDIR)/voice_section_99705324.o \
  $(JUCE_OBJDIR)/volume_section_9cff0be.o \
  $(JUCE_OBJDIR)/browser_look_and_feel_d6654391.o \
  $(JUCE_OBJDIR)/colors_9855f240.o \
  $(JUCE_OBJDIR)/default_look_and_feel_f168ba6a.o \
  $(JUCE_OBJDIR)/fonts_7e4e153e.o \
  $(JUCE_OBJDIR)/modulation_look_and_feel_605bddbb.o \
  $(JUCE_OBJDIR)/shaders_8f61ea28.o \
  $(JUCE_OBJDIR)/text_look_and_feel_4af8536c.o \
  $(JUCE_OBJDIR)/helm_editor_cad94f01.o \
  $(JUCE_OBJDIR)/helm_plugin_108c49c7.o \
  $(JUCE_OBJDIR)/dc_filter_3d140d58.o \
  $(JUCE_OBJDIR)/detune_lookup_ea628520.o \
  $(JUCE_OBJDIR)/fixed_point_oscillator_66a86444.o \
  $(JUCE_OBJDIR)/fixed_point_wave_2344895d.o \
  $(JUCE_OBJDIR)/gate_73f8a3b5.o \
  $(JUCE_OBJDIR)/helm_engine_2e44f843.o \
  $(JUCE_OBJDIR)/helm_lfo_c32ba99e.o \
  $(JUCE_OBJDIR)/helm_module_a4927f6d.o \
  $(JUCE_OBJDIR)/helm_oscillators_bb02f03c.o \
  $(JUCE_OBJDIR)/helm_voice_handler_35395fa6.o \
  $(JUCE_OBJDIR)/noise_oscillator_93de254f.o \
  $(JUCE_OBJDIR)/peak_meter_cadcb853.o \
  $(JUCE_OBJDIR)/resonance_cancel_67415ef5.o \
  $(JUCE_OBJDIR)/trigger_random_750c5e54.o \
  $(JUCE_OBJDIR)/value_switch_f497502c.o \
  $(JUCE_OBJDIR)/BinaryData_51699c3.o \
  $(JUCE_OBJDIR)/include_juce_audio_basics_68f957b.o \
  $(JUCE_OBJDIR)/include_juce_audio_devices_6eefc5f1.o \
  $(JUCE_OBJDIR)/include_juce_audio_formats_21d6c8f0.o \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_utils_baa4955d.o \
  $(JUCE_OBJDIR)/include_juce_audio_processors_5ced3317.o \
  $(JUCE_OBJDIR)/include_juce_audio_utils_a3a1cb85.o \
  $(JUCE_OBJDIR)/include_juce_core_d832d60c.o \
  $(JUCE_OBJDIR)/include_juce_data_structures_349db12.o \
  $(JUCE_OBJDIR)/include_juce_events_9b26cc86.o \
  $(JUCE_OBJDIR)/include_juce_graphics_eb811ef8.o \
  $(JUCE_OBJDIR)/include_juce_gui_basics_a2082cf6.o \
  $(JUCE_OBJDIR)/include_juce_gui_extra_e7ac9489.o \
  $(JUCE_OBJDIR)/include_juce_opengl_95d8f91c.o \
  $(JUCE_OBJDIR)/include_juce_osc_da4097fc.o \

.PHONY: clean all strip VST VST3

all : VST VST3

VST : $(JUCE_OUTDIR)/$(JUCE_TARGET_VST)
VST3 : $(JUCE_OUTDIR)/$(JUCE_TARGET_VST3)


$(JUCE_OUTDIR)/$(JUCE_TARGET_VST) : $(OBJECTS_VST) $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa libcurl x11 xinerama xext freetype2
	@echo Linking "Helm - VST"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_VST) $(OBJECTS_VST) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_VST) $(RESOURCES) $(TARGET_ARCH)
	-$(V_AT)mkdir -p $(JUCE_VSTDESTDIR)
	-$(V_AT)cp -R $(JUCE_COPYCMD_VST)

$(JUCE_OUTDIR)/$(JUCE_TARGET_VST3) : $(OBJECTS_VST3) $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa libcurl x11 xinerama xext freetype2
	@echo Linking "Helm - VST3"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)/$(JUCE_VST3DIR)/$(JUCE_VST3SUBDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_VST3) $(OBJECTS_VST3) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_VST3) $(RESOURCES) $(TARGET_ARCH)
	-$(V_AT)mkdir -p $(JUCE_VST3DESTDIR)
	-$(V_AT)cp -R $(JUCE_COPYCMD_VST3)

$(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) : $(OBJECTS_SHARED_CODE) $(RESOURCES)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa libcurl x11 xinerama xext freetype2
	@echo Linking "Helm - Shared Code"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(AR) -rcs $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(OBJECTS_SHARED_CODE)

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_VST2_1e16d9b9.o: ../../../JuceLibraryCode/include_juce_audio_plugin_client_VST2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_VST2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_VST) $(JUCE_CFLAGS_VST) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_VST3_1e24f13a.o: ../../../JuceLibraryCode/include_juce_audio_plugin_client_VST3.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_VST3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_VST3) $(JUCE_CFLAGS_VST3) -o "$@" -c "$<"

$(JUCE_OBJDIR)/alias_3b4a33b.o: ../../../mopo/src/alias.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling alias.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/arpeggiator_89770ba4.o: ../../../mopo/src/arpeggiator.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling arpeggiator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/biquad_filter_5a44dd34.o: ../../../mopo/src/biquad_filter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling biquad_filter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/bit_crush_6b16ce74.o: ../../../mopo/src/bit_crush.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling bit_crush.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/bypass_router_40c9316b.o: ../../../mopo/src/bypass_router.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling bypass_router.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/delay_8860f4ee.o: ../../../mopo/src/delay.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling delay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/distortion_f480ec5c.o: ../../../mopo/src/distortion.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling distortion.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/envelope_e820148f.o: ../../../mopo/src/envelope.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling envelope.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/feedback_dd650dc4.o: ../../../mopo/src/feedback.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling feedback.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/formant_manager_f436e2fc.o: ../../../mopo/src/formant_manager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling formant_manager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ladder_filter_a3cf6a0.o: ../../../mopo/src/ladder_filter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ladder_filter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/linear_slope_44537f50.o: ../../../mopo/src/linear_slope.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling linear_slope.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/magnitude_lookup_8a3238c8.o: ../../../mopo/src/magnitude_lookup.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling magnitude_lookup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/memory_5ee1bbc0.o: ../../../mopo/src/memory.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling memory.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/midi_lookup_33d3b4c3.o: ../../../mopo/src/midi_lookup.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling midi_lookup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/mono_panner_cf566c25.o: ../../../mopo/src/mono_panner.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling mono_panner.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/operators_8e60d6ba.o: ../../../mopo/src/operators.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling operators.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/oscillator_53287adf.o: ../../../mopo/src/oscillator.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling oscillator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/portamento_slope_c638d2fc.o: ../../../mopo/src/portamento_slope.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling portamento_slope.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/processor_c4855d7d.o: ../../../mopo/src/processor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling processor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/processor_router_80596755.o: ../../../mopo/src/processor_router.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling processor_router.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/resonance_lookup_6f824fca.o: ../../../mopo/src/resonance_lookup.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling resonance_lookup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/reverb_b8f91811.o: ../../../mopo/src/reverb.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling reverb.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/reverb_all_pass_2b685f27.o: ../../../mopo/src/reverb_all_pass.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling reverb_all_pass.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/reverb_comb_38882eb9.o: ../../../mopo/src/reverb_comb.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling reverb_comb.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/sample_decay_lookup_eafa367f.o: ../../../mopo/src/sample_decay_lookup.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling sample_decay_lookup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/simple_delay_53bccd75.o: ../../../mopo/src/simple_delay.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling simple_delay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/smooth_filter_f6a9594.o: ../../../mopo/src/smooth_filter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling smooth_filter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/smooth_value_7af0775f.o: ../../../mopo/src/smooth_value.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling smooth_value.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/state_variable_filter_4b869558.o: ../../../mopo/src/state_variable_filter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling state_variable_filter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/step_generator_7143a5f.o: ../../../mopo/src/step_generator.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling step_generator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/stutter_3fda664c.o: ../../../mopo/src/stutter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling stutter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/trigger_operators_54fe0673.o: ../../../mopo/src/trigger_operators.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling trigger_operators.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/value_76b325dc.o: ../../../mopo/src/value.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling value.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/voice_handler_49cbc5a8.o: ../../../mopo/src/voice_handler.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling voice_handler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/border_bounds_constrainer_b5a34af8.o: ../../../src/common/border_bounds_constrainer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling border_bounds_constrainer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/file_list_box_model_85bc4022.o: ../../../src/common/file_list_box_model.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling file_list_box_model.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/helm_common_ef933337.o: ../../../src/common/helm_common.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling helm_common.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/load_save_2c95b2e1.o: ../../../src/common/load_save.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling load_save.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/midi_manager_80d96a0e.o: ../../../src/common/midi_manager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling midi_manager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/startup_52cb2a28.o: ../../../src/common/startup.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling startup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/synth_base_c3ad3b73.o: ../../../src/common/synth_base.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling synth_base.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/synth_gui_interface_6337839d.o: ../../../src/common/synth_gui_interface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling synth_gui_interface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/bpm_slider_64fb0d57.o: ../../../src/editor_components/bpm_slider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling bpm_slider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/filter_response_7394009c.o: ../../../src/editor_components/filter_response.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling filter_response.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/filter_selector_c70de13a.o: ../../../src/editor_components/filter_selector.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling filter_selector.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/global_tool_tip_5f078e04.o: ../../../src/editor_components/global_tool_tip.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling global_tool_tip.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/graphical_step_sequencer_763b67a0.o: ../../../src/editor_components/graphical_step_sequencer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling graphical_step_sequencer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/midi_keyboard_ec3d63f9.o: ../../../src/editor_components/midi_keyboard.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling midi_keyboard.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/modulation_button_8258c889.o: ../../../src/editor_components/modulation_button.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling modulation_button.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/modulation_highlight_dd34a287.o: ../../../src/editor_components/modulation_highlight.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling modulation_highlight.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/modulation_meter_3f248ddc.o: ../../../src/editor_components/modulation_meter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling modulation_meter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/modulation_slider_aee8aab8.o: ../../../src/editor_components/modulation_slider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling modulation_slider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/open_gl_background_fe08efa9.o: ../../../src/editor_components/open_gl_background.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling open_gl_background.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/open_gl_component_d67359ac.o: ../../../src/editor_components/open_gl_component.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling open_gl_component.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/open_gl_envelope_96eb82cb.o: ../../../src/editor_components/open_gl_envelope.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling open_gl_envelope.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/open_gl_modulation_meter_7446c901.o: ../../../src/editor_components/open_gl_modulation_meter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling open_gl_modulation_meter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/open_gl_oscilloscope_48c4088a.o: ../../../src/editor_components/open_gl_oscilloscope.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling open_gl_oscilloscope.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/open_gl_peak_meter_28e8c124.o: ../../../src/editor_components/open_gl_peak_meter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling open_gl_peak_meter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/open_gl_wave_viewer_bb6a02c7.o: ../../../src/editor_components/open_gl_wave_viewer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling open_gl_wave_viewer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/oscilloscope_48e472e5.o: ../../../src/editor_components/oscilloscope.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling oscilloscope.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/patch_selector_fe7c390c.o: ../../../src/editor_components/patch_selector.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling patch_selector.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/retrigger_selector_9cbab7cf.o: ../../../src/editor_components/retrigger_selector.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling retrigger_selector.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/synth_button_de20ed6b.o: ../../../src/editor_components/synth_button.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling synth_button.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/synth_slider_ab0cf9a.o: ../../../src/editor_components/synth_slider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling synth_slider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/tempo_selector_e5a31119.o: ../../../src/editor_components/tempo_selector.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling tempo_selector.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/text_selector_f115b965.o: ../../../src/editor_components/text_selector.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling text_selector.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/text_slider_41684da7.o: ../../../src/editor_components/text_slider.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling text_slider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/wave_selector_6d163719.o: ../../../src/editor_components/wave_selector.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling wave_selector.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/wave_viewer_e4b5610c.o: ../../../src/editor_components/wave_viewer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling wave_viewer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/xy_pad_77c192eb.o: ../../../src/editor_components/xy_pad.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling xy_pad.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/about_section_7d82e4ff.o: ../../../src/editor_sections/about_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling about_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/arp_section_a0da8a91.o: ../../../src/editor_sections/arp_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling arp_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/bpm_section_6b714e11.o: ../../../src/editor_sections/bpm_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling bpm_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/contribute_section_44364711.o: ../../../src/editor_sections/contribute_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling contribute_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/delay_section_9df39f5.o: ../../../src/editor_sections/delay_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling delay_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/delete_section_a0453dcf.o: ../../../src/editor_sections/delete_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling delete_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/distortion_section_2cd18141.o: ../../../src/editor_sections/distortion_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling distortion_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/dynamic_section_846a1cf1.o: ../../../src/editor_sections/dynamic_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling dynamic_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/envelope_section_a35a0a34.o: ../../../src/editor_sections/envelope_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling envelope_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/extra_mod_section_ea9c0745.o: ../../../src/editor_sections/extra_mod_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling extra_mod_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/feedback_section_56c0fe69.o: ../../../src/editor_sections/feedback_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling feedback_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/filter_section_9094381c.o: ../../../src/editor_sections/filter_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling filter_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/formant_section_51494d55.o: ../../../src/editor_sections/formant_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling formant_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/full_interface_29b5f507.o: ../../../src/editor_sections/full_interface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling full_interface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/lfo_section_f85fa167.o: ../../../src/editor_sections/lfo_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling lfo_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/mixer_section_9e4a737b.o: ../../../src/editor_sections/mixer_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling mixer_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/noise_section_31fdfd0c.o: ../../../src/editor_sections/noise_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling noise_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/open_gl_modulation_manager_939ca4d.o: ../../../src/editor_sections/open_gl_modulation_manager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling open_gl_modulation_manager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/oscillator_section_c90fdcc4.o: ../../../src/editor_sections/oscillator_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling oscillator_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/patch_browser_b6fb73fd.o: ../../../src/editor_sections/patch_browser.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling patch_browser.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/reverb_section_a0ad9f76.o: ../../../src/editor_sections/reverb_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling reverb_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/save_section_1a7d8ce1.o: ../../../src/editor_sections/save_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling save_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/step_sequencer_section_13843dc2.o: ../../../src/editor_sections/step_sequencer_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling step_sequencer_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/stutter_section_943b1913.o: ../../../src/editor_sections/stutter_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling stutter_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/sub_section_8cb704f2.o: ../../../src/editor_sections/sub_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling sub_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/synth_section_3b1ee2e.o: ../../../src/editor_sections/synth_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling synth_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/synthesis_interface_a75e885a.o: ../../../src/editor_sections/synthesis_interface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling synthesis_interface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/update_check_section_c57e00f6.o: ../../../src/editor_sections/update_check_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling update_check_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/voice_section_99705324.o: ../../../src/editor_sections/voice_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling voice_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/volume_section_9cff0be.o: ../../../src/editor_sections/volume_section.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling volume_section.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/browser_look_and_feel_d6654391.o: ../../../src/look_and_feel/browser_look_and_feel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling browser_look_and_feel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/colors_9855f240.o: ../../../src/look_and_feel/colors.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling colors.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/default_look_and_feel_f168ba6a.o: ../../../src/look_and_feel/default_look_and_feel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling default_look_and_feel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/fonts_7e4e153e.o: ../../../src/look_and_feel/fonts.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling fonts.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/modulation_look_and_feel_605bddbb.o: ../../../src/look_and_feel/modulation_look_and_feel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling modulation_look_and_feel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/shaders_8f61ea28.o: ../../../src/look_and_feel/shaders.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling shaders.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/text_look_and_feel_4af8536c.o: ../../../src/look_and_feel/text_look_and_feel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling text_look_and_feel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/helm_editor_cad94f01.o: ../../../src/plugin/helm_editor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling helm_editor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/helm_plugin_108c49c7.o: ../../../src/plugin/helm_plugin.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling helm_plugin.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/dc_filter_3d140d58.o: ../../../src/synthesis/dc_filter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling dc_filter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/detune_lookup_ea628520.o: ../../../src/synthesis/detune_lookup.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling detune_lookup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/fixed_point_oscillator_66a86444.o: ../../../src/synthesis/fixed_point_oscillator.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling fixed_point_oscillator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/fixed_point_wave_2344895d.o: ../../../src/synthesis/fixed_point_wave.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling fixed_point_wave.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/gate_73f8a3b5.o: ../../../src/synthesis/gate.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling gate.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/helm_engine_2e44f843.o: ../../../src/synthesis/helm_engine.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling helm_engine.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/helm_lfo_c32ba99e.o: ../../../src/synthesis/helm_lfo.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling helm_lfo.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/helm_module_a4927f6d.o: ../../../src/synthesis/helm_module.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling helm_module.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/helm_oscillators_bb02f03c.o: ../../../src/synthesis/helm_oscillators.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling helm_oscillators.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/helm_voice_handler_35395fa6.o: ../../../src/synthesis/helm_voice_handler.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling helm_voice_handler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/noise_oscillator_93de254f.o: ../../../src/synthesis/noise_oscillator.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling noise_oscillator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/peak_meter_cadcb853.o: ../../../src/synthesis/peak_meter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling peak_meter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/resonance_cancel_67415ef5.o: ../../../src/synthesis/resonance_cancel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling resonance_cancel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/trigger_random_750c5e54.o: ../../../src/synthesis/trigger_random.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling trigger_random.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/value_switch_f497502c.o: ../../../src/synthesis/value_switch.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling value_switch.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_51699c3.o: ../../../JuceLibraryCode/BinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_basics_68f957b.o: ../../../JuceLibraryCode/include_juce_audio_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_devices_6eefc5f1.o: ../../../JuceLibraryCode/include_juce_audio_devices.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_devices.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_formats_21d6c8f0.o: ../../../JuceLibraryCode/include_juce_audio_formats.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_formats.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_utils_baa4955d.o: ../../../JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_processors_5ced3317.o: ../../../JuceLibraryCode/include_juce_audio_processors.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_processors.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_utils_a3a1cb85.o: ../../../JuceLibraryCode/include_juce_audio_utils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_core_d832d60c.o: ../../../JuceLibraryCode/include_juce_core.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_core.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_data_structures_349db12.o: ../../../JuceLibraryCode/include_juce_data_structures.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_data_structures.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_events_9b26cc86.o: ../../../JuceLibraryCode/include_juce_events.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_events.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_graphics_eb811ef8.o: ../../../JuceLibraryCode/include_juce_graphics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_graphics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_basics_a2082cf6.o: ../../../JuceLibraryCode/include_juce_gui_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_extra_e7ac9489.o: ../../../JuceLibraryCode/include_juce_gui_extra.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_extra.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_opengl_95d8f91c.o: ../../../JuceLibraryCode/include_juce_opengl.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_opengl.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_osc_da4097fc.o: ../../../JuceLibraryCode/include_juce_osc.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_osc.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

clean:
	@echo Cleaning Helm
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping Helm
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

-include $(OBJECTS_VST:%.o=%.d)
-include $(OBJECTS_VST3:%.o=%.d)
-include $(OBJECTS_SHARED_CODE:%.o=%.d)
