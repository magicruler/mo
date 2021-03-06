cmake_minimum_required(VERSION 3.8.0)
project(Mo VERSION 0.1.0)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Output Directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/bin>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG $<1:${CMAKE_SOURCE_DIR}/bin>)

include_directories(
    include/
    thirdparty/spdlog/include/
    thirdparty/imgui/
    thirdparty/glad/include/
    thirdparty/glfw/include/
    thirdparty/glm/
    thirdparty/assimp/include/
)

# Spdlog Building
add_subdirectory(thirdparty/spdlog)

# GLFW Building
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(thirdparty/glfw)

# GLAD Building
add_subdirectory(thirdparty/glad)

# IMGUI Building
add_subdirectory(thirdparty/imgui)

# GLM Building
add_subdirectory(thirdparty/glm)

# Assimp Building
add_subdirectory(thirdparty/assimp)

file(GLOB HEADERS include/*.h include/*.hpp)
file(GLOB SOURCES src/*.cpp)

add_executable(Mo ${SOURCES} ${HEADERS})

target_link_libraries(Mo spdlog glfw ${GLFW_LIBRARIES} glad imgui glm assimp)