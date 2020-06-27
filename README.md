# 3D_Fundamentals

Building a 3d rendering pipeline
Following Chili tutorials 3d Fundamentals

Stage: Backface culling, independent rotations
![cube demo](cube.gif)

Stage: Texture mapping (with clamping, wrapping of textures)
![pyramid demo](pyramid_wood.gif)

Stage: Added pixel shader to the pipeline (templated function object for flexibility in rendering effects)
![pyramid demo](VertexColorEffect.gif)

Stage: Z-buffer added

![depth buffer demo](twocubes.gif)

Stage: Triangle Intersection rendering
(Own implementation of "A Fast Triangle-Triangle Intersection Test" by Tomas Moeller)
![Triangle intersection](trianIsect.gif)

Applying triangle intersection rendering to other shapes
![Cube intersection lines](cubies_final.gif)
