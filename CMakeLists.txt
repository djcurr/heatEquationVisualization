cmake_minimum_required(VERSION 3.27)
project(heatEquationVisualization)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)

add_executable(heatEquationVisualization
        src/models/Node.h
        src/models/Node.cpp
        src/models/Grid.h
        src/models/Grid.cpp
        src/solver/Solver.h
        src/solver/Solver.cpp
        src/models/Element.h
        src/models/Element.cpp
        src/models/Material.h
        src/models/Material.cpp
        src/ui/UI.h
        src/ui/UI.cpp
        src/main.cpp
        src/manager/MaterialManager.cpp
        src/manager/MaterialManager.h
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
target_link_libraries(heatEquationVisualization glfw3.lib OpenGL::GL)
