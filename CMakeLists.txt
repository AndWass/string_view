cmake_minimum_required(VERSION 3.14)
project(andwass_string_view)

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
else()
    include(CTest)
endif()

add_library(andwass_string_view INTERFACE)
add_library(andwass::string_view ALIAS andwass_string_view)

target_include_directories(andwass_string_view INTERFACE include)
target_compile_features(andwass_string_view INTERFACE cxx_std_17)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
