cmake_minimum_required(VERSION 3.27)
project(heatEquationVisualization)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)
# Debug specific flags
if(MSVC)
    # Flags for Microsoft Visual C++
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
    #set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    # Flags for GCC/Clang
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif()

if(WIN32)
    # Link against the Windows library
    set(GLFW_LIB_NAME glfw3.lib)
elseif(UNIX AND NOT APPLE)
    # Link against the Unix library (usually Linux)
    set(GLFW_LIB_NAME glfw)
elseif(APPLE)
    # Link against the macOS library
    set(GLFW_LIB_NAME glfw)
endif()

set(Boost_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/boost")
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(heatEquationVisualization
        src/models/Node.h
        src/models/Grid.h
        src/models/Grid.cpp
        src/solver/HeatSolver.h
        src/solver/HeatSolver.cpp
        src/models/Element.h
        src/models/Element.cpp
        src/models/Material.h
        src/main.cpp
        src/manager/MaterialManager.cpp
        src/manager/MaterialManager.h
        src/threads/ThreadPool.cpp
        src/threads/ThreadPool.h
        src/ui/WindowManager.cpp
        src/ui/WindowManager.h
        src/ui/Renderer.cpp
        src/ui/Renderer.h
        src/ui/SimulationController.cpp
        src/ui/SimulationController.h
        src/ui/UIManager.cpp
        src/ui/UIManager.h
        src/config/Config.h
        src/ui/IRenderComponent.h
        src/ui/GridRenderer.cpp
        src/ui/GridRenderer.h
        src/ui/ControlsRenderer.cpp
        src/ui/ControlsRenderer.h
        src/events/Event.h
        src/events/ISubscriber.h
        src/events/Broker.cpp
        src/events/Broker.h
        src/ui/StyleManager.cpp
        src/ui/StyleManager.h
        src/events/EventTypes.h
        src/config/Config.cpp
        src/models/Point.h
        src/threads/ThreadWrapper.h
        src/ui/ColorMap.h
        src/models/Particle.cpp
        src/models/Particle.h
        src/ui/Roboto-Regular.h
)

# Include Eigen library
target_include_directories(heatEquationVisualization PRIVATE include/Eigen)

# Include GLFW library
target_include_directories(heatEquationVisualization PRIVATE include/GLFW)

# Include Dear imGui library
target_include_directories(heatEquationVisualization PRIVATE include/imgui)
target_sources(heatEquationVisualization PRIVATE lib/imgui/imgui.cpp lib/imgui/imgui_draw.cpp lib/imgui/imgui_impl_glfw.cpp lib/imgui/imgui_impl_opengl3.cpp lib/imgui/imgui_tables.cpp lib/imgui/imgui_widgets.cpp)

# Include glad and KHR
target_include_directories(heatEquationVisualization PRIVATE include/glad)
target_sources(heatEquationVisualization PRIVATE lib/glad/glad.c)

# Link GLFW
target_link_directories(heatEquationVisualization PUBLIC bin/GLFW)

# Link against the selected library
target_link_libraries(heatEquationVisualization PUBLIC ${GLFW_LIB_NAME} OpenGL::GL)

target_include_directories(heatEquationVisualization PRIVATE include/amgcl)
target_include_directories(heatEquationVisualization PRIVATE ${Boost_INCLUDE_DIR})

if(OpenMP_CXX_FOUND)
    target_link_libraries(heatEquationVisualization PUBLIC OpenMP::OpenMP_CXX)
endif()