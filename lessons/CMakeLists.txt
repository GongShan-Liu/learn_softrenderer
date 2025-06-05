file(GLOB lessons_sources *.cpp *.h)

# 创建静态库 lessons
add_library(lessons ${lessons_sources})

# 链接项目其他库的头文件
target_include_directories(lessons PUBLIC ${CMAKE_SOURCE_DIR})