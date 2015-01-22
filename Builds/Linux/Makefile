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

  CPPFLAGS := $(DEPFLAGS) -std=c++11 -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I "~/SDKs/VST3 SDK" -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../srcs/JUCE/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -fPIC -O0
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I "~/SDKs/VST3 SDK" -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../srcs/JUCE/modules
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

  CPPFLAGS := $(DEPFLAGS) -std=c++11 -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I "~/SDKs/VST3 SDK" -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../srcs/JUCE/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -Os
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -fvisibility=hidden -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I "~/SDKs/VST3 SDK" -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../srcs/JUCE/modules
  TARGET := twytch.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/ACBaseCodec_7bc443b9.o \
  $(OBJDIR)/ACCodec_9a27c70a.o \
  $(OBJDIR)/ACPlugInDispatch_a900acc3.o \
  $(OBJDIR)/ACSimpleCodec_3230d238.o \
  $(OBJDIR)/GetCodecBundle_ca937c76.o \
  $(OBJDIR)/AudioFileComponentBase_5ab7380c.o \
  $(OBJDIR)/AudioFileFormat_94afb063.o \
  $(OBJDIR)/AudioFileObject_c0f977ab.o \
  $(OBJDIR)/CompressedPacketTable_876d4b7f.o \
  $(OBJDIR)/DataSource_227d2855.o \
  $(OBJDIR)/AUBase_8777d8bf.o \
  $(OBJDIR)/AUDispatch_16659568.o \
  $(OBJDIR)/AUInputElement_ce7a8840.o \
  $(OBJDIR)/AUOutputElement_1f85a857.o \
  $(OBJDIR)/AUPlugInDispatch_e5dbe19b.o \
  $(OBJDIR)/AUScopeElement_c3ea0776.o \
  $(OBJDIR)/ComponentBase_54bba9fe.o \
  $(OBJDIR)/AUCarbonViewBase_c6533eff.o \
  $(OBJDIR)/AUCarbonViewControl_94814cf9.o \
  $(OBJDIR)/AUCarbonViewDispatch_10a3dba8.o \
  $(OBJDIR)/AUControlGroup_3f03b8be.o \
  $(OBJDIR)/CarbonEventHandler_9746c7c5.o \
  $(OBJDIR)/AUInstrumentBase_d94f2f9f.o \
  $(OBJDIR)/SynthElement_37f90c33.o \
  $(OBJDIR)/SynthNote_b9131065.o \
  $(OBJDIR)/SynthNoteList_4dd04e23.o \
  $(OBJDIR)/AUEffectBase_9197bb43.o \
  $(OBJDIR)/AUMIDIBase_b0192bb3.o \
  $(OBJDIR)/AUMIDIEffectBase_7e1ab2a4.o \
  $(OBJDIR)/AUOutputBase_5ca3aab3.o \
  $(OBJDIR)/AUPannerBase_d731bc90.o \
  $(OBJDIR)/MusicDeviceBase_cf340ba9.o \
  $(OBJDIR)/AUBaseHelper_3a627030.o \
  $(OBJDIR)/AUBuffer_faeef811.o \
  $(OBJDIR)/AUTimestampGenerator_5519adae.o \
  $(OBJDIR)/AUOutputBL_12491809.o \
  $(OBJDIR)/AUParamInfo_43dadb67.o \
  $(OBJDIR)/CAAudioBufferList_60a3cfd6.o \
  $(OBJDIR)/CAAudioChannelLayout_5796803f.o \
  $(OBJDIR)/CAAudioChannelLayoutObject_d4cf789e.o \
  $(OBJDIR)/CAAudioFileFormats_1ce1fa32.o \
  $(OBJDIR)/CAAudioTimeStamp_18cb4d28.o \
  $(OBJDIR)/CAAudioUnit_7cb7339c.o \
  $(OBJDIR)/CAAudioValueRange_3b67d624.o \
  $(OBJDIR)/CAAUMIDIMap_f72ece89.o \
  $(OBJDIR)/CAAUMIDIMapManager_de6ae96e.o \
  $(OBJDIR)/CAAUParameter_6d1be657.o \
  $(OBJDIR)/CAAUProcessor_223e2c0.o \
  $(OBJDIR)/CABufferList_c3c9bae6.o \
  $(OBJDIR)/CABundleLocker_4e6cc962.o \
  $(OBJDIR)/CACFArray_1c5b638.o \
  $(OBJDIR)/CACFDictionary_e79bece1.o \
  $(OBJDIR)/CACFDistributedNotification_d441120d.o \
  $(OBJDIR)/CACFMachPort_5abeb4e5.o \
  $(OBJDIR)/CACFMessagePort_80f08e07.o \
  $(OBJDIR)/CACFNumber_6a7e8114.o \
  $(OBJDIR)/CACFPreferences_5ac53d7.o \
  $(OBJDIR)/CACFString_241b61dc.o \
  $(OBJDIR)/CAComponent_670d489f.o \
  $(OBJDIR)/CAComponentDescription_26b27467.o \
  $(OBJDIR)/CADebugger_28d6bd49.o \
  $(OBJDIR)/CADebugMacros_606c243c.o \
  $(OBJDIR)/CADebugPrintf_b953006e.o \
  $(OBJDIR)/CAFilePathUtils_15ff5d12.o \
  $(OBJDIR)/CAGuard_3855f4c7.o \
  $(OBJDIR)/CAHALAudioDevice_981f7821.o \
  $(OBJDIR)/CAHALAudioObject_a926608a.o \
  $(OBJDIR)/CAHALAudioStream_2259044b.o \
  $(OBJDIR)/CAHALAudioSystemObject_f3049799.o \
  $(OBJDIR)/CAHostTimeBase_be16ee2e.o \
  $(OBJDIR)/CAMutex_89f1caa1.o \
  $(OBJDIR)/CAPersistence_c430a601.o \
  $(OBJDIR)/CAProcess_8ace3851.o \
  $(OBJDIR)/CAPThread_d2faf2fc.o \
  $(OBJDIR)/CARingBuffer_b2e28f78.o \
  $(OBJDIR)/CASettingsStorage_8eac813a.o \
  $(OBJDIR)/CASharedLibrary_5c5c7d38.o \
  $(OBJDIR)/CASpectralProcessor_7cd5e5b0.o \
  $(OBJDIR)/CAStreamBasicDescription_7a1647f6.o \
  $(OBJDIR)/CAStreamRangedDescription_d91847f7.o \
  $(OBJDIR)/CAVectorUnit_b7919acf.o \
  $(OBJDIR)/CAVolumeCurve_32dd5257.o \
  $(OBJDIR)/CAXException_4b95941f.o \
  $(OBJDIR)/MatrixMixerVolumes_2141177b.o \
  $(OBJDIR)/filter_response_c5a98742.o \
  $(OBJDIR)/full_interface_d7b651b9.o \
  $(OBJDIR)/graphical_envelope_81dd68ac.o \
  $(OBJDIR)/graphical_step_sequencer_849bfbba.o \
  $(OBJDIR)/modulation_destination_e92c1ffb.o \
  $(OBJDIR)/modulation_source_4d88e218.o \
  $(OBJDIR)/synthesis_interface_e6f64ae8.o \
  $(OBJDIR)/wave_form_selector_d4921d64.o \
  $(OBJDIR)/twytch_10347fac.o \
  $(OBJDIR)/twytch_editor_e7ee1a6a.o \
  $(OBJDIR)/twytch_engine_9a8780bf.o \
  $(OBJDIR)/twytch_look_and_feel_4eb9069b.o \
  $(OBJDIR)/twytch_oscillators_8eae5540.o \
  $(OBJDIR)/twytch_strings_fab1886f.o \
  $(OBJDIR)/twytch_voice_handler_cb9f93aa.o \
  $(OBJDIR)/alias_1d388dcc.o \
  $(OBJDIR)/arpeggiator_9d4b0575.o \
  $(OBJDIR)/bit_crush_f4910885.o \
  $(OBJDIR)/delay_a1e4df7f.o \
  $(OBJDIR)/envelope_265dd45e.o \
  $(OBJDIR)/feedback_1ba2cd93.o \
  $(OBJDIR)/filter_a2f71e46.o \
  $(OBJDIR)/linear_slope_aafebf9f.o \
  $(OBJDIR)/magnitude_lookup_2d96b997.o \
  $(OBJDIR)/midi_lookup_47a7ae94.o \
  $(OBJDIR)/mono_panner_e32a65f6.o \
  $(OBJDIR)/operators_17db10cb.o \
  $(OBJDIR)/oscillator_f8f582ee.o \
  $(OBJDIR)/processor_4dff978e.o \
  $(OBJDIR)/processor_router_23bde824.o \
  $(OBJDIR)/resonance_lookup_12e6d099.o \
  $(OBJDIR)/send_receive_dbc9033a.o \
  $(OBJDIR)/smooth_filter_7e275f25.o \
  $(OBJDIR)/smooth_value_e19bb7ae.o \
  $(OBJDIR)/step_generator_6ff0a2ee.o \
  $(OBJDIR)/trigger_operators_1e299f84.o \
  $(OBJDIR)/value_9037106d.o \
  $(OBJDIR)/voice_handler_b8888f39.o \
  $(OBJDIR)/wave_42d06f87.o \
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

