//
// Created by djcur on 4/10/2024.
//

#ifndef HEATEQUATIONVISUALIZATION_SOLVER_H
#define HEATEQUATIONVISUALIZATION_SOLVER_H

#include "../config/Config.h"
#include "../events/Broker.h"
#include "../models/Element.h"
#include "../models/Grid.h"

#include <amgcl/amg.hpp>
#include <amgcl/coarsening/smoothed_aggregation.hpp>
#include <amgcl/relaxation/spai0.hpp>
#include <amgcl/make_solver.hpp>
#include <amgcl/solver/bicgstab.hpp>


namespace solver {
    /**
     * @class HeatSolver
     * @brief A solver class for heat transfer simulations using finite element methods.
     *
     * This solver handles setup and execution of simulations, including grid management,
     * material properties, and computation of temperatures. It utilizes a singleton pattern
     * to ensure only one instance is used throughout the application.
     */
    class HeatSolver {
    public:
        /**
         * @brief Returns a reference to the singleton instance of HeatSolver.
         * @return Reference to the singleton HeatSolver instance.
         */
        static HeatSolver &getInstance() {
            static HeatSolver instance(config::Config::config.gridWidth, config::Config::config.gridHeight);
            return instance;
        }

        HeatSolver(HeatSolver const &) = delete;

        void operator=(HeatSolver const &) = delete;

        /**
         * @brief Performs the heat simulation over a specified number of timesteps.
         * @param numTimesteps Number of timesteps to simulate.
         * @param timeStep Time step size.
         * @return Vector of temperature distributions for each timestep.
         */
        std::vector<Eigen::VectorXd> performSimulation(int numTimesteps, int timeStep);

        /**
         * @brief Retrieves the grid elements.
         * @return Vector of elements from the current grid.
         */
        const std::vector<models::Element> &getElements() const { return *grid.getElements(); }

        /**
         * @brief Updates the size of the simulation grid.
         * @param newWidth New grid width.
         * @param newHeight New grid height.
         */
        void updateGridSize(int newWidth, int newHeight);

        /**
         * @brief Resets the grid to its initial state.
         */
        void resetGrid();

        /**
         * @brief Sets the initial temperature of a specified element in the grid.
         * @param x X-coordinate of the element.
         * @param y Y-coordinate of the element.
         * @param temperature Initial temperature in Kelvin.
         */
        void setInitialTemperatureKelvin(int x, int y, float temperature) {
            grid.setInitialTemperature(x, y, temperature);
            initialized = false;
        }

        /**
         * @brief Sets the heat source intensity of a specified element in the grid.
         * @param x X-coordinate of the element.
         * @param y Y-coordinate of the element.
         * @param source Heat source intensity.
         */
        void setElementSource(int x, int y, float source) {
            grid.setElementSource(x, y, source);
            initialized = false;
        }

        /**
         * @brief Sets the material of a specified element in the grid using a material name.
         * @param x X-coordinate of the element.
         * @param y Y-coordinate of the element.
         * @param materialName Name of the material.
         */
        void setMaterial(int x, int y, const std::string &materialName) {
            grid.setMaterial(x, y, material_manager.getMaterial(materialName));
            initialized = false;
        }

        /**
         * @brief Returns the number of time steps completed in the current or most recent simulation.
         * @return Number of completed time steps.
         */
        int getTimeStepsCompleted() const { return timeStepsCompleted.load(); }

    private:
        /**
         * @brief Constructor for the HeatSolver class.
         * @param width Width of the simulation grid.
         * @param height Height of the simulation grid.
         *
         * Initializes a new heat solver with a specified grid size. Sets up initial configurations,
         * including allocation of global matrices and vectors based on grid dimensions.
         */
        HeatSolver(int width, int height);

        /// Stores the global stiffness matrix used in finite element analysis.
        Eigen::SparseMatrix<double> globalStiffnessMatrix;
        Eigen::SparseVector<double> globalLoadVector; ///< Stores the global load vector, representing external forces.
        Eigen::SparseMatrix<double> globalMassMatrix; ///< Stores the global mass matrix, used in dynamic simulations.
        Eigen::VectorXd globalMassTempMatrix; ///< Precomputes mass matrix * temp vector
        Eigen::VectorXd globalTemperatureVector; ///< Stores the global temperatures at each node.

        std::vector<double> lastTemp;
        std::mutex lastTempMutex;
        events::Broker &broker = events::Broker::getInstance(); ///< Handles publishing of events related to the simulation.

        bool initialized{}; ///< Indicates whether the solver has been initialized with initial conditions.

        manager::MaterialManager &material_manager = manager::MaterialManager::getInstance(); ///< Manages access to material properties used in the simulation.

        std::atomic<int> timeStepsCompleted; ///< Tracks the number of time steps that have been completed.

        models::Grid grid; ///< Represents the computational grid used in the simulation.

     typedef amgcl::backend::builtin<double> Backend;
     typedef amgcl::make_solver<
         amgcl::amg<
             Backend,
             amgcl::coarsening::smoothed_aggregation,
             amgcl::relaxation::spai0
         >,
         amgcl::solver::bicgstab<Backend>
     > Solver;

        /**
         * @brief Solves the system for a single time step.
         * @param timeStep The time step size.
         * @return The new temperature distribution after the time step.
         *
         * Calculates the next time step's temperature distribution by solving the system of equations
         * formed by the mass matrix, stiffness matrix, and load vector.
         */
        Eigen::VectorXd solveTimeStep(float timeStep);

        /**
         * @brief Assembles the global stiffness, mass, and load matrices.
         *
         * Constructs the global system matrices from the local element matrices. This method should be called
         * whenever the grid or material properties change significantly.
         */
        void assembleGlobalSystem();

        void setupAmgMatrix();

        /**
         * @brief Creates a stiffness matrix for a given element.
         * @param element The element for which to create the stiffness matrix.
         * @return A 4x4 stiffness matrix.
         */
        static Eigen::Matrix4d createStiffnessMatrix(const models::Element &element);

        /**
         * @brief Creates a load vector for a given element.
         * @param element The element for which to create the load vector.
         * @return A 4-element vector representing the load at each node of the element.
         */
        static Eigen::Vector4d createLoadVector(const models::Element &element);

        /**
         * @brief Creates a mass matrix for a given element.
         * @param element The element for which to create the mass matrix.
         * @return A 4x4 mass matrix.
         */
        static Eigen::Matrix4d createMassMatrix(const models::Element &element);

        /**
         * @brief Initializes the solver's system matrices and temperature vector.
         *
         * This method is called to set up the initial state of the solver before beginning a simulation.
         */
        void initializeSystem();

        /**
         * @brief Computes the initial temperature distribution based on element properties.
         *
         * This method calculates the initial temperatures at each node by averaging the initial temperatures
         * of the elements that share each node.
         */
        void computeInitialTemperatures();

        /**
         * @brief Resets the solver's matrices to prepare for a new simulation or after a significant change.
         *
         * This includes reinitializing the stiffness, mass, and load matrices as well as the temperature vector.
         */
        void resetMatrices();
    };
} // namespace solver

#endif //HEATEQUATIONVISUALIZATION_SOLVER_H
