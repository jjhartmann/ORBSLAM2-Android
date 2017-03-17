# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
set(pathToProject "D:/Git_Dir/VisualInertialOdometry-Android-UW-SAR-2")
set(pathToOpenCv "D:/ThirdParty/opencv-3.2.0-android-sdk/OpenCV-android-sdk")

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")



####################################################################################################
## Pangolin Libray
####################################################################################################\
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "D:/Git_Dir/Pangolin")
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "D:/Git_Dir/Pangolin/CMakeModules/" "${pathToProject}/app/CMakeModules/" )
#set(Pangolin_DIR "D:/Git_Dir/Pangolin/")
#find_package(Pangolin 0.4 REQUIRED)
#include_directories(${Pangolin_INCLUDE_DIRS})


####################################################################################################
## Thirdparty
####################################################################################################\

## DBoW
file(GLOB_RECURSE dbow2_cpp_files "${pathToProject}/app/src/main/cpp/Thirdparty/DBoW2/DBoW2/*.cpp"
    "${pathToProject}/app/src/main/cpp/Thirdparty/DBoW2/DUtils/*.cpp")
file(GLOB_RECURSE dbow2_util_cpp_files "${pathToProject}/app/src/main/cpp/Thirdparty/DBoW2/DUtils/*.cpp")

## g2o
file(GLOB_RECURSE g2o_cpp_files "${pathToProject}/app/src/main/cpp/Thirdparty/g2o/g2o/core/*.cpp"
     "${pathToProject}/app/src/main/cpp/Thirdparty/g2o/g2o/solvers/*.cpp"
     "${pathToProject}/app/src/main/cpp/Thirdparty/g2o/g2o/stuff/*.cpp"
     "${pathToProject}/app/src/main/cpp/Thirdparty/g2o/g2o/types/*.cpp")

## ORBSLAM2
file(GLOB_RECURSE orbslam2_cc_files "${pathToProject}/app/src/main/cpp/ORB_SLAM2/src/*.cc")

## Includes
include_directories(${pathToProject}/app/src/main/cpp/ORB_SLAM2/include/)


####################################################################################################
## Native Library
####################################################################################################\
add_library( native-lib
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp
             src/main/cpp/CEngine.cpp
             src/main/cpp/VIOImage.cpp
             src/main/cpp/VIOEngine.cpp
             src/main/cpp/VOPose.cpp


             # ThirdParty
             ${dbow2_cpp_files}
             ${g2o_cpp_files}
             ${orbslam2_cc_files}
             )

####################################################################################################
## Open CV 3.2
####################################################################################################\
include_directories(${pathToProject}/app/src/main/cpp/include/)
add_library( lib_opencv SHARED IMPORTED )
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${pathToProject}/app/src/main/cpp/lib/opencv/${ANDROID_ABI}/libopencv_java3.so)

####################################################################################################
## Boost C++ v. 1_49
####################################################################################################\
#include_directories(${pathToProject}/app/src/main/cpp/Thirdparty/Boost/include/boost-1_49/)
#add_library( lib_boost SHARED IMPORTED )
#file(GLOB_RECURSE boost_lib_resources "${pathToProject}/app/src/main/cpp/Thirdparty/Boost/lib/*.a")
#set_target_properties(lib_boost PROPERTIES IMPORTED_LOCATION ${boost_lib_resources})





# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )


####################################################################################################
## Set Target Link Libraries
####################################################################################################\

target_link_libraries( # Specifies the target library.
                       native-lib
                       lib_opencv
                       lib_boost
                       ${Pangolin_LIBRARIES}
                       ${log-lib} )