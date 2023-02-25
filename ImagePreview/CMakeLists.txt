# CMakeList.txt : CMake project for ImagePreview, include source and define
# project specific logic here.
#
find_package(ftxui CONFIG)
if(NOT ftxui_FOUND)
  include(FetchContent)
  set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
  FetchContent_Declare(
    ftxui GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v4.0.0
  )

  FetchContent_GetProperties(ftxui)
  if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()
endif()

find_package(stb)
if(NOT stb_FOUND)
  include(FetchContent)
  set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
  FetchContent_Declare(
    stb GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
  )

  FetchContent_GetProperties(stb)
  if(NOT stb_POPULATED)
    FetchContent_Populate(stb)
  endif()
  add_library(stb INTERFACE)
  set(stb_INCLUDE_DIR ${stb_SOURCE_DIR} CACHE PATH "stb librray")
  message(STATUS ${stb_SOURCE_DIR})
endif()

# Add source to this project's executable.
add_executable(ImagePreview "ImagePreview.cpp" "ImagePreview.h" "Image.h"
                            "Console.h")
target_include_directories(ImagePreview PRIVATE ${stb_INCLUDE_DIR})
target_link_libraries(ImagePreview PRIVATE ftxui::screen ftxui::dom ftxui::component)

if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ImagePreview PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
