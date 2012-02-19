Ian Lilley - HW1 - Jello - CIS563

READ THE WHOLE THING BEFORE RUNNING!!! (also read with word wrap)
-- 3 late days used

For this assignment I decided to port the starter code to the openGL renderer I've been working on. Although this process was quite time consuming, it feels like time well spent because it helped me understand the starter code on a very low level (which helped with learning mass-spring systems). Some things to point out:
1) IMPORTANT: To build the project, go into the tools folder and double click "build_automator.bat". The visual studio solution file will be generated in the "build" folder (in the root level directory)
2) Run in Release mode.
3) There is a config file that sets the scene (located in data/world/). Unfortunately, I did not have time to use an XML scene parser. The file is split up by different objects, including lights, render objects, spheres, cylinders, and jello. The corresponding fields define the shape, material, and position of the objects. For example, you might see:

Jello -- type of object
name jelly1 -- name (not important)
origin 0 3 0 -- original position
size 1 1 1 -- size of jello
divisions 6 6 6 -- number of springs per dimension
material red -- color (there must be a corresponding file in the material/ folder)
program Jello -- shader. Technically, the jello has its own shader, but it's no different from the Material shader. For fun you can use the "White" shader.

The order of the objects in this file do not matter, but the order of the fields themselves do matter (this can be fixed as I continue to work on this project). To remove an object from the scene, you can delete its entry or add a "//" before the type name. If the scene is laggy in release mode, remove some of the jellos (I'm running on a GTX460).

4) My non-RK4 integration methods are not great (the jello resembles cloth).
5) Keyboard commands:

Num1 - Toggle jello mesh
Num2 - Toggle force vectors
Num3 - Toggle collision normals (doesn't do anything)
Num4 - Toggle structural springs
Num5 - Toggle shear springs
Num6 - Toggle bend springs
Num7 - Toggle normals
Num8 - Set Euler
Num9 - Set Midpoint
Num0 - Set RK4 (best one by far)

Space - Apply upward force to the jello
Arrows - Apply x and z forces to jello (not as good as upward force)

6) I use a few fudge factors to make the jello respond to collision/contacts appropriately.
7) I'm still working on this project (mostly the rendering system), so if there are confusing areas of code those will be fixed later.

Extra Credit:
- Collision with spheres
- Limited user generated forces with the space and arrow keys
- Ported to a different rendering system (this might fall under "Support Advanced Rendering")

Questions:
1) Increased Ks increases stiffness. Increased Kd increases wobblyness.
2) It might be good to have a system that tries to rebuild the jello's original shape when it has been deformed. Currently, when my jello gets deformed the distance between any two particle might in fact be restLength apart, so the simulation thinks the jello is all set. It would be good to have a system that looks at the relative position of a particle relative to its neighbors, and corrects it when it gets out of place. The good thing about this simulation is it is physically accurate for the most part without being too laggy.
3) Bead on a wire
4) Explicit integration methods decouple the particles while doing the integration calculations (meaning each particle is evaluated separately). Implicit integration methods treat the particles as one mesh and solve their positions together. This is much harder to do efficiently.= but is more accurate.
5) You would represent the particles motion with some parametric function, that way accurately representing the particle's position.
6) My jello is probably much more stiff than real jello. The thing is, if I made mine much looser the integration would start to fail.

Sources:
High level discussion with several DMD'13 people at various times.

