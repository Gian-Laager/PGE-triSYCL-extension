cmake_minimum_required(VERSION 3.17)
project(PGE_triSYCL_extension)

set(CMAKE_CXX_STANDARD 20)
cmake_minimum_required(VERSION 3.17)
project(PGE_triSYCL_extension)

set(CMAKE_CXX_STANDARD 20)

set(SRC ${CMAKE_CURRENT_LIST_DIR}/src/olcSYCL.cpp)

set(HEAD include/olcSYCL.h)

set(PCH ${CMAKE_CURRENT_LIST_DIR}/include/pch.h)

set(Boost_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/deps/boost)
include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${Boost_INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/deps/SYCL/include
        ${CMAKE_CURRENT_LIST_DIR}/deps/SYCL/contrib
        ${CMAKE_CURRENT_LIST_DIR}/deps/olcPGE/OneLoneCoder
)


add_library(${PROJECT_NAME} STATIC ${SRC} ${HEAD})

target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH})

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)

#PGE dependencies
find_package(PNG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
link_libraries(
        ${PNG_LIBRARY}
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARY}
)

include_directories(
        ${PNG_INCLUDE_DIR}
        ${OPENGL_INCLUDE_DIR}
        ${GLUT_INCLUDE_DIRS}
)

set(TESTS_DIR ${CMAKE_CURRENT_LIST_DIR}/tests)

set(TESTS_SRC ${TESTS_DIR}/src/test/main.cpp)

set(TESTS_PCH ${TESTS_DIR}/include/test/pch.h)

set(TESTS_HEAD)

set(TESTS_LIBS
        ${TESTS_DIR}/deps/googletest/build/lib/libgtest.a
        ${PROJECT_NAME}
        )

set(TESTS_INCLUDES
        ${TESTS_DIR}/include
        ${TESTS_DIR}/src
        ${TESTS_DIR}/deps/googletest/googletest/include
)

add_executable(tests ${TESTS_SRC} ${TESTS_HEAD})

target_precompile_headers(tests PRIVATE ${TESTS_PCH})

set_target_properties(tests PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(tests PRIVATE ${TESTS_LIBS})

target_include_directories(tests PRIVATE ${TESTS_INCLUDES})