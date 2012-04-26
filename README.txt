Note: Only runs on Windows. The code itself should be supported by most OS's, but the lib file are all built for 32-bit Windows

How to build and run:
1) Go into "tools" folder and double click "build_automator.bat"
2) Go into "build" folder and open the .sln file
3) Run in Release!

Controls:
Left mouse to rotate
Middle mouse click to pan
Middle mouse roll to zoom
Right mouse to zoom

Alt+left mouse click to shoot sphere
Alt+middle mouse click to shoot cow
Alt+right mouse click to shoot cube

Notes:
-The cow physics look strange at times because they are represented with cubes
-The default scene might be slow on some computers. The one I'm testing on is a NVIDIA GTX460 and it rarely goes below 50fps.
-The physics assumes a timestep of 1/60, so if the fps is greater than 60, the physics will go really fast. I will fix this later.
-I'm still working on the code, so not everything is going to be perfect.