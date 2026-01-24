# 🕹️ Node Runner

**Node Runner** is a survival maze game built from scratch using **C++** and **OpenGL (GLUT)**. The project demonstrates core Computer Graphics concepts including rasterization, 2D transformations, and collision detection without the use of a game engine.

## 🚀 Features

*   **Dynamic Player Movement:** Smooth 2D translation controlled by user input.
*   **Smart Enemy AI:** "Devils" that use greedy pathfinding logic to actively chase the player.
*   **Trap System:** Static "Bomb" tiles hidden in the grid that trigger an instant game over.
*   **State Management:** Finite State Machine handling (Playing, Win, Game Over).
*   **Grid-Based Rendering:** Utilizing Raster Scan Conversion principles to render the map from a 2D array.

## 📚 Syllabus Mapping (CSC209)

This project was designed to meet the requirements of the B.Sc.CSIT 3rd Semester Computer Graphics syllabus:

*   **Unit 2 (Scan Conversion):** The map is rendered by rasterizing geometric primitives (`GL_QUADS`) based on a grid array.
*   **Unit 3 (2D Transformations):** Player and Enemy movements are implemented via 2D Translation logic ($x' = x + t_x$).
*   **Unit 3 (Clipping/Windowing):** Collision detection acts as a logical "clipping" method, preventing entities from entering Wall nodes.
*   **Unit 10 (OpenGL & Animation):** Utilizes `glutTimerFunc` for real-time enemy animation loops independent of the rendering frame rate.

## 🛠️ Tech Stack

*   **Language:** C++
*   **Graphics Library:** OpenGL / FreeGLUT
*   **IDE:** Dev-C++ (TDM-GCC 64-bit)

## ⚙️ How to Compile (Dev-C++)

This project requires **Dev-C++** with the **GLUT/FreeGLUT** packages installed.
Tutorial on how to install **Dev-C++ 5.11** and **GLUT/FreeGLUT** : https://youtu.be/f6ijyf6WJ4s?si=4xq4AC8YNYPQbtgH

1.  **Clone the Repo:**
    ```bash
    git clone https://github.com/YOUR_USERNAME/Node-Runner.git
    ```
2.  Open `Node Runner.dev` (The project file) in Dev-C++.
3.  **Linker Configuration:**
    Go to `Project` -> `Project Options` -> `Parameters` -> `Linker` and paste:
    ```text
    -lfreeglut -lglu32 -lopengl32 -lwinmm -lgdi32
    ```
4.  **DLL Setup:**
    Ensure `freeglut.dll` is present in the same folder as the compiled `.exe`.
5.  **Compile & Run:** Press `F11`.

## 🎮 Controls

*   **W:** Move Up
*   **A:** Move Left
*   **S:** Move Down
*   **D:** Move Right
*   **Goal:** Reach the **Green Tile**.
*   **Avoid:** Red Squares (Enemies) and Dark Red Tiles (Bombs).

## 🧩 Code Structure

*   `main.cpp`: Contains the game loop, rendering logic, and input handling.
    *   `init()`: Sets up the OpenGL environment and Orthographic projection.
    *   `display()`: Renders the grid, player, and enemies based on `gameState`.
    *   `moveEnemies()`: The AI logic calculation (Timer Callback).
    *   `keyboard()`: Handles player input and collision checks.

## 📜 License

This project is open-source and created for educational purposes.
