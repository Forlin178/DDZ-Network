cmake_minimum_required(VERSION 3.19)
project(http)
set(CMAKE_CXX_STANDARD 14)

# 取出当前目录所有源文件，并存储到一个变量中
aux_source_directory(./ SRC)
# 把源文件制作成静态库
add_library(http STATIC ${SRC})
