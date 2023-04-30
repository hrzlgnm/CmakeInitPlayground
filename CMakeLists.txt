cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    gol-cpp
    VERSION 0.1.0
    DESCRIPTION "Conways Game Of Life in C++"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare executable ----
add_executable(gol-cpp_exe source/main.cpp)
add_executable(gol-cpp::exe ALIAS gol-cpp_exe)

set_property(TARGET gol-cpp_exe PROPERTY OUTPUT_NAME gol-cpp)

target_compile_features(gol-cpp_exe PRIVATE cxx_std_17)
# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT gol-cpp_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of gol-cpp"
  )
endif()

include(cmake/dev-mode.cmake)
