
#-------------------------------------------------------------------------------
# Platform
#-------------------------------------------------------------------------------

function(smtg_set_platform_ios target)
    if(NOT MAC)
        message(FATAL_ERROR "smtg_set_platform_ios only works on macOS, use it in an if(MAC) block")
    endif()
    set_target_properties(${target} PROPERTIES OSX_ARCHITECTURES "armv7;arm64")
    set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "8.0")
    set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_SDKROOT "iphoneos8.0")
endfunction()
