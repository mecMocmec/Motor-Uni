This project is a simple game engine made in C++ for the Game Engines subject at CITM (UPC). https://github.com/mecMocmec/Motor-Uni
Made by: 
Elena Ibañez Armora: https://github.com/mecMocmec
Marcel Pascual Martinez: https://github.com/guifre1714
Marc Port Prat: https://github.com/Marcoto06

Hold Right Mouse button to move using WASD and Q and E to lower and raise the camera; while holding this button you can also use the mouse wheel to increase or decrease the camera speed. If you don't hold the right mouse button, the mouse wheel zooms in and out.
In the hierarchy, you can access all GameObjects and focus on them by pressing the F key. When a GameObject is selected, you can see its components and values and modify them.
In the GameObject tab, you can create empy GameObjects or primitive shapes.
In the view tab you can select which windows are open: Scene, Hierarchy, Inspector, Console, Configuration and About.
In the Help tab you can open the github documentation tab (we didn't know how to implement it so it appears as a page that does not exist), report bugs, access the latest releases and open the About window.
In the About window you can see the information about this program, the license, group members and access the GitHub page.
In the console window the program outputs different information while it runs.
In the configuration window you can change things like if the program is in fullscreen, how textures are loaded and more, as well as see information about your device, and check how the program is running.
To load an fbx file or textures, you can drag & drop them into the scene.

There are no extra functionalities.

Additional comments and instructions:

The different windows initially appear small and you may need to rescale them yourself.
We initially installed Cmake manually but we had several errors (different in each device) which didn't allow us to work propperly. After a few days trying to solve them, we ended up starting from scratch and using Claude AI to install CMake and configure the overall project. Later on, during the ongoing development project, this same program was used when we faced bugs, errors, or things that didn't work as intended and we didn't know why.