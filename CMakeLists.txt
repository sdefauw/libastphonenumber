cmake_minimum_required(VERSION 2.8)

project(libastphonenumber)
set(libastphonenumber_VERSION_MAJOR 1)
set(libastphonenumber_VERSION_MINOR 0)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

set(
        ASTPHONENUMBER_SOURCE
        src/formatting.cpp
        src/numinfo.cpp
        src/regioninfo.cpp)

# STATIC LIBRARY
add_library(astphonenumber STATIC ${ASTPHONENUMBER_SOURCE})
target_link_libraries(astphonenumber phonenumber)
set_property(TARGET astphonenumber PROPERTY POSITION_INDEPENDENT_CODE ON)

# SHARED LIBRARY
add_library(astphonenumber-shared SHARED ${ASTPHONENUMBER_SOURCE})
set_target_properties(astphonenumber-shared
        PROPERTIES
        OUTPUT_NAME "astphonenumber"
        PREFIX "lib"
        SOVERSION ${libastphonenumber_VERSION_MAJOR}
        VERSION ${libastphonenumber_VERSION_MAJOR}.${libastphonenumber_VERSION_MINOR})
target_link_libraries(astphonenumber-shared phonenumber)
set_property(TARGET astphonenumber-shared PROPERTY POSITION_INDEPENDENT_CODE ON)

# INSTALL
SET(CMAKE_INSTALL_PREFIX /usr)
install(FILES
        src/formatting.h
        src/numinfo.h
        src/regioninfo.h
        DESTINATION include/astphonenumbers/
        )
install(TARGETS astphonenumber LIBRARY DESTINATION lib/ ARCHIVE DESTINATION lib/)
install(TARGETS astphonenumber-shared LIBRARY DESTINATION lib/ ARCHIVE DESTINATION lib/)

# TEST
add_executable(
        test_dyn_load
        "test/test_lib_dyn_load.c"
)
target_link_libraries(test_dyn_load dl)

add_executable(
        test_num_info
        "test/test_info.c"
)
target_link_libraries(test_num_info astphonenumber)