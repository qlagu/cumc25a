# CMakeList.txt: cumc25a 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

# 为目标启用 `import std` 功能的函数。
function(enable_import_std tgt)
  set_property(TARGET ${tgt} PROPERTY 
  CXX_MODULE_STD ON)
endfunction()

# 将源代码添加到此项目的库。
# 注意：如果要使用std模块，则需要使用enable_import_std()函数。

## stdx 库 
## 用于解决vs的intelligence无法正确处理std模块的问题
add_library(stdx OBJECT
   libs/stdx.ixx
)

target_sources(stdx
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
      libs/stdx.ixx
)

enable_import_std(stdx)

## OriginData库
## 记录原始数据
add_library(OriginData STATIC)

target_sources(OriginData
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
      "libs/OriginData/OriginData.ixx"
  PRIVATE 
     "libs/OriginData/OriginData.cpp"
)

enable_import_std(OriginData)

target_link_libraries(OriginData   
PUBLIC
    Eigen3::Eigen
    Time
    stdx
)

## Time库
## 时间处理

add_library(Time STATIC)

target_sources(Time
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES 
    FILES
      libs/time/time.ixx
  PRIVATE
      libs/time/time.cpp    
)

enable_import_std(Time)

target_link_libraries(Time   
PUBLIC
    stdx
)

## kinematics库
## 运动学计算

add_library(kinematics STATIC)

target_sources(kinematics
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES 
    FILES
      libs/kinematics/kinematics.ixx
   PRIVATE
      libs/kinematics/kinematics.cpp    
)

enable_import_std(kinematics)

target_link_libraries(kinematics   PRIVATE
    stdx
    Eigen3::Eigen
    OriginData
    Time
)

## basic库
## export基础设施

add_library(basic STATIC)

target_sources(basic
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES 
    FILES
      "libs/basic.ixx"
)

target_link_libraries(basic
    PRIVATE
     stdx
     OriginData
     Time
     kinematics
     Eigen3::Eigen
)

enable_import_std(basic)

## occlusion库
## 遮挡模型
add_library(occlusion STATIC)

target_sources(occlusion
    PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES 
        FILES
        "libs/occlusion/occlusion.ixx"
        "libs/occlusion/occlusion.base.ixx"
        "libs/occlusion/occlusion.simple.ixx"
        "libs/occlusion/occlusion.advance.ixx"
    PRIVATE
        FILE_SET cxx_modules_private TYPE CXX_MODULES
        FILES
        "libs/occlusion/occlusion.simple.cpp"
        "libs/occlusion/occlusion.advance.cpp"
)


target_link_libraries(occlusion   PRIVATE
    basic
    fmt::fmt
)

enable_import_std(occlusion)

## PSO库
## 粒子群优化算法

add_library(PSO STATIC)

target_sources(PSO
    PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES 
        FILES
            "pso/pso.ixx"
)

target_link_libraries(PSO 
    PUBLIC
        basic
        Eigen3::Eigen
        fmt::fmt
)

# 添加外部依赖
include(FetchContent)

## fmt库
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        11.0.2   
)

set(FMT_MODULE OFF CACHE BOOL "" FORCE)# TODO
set(FMT_HEADER_ONLY ON  CACHE BOOL "" FORCE)# module版本有问题，先用header-only版本
FetchContent_MakeAvailable(fmt)

## eigen库

FetchContent_Declare(
  eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG        3.4.0   
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(eigen)

# 将源代码添加到此项目的可执行文件。
# 注意：如果要使用std模块，则需要使用enable_import_std()函数。

## 主程序cumc25a
add_executable(cumc25a
  "cumc25a.cpp"
)

target_link_libraries(cumc25a   PRIVATE
    stdx
    fmt::fmt
    Eigen3::Eigen
    OriginData
)

enable_import_std(cumc25a)

add_executable(test_time
    "tests/time.cpp"
)

target_link_libraries(test_time   PUBLIC
    stdx
    fmt::fmt
    Eigen3::Eigen
    Time
    kinematics
)

enable_import_std(test_time)

add_executable(test_occlusion_simple
    "tests/occlusion.simple.cpp"
)

target_link_libraries(test_occlusion_simple   PRIVATE
    basic
    occlusion
    fmt::fmt
    Eigen3::Eigen
)

enable_import_std(test_occlusion_simple)

#set_target_properties(cumc25a PROPERTIES
#  CXX_STANDARD 23
#  CXX_STANDARD_REQUIRED ON
#  CXX_MODULE_STD ON        # 让 `import std` 生效
#)

#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET cumc25a PROPERTY CXX_STANDARD 20)
#endif()

# TODO: 如有需要，请添加测试并安装目标。
