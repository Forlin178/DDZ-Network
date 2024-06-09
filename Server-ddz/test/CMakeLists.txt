cmake_minimum_required(VERSION 3.19)
project(test)
set(CMAKE_CXX_STANDARD 14)

# 取出当前目录所有源文件，并存储到一个变量中
aux_source_directory(./ SRC)
# 把源文件制作成静态库 名字就是test(static可以不写，动态库才需要些)
add_library(test STATIC ${SRC})

target_link_libraries(test protobuf)