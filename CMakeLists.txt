cmake_minimum_required(VERSION 3.2)
project(PicCompress)

set(CMAKE_CXX_STANDARD 11)
set(BOOST_ROOT /usr/local/include/Boost)
set(Boost_NO_SYSTEM_PATHS ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT}/bin)

find_package(Boost COMPONECNTS system filesystem thread REQUIRED)

if(Boost_FOUND)
{
    include_directories(${Boost_INCLUDE_DIRS})

    MESSAGE(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
    MESSAGE(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
    MESSAGE(STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")
}
endif()

aux_source_directory(
        ${PROJECT_SOURCE_DIR}/PicCompress 
        ${PROJECT_SOURCE_DIR}/Src 
        App_SRC) 

add_executable(PicCompress ${App_SRC})

target_include_directories(PicCompress 
        ${PROJECT_SOURCE_DIR}/Include 
        ${PROJECT_SOURCE_DIR}/Src
        ${Boost_INCLUDE_DIRS})

target_link_libraries(PicCompress 
        ${PROJECT_SOURCE_DIR}/Lib 
        Boost::boost)
