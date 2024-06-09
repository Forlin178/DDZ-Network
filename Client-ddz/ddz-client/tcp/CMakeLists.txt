cmake_minimum_required(VERSION 3.5)
project(tcp)

# 搜索当前目录所有源文件
aux_source_directory(./ SRC)

# 搜索头文件和 UI文件
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB UIS ${PROJECT_SOURCE_DIR}/*.ui)

# 将当前目录下源文件制作成静态库
add_library(tcp ${SRC} ${HEADERS} ${UIS})

target_link_libraries(tcp PRIVATE Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Ws2_32
)
