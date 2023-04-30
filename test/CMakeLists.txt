# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(gol-cppTests LANGUAGES CXX)

# ---- Tests ----

add_executable(gol-cpp_test source/gol-cpp_test.cpp)
target_compile_features(gol-cpp_test PRIVATE cxx_std_17)

add_test(NAME gol-cpp_test COMMAND gol-cpp_test)

# ---- End-of-file commands ----

add_folders(Test)
