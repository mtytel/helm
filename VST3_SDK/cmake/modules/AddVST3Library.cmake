
# Output Directory for VST3 Plug-Ins
set(VST3_OUTPUT_DIR ${CMAKE_BINARY_DIR}/VST3)

option(SMTG_RUN_VST_VALIDATOR "Run VST validator on VST3 plug-ins" ON)

# Run the validator after building
function(smtg_run_vst_validator target)
    add_dependencies(${target} validator)
    if(WIN)
        set(TARGET_PATH $<TARGET_FILE:${target}>)
    elseif(XCODE)
        set(TARGET_PATH "${VST3_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/${target}.vst3")
    else()
        set(TARGET_PATH "${VST3_OUTPUT_DIR}/${target}.vst3")
    endif()
    add_custom_command(TARGET ${target} POST_BUILD COMMAND $<TARGET_FILE:validator> "${TARGET_PATH}" WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endfunction()

#-------------------------------------------------------------------------------
# VST3 Library Template
#-------------------------------------------------------------------------------
function(smtg_add_vst3plugin target sdkroot)
    set(sources ${ARGN})
    if(MAC)
        list(APPEND sources "${sdkroot}/public.sdk/source/main/macmain.cpp")
    elseif(WIN)
        list(APPEND sources "${sdkroot}/public.sdk/source/main/dllmain.cpp")
    elseif(LINUX)
        list(APPEND sources "${sdkroot}/public.sdk/source/main/linuxmain.cpp")
    endif()

    add_library(${target} MODULE ${sources})
    set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${VST3_OUTPUT_DIR})

    if(MAC)
        set_target_properties(${target} PROPERTIES BUNDLE TRUE)
        if(XCODE)
            set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE "YES")
            set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_WRAPPER_EXTENSION "vst3")
        else()
            set_target_properties(${target} PROPERTIES BUNDLE_EXTENSION "vst3")
        endif()
        smtg_set_exported_symbols(${target} "${sdkroot}/public.sdk/source/main/macexport.exp")

        target_link_libraries(${target} PRIVATE "-framework CoreFoundation")
    elseif(WIN)
        set_target_properties(${target} PROPERTIES SUFFIX ".vst3")
        set_target_properties(${target} PROPERTIES LINK_FLAGS "/EXPORT:GetPluginFactory")
    elseif(LINUX)
        # ...
        EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
        set(target_lib_dir ${ARCHITECTURE}-linux)
        set_target_properties(${target} PROPERTIES PREFIX "")
        set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${VST3_OUTPUT_DIR}/${target}.vst3/Contents/${target_lib_dir}")
        add_custom_command(TARGET ${target} PRE_LINK
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "${VST3_OUTPUT_DIR}/${target}.vst3/Contents/Resources"
        )
    endif()
    if(SMTG_RUN_VST_VALIDATOR)
        smtg_run_vst_validator(${target})
    endif()
endfunction()

function(smtg_add_vst3_resource target input_file)
    if (LINUX)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_LIST_DIR}/${input_file}"
            "${VST3_OUTPUT_DIR}/${target}.vst3/Contents/Resources/"
        )
	elseif(MAC)
	    target_sources(${target} PRIVATE ${input_file})
        set_source_files_properties(${input_file} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    endif(LINUX)
endfunction()
