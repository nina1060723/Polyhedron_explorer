# Polyhedron Explorer — v1.1

## Quick start
1. Extract the ZIP.
2. Open `index.html` in Chrome or Edge with an internet connection. If module loading is blocked, use VS Code Live Server.
3. Choose a regular polyhedron, change its edge length, drag to rotate, scroll to zoom, right-drag to pan.
4. Toggle Wireframe to see **only actual polyhedron edges**. Toggle off to return to the solid shaded model.

## v1.1 fix
The previous Wireframe mode enabled `MeshStandardMaterial.wireframe`, which exposed the triangle edges used internally for rendering polygonal faces. v1.1 instead hides the filled mesh in Wireframe mode and shows the existing `EdgesGeometry` line segments (threshold 15 degrees), removing the internal face diagonals. Solid mode remains unchanged.

## Scope
The HTML 3D viewer is JavaScript/Three.js. The separate `main.cpp` is a C++ command-line geometry calculator; they are **not integrated**. The browser viewer requires online access to Three.js CDN modules. Cutting animation is not yet implemented.

## C++ build (optional)
`g++ -std=c++17 main.cpp -o polyhedron`

On macOS/Linux: `./polyhedron` ; on Windows: `polyhedron.exe`.
