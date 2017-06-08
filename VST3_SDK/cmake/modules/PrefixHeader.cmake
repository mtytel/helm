
#-------------------------------------------------------------------------------
# Prefix Header
#-------------------------------------------------------------------------------

function(smtg_set_prefix_header target header precompile)
    if(MSVC)
    elseif(XCODE)
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${header}")
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "${precompile}")
    else()
        target_compile_options(${target} PRIVATE -include "${header}")
    endif()
endfunction()
