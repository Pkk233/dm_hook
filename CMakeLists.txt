# ============================================================================
# CMakeLists.txt — 大漠插件 (dm.dll) 开源重新实现 构建配置
# ============================================================================
cmake_minimum_required(VERSION 3.16)
project(dm_hook VERSION 3.1233 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 支持 __stdcall 导出
# 使用 .def 文件精确控制导出（避免 CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 导出 RTTI 符号）
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)

# 源文件
set(SOURCES
    dm.cpp
    dllmain.cpp
    dm_com.cpp
)

# 头文件
set(HEADERS
    dm.h
    dm_com.h
)

# 生成 DLL
add_library(dm_hook SHARED ${SOURCES} ${HEADERS} dm_com.def)

# 目标名称兼容
set_target_properties(dm_hook PROPERTIES
    OUTPUT_NAME "dm_hook"
    PREFIX ""
    LINKER_LANGUAGE CXX
)

# Windows 链接库
target_link_libraries(dm_hook PRIVATE
    gdiplus
    comctl32
    comdlg32
    urlmon
    winmm
    shell32
    ole32
    oleaut32
    User32
    Gdi32
    Kernel32
    Advapi32
)

# 定义导出宏
target_compile_definitions(dm_hook PRIVATE
    DM_EXPORTS
    NOMINMAX
)

# ============================================================================
# 可选依赖: Tesseract OCR (用于文字识别)
# 启用方式: cmake -B build -DENABLE_TESSERACT=ON
# ============================================================================
option(ENABLE_TESSERACT "Enable Tesseract OCR support" OFF)

if(ENABLE_TESSERACT)
    # 自动下载 Tesseract (使用 vcpkg)
    include(FetchContent)
    FetchContent_Declare(
        tesseract
        URL https://github.com/UB-Mannheim/tesseract/archive/refs/tags/v5.3.3.zip
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/deps/tesseract"
    )
    # 注意: 实际使用建议通过 vcpkg 安装
    # vcpkg install tesseract:x64-windows
    find_package(Tesseract QUIET)
    if(Tesseract_FOUND)
        target_link_libraries(dm_hook PRIVATE Tesseract::libtesseract)
        target_compile_definitions(dm_hook PRIVATE DM_HAVE_TESSERACT)
        message(STATUS "Tesseract OCR enabled")
    else()
        message(WARNING "Tesseract not found. OCR functions will be stubs.")
    endif()
endif()

# ============================================================================
# 可选依赖: ONNX Runtime (用于 YOLO AI 推理)
# 启用方式: cmake -B build -DENABLE_ONNX=ON
# ============================================================================
option(ENABLE_ONNX "Enable ONNX Runtime for YOLO" OFF)

if(ENABLE_ONNX)
    # 建议通过 vcpkg 安装
    # vcpkg install onnxruntime:x64-windows
    find_package(onnxruntime QUIET)
    if(onnxruntime_FOUND)
        target_link_libraries(dm_hook PRIVATE onnxruntime::onnxruntime)
        target_compile_definitions(dm_hook PRIVATE DM_HAVE_ONNX)
        message(STATUS "ONNX Runtime enabled")
    else()
        message(WARNING "ONNX Runtime not found. YOLO functions will be stubs.")
    endif()
endif()

# ============================================================================
# 安装规则
# ============================================================================
install(TARGETS dm_hook
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

install(FILES dm.h DESTINATION include)

# ============================================================================
# 使用说明
# ============================================================================
message(STATUS "")
message(STATUS "=== 大漠插件 (dm_hook) 构建 ===")
message(STATUS "构建命令:")
message(STATUS "  cmake -B build -G \"Visual Studio 17 2022\" -A x64")
message(STATUS "  cmake --build build --config Release")
message(STATUS "")
message(STATUS "启用 OCR (需要安装 Tesseract):")
message(STATUS "  cmake -B build -DENABLE_TESSERACT=ON")
message(STATUS "")
message(STATUS "启用 YOLO (需要安装 ONNX Runtime):")
message(STATUS "  cmake -B build -DENABLE_ONNX=ON")
message(STATUS "")