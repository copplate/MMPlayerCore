# CMakeList.txt: MMPlayer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

#从上一级目录开始搜索头文件
include_directories(../)
link_directories(../3rdpart/ffmpeg/lib)
#include_directories(../3rdpart/ffmpeg/include)#虽然这样写也可以引入ffmpeg的库，但我们不希望MMAV这个库写完之后，别人或者别的模块
#使用这个库，还要引用一堆ffmpeg的东西。

# 将源代码添加到此项目的可执行文件。
add_executable (MMPlayer "MMPlayer.cpp" "MMPlayer.h")

#引进来MMThread.cpp生成的静态库
#由于它们都在一个工程里，所以没有必要配置路径，它可以自己找到
target_link_libraries(MMPlayer MMThread)
target_link_libraries(MMPlayer MMAV)

target_link_libraries(MMPlayer avcodec)
target_link_libraries(MMPlayer avdevice)#可能用不到avdevice，但还是写一下
target_link_libraries(MMPlayer avformat) 
target_link_libraries(MMPlayer avutil) 
target_link_libraries(MMPlayer swresample) 
target_link_libraries(MMPlayer swscale)

# TODO: 如有需要，请添加测试并安装目标。
