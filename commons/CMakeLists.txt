file(GLOB commons_sources *.cpp *.h)

# 创建静态库 commons
add_library(commons ${commons_sources})

# 链接项目其他库的头文件
target_include_directories(commons PUBLIC ${CMAKE_SOURCE_DIR})