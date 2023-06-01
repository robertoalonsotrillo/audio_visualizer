cmake_minimum_required(VERSION 3.15)

project(AudioFeatureVisualiser VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/cache" CACHE PATH "Directory where downloaded dependencies will be cached")
include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage(NAME JUCE
        GITHUB_REPOSITORY juce-framework/JUCE
        GIT_TAG master)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

juce_add_gui_app(${PROJECT_NAME}
        # VERSION ...                       # Set this if the app version is different to the project version
        # ICON_BIG ...                      # ICON_* arguments specify a path to an image file to use as an icon
        # ICON_SMALL ...
        # DOCUMENT_EXTENSIONS ...           # Specify file extensions that should be associated with this app
        # COMPANY_NAME ...                  # Specify the name of the app's author
        MICROPHONE_PERMISSION_ENABLED TRUE
        MICROPHONE_PERMISSION_TEXT "Audio Feature Visualiser needs access to your audio interface"
        HARDENED_RUNTIME_ENABLED TRUE
        HARDENED_RUNTIME_OPTIONS com.apple.security.device.audio-input
        PRODUCT_NAME "Audio Feature Visualiser")     # The name of the final executable, which can differ from the target name

target_sources(${PROJECT_NAME}
        PRIVATE
        Source/Windows/Window.cpp
        Source/Windows/GraphWindow.cpp
        Source/Windows/WindowContainer.cpp
        Source/Main.cpp
        Source/MainComponent.cpp
        Source/LockFreeFifo.h
        Source/Config.cpp)

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_VERSION>"
        JUCE_USE_MP3AUDIOFORMAT
        JUCE_MODAL_LOOPS_PERMITTED=1)

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

# juce_add_binary_data(AppData SOURCES ...)

# `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_gui_extra` module. Inter-module
# dependencies are resolved automatically, so `juce_core`, `juce_events` and so on will also be
# linked automatically. If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.

find_library(ESSENTIA_LIB essentia essentia.a)
message(STATUS "essentia lib: [${ESSENTIA_LIB}]")

find_path(ESSENTIA_HEADERS essentia)
find_path(EIGEN3_HEADERS eigen3)
message(STATUS "essentia headers: [${ESSENTIA_HEADERS}]")
message(STATUS "eigen3 headers: [${EIGEN3_HEADERS}]")

target_include_directories(${PROJECT_NAME} PRIVATE
        ${ESSENTIA_HEADERS}/essentia
        ${EIGEN3_HEADERS}/eigen3
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        # AppData            # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_gui_extra
        juce::juce_osc
        ${ESSENTIA_LIB}
        nlohmann_json::nlohmann_json
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)