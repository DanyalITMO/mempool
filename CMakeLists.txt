cmake_minimum_required(VERSION 3.17)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
project(mempool VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)

option(ASAN_BUILD "Enable sanitizer" ON)
option(TEST_BUILD "Enable tests" ON)

if(TEST_BUILD)
    find_package(GTest REQUIRED)
endif()

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(ASAN_BUILD)
    target_compile_options(${PROJECT_NAME} PRIVATE -fno-omit-frame-pointer -fsanitize=thread)
    target_link_options(${PROJECT_NAME} PRIVATE -fno-omit-frame-pointer -fsanitize=thread)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_ASAN)
endif ()

set(CPACK_PACKAGE_CONTACT mugutdinov@yandex.ru)
set(CPACK_GENERATOR DEB)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
include(CPack)

if(TEST_BUILD)
    set(TEST_PROJECT_NAME ${PROJECT_NAME}_test)

    file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

    add_executable(${TEST_PROJECT_NAME} ${SOURCES} ${TEST_SOURCES})

    get_target_property(include_prop ${PROJECT_NAME} INCLUDE_DIRECTORIES)
    get_target_property(link_prop ${PROJECT_NAME} LINK_LIBRARIES)
    get_target_property(link_opt ${PROJECT_NAME} LINK_OPTIONS)
    get_target_property(compile_def_prop ${PROJECT_NAME} COMPILE_DEFINITIONS)
    get_target_property(compile_opt_prop ${PROJECT_NAME} COMPILE_OPTIONS)

    if(include_prop)
        set_property(TARGET ${TEST_PROJECT_NAME} PROPERTY INCLUDE_DIRECTORIES ${include_prop})
    endif()

    if(link_prop)
        set_property(TARGET ${TEST_PROJECT_NAME} PROPERTY LINK_LIBRARIES ${link_prop})
    endif()

    if(link_opt)
        set_property(TARGET ${TEST_PROJECT_NAME} PROPERTY LINK_OPTIONS ${link_opt})
    endif()

    if(compile_def_prop)
        set_property(TARGET ${TEST_PROJECT_NAME} PROPERTY COMPILE_DEFINITIONS ${compile_def_prop})
    endif()

    if(compile_opt_prop)
        set_property(TARGET ${TEST_PROJECT_NAME} PROPERTY COMPILE_OPTIONS ${compile_opt_prop})
    endif()

    target_link_libraries(${TEST_PROJECT_NAME} PRIVATE GTest::GTest)

endif()