$(OBJDIR)/ACBaseCodec_7bc443b9.o: ../../../srcs/CoreAudio/AudioCodecs/ACPublic/ACBaseCodec.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ACBaseCodec.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ACCodec_9a27c70a.o: ../../../srcs/CoreAudio/AudioCodecs/ACPublic/ACCodec.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ACCodec.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ACPlugInDispatch_a900acc3.o: ../../../srcs/CoreAudio/AudioCodecs/ACPublic/ACPlugInDispatch.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ACPlugInDispatch.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ACSimpleCodec_3230d238.o: ../../../srcs/CoreAudio/AudioCodecs/ACPublic/ACSimpleCodec.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ACSimpleCodec.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GetCodecBundle_ca937c76.o: ../../../srcs/CoreAudio/AudioCodecs/ACPublic/GetCodecBundle.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GetCodecBundle.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioFileComponentBase_5ab7380c.o: ../../../srcs/CoreAudio/AudioFile/AFPublic/AudioFileComponentBase.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioFileComponentBase.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioFileFormat_94afb063.o: ../../../srcs/CoreAudio/AudioFile/AFPublic/AudioFileFormat.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioFileFormat.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioFileObject_c0f977ab.o: ../../../srcs/CoreAudio/AudioFile/AFPublic/AudioFileObject.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioFileObject.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CompressedPacketTable_876d4b7f.o: ../../../srcs/CoreAudio/AudioFile/AFPublic/CompressedPacketTable.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CompressedPacketTable.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DataSource_227d2855.o: ../../../srcs/CoreAudio/AudioFile/AFPublic/DataSource.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DataSource.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUBase_8777d8bf.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUBase/AUBase.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUBase.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUDispatch_16659568.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUBase/AUDispatch.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUDispatch.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUInputElement_ce7a8840.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUBase/AUInputElement.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUInputElement.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUOutputElement_1f85a857.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUBase/AUOutputElement.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUOutputElement.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUPlugInDispatch_e5dbe19b.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUBase/AUPlugInDispatch.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUPlugInDispatch.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUScopeElement_c3ea0776.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUBase/AUScopeElement.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUScopeElement.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ComponentBase_54bba9fe.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUBase/ComponentBase.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ComponentBase.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUCarbonViewBase_c6533eff.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUCarbonViewBase/AUCarbonViewBase.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUCarbonViewBase.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUCarbonViewControl_94814cf9.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUCarbonViewBase/AUCarbonViewControl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUCarbonViewControl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUCarbonViewDispatch_10a3dba8.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUCarbonViewBase/AUCarbonViewDispatch.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUCarbonViewDispatch.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUControlGroup_3f03b8be.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUCarbonViewBase/AUControlGroup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUControlGroup.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CarbonEventHandler_9746c7c5.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUCarbonViewBase/CarbonEventHandler.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CarbonEventHandler.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUInstrumentBase_d94f2f9f.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUInstrumentBase/AUInstrumentBase.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUInstrumentBase.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SynthElement_37f90c33.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUInstrumentBase/SynthElement.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SynthElement.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SynthNote_b9131065.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUInstrumentBase/SynthNote.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SynthNote.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SynthNoteList_4dd04e23.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/AUInstrumentBase/SynthNoteList.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SynthNoteList.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUEffectBase_9197bb43.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/OtherBases/AUEffectBase.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUEffectBase.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUMIDIBase_b0192bb3.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/OtherBases/AUMIDIBase.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUMIDIBase.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUMIDIEffectBase_7e1ab2a4.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/OtherBases/AUMIDIEffectBase.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUMIDIEffectBase.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUOutputBase_5ca3aab3.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/OtherBases/AUOutputBase.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUOutputBase.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUPannerBase_d731bc90.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/OtherBases/AUPannerBase.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUPannerBase.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MusicDeviceBase_cf340ba9.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/OtherBases/MusicDeviceBase.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MusicDeviceBase.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUBaseHelper_3a627030.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/Utility/AUBaseHelper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUBaseHelper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUBuffer_faeef811.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/Utility/AUBuffer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUBuffer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUTimestampGenerator_5519adae.o: ../../../srcs/CoreAudio/AudioUnits/AUPublic/Utility/AUTimestampGenerator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUTimestampGenerator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUOutputBL_12491809.o: ../../../srcs/CoreAudio/PublicUtility/AUOutputBL.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUOutputBL.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AUParamInfo_43dadb67.o: ../../../srcs/CoreAudio/PublicUtility/AUParamInfo.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AUParamInfo.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAAudioBufferList_60a3cfd6.o: ../../../srcs/CoreAudio/PublicUtility/CAAudioBufferList.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAAudioBufferList.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAAudioChannelLayout_5796803f.o: ../../../srcs/CoreAudio/PublicUtility/CAAudioChannelLayout.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAAudioChannelLayout.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAAudioChannelLayoutObject_d4cf789e.o: ../../../srcs/CoreAudio/PublicUtility/CAAudioChannelLayoutObject.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAAudioChannelLayoutObject.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAAudioFileFormats_1ce1fa32.o: ../../../srcs/CoreAudio/PublicUtility/CAAudioFileFormats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAAudioFileFormats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAAudioTimeStamp_18cb4d28.o: ../../../srcs/CoreAudio/PublicUtility/CAAudioTimeStamp.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAAudioTimeStamp.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAAudioUnit_7cb7339c.o: ../../../srcs/CoreAudio/PublicUtility/CAAudioUnit.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAAudioUnit.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAAudioValueRange_3b67d624.o: ../../../srcs/CoreAudio/PublicUtility/CAAudioValueRange.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAAudioValueRange.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAAUMIDIMap_f72ece89.o: ../../../srcs/CoreAudio/PublicUtility/CAAUMIDIMap.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAAUMIDIMap.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAAUMIDIMapManager_de6ae96e.o: ../../../srcs/CoreAudio/PublicUtility/CAAUMIDIMapManager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAAUMIDIMapManager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAAUParameter_6d1be657.o: ../../../srcs/CoreAudio/PublicUtility/CAAUParameter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAAUParameter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAAUProcessor_223e2c0.o: ../../../srcs/CoreAudio/PublicUtility/CAAUProcessor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAAUProcessor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CABufferList_c3c9bae6.o: ../../../srcs/CoreAudio/PublicUtility/CABufferList.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CABufferList.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CABundleLocker_4e6cc962.o: ../../../srcs/CoreAudio/PublicUtility/CABundleLocker.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CABundleLocker.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CACFArray_1c5b638.o: ../../../srcs/CoreAudio/PublicUtility/CACFArray.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CACFArray.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CACFDictionary_e79bece1.o: ../../../srcs/CoreAudio/PublicUtility/CACFDictionary.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CACFDictionary.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CACFDistributedNotification_d441120d.o: ../../../srcs/CoreAudio/PublicUtility/CACFDistributedNotification.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CACFDistributedNotification.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CACFMachPort_5abeb4e5.o: ../../../srcs/CoreAudio/PublicUtility/CACFMachPort.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CACFMachPort.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CACFMessagePort_80f08e07.o: ../../../srcs/CoreAudio/PublicUtility/CACFMessagePort.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CACFMessagePort.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CACFNumber_6a7e8114.o: ../../../srcs/CoreAudio/PublicUtility/CACFNumber.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CACFNumber.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CACFPreferences_5ac53d7.o: ../../../srcs/CoreAudio/PublicUtility/CACFPreferences.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CACFPreferences.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CACFString_241b61dc.o: ../../../srcs/CoreAudio/PublicUtility/CACFString.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CACFString.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAComponent_670d489f.o: ../../../srcs/CoreAudio/PublicUtility/CAComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAComponentDescription_26b27467.o: ../../../srcs/CoreAudio/PublicUtility/CAComponentDescription.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAComponentDescription.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CADebugger_28d6bd49.o: ../../../srcs/CoreAudio/PublicUtility/CADebugger.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CADebugger.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CADebugMacros_606c243c.o: ../../../srcs/CoreAudio/PublicUtility/CADebugMacros.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CADebugMacros.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CADebugPrintf_b953006e.o: ../../../srcs/CoreAudio/PublicUtility/CADebugPrintf.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CADebugPrintf.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAFilePathUtils_15ff5d12.o: ../../../srcs/CoreAudio/PublicUtility/CAFilePathUtils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAFilePathUtils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAGuard_3855f4c7.o: ../../../srcs/CoreAudio/PublicUtility/CAGuard.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAGuard.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAHALAudioDevice_981f7821.o: ../../../srcs/CoreAudio/PublicUtility/CAHALAudioDevice.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAHALAudioDevice.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAHALAudioObject_a926608a.o: ../../../srcs/CoreAudio/PublicUtility/CAHALAudioObject.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAHALAudioObject.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAHALAudioStream_2259044b.o: ../../../srcs/CoreAudio/PublicUtility/CAHALAudioStream.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAHALAudioStream.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAHALAudioSystemObject_f3049799.o: ../../../srcs/CoreAudio/PublicUtility/CAHALAudioSystemObject.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAHALAudioSystemObject.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAHostTimeBase_be16ee2e.o: ../../../srcs/CoreAudio/PublicUtility/CAHostTimeBase.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAHostTimeBase.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAMutex_89f1caa1.o: ../../../srcs/CoreAudio/PublicUtility/CAMutex.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAMutex.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAPersistence_c430a601.o: ../../../srcs/CoreAudio/PublicUtility/CAPersistence.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAPersistence.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAProcess_8ace3851.o: ../../../srcs/CoreAudio/PublicUtility/CAProcess.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAProcess.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAPThread_d2faf2fc.o: ../../../srcs/CoreAudio/PublicUtility/CAPThread.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAPThread.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CARingBuffer_b2e28f78.o: ../../../srcs/CoreAudio/PublicUtility/CARingBuffer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CARingBuffer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CASettingsStorage_8eac813a.o: ../../../srcs/CoreAudio/PublicUtility/CASettingsStorage.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CASettingsStorage.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CASharedLibrary_5c5c7d38.o: ../../../srcs/CoreAudio/PublicUtility/CASharedLibrary.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CASharedLibrary.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CASpectralProcessor_7cd5e5b0.o: ../../../srcs/CoreAudio/PublicUtility/CASpectralProcessor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CASpectralProcessor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAStreamBasicDescription_7a1647f6.o: ../../../srcs/CoreAudio/PublicUtility/CAStreamBasicDescription.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAStreamBasicDescription.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAStreamRangedDescription_d91847f7.o: ../../../srcs/CoreAudio/PublicUtility/CAStreamRangedDescription.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAStreamRangedDescription.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAVectorUnit_b7919acf.o: ../../../srcs/CoreAudio/PublicUtility/CAVectorUnit.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAVectorUnit.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAVolumeCurve_32dd5257.o: ../../../srcs/CoreAudio/PublicUtility/CAVolumeCurve.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAVolumeCurve.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CAXException_4b95941f.o: ../../../srcs/CoreAudio/PublicUtility/CAXException.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CAXException.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MatrixMixerVolumes_2141177b.o: ../../../srcs/CoreAudio/PublicUtility/MatrixMixerVolumes.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MatrixMixerVolumes.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/filter_response_c5a98742.o: ../../Source/editor_components/filter_response.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling filter_response.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/full_interface_d7b651b9.o: ../../Source/editor_components/full_interface.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling full_interface.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/graphical_envelope_81dd68ac.o: ../../Source/editor_components/graphical_envelope.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling graphical_envelope.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/graphical_step_sequencer_849bfbba.o: ../../Source/editor_components/graphical_step_sequencer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling graphical_step_sequencer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modulation_destination_e92c1ffb.o: ../../Source/editor_components/modulation_destination.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_destination.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modulation_source_4d88e218.o: ../../Source/editor_components/modulation_source.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modulation_source.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/synthesis_interface_e6f64ae8.o: ../../Source/editor_components/synthesis_interface.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling synthesis_interface.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/wave_form_selector_d4921d64.o: ../../Source/editor_components/wave_form_selector.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling wave_form_selector.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_10347fac.o: ../../Source/twytch.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_editor_e7ee1a6a.o: ../../Source/twytch_editor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_editor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_engine_9a8780bf.o: ../../Source/twytch_engine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_engine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_look_and_feel_4eb9069b.o: ../../Source/twytch_look_and_feel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_look_and_feel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_oscillators_8eae5540.o: ../../Source/twytch_oscillators.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_oscillators.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_strings_fab1886f.o: ../../Source/twytch_strings.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_strings.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/twytch_voice_handler_cb9f93aa.o: ../../Source/twytch_voice_handler.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling twytch_voice_handler.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

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

$(OBJDIR)/magnitude_lookup_2d96b997.o: ../../mopo/src/magnitude_lookup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling magnitude_lookup.cpp"
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

$(OBJDIR)/resonance_lookup_12e6d099.o: ../../mopo/src/resonance_lookup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling resonance_lookup.cpp"
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
