if(MAC AND XCODE AND IOS_DEVELOPMENT_TEAM)
	set(target interappaudio)

	set(${target}_sources
		AudioIO.mm
		AudioIO.h
		HostApp.mm
		HostApp.h
		MidiIO.mm
		MidiIO.h
		PresetBrowserViewController.mm
		PresetBrowserViewController.h
		PresetManager.mm
		PresetManager.h
		PresetSaveViewController.mm
		PresetSaveViewController.h
		SettingsViewController.mm
		SettingsViewController.h
		VST3Editor.mm
		VST3Editor.h
		VST3Plugin.mm
		VST3Plugin.h
		VSTInterAppAudioAppDelegateBase.mm
		VSTInterAppAudioAppDelegateBase.h
	)
	add_library(${target} STATIC ${${target}_sources})
	smtg_set_platform_ios(${target})
	target_link_libraries(${target} PRIVATE sdk_ios "-framework CoreGraphics" "-framework UIKit" "-framework CoreMIDI" "-framework AudioToolbox" "-framework AVFoundation")
endif()
