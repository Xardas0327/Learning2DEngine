# Learning2DEngine

## Introduction:
This project was created for my own development and
I want to use this one for reference too.  
I have relearned the C++ and I have learned the OpenGL form
[learnopengl.com](https://learnopengl.com/).
But I think the Game Engine Architecture book
and the Unity Engine also influenced me a lot.
Maybe the project's name is a bit tricky, because I guess it is only a library (yet).

Futhermore, I know lots of features are missing,
some features could be better and why it is Visual Studio Project and not
a CMake project.
I plan to continue its developing. But I think one full life is not enough
for a huge project like a Game Engine (mostly, if I can work on it only in my free time)
and we have to accept it, that similar projects will never be completed, just stopped
or restarted.

Last, but  not least, the project's starting point was the Breakout, which is reimplemented in my Engine.
Maybe today, there is not any code line, which came from that project.
But everything was started from [here](https://learnopengl.com/In-Practice/2D-Game/Breakout).  
The code owner is Joey de Vries, who has the
[learnopengl.com](https://learnopengl.com/) webpage too.
Its [License](https://learnopengl.com/About).

So, if you want to try Learning2DEngine, you can do it.
But you should check the licenses first. Their licences are in their own documentation.  
Moreover, if I create a new version, maybe it won't be compatible with the older versions.

## Requirements:
- Visual Studio 2022
- With "Desktop development with C++" Module

## Used Libraries:
The most project can't really be started from zero, so I also use some library,
but I tried to keep them minimum.
- GLAD for OpenGL [License](https://github.com/Dav1dde/glad?tab=License-1-ov-file)
- GLFW for OpenGL [License](https://www.glfw.org/license.html)
- GLM for OpenGL math [License](https://github.com/g-truc/glm?tab=License-1-ov-file)
- stb_image for load images into textures [License](https://github.com/nothings/stb?tab=License-1-ov-file)
- Freetype for write texts on the screen [License](https://freetype.org/license.html)

## Documentation:
It is recommended to use the documentation links in the released branches.
### [Learning2DEngine](Documentations/Learning2DEngine.md)

### Games
- [Breakout](Documentations/Breakout.md)
- [Snake](Documentations/Snake.md)
