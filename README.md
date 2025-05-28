# 🌌 Solar System Model (OpenGL + GLFW)
A 3D simulation of the solar system built using OpenGL, GLFW, and GLM. This educational and visual project models planetary motion, rotation, and orbiting around the sun with realistic relative scales and speeds.

🌠 Features
🪐 Realistic simulation of planetary orbits and rotations

☀️ Central sun with rotating planets and moons

🌍 Scaled planetary distances (relative, not to real-world scale)

🕹️ Camera controls for user interaction and observation

🎨 Basic lighting and shading for visual clarity

🖼️ Textured planets (optional depending on asset availability)

🚀 Technologies Used
C++

OpenGL – Rendering engine

GLFW – Window/context/input management

GLM – OpenGL Mathematics library for 3D transformations

SOIL (optional) – For loading textures (if textures are included)

📸 Preview
(Include a screenshot or GIF here if available)

🛠️ Setup Instructions
Prerequisites
C++ compiler (GCC, Clang, MSVC)

CMake

OpenGL-compatible system

Git

Dependencies
Make sure the following libraries are available (can be included via submodules or package manager):

GLFW

GLM

OpenGL

SOIL (optional for texture support)

Build & Run
bash
Copy
Edit
git clone https://github.com/Y-rocket/solar-system-model-openglv.git
cd solar-system-model-openglv
mkdir build && cd build
cmake ..
make
./SolarSystem
🎮 Controls
Key	Action
W / A / S / D	Move camera
Mouse	Rotate view
Scroll	Zoom in/out
ESC	Exit the program

🧠 Educational Value
This project is great for:

Learning the basics of 3D rendering in OpenGL

Understanding hierarchical transformations (orbit + rotation)

Practicing camera control and interaction in a 3D space

Experimenting with lighting and shading techniques

📁 Project Structure
makefile
Copy
Edit
.
├── src/              # Source code files
├── include/          # Header files
├── shaders/          # Vertex and fragment shaders
├── textures/         # (Optional) Texture images for planets
├── assets/           # (Optional) Models or extra visuals
├── CMakeLists.txt    # Build configuration
└── README.md
🙌 Credits
Developed by Y-rocket
Inspired by various OpenGL solar system visualizations and educational simulations.

📄 License
This project is open-source and available under the MIT License. See LICENSE for more details.

