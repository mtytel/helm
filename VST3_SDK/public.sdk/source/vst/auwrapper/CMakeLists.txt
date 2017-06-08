if(MAC)
	if (XCODE AND SMTG_COREAUDIO_SDK_PATH)
		set(target auwrapper)
		set(${target}_sources
			aucarbonview.mm
			aucarbonview.h
			aucocoaview.mm
			aucocoaview.h
			ausdk.mm
			auwrapper.mm
			auwrapper.h
			NSDataIBStream.mm
			NSDataIBStream.h
			${SDK_ROOT}/pluginterfaces/base/funknown.cpp
			${SDK_ROOT}/pluginterfaces/base/ustring.cpp
			${SDK_ROOT}/public.sdk/source/vst/hosting/eventlist.cpp
			${SDK_ROOT}/public.sdk/source/vst/hosting/eventlist.h
			${SDK_ROOT}/public.sdk/source/vst/hosting/hostclasses.cpp
			${SDK_ROOT}/public.sdk/source/vst/hosting/hostclasses.h
			${SDK_ROOT}/public.sdk/source/vst/hosting/parameterchanges.cpp
			${SDK_ROOT}/public.sdk/source/vst/hosting/parameterchanges.h
			${SDK_ROOT}/public.sdk/source/vst/hosting/processdata.cpp
			${SDK_ROOT}/public.sdk/source/vst/hosting/processdata.h
		)
		add_library(${target} STATIC ${${target}_sources})
		target_link_libraries(${target} PRIVATE base "-framework AudioUnit" "-framework CoreMIDI" "-framework AudioToolbox" "-framework CoreFoundation" "-framework Carbon" "-framework Cocoa" "-framework CoreAudio")
		target_include_directories(${target} PRIVATE "${SMTG_COREAUDIO_SDK_PATH}/**")
	    add_custom_command(TARGET ${target} PRE_BUILD COMMAND /usr/bin/ruby "./generateCocoaClassNamePrefix.rb" "${CMAKE_CURRENT_LIST_DIR}" WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
	else()
		message("* To enable building the AudioUnit wrapper, you need to use the Xcode generator and set SMTG_COREAUDIO_SDK_PATH to the path of your installation of the CoreAudio SDK!")
	endif()
endif()