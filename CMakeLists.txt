# CMake 最低版本号
cmake_minimum_required (VERSION 3.6.1)

# 项目信息
project (BencodeParser)

# 指定编译器
set (CMAKE_CXX_COMPILER "clang++")

# 设置编译选项
set (CMAKE_BUILD_TYPE "DEBUG")
set (CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -g -Wall")
set (CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -Wall")

# 查找目录下的所有文件并保存到 DIR_SRC 变量中
aux_source_directory (./src DIR_SRCS)

# 指定生成目标
add_executable (findFiles ${DIR_SRCS})

