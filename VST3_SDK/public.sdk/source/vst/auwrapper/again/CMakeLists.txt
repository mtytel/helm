if(MAC AND XCODE AND SMTG_COREAUDIO_SDK_PATH)
	set(target again_au)
	set(${target}_sources
		doc.cpp
		audiounitconfig.h
		Info.plist
	)
	add_library(${target} MODULE ${${target}_sources})
	set_target_properties(${target} PROPERTIES BUNDLE TRUE)
	set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_GENERATE_MASTER_OBJECT_FILE "YES")
	set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_OTHER_LDFLAGS "-all_load")
	set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE "YES")
	set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_WRAPPER_EXTENSION "component")
	set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${VST3_OUTPUT_DIR})
	target_link_libraries(${target} PRIVATE auwrapper)
	smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/Info.plist" PREPROCESS)
	
	set(outputdir ${VST3_OUTPUT_DIR}/$<CONFIG>)
	
	add_dependencies(${target} again)
	add_custom_command(TARGET ${target} POST_BUILD COMMAND /bin/mkdir "-p" ${outputdir}/${target}.component/Contents/Resources)
	add_custom_command(TARGET ${target} POST_BUILD COMMAND /bin/ln "-sf" "${outputdir}/again.vst3" "${outputdir}/${target}.component/Contents/Resources/plugin.vst3")
	add_custom_command(TARGET ${target} POST_BUILD COMMAND /bin/ln "-sf" "${outputdir}/${target}.component" "~/Library/Audio/Plug-Ins/Components/")

	execute_process(COMMAND xcrun --find Rez OUTPUT_VARIABLE OSX_REZ_COMMAND OUTPUT_STRIP_TRAILING_WHITESPACE)
	add_custom_command(TARGET ${target} POST_BUILD COMMAND "${OSX_REZ_COMMAND}"
		"-d" "SystemSevenOrLater=1"
		"-script" "Roman"
		"-d" "i386_YES"
		"-d" "x86_64_YES"
		"-is" "${CMAKE_OSX_SYSROOT}"
		"-I" "${CMAKE_OSX_SYSROOT}/System/Library/Frameworks/CoreServices.framework/Frameworks/CarbonCore.framework/Versions/A/Headers"
		"-I" "/System/Library/Frameworks/CoreServices.framework/Frameworks/CarbonCore.framework/Versions/A/Headers"
		"-I" "${SMTG_COREAUDIO_SDK_PATH}/AudioUnits/AUPublic/AUBase"
		"-I" "${CMAKE_CURRENT_LIST_DIR}"
		"-o" "${outputdir}/${target}.component/Contents/Resources/again_au.rsrc"
		"-useDF"
		"${CMAKE_CURRENT_LIST_DIR}/../auresource.r"
	)

endif()
