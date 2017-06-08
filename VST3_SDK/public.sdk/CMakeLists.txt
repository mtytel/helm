
set(sdk_sources
	source/common/pluginview.cpp
	source/main/pluginfactoryvst3.cpp
	source/vst/vstaudioeffect.cpp
	source/vst/vstbus.cpp
	source/vst/vstbypassprocessor.cpp
	source/vst/vstcomponent.cpp
	source/vst/vstcomponentbase.cpp
	source/vst/vsteditcontroller.cpp
	source/vst/vstinitiids.cpp
	source/vst/vstnoteexpressiontypes.cpp
	source/vst/vstparameters.cpp
	source/vst/vstpresetfile.cpp
	source/vst/vstrepresentation.cpp
)

set(pluginterfaces_sources
	${SDK_ROOT}/pluginterfaces/base/falignpop.h
	${SDK_ROOT}/pluginterfaces/base/falignpush.h
	${SDK_ROOT}/pluginterfaces/base/fplatform.h
	${SDK_ROOT}/pluginterfaces/base/fstrdefs.h
	${SDK_ROOT}/pluginterfaces/base/ftypes.h
	${SDK_ROOT}/pluginterfaces/base/funknown.h
	${SDK_ROOT}/pluginterfaces/base/futils.h
	${SDK_ROOT}/pluginterfaces/base/fvariant.h
	${SDK_ROOT}/pluginterfaces/base/geoconstants.h
	${SDK_ROOT}/pluginterfaces/base/ibstream.h
	${SDK_ROOT}/pluginterfaces/base/icloneable.h
	${SDK_ROOT}/pluginterfaces/base/ierrorcontext.h
	${SDK_ROOT}/pluginterfaces/base/ipersistent.h
	${SDK_ROOT}/pluginterfaces/base/ipluginbase.h
	${SDK_ROOT}/pluginterfaces/base/istringresult.h
	${SDK_ROOT}/pluginterfaces/base/iupdatehandler.h
	${SDK_ROOT}/pluginterfaces/base/keycodes.h
	${SDK_ROOT}/pluginterfaces/base/pluginbasefwd.h
	${SDK_ROOT}/pluginterfaces/base/smartpointer.h
	${SDK_ROOT}/pluginterfaces/base/ucolorspec.h
	${SDK_ROOT}/pluginterfaces/base/conststringtable.cpp
	${SDK_ROOT}/pluginterfaces/base/conststringtable.h
	${SDK_ROOT}/pluginterfaces/base/funknown.cpp
	${SDK_ROOT}/pluginterfaces/base/ustring.cpp
	${SDK_ROOT}/pluginterfaces/base/ustring.h
)

set(vst_includes
	${SDK_ROOT}/pluginterfaces/gui/iplugview.h
	${SDK_ROOT}/pluginterfaces/gui/iplugviewcontentscalesupport.h
	${SDK_ROOT}/pluginterfaces/vst/ivstattributes.h
	${SDK_ROOT}/pluginterfaces/vst/ivstaudioprocessor.h
	${SDK_ROOT}/pluginterfaces/vst/ivstautomationstate.h
	${SDK_ROOT}/pluginterfaces/vst/ivstchannelcontextinfo.h
	${SDK_ROOT}/pluginterfaces/vst/ivstcomponent.h
	${SDK_ROOT}/pluginterfaces/vst/ivstcontextmenu.h
	${SDK_ROOT}/pluginterfaces/vst/ivsteditcontroller.h
	${SDK_ROOT}/pluginterfaces/vst/ivstevents.h
	${SDK_ROOT}/pluginterfaces/vst/ivsthostapplication.h
	${SDK_ROOT}/pluginterfaces/vst/ivstinterappaudio.h
	${SDK_ROOT}/pluginterfaces/vst/ivstmessage.h
	${SDK_ROOT}/pluginterfaces/vst/ivstmidicontrollers.h
	${SDK_ROOT}/pluginterfaces/vst/ivstnoteexpression.h	
	${SDK_ROOT}/pluginterfaces/vst/ivstparameterchanges.h
	${SDK_ROOT}/pluginterfaces/vst/ivstplugview.h
	${SDK_ROOT}/pluginterfaces/vst/ivstprefetchablesupport.h
	${SDK_ROOT}/pluginterfaces/vst/ivstprocesscontext.h	
	${SDK_ROOT}/pluginterfaces/vst/ivstrepresentation.h	
	${SDK_ROOT}/pluginterfaces/vst/ivstunits.h
	${SDK_ROOT}/pluginterfaces/vst/vstpresetkeys.h
	${SDK_ROOT}/pluginterfaces/vst/vstpshpack4.h
	${SDK_ROOT}/pluginterfaces/vst/vsttypes.h
)

add_library(sdk STATIC ${sdk_sources} ${pluginterfaces_sources} ${vst_includes})
target_link_libraries(sdk PRIVATE base)

# iOS target
if(MAC AND XCODE AND IOS_DEVELOPMENT_TEAM)
	add_library(sdk_ios STATIC ${sdk_sources} ${pluginterfaces_sources} ${vst_includes})
	smtg_set_platform_ios(sdk_ios)
	target_link_libraries(sdk_ios PRIVATE base_ios)
endif()

source_group("public.sdk"	FILES ${sdk_sources})
source_group("vst"			FILES ${vst_includes})
source_group("base"			FILES ${pluginterfaces_sources})
