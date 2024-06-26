# Heat Simulation

![Program Animation](https://github.com/djcurr/heatEquationVisualization/blob/main/docs/animation.gif)

## Table of Contents

- [How to Run](#how-to-run)
    - [Prerequisites](#prerequisites)
    - [Clone the Repository](#clone-the-repository)
    - [Build Instructions](#build-instructions)
    - [Running the Application](#running-the-application)
    - [Troubleshooting](#troubleshooting)
- [Documentation](#documentation)
- [Overview](#overview)
    - [Functionality](#functionality)
    - [Technologies](#technologies)
    - [Math](#math)
    - [Design](#design)
        - [MVC](#mvc)
        - [Pub/Sub](#pubsub)
        - [Thread Pool](#thread-pool)
        - [Architecture/OOP](#architectureoop)

## How to Run

### Prerequisites

Start by installing the prerequisites.

```bash
sudo apt-get install build-essential libglfw3-dev
```

### Clone the Repository

Start by cloning the repository to your local machine. You can clone it using the following
command:

```bash
git clone https://github.com/djcurr/heatEquationVisualization.git
cd heatEquationVisualization
```

### Build Instructions

1. **Create a build directory:**
   ```bash
   mkdir build
   cd build
   ```

2. **Configure the project with CMake:**
   ```bash
   cmake ..
   ```
   This command configures the project and checks for the required dependencies.

3. **Build the project:**
   ```bash
   cmake --build . --config Release
   ```

### Running the Application

After building the project, you can run the application directly from the build directory:

```bash
./heatEquationVisualization
```

This command assumes you're on a Unix-like system. If you're using Windows, you might need to navigate to the directory
containing the executable or adjusting the command to match the generated executable's name and path, like so:

```bash
.\heatEquationVisualization.exe
```

### Troubleshooting

- If you encounter issues finding the OpenGL or OpenMP libraries, ensure they are properly installed on your system and
  accessible in your environment path.
- If CMake can't find Boost or other included libraries, verify that their paths are set correctly in
  the `CMakeLists.txt` or pass their paths as command-line arguments to CMake like so:
  ```bash
  cmake .. -DBoost_INCLUDE_DIR=path/to/boost
  ```

## Documentation

For detailed documentation on the architecture and code of `heatEquationVisualization`, check out the Doxygen
documentation. It provides a complete overview of all classes, methods, and their interactions and
dependencies. You can access this documentation by navigating to the `/docs` directory after cloning the repository or
view it online at [GitHub Pages](https://djcurr.github.io/heatEquationVisualization/inherits.html).

## Overview

### Functionality

I created a simulator based
on the heat differential equation for my Differential Equations II class. This project aimed to teach me C++,
how to implement computational math in C++, implement parallel computing
concepts, practice software design concepts, and create an interesting visualization.
This project allows users to draw initial conditions on three grids: temperature, sources, and material. The initial
temperature is
Celsius, the sources are Watts, and the materials are from a pre-defined list.
The user can modify the brush size and drag to draw on the grid. The user can modify the grid size, time step size, and
time duration to determine the temporal and spatial resolution. The user can start the simulation and
change the color map.

[![Simulator Video](http://img.youtube.com/vi/51_h_49tlVk/0.jpg)](http://www.youtube.com/watch?v=51_h_49tlVk "Heat Equation Visualization")

### Technologies

Technologies used in this project (all cross-platform):

- Computation
    - [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)
        - Matrix manipulation
    - [AMGCL](https://github.com/ddemidov/amgcl)
        - Matrix solving
    - [OpenMP](https://www.openmp.org/)
        - Multi-processing
- UI
    - [OpenGL](https://www.opengl.org/)
        - Graphics rendering
    - [GLFW](https://www.glfw.org/)
        - Window manager
    - [glad](https://github.com/Dav1dde/glad)
        - OpenGL loading library
    - [Dear ImGui](https://github.com/ocornut/imgui)
        - User interface
- Documentation
    - [Doxygen](https://www.doxygen.nl/)
        - UML and docs HTML generation

### Math

The backbone of this project centers around solving the heat equation, a partial differential equation (**PDE**) that
describes the distribution of heat in a region over time. The heat **PDE** is:

`∂u/∂t = α∇²u + f`, Where:

- `u` is the temperature distribution function,
- `t` is time,
- `α` (alpha) is the thermal diffusivity of the material,
- `∇²` is the Laplacian operator, representing the diffusion term,
- `f` is a source term representing internal heat sources.

The Backward Euler method generates a time-dependent solution. The Backward Euler solution for the heat
**PDE** is:

`(M + Δt * K) * uᵏ⁺¹ = M * uᵏ + Δt * fᵏ⁺¹`, Where:

- `M` is the mass matrix,
- `Δt` is the time step,
- `K` is the stiffness matrix (arising from the discretization of the Laplacian operator),
- `uᵏ` is the vector of nodal temperatures at the current time step,
- `uᵏ⁺¹` is the vector of nodal temperatures at the next time step,
- `fᵏ⁺¹` is the source term at the next time step.

This equation is essentially a linear algebraic equation that can be solved for `uᵏ⁺¹`, giving the temperature
distribution at the next time step based on the known values at the current time step. The mass matrix `M` and the
stiffness matrix `K` define how the material's physical properties (like thermal conductivity and density) affect the
temperature distribution.

The simulation utilizes the finite element method (**FEM**), discretizing the heat equation into a mesh of nodes.
The global stiffness matrix, mass matrix, and load vector are generated from matrices and vectors computed for individual elements. The algebraic multigrid (**AMG**) method significantly improves solving time. Smoothed aggregation
coarsening and the sparse approximate inverse relaxation scheme performed best.

### Design

![Task Manager](https://github.com/djcurr/heatEquationVisualization/blob/main/docs/taskmanager.png)
This design is highly multithreaded.

#### MVC

This application has two major components: the user interface and the solver. We use the
Model-View-Controller (**MVC**) pattern to organize these components. Within this architecture, the `HeatSolver` is the
**Model**, handling the logic and data. The renderers (`ControlsRenderer` and `GridRenderer`) represent the **View** and
present the output. Finally, the `SimulationController` represents the **Controller**,
passing data between the `HeatSolver` and the renderers.

#### Pub/Sub

The publish-subscribe (**Pub/Sub**) model facilitates communication
between this application's **View** and **Model**. **Pub/Sub** gives three main benefits:

- **Decoupling of Components**: Promotes modularity and maintainability as publishers and subscribers have no
  knowledge of each other. Simplifying updates to the system.


- **Asynchronous Communication**: Publishers can push messages to a topic without waiting for a response, allowing
  non-blocking communication. This communication is crucial for a responsive UI, especially when the simulator runs.


- **Event-Driven**: Subscribers react to events as they occur without polling the publishers. This pushing updates
  eliminates the need
  to continuously poll for changes in the model.

**Pub/Sub** is an essential part of this program, allowing all the components to operate independently and concurrently.
Furthermore, allowing communication on a separate thread, preventing a single operation from blocking the UI. Here are
the [EventTypes](https://github.com/djcurr/heatEquationVisualization/blob/main/src/events/EventTypes.h) in use.

#### Thread Pool

A **Thread Pool** allows the system to execute simulator threads concurrently. The `ThreadPool` class provides a way to
create a `ThreadPool` instance and enqueue new tasks, which are then executed by the workers. Rather than iteratively
generating the solution, we can assign each thread a time step offset from the initial conditions and provide all
threads with the same initial conditions. A **Thread Pool** is another crucial program design pattern, significantly
increasing the speed and multi-threading ability.

#### Architecture/OOP

In this application, Object-Oriented Programming is used to aid in maintainability and scalability
by implementing key patterns, primarily encapsulation, polymorphism, and abstraction. High-level classes
are abstracted for easy swapping, following the saying, "Program to an interface, not an implementation." Computational
classes continuously build on lower-level classes.

The entry point to the program initializes the `UIManager`, then loops over processing and rendering.

The `UIManager` has a `WindowManager` and a `Renderer`.
The `WindowManager` initializes GLFW and glad, creates the window, and handles cleaning up resources. The `Renderer`
initializes ImGui, subscribes components to event types, contains a list of renderers that implement
the `IRenderComponent` interface, and initializes the `SimulationController` and renderers. The
components currently used are `SimulationController`, `GridRenderer`, and `ControlsRenderer`. These components, as well
as the `HeatSolver` and `Renderer`, contain an instance of the `Broker` for events.

![UI Manager UML](https://djcurr.github.io/heatEquationVisualization/classui_1_1_u_i_manager__coll__graph.png)

The `Broker` is a singleton instance that allows publishing events, subscribing, and unsubscribing. The `Broker`
contains a list of subscribers that implement the `ISubscriber` interface, which requires implementing the `onEvent`
function. `EventType`s are mapped to subscribers, but subscribers to all events are permitted. `Event`s are processed
concurrently. The `Event` interface must be implemented by all `Event`s. The required functions are `getType`
and `getData`. `Event` data is stored in a map of permitted data types. Each `Event` data element is indexed by
an `EventDataKey` corresponding to an event's specific data element.

![Broker UML](https://djcurr.github.io/heatEquationVisualization/classevents_1_1_broker__coll__graph.png)

The `GridRenderer` receives a list of points in an `ElementsUpdate` event, constantly rendering these elements,
detecting drawing while rendering, and emitting events. The user can change the color map; color maps
must implement the `ColorMap` interface, which returns colors based on a normalized input. Events communicate all inputs
and outputs.

![GridRenderer UML](https://djcurr.github.io/heatEquationVisualization/classui_1_1_grid_renderer__coll__graph.png)

The `ControlsRenderer`renders the controls pane and input elements and emits events for button clicks and settings
changes. Events process all communication.

![Controls Renderer UML](https://djcurr.github.io/heatEquationVisualization/classui_1_1_controls_renderer__coll__graph.png)

The `SimulationController` is the final component, the **Controller** in the MVC model. The `SimulationController`
contains an instance of the `HeatSolver` and mainly receives events to modify the **Model**, emitting events such as
when the simulation is complete. A `ThreadWrapper` wraps `std::thread` to ensure the simulation thread joins on
destruction.

![Simulation Controller and Heat Solver UML](https://djcurr.github.io/heatEquationVisualization/class_simulation_controller__coll__graph.png)

The `HeatSolver` is a singleton instance that contains the state of the simulation and allows for modifications and
starting the simulation. The `HeatSolver` contains a `Grid` which contains `Element`s and allows resizing, modifying
elements, and getting details.

![Grid UML](https://djcurr.github.io/heatEquationVisualization/classmodels_1_1_grid__coll__graph.png)

`Element`s contain the data of the simulation. `Element`s can have an initial temperature and source value, as well as
4 `Node`s that represent the element's corners, and a `Material`. The fields can be queried and set, and the `Node`s
and `Material` retrieved. A `Node` is the most basic component containing a global ID to identify it in the global
matrix.

`Material`s are stored and retrieved from the `MaterialManager`. The `MaterialManager` allows listing, inserting, and
retrieving `Material`s. The `MaterialManager` uses the singleton design pattern. A `Material` stores and allows
retrieval of the density, thermal conductivity, specific heat, and `Color`. A `Color` stores a red, green, and
blue value.

![Materials](https://github.com/djcurr/heatEquationVisualization/blob/main/docs/material.png)

![Material manager UML](https://djcurr.github.io/heatEquationVisualization/classmanager_1_1_material_manager__coll__graph.png)
