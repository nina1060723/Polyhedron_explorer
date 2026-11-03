# Polyhedron Explorer

An interactive 3D geometry visualisation tool designed to help students and teachers explore and understand regular polyhedra.

## 1. Project Overview

Polyhedron Explorer allows users to interact with three-dimensional geometric models, examine their properties, and explore their structures from different perspectives.

The project aims to make 3D geometry more accessible for learning and classroom demonstrations.

## 2. Current Features

### Five Regular Polyhedra
- Tetrahedron — 4 faces
- Cube — 6 faces
- Octahedron — 8 faces
- Dodecahedron — 12 faces
- Icosahedron — 20 faces

### Interactive 3D Viewer
- Rotate models using the mouse
- Zoom in and out
- Pan the camera
- Switch between solid and wireframe views
- Display or hide vertex labels
- Reset the camera view

### Geometry Properties
- Polyhedron name
- Number of faces, edges, and vertices
- Adjustable uniform edge length
- Surface area calculation
- Volume calculation

## 3. How to Use

1. Open the web application.
2. Select a regular polyhedron from the dropdown menu.
3. Adjust the edge length if needed.
4. Use the mouse to explore the model:
   - Left-click and drag: Rotate
   - Scroll: Zoom
   - Right-click and drag: Pan
5. Enable Wireframe to view the edges without the solid surfaces.
6. Enable vertex labels to identify vertices.
7. Click Reset View to restore the camera position.

## 4. Technologies Used

- HTML and CSS — User interface
- JavaScript — Interactive functionality
- Three.js — 3D rendering and camera controls
- C++ — Separate command-line geometry calculations
- GitHub Pages — Web hosting

The browser-based 3D viewer and the C++ program are currently separate components and are not directly integrated.

The web viewer requires an internet connection to load Three.js modules.

## 5. Running the Project

### Web Application

1. Open `index.html` in Chrome or Edge.
2. Make sure an internet connection is available.
3. If browser security restrictions prevent the JavaScript modules from loading, use a local development server such as VS Code Live Server.

### C++ Program

A C++17-compatible compiler is required.

Compile using:

```bash
g++ -std=c++17 main.cpp -o polyhedron
```

Run on macOS or Linux:

```bash
./polyhedron
```

Run on Windows:

```bash
polyhedron.exe
```

## 6. Current Limitations

- Some interface elements are still in Chinese.
- The 3D viewer and C++ program are not integrated.
- Vertex labels do not currently account for occlusion by the solid model.
- Advanced features such as unfolding and interactive cutting are not yet implemented.

## 
