install(
    TARGETS gol-cpp_exe
    RUNTIME COMPONENT gol-cpp_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
