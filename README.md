# 🌌 Solar System Model (OpenGL + GLFW)

A 3D simulation of the solar system using OpenGL, GLFW, and GLM. This project demonstrates planetary motion, camera control, and lighting in a space environment.

---

## 🌠 Features

- Realistic orbit and rotation of planets
- Central sun with orbiting celestial bodies
- Camera navigation in 3D space
- Lighting and basic shading
- (Optional) Textured planets using SOIL

---

## 🚀 Technologies Used

- C++  
- OpenGL  
- GLFW  
- GLM  
- SOIL (for texture support)

---

## 📸 Preview

https://github.com/user-attachments/assets/435ac1e9-c4b1-474b-9dac-646b627b69d6

---

## 🛠️ Setup Instructions

### Prerequisites

- C++ Compiler (GCC/Clang/MSVC)
- CMake
- OpenGL-capable device
- Git

### Dependencies

- GLFW
- GLM
- SOIL (optional)

### Build Steps

```bash
git clone https://github.com/Y-rocket/solar-system-model-openglv.git
cd solar-system-model-openglv
mkdir build && cd build
cmake ..
make
./SolarSystem
```
## 🎮 Controls

| Key/Mouse | Action           |
|-----------|------------------|
| W / A / S / D | Move camera  |
| Mouse     | Rotate view     |
| Scroll    | Zoom in/out     |
| ESC       | Exit application|
