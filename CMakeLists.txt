cmake_minimum_required(VERSION 3.7)
project(
        opengl_template
        VERSION 0.0.1
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "Detected 64 bit platform")
    set(IS_64_BIT 1)
endif ()

SET(GLFW_LIBRARY)
SET(ASSIMP_LIBRARY)
SET(GLEW_LIBRARY)
SET(STB_IMAGE_LIBRARY stb_image)
SET(INCLUDE_DIRECTORIES)
SET(PLATFORM)

find_package(OpenGL REQUIRED)

#TODO - have to add linux and win32 dependencies
if (${APPLE})
    SET(GLFW_LIBRARY glfw.3.2)
    SET(ASSIMP_LIBRARY assimpd)
    SET(PLATFORM macos)

    add_definitions(-DOS_APPLE)
elseif (${WIN32})
    SET(GLFW_LIBRARY glfw3)
    SET(ASSIMP_LIBRARY assimp)

    if (IS_64_BIT)
        SET(PLATFORM win64)
    else ()
        SET(PLATFORM win32)
    endif ()

    include_directories(third_party/glew/include)

    SET(GLEW_LIBRARY glew32)

    add_definitions(-DGLEW_STATIC -DOS_WINDOWS)
endif ()

link_directories(third_party/assimp/lib/${PLATFORM})
link_directories(third_party/glfw/lib/${PLATFORM})

if (GLEW_LIBRARY)
    link_directories(third_party/glew/lib/${PLATFORM})
endif ()

add_definitions(-DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED)

add_library(${STB_IMAGE_LIBRARY} third_party/stb_image/stb_image.cpp)

include_directories(
        ${OPENGL_INCLUDE_DIR}
        third_party/glfw/include
        third_party/glfw/include/glfw
        third_party/glm/
        third_party/glm/glm # required by glm library
        third_party/assimp/include
        third_party/stb_image
        third_party/gl
)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(
        ${PROJECT_NAME}
        ${ASSIMP_LIBRARY}
        ${STB_IMAGE_LIBRARY}
        ${GLFW_LIBRARY}
        ${GLEW_LIBRARY}
        ${OPENGL_LIBRARY}
)

file (COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})