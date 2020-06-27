# 3D_Fundamentals

Building a 3d rendering pipeline. Following Chili tutorials 3d Fundamentals<br />

Stage: Backface culling, independent rotations <br />
![cube demo](cube.gif)

Stage: Texture mapping (with clamping, wrapping of textures)<br />
![pyramid demo](pyramid_wood.gif)

Stage: Added pixel shader to the pipeline (templated function object for flexibility in rendering effects)<br />
![pyramid demo](VertexColorEffect.gif)

Stage: Z-buffer added<br />
![depth buffer demo](twocubes.gif)

Stage: Triangle Intersection rendering
(Own implementation of "A Fast Triangle-Triangle Intersection Test" by Tomas Moeller)<br />
![Triangle intersection](trianIsect.gif)

Applying triangle intersection rendering to other shapes<br />
![Cube intersection lines](cubies_final.gif)
