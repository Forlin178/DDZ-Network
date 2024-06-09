cmake_minimum_required(VERSION 3.19)
project(Server_ddz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS}  -std=c++17")

# 添加头文件路径，可以直接包含其他目录的头文件，而不需要写完整的相对路径
include_directories(
        ${PROJECT_SOURCE_DIR}/common
        ${PROJECT_SOURCE_DIR}/http
        ${PROJECT_SOURCE_DIR}/reactor
        ${PROJECT_SOURCE_DIR}/tcp
        ${PROJECT_SOURCE_DIR}/thread
        ${PROJECT_SOURCE_DIR}/serialize
        ${PROJECT_SOURCE_DIR}/crypto
        ${PROJECT_SOURCE_DIR}/game
        ${PROJECT_SOURCE_DIR}/database
        "/usr/include/mysql"
        "/usr/local/include"
)

# 添加子目录
add_subdirectory(common)
add_subdirectory(http)
add_subdirectory(reactor)
add_subdirectory(tcp)
add_subdirectory(thread)
add_subdirectory(serialize)
add_subdirectory(crypto)
add_subdirectory(game)
add_subdirectory(database)

link_directories(/usr/local/ssl/lib64/)
link_directories(/usr/local/lib)
link_directories(/usr/lib/x86_64-linux-gnu/)
# 链接子目录中生成的静态库文件(前面的库可以链接后面的)
link_libraries(tcp reactor thread common mycrypto serialize game database)

# 指定可执行文件生成到对应的目录中
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(Server_ddz main.cpp)
