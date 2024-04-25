cmake_minimum_required(VERSION 3.27)
project(heatEquationVisualization)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)
# Debug specific flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
#set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    set(CMAKE_CXX_FLAGS_DEBUG  "-fsanitize=thread")
endif()

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
        src/config/config.h
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
        src/solver/ISolver.h
        src/solver/WaterSolver.cpp
        src/solver/WaterSolver.h
        src/models/Particle.cpp
        src/models/Particle.h
        src/solver/SmoothingKernel.cpp
        src/solver/SmoothingKernel.h
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
target_link_libraries(heatEquationVisualization PUBLIC glfw3.lib OpenGL::GL)

if(OpenMP_CXX_FOUND)
    target_link_libraries(heatEquationVisualization PUBLIC OpenMP::OpenMP_CXX)
endif()
