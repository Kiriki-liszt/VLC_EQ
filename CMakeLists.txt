cmake_minimum_required(VERSION 3.14.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

if(SMTG_MAC)
    set(vst3sdk_SOURCE_DIR "/Users/yoon/VST_SDK/vst3sdk")
elseif(SMTG_WIN)
    set(vst3sdk_SOURCE_DIR "C:/VST_SDK/vst3sdk")
endif(SMTG_MAC)

if(GITHUB_ACTIONS)
    set(vst3sdk_SOURCE_DIR ${CMAKE_SOURCE_DIR}/vst3sdk)
endif()

project(VLC_EQ
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "VLC_EQ VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(VLC_EQ
    source/version.h
    source/VLCEQ_cids.h
    source/VLCEQ_shared.h
    source/VLCEQ_processor.h
    source/VLCEQ_processor.cpp
    source/VLCEQ_controller.h
    source/VLCEQ_controller.cpp
    source/VLCEQ_entry.cpp
)

#- VSTGUI Wanted ----
if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(VLC_EQ
        PRIVATE
            resource/VLCEQ_editor.uidesc
    )
    target_link_libraries(VLC_EQ
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(VLC_EQ
        RESOURCES
            "resource/VLCEQ_editor.uidesc"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)
# -------------------

smtg_target_add_plugin_snapshots (VLC_EQ
    RESOURCES
        resource/873369657970557299B67BD062809F5C_snapshot.png
        resource/873369657970557299B67BD062809F5C_snapshot_2.0x.png
)

target_link_libraries(VLC_EQ
    PRIVATE
        sdk
)

smtg_target_configure_version_file(VLC_EQ)

if(SMTG_MAC)
    smtg_target_set_bundle(VLC_EQ
        BUNDLE_IDENTIFIER io.github.yg331.VLC.EQ
        COMPANY_NAME "yg331"
    )
    smtg_target_set_debug_executable(VLC_EQ
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(VLC_EQ PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT VLC_EQ)

        smtg_target_set_debug_executable(VLC_EQ
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)

# Add an AUv2 target
if (SMTG_MAC AND XCODE AND SMTG_COREAUDIO_SDK_PATH)
	smtg_target_add_auv2(VLC_EQ-au
		BUNDLE_NAME VLC_EQ
		BUNDLE_IDENTIFIER io.github.yg331.VLC.EQ.audiounit
		INFO_PLIST_TEMPLATE resource/au-info.plist
		VST3_PLUGIN_TARGET VLC_EQ)
endif(SMTG_MAC AND XCODE AND SMTG_COREAUDIO_SDK_PATH)
