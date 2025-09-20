# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required(VERSION 3.31)

# 一定在 project() 之前，且当 STRING 写入缓存
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508" CACHE STRING "" FORCE)

# 禁用不需要的语言。因为eigen库中有fortran代码会被检测到后引发冲突
set(CMAKE_Fortran_COMPILER "" CACHE FILEPATH "" FORCE)

# 让vs知道有compile_commands.json文件从而启用IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)


# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(cumc25a LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 包含子项目。
add_subdirectory ("cumc25a")
