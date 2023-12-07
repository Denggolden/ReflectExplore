cmake_minimum_required(VERSION 3.10.0)
project(ExploreFeatures)

#set(CMAKE_CXX_STANDARD 11)
#显式定义变量，如果有多个源文件，可以在 main.cpp 后面追加，可以用空格 " " 或分号隔开 ";"
set(SRC_LISTS1 
${CMAKE_SOURCE_DIR}/ReflectExplore/main.cpp)
set(SRC_LISTS2 
${CMAKE_SOURCE_DIR}/ReflexAny/main.cpp
${CMAKE_SOURCE_DIR}/ReflexAny/ReflexBase.cpp
${CMAKE_SOURCE_DIR}/ReflexAny/ReflexBase.h
${CMAKE_SOURCE_DIR}/ReflexAny/WorkClass.cpp
${CMAKE_SOURCE_DIR}/ReflexAny/WorkClass.h)
set(SRC_LISTS3 
${CMAKE_SOURCE_DIR}/ReflexInherit/main.cpp
${CMAKE_SOURCE_DIR}/ReflexInherit/ReflexBase.cpp
${CMAKE_SOURCE_DIR}/ReflexInherit/ReflexBase.h
${CMAKE_SOURCE_DIR}/ReflexInherit/WorkClass.cpp
${CMAKE_SOURCE_DIR}/ReflexInherit/WorkClass.h)
set(SRC_LISTS4 
${CMAKE_SOURCE_DIR}/PreTest/TestMain.cpp)

# 添加第一个可执行文件
add_executable(ReflectExplore ${SRC_LISTS1})

# 添加第二个可执行文件
add_executable(ReflexAny ${SRC_LISTS2})

# 添加第三个可执行文件
add_executable(ReflexInherit ${SRC_LISTS3})

# 添加第四个可执行文件
add_executable(PreTest ${SRC_LISTS4})