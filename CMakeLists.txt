# 设置 cmake最小版本
# cmake_minimum_required(VERSION 3.27)

# 设置输出项目名称
project(learn_softrenderer)

# 设置c++版本
set(cmake_cxx_standard 17)
set(cmake_cxx_standard_required True)

# 查找OpenMP包
find_package(OpenMP)

# 找到OpenMP包之后，设置变量
if(OpenMP_FOUND)
  message("Found to OpenMp!!!")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
else()
  message("Not found to OpenMp!!!")
endif()

# 设置构建类型
if(NOT cmake_build_type)
  message("set cmake_build_type = Release")
  set(cmake_build_type Release)
endif()

# 添加子目录
add_subdirectory(commons)
add_subdirectory(lessons)

# 把c+文件存储到变量
file(GLOB sources *.cpp *.h)

# 添加执行文件
add_executable(${PROJECT_NAME} ${sources})

# 链接commons库和头文件
target_link_libraries(${PROJECT_NAME} PRIVATE commons)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/commons)

# 链接lessons库和头文件
target_link_libraries(${PROJECT_NAME} PRIVATE lessons)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lessons)