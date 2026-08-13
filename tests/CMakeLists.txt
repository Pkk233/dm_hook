# tests/CMakeLists.txt
# 在根 CMakeLists.txt 末尾添加：add_subdirectory(tests)
# 然后即可用：cmake --build build --target sample_tests
#            ./build/tests/sample_tests   （或 sample_tests.exe）
add_executable(sample_tests
    sample_utils.cpp
    sample_utils.h
    test_framework.h
    test_sample_utils.cpp
)

target_include_directories(sample_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(sample_tests PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
