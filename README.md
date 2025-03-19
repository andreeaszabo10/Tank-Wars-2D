# Tank Wars 2D

## Description
Tank Wars 2D is a simple 2D tank battle game where two players control tanks and fire projectiles at each other. The objective is to hit the opponent's tank as many times as possible while navigating a terrain that affects projectile trajectories. The game features realistic physics for the projectiles and dynamic changes in the environment such as clouds and day-night cycles.

## Features
- Two-player gameplay with independent controls for each player.
- Realistic projectile trajectory with physics-based gravity.
- Dynamic terrain generation and deformation due to projectile impacts.
- Day-night cycle with clouds that move across the screen.
- Tank models with customizable barrel angles.
- Scoring system based on successful hits.

## Controls
- **Player 1 (Tank 1)**:
  - Move Left: `A`
  - Move Right: `D`
  - Aim Barrel Up: `W`
  - Aim Barrel Down: `S`
  - Fire Projectile: `SPACE`
  
- **Player 2 (Tank 2)**:
  - Move Left: `J`
  - Move Right: `L`
  - Aim Barrel Up: `I`
  - Aim Barrel Down: `K`
  - Fire Projectile: `ENTER`

## Installation
1. Clone or download the repository.
2. Ensure you have a C++ environment set up with OpenGL and GLFW libraries.
3. Compile the project using a compatible build system (e.g., CMake or Makefiles).
4. Run the executable to start the game.

## File Overview
- **`object2D.h` and `object2D.cpp`**: Contain functions for creating 2D objects like tanks, squares, rectangles, and circles used in the game.
- **`tema1.h` and `tema1.cpp`**: Implement the main game logic, including initialization, rendering, projectile mechanics, and input handling.
- **`transform2D.h`**: Provides helper functions for translating, rotating, and scaling 2D objects during rendering.

## How the Game Works
### Terrain Generation
The terrain is generated using a sine wave function, where the height of the terrain is based on the `Terrain()` function. The terrain is rendered as a series of connected squares, and it deforms when hit by projectiles.

### Tank Rendering
The tanks are represented by meshes, with separate parts for the tank body and barrel. The player's movement and barrel aim are controlled by keyboard input, and the tanks are rendered based on their position and orientation in the game world.

### Projectile Mechanics
Projectiles are created when a player fires their tank's barrel. The projectiles follow a physics-based trajectory, influenced by gravity, and can impact the terrain or the opponent's tank.

### Day-Night Cycle
The game includes a day-night cycle that affects the visual appearance of the environment. The sun and moon rotate across the screen, and clouds move in the background.

## Dependencies
- OpenGL
- GLFW
- GLM
