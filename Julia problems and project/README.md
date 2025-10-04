# Julia Labs and Projects

This folder contains my Julia exercises and labs, with a highlight on a major optimization project I completed with a teammate. The project demonstrates gradient-based optimization techniques applied to classical test functions.

## Contents

- **Labs and Exercises** – Smaller practice assignments and numerical experiments to get familiar with Julia syntax and libraries.
- **Optimization Project** – A comprehensive project implementing Gradient Descent, Gradient Descent with Momentum, and Nesterov Accelerated Gradient on multiple test functions such as Rosenbrock, Rastrigin, Himmelblau, Ackley, Goldstein-Price, and others.

## Features of the Optimization Project

- Classic Gradient Descent, Momentum-based, and Nesterov Accelerated Gradient implementations.
- Trajectory tracking for convergence visualization.
- Error computation and plotting tools (some commented out for customization).
- Parameter adjustment for learning rate, momentum coefficient, and initial values.
- Ability to explore convergence across a grid of starting points for each function.

## How to Run

Ensure the required packages are installed:

```julia
using Pkg
Pkg.add("ForwardDiff")
Pkg.add("Plots")
```

Run the main project file in Julia (VSCode, Jupyter Notebook, or Julia REPL). Modify parameters as needed in the `PODESAVANJE PARAMETARA` section.

```julia
include("OptimizationProject.jl")
```

## Notes

- The project demonstrates optimization over well-known test functions and allows for experimentation with algorithm parameters.
- Visualizations include trajectory plots and convergence landscapes (commented sections can be activated for detailed analysis).
- Screenshots of results can be labeled as `screenshot1`, `screenshot2`, etc., for documentation purposes.

✨ This project showcases my Julia proficiency and deep dive into numerical optimization techniques.

