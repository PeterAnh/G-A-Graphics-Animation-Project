# G&A (Graphics & Animation) Project

![Alt Text](https://media.giphy.com/media/vcdF5wgMBdyylVaQ7X/giphy.gif)

## Project Part 1: Spec, Starting point, Sample Videos

### Group Report
As well as your source code, each group must submit a PDF report (about 3-4 pages) that describes:

How well your program works, including a list of any functionality that doesn't work.
The steps you followed in building your program, focusing on the problems that you needed to solve in this process.
An outline of personal contribution of each group member to the project. (Unless you worked alone.)
An explanation of the design and implementation of your individual additional functionality and any other novel features of your project that you'd like to draw attention to.
A reflection on your experience with this project. (Largely for future improvements to the project.)
Please use Times font size 12, single spacing, and 2.0 to 2.5cm margin for your report. Feel free to insert diagrams if they aid explanation in your report. However, do not try to fill up your report with just diagrams.
### Assessment Details
Your project part 1 has 60 marks and will be assessed on correctness of the functionalities and coding (structure, clarity and appropriate use of OpenGL). Tasks A to I carry 5 marks each. Task J carries 5+5 marks for the two additional functionalities each student is required to implement. The last 5 marks are researved for the report. This equals the 60 marks allocated to part 1. You can get upto 5 bonus marks by optionally completing task K.

Specific tasks that your scene editor should implement
Your scene editor needs to implement all the functionality described in the items A to J below (and optionally item K as well if you want to get some bonus marks). Some demonstration and explanation will be given in a lecture. Please view the lecture recording if you miss that lecture. Please also view messages/announcement on help3003 during the project weeks.

Don't panic if the videos provided alongside the items seem beyond what we've done in the labs. The project tasks have been chosen to cover all the relevant concepts without requiring you to implement too much extra. Thus, much of the functionality is already implemented in the skeleton code, and some of the tasks require similar code to what is in the skeleton code.

Also, you are allowed to improve upon the specification and implement some items differently, but you should explain in your report why it is an improvement.

Although the videos don't have any audio, all the mouse actions were captured in the videos. We suggest that you view each video in full screen mode carefully. Please pay attention to the mouse icon shown inside the window.
When the circle next to the mouse icon turns into red, it indicates that the left mouse button is held down.
When the circle becomes green, it indicates that the middle mouse button is held down. When the wheel of the middle mouse button is rolled, you should see an arrow and its direction.
The right mouse button is reserved for bringing up the menu.
For your convenience, you can download the Zip file [87MB] of all the videos below.
You can also download a movie file (including audio) [252MB] (when unzipped, it would come to approximately 260MB) that explains about the camera's position, viewing direction, and some basic mouse events.
The skeleton code draws the ground, an initial mesh object and a sphere showing the position of a single point light source; however, it has the camera always facing the same way. You can move it forwards and backwards using the scroll wheel, via the viewDist variable, but there's no way to move it otherwise. You should fix this problem in the code. The variables camRotUpAndOverDeg and camRotSidewaysDeg are already set appropriately when moving the mouse with the left button down, or the middle button down (or shift + left button). Modify the display function so that the camera rotates around the centre point according to these variables, in the same way as the sample solution shown in the video below. 

### Tasks:
* **Task A:**<br/> 
Your scene editor should do the following:
When the left mouse button is held down and dragged horizontally across the window, it should rotate the scene about the axis that is vertical to the ground plane.
When the left mouse button is held down and dragged vertically up (or down) in the window, it should zoom into (or out of) the scene.
Dragging the middle mouse button horizontally is the same as dragging the left button horizontally.
Dragging the middle mouse button vertically up (or down) across the window should change the elevation angle of the camera looking at the ground plane. Correspondingly, this tilts the entire scene up (or down).
n.The skeleton code draws the ground, an initial mesh object and a sphere showing the position of a single point light source; however, it has the camera always facing the same way. You can move it forwards and backwards using the scroll wheel, via the viewDist variable, but there's no way to move it otherwise. You should fix this problem in the code. The variables camRotUpAndOverDeg and camRotSidewaysDeg are already set appropriately when moving the mouse with the left button down, or the middle button down (or shift + left button). Modify the display function so that the camera rotates around the centre point according to these variables, in the same way as the sample solution shown in the video below. 


* **Task B:**<br/>
Similarly, changing the location and scale of objects is already implemented, via the loc and scale members of the SceneObject structure, which is stored in the variable sceneObjs for each object in the scene. However, the angles array in this structure doesn't affect what's drawn, even though it is appropriately set to contain the rotations around the X, Y and Z axes, in degrees. Modify the drawMesh function so that it appropriately rotates the object when drawing it in the same way as the sample solution.
Note: The skeleton code also moves objects in different directions compared to the sample solution -- you should also fix this (Hint: there is more than one way to do this).

  As shown in the video, your scene editor should do the following:
Dragging the left mouse button vertically across the window should rotate the current object about the x-axis (parallel to the ground plane and pointing to the right).
Dragging the middle mouse button horizontally across the window should rotate the current object about the z-axis (parallel to the ground plane and pointing out of the screen).
Dragging the left mouse button horizontally across the window should rotate the current object about the y-axis (vertical to the ground plane).
Dragging the middle mouse button vertically across the window should change the texture scale on the object.


  Sometimes two of the rotations go the same way, as in the video below. 



* **Task C:**<br/>
Implement a menu item for adjusting the amounts of ambient, diffuse and specular light, as well as the amount of shine. Recall that the shine value needs to be able to increase to about 100 or so. Modify the materialMenu and makeMenuM functions so that they change the appropriate members of the SceneObject structure. The code to use these to affect the shading via the calculation for the light is already implemented in the skeleton code provided.
Follow the corresponding implementations of other similar menu items. Use the setToolCallbacks function which has four arguments which are pointers to four float's that should be modified when moving the mouse in the x and y directions while pressing the left button or the middle button (or shift + left button). After each callback function accepting a pair of (x,y) relative movements is a 2×2 matrix which can be used to scale and rotate the effect of the mouse movement vector. See the calls to setToolCallbacks in scene-start.cpp for example. The setToolCallbacks function is defined in gnatidread.h.


  The sample video shows that
the ambient or diffuse light of the object can be interactively changed by dragging the left mouse button horizontally or vertically;
the specular light and amount of shine can be interactively adjusted by dragging the middle mouse button horizontally or vertically.
the position of the light source can be modified by dragging the mouse button. For this functionality, you are suggested to use
the left mouse button for changing the x- and z-coordinates
the middle mouse button for changing the y-coordinates
of the light source's position


* **Task D:**<br/>
In the skeleton code, triangles are "clipped" (not displayed) if they are even slightly close to the camera. Fix the reshape callback function so that the camera can give more "close up" views of objects. 
 

 


* **Task E:**<br/>
Also modify the reshape function, so that it behaves differently when the width is less than the height: basically whatever is visible when the window is square should continue to be visible as the width is decreased, similar to what happens if the window height is decreased starting with a square. 
 


* **Task F:**<br/>
Modify the vertex shader so that the light reduces with distance - exactly how it reduces is up to you, but you should aim for the reduction to be noticeable without quickly becoming very dark, similar to the sample solution. This should apply to the first light, but the video shows a slightly different implementation so the second light was chosen from the menu to demonstrate the concept. Also, you should implement the next item first to see whether the ground plane is correctly lit. 
 


* **Task G:**<br/>
Move the main lighting calculations into the fragment shader, so that the directions are calculated for individual fragments rather than for the vertices. This should have a very noticeable effect on the way the light interacts with the ground (especially when the ground has a plain texture map), since the ground only has vertices at the corners of a coarse grid. 
 


* **Task H:**<br/> Generally specular highlights tend towards white rather than the colour of the object. Modify the shader code so that the specular component always shines towards white, regardless of the texture and the colour assigned to the object.   


* **Task I:**<br/> Add a second light to both the C++ code and the shaders. The second light should be directional, i.e., the direction that the light hits each surface is constant and independent of the position of the surface. In particular, use the direction from the second light's location to the origin as the direction of the light. For example, moving the light source upward should increase the y-component of the direction of the light source, making the light source behave like the mid-day sun. Like the first light source, use sceneObj[2] to store the coordinates and colours of the second light, and make it similar to the first light, including placing a sphere at the coordinates of the light. 
 


* **Task J:**<br/> Whether you work alone or with another student, each group member must individually design and implement two small, but non-trivial additional functionality. E.g., (1) allowing objects in the scene to be deleted, or (2) duplicated. (3) Adding a wave motion to the ground or (4) to the scene objects. (5) Adding a spot light to the scene and allowing its illumination direction to be changed interactively. (6) Varying colours over time. (7) Toggling on/off the global coordinate system on the ground. (8) Allowing the user to select another object as the current object. (All of these are acceptable, but towards the simpler end).
Individual's report should clearly describe the additional functionality implemented. 


* **Task K:**<br/> Other additional functionality by the group may earn a small number of bonus marks, but not more than 5 marks. Generally, focusing on other tasks is a better use of your time. E.g., (1) loading and saving scenes, (2) clicking on objects to edit them, (3) setting paths for objects to move along over time, (4) motion under physics, (5) particle effects, (6) attaching one object to another so that they move and rotate together. (These are tougher than above, but also suitable as your individual additional functionality.)

## Part 2: Modeling and Animation

You are required to produce at least three animated human models in your program. The 3D human models can be produced by MakeHuman software and animation by Blender. You are suggested to follow the instructions given below and also take help from online tutorials when needed. You should export your animated model from blender and add it to your scene editor. When a user adds the animated model to the scene, it should perform the animation automatically. Some sample code and instructions are supplied to help you accomplish these tasks.
Files provided
You should build part 2 using your work for part 1 as the starting point. For part 2 of the project,
follow the instruction given in addingAnimation.txt.
download the additional header file gnatidread2.h and put it in the same directory as scene-start.cpp.
download the human animation example (model56.x) and put it in the models-textures directory. You can use model56.x for testing your code. To see how the animation looks download model56.blend and play it inside blender.
Task Details

### Tasks

* **Task A: Texture Scale**</br> Implement the scene-editor texture scale menu item (alongside the rotation angles). The texture scale is already stored in sceneObjs, you just need to arrange for it to modify the call to texture2D in the fragment shader. Dragging the middle mouse button should change the texture scale.

* **Task B: Modelling a 3D Human**</br> In part 2, you will animate realistic 3D human models. To generate the human models, use the open source MakeHuman software. We recommend using this version MakeHuman for Windows as it does not require installation. Unzip the folder and doubleclick the exe file. You will see a generic 3D human model. Adjust the human body and face parameters to generate three custom models. Do not use clothes for the model as it will complicate the animation part. Follow the steps below for each model you create.

* Select the "Pose/Animate" tab from the top.
* Select "game.json" from the right tab (to rig a skeleton to the model).
* Click Files-> Export -> Blender exchange (mhx)
* Give the model a name e.g. Mate and click on the "..." button -> choose directory and save.
* Check the directory to see if "Mate.mhx" has been successfully written.


* **Modelling a 3D Human (UPDATED 14-5-2019)** </br> You can also use the latest stable version of MakeHuman 1.1.1 from here. If you are using this version, you will need to install plugins following these instructions. Adjust the parameters to get the desired human shape and then follow the steps below to save the human file.

* Select the "Pose/Animate" tab from the top.
* Select "Cmu mb" from the right tab (to rig a skeleton to the model).
* Click Files-> Export -> MakeHuman Exchange (mhx2). This option will be availabe only if "9_export_mhx2" folder is inside the MakeHuman * plugins folder as detailed in the instructions above.
* Give the model a name e.g. Mate and click on the "..." button -> choose directory and save.
* Check the directory to see if "Mate.mhx2" has been successfully written.
* Put variations in your models so that they are different from each other and from the models of other groups. You can download a sample model Mate.mhx.

* **Task C Animating the 3D Human in Blender** </br> You can use the older version of Blender i.e. version 2.76 which can be downloaded from https://www.blender.org/features/past-releases/2-76/, or the latest version of Blender i.e. version 2.79b. Depending on your version and plugins (see below), you will be able to import .mhx or .mhx2 files generated by MakeHuman.

* Required Plugins: You will need to ensure that the blender plugins are installed i.e. the "makeclothes", "maketarget", and "makewalk" folders are present in Blender addons e.g. ../Blender/2.76/scripts/addons/. If they are not, go to here, download blender_plugins_for_1.1.1.zip and unzip the three folders to the addons folder or if you do not have access to the folder go to (inside Blender) "File->User Preferences->Install Add-on from File" and then select the blender_plugins_for_1.1.1.zip file. Step by step instructions are in this video .

* Starting Blender: When you start Blender, you will see a cube at the origin. Press the "A" key (multiple times) until everything is selected/highlighted. Press the "X" key and select "delete" to get a clean canvas.

* Setting Preferences: Ensure that the MakeHuman related functionalities are enabled. Select File -> user preferences and search for "makehuman". This will show all preferences with the "makehuman" keyword. Enable the "Import-Runtime:...", "MakeHuman:Make clothes", "MakeHuman: Make Target", MakeHuman:MakeWalk". Save and close the sub-window.

* Motion Capture Files: Motion capture (MoCap) files contain articulated motions of joints. You can visualize them as moving stick figures. For realistic animation, real human motion is captured in laboratories using multiple cameras and markers installed on the human body. Markerless capture is also possible with the Kinect sensor for Xbox. This motion can then be transferred to any 3D model with similar joints. For example, capture the motion of a real human and then make a robot perform the same movement in graphics software.

* MoCap files are available online. In this project, we will use the CMU MoCap files in BVH format. These are over 2,500 motion capture files available here. You can get MoCap files from another source and in other formats if you like as we will only test your final animation in your program.

* Retargetting MoCap in Blender: From the left tab select "Misc" (Blender 2.76) or "MakeWalk" (Blender 2.79b) and then modify "Start Frame" and "Last Frame" values. Click on "Load and Retarget" then locate a MoCap .bvh file on your computer. Double click the bvh file. You will see vertical bars at the bottom of the window representing the frames. You can run the animation by clicking on the 'play' button.

* Saving the Animation: Click on File -> Save as, to save the animation as a .blend file for later use in Blender.

* Exporting Animation to DirectX: To display the animation in your graphics program you will need to export it in the DirectX format similar to the provided model56.x file. To export the animation click File -> Export -> DirectX. Select the appropriate options (I am leaving this part to you to figure out) from the bottom left tab and then save it to the "models-textures" directory. Recall that you will name your animation files as model57.x, model58.x, model59.x. A minimum of three are required.

* You don't need to add materials, textures, lighting or ground since these are available in your graphics program. Note that you will need to scale up the human models by 10 in Blender (or inside your program) to match its size with the rest of the objects.

* You are not required to make articulated models from scratch (i.e. without using MoCap) in blender as part of this project. However, if you are interested, below are some useful Youtube tutorials:
Part 1: meshing. https://m.youtube.com/watch?v=bblqJCFsiP0
Part 2: materials and texture. https://m.youtube.com/watch?v=HFQvYb0wWH4
Part 3: adding eyes, nose, buttons, etc. https://m.youtube.com/watch?v=mX-lENGLhS0
Part 4: rigging. (you may only need some bones depending on what you want to create). https://m.youtube.com/watch?v=SmlhC2DAdjs
Part 5: weight painting. https://m.youtube.com/watch?v=Sv8F6MWCJgI
Part 6: animation. https://m.youtube.com/watch?v=Pdn5t5rfQcI

* **Task D: Coding to show animations** </br> To complement the animation, you should modify your scene-editor so that each animated scene object repeatedly performs its animation. The animation speed should be configurable using the menus and the setToolCallbacks function (or similar).


* Each group will need to make at least three unique human models and three unique animations.

* Implement one small extension to each animation, e.g., varying speed, forward+backward animation etc.
Any extra functionality in addition to the basic requirements above may fetch bonus marks e.g. make a human walk like a spider or run like a cheetah. Generally, focusing on the basic functionality is a better use of your time.

* **Assessment Details** </br> Part 2 has 40 marks. Part 1 and 2 both have 100 marks and contribute towards 40% of the total unit marks.

Task A, B have 5 marks each
Task C has 10 marks.
Task D has 15 marks
Report has 5 marks from Part 2 and another 5 from Part 1 (hence total marks for the report are 10).
Groups: You should complete this part in the same group as for part 1. As mentioned in part 1, group size can be 1.

* **Submission:** </br> Each group should submit the project once only by the deadline with completed Part 1 and Part 2. Assuming that you modified the code inside the LINUX_VERSIONS (or MAC_VERSIONS) folder, follow the instructions below:

Remove the "models-textures" folder from the LINUX_VERSIONS folder.
Name your animated models as model57.x, model58.x, model59.x, and so on and copy them into the LINUX_VERSIONS folder.
Copy your PDF report in the LINUX_VERSIONS folder
Rename the LINUX_VERSIONS folder to LINUX_VERSIONS_lastname1_lastName2.
Zip the folder and submit on cssumbit.
Use the lab computers to Zip the folder.
Ensure that the folder contains all your code (C++ files, header files, GLSL files etc). Follow the above instructions carefully to avoid submitting incorrect, incomplete and corrupted files. Late penalties will apply if we have to approach you again in case there are missing files or the zip file is corrupt. 


The project report (please see above) should cover the following:

How well your program works for part 2, including any functionality that doesn't work and why it doesn't work.
The steps you followed in building your program for part 2, focusing on the problems that you needed to solve in this process.
An outline of your personal contribution to your group project (unless you worked alone).
An explanation of the design and implementation of additional functionality and any other novel features of your project that you'd like to draw attention to.
A reflection on your experience with this project (for future improvements to the project).

* **Originality:** You may discuss with other students the general principles required to understand this project, but the work you submit must be the sole effort of your group, and the individual components must be the sole effort of yourself. Your code should be based on the supplied skeleton code, unless you have special permission to use another environment. You are also allowed to base parts of your code on the lab sample solutions, as well as the examples from lectures and the recommended text. If you base your code on code from any other source you must clearly reference the origin (in a comment in the code) that also says exactly which parts of your code are based on code from that origin – it will be considered academic dishonesty if you omit any such references.
