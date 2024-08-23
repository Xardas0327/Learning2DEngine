# Learning2DEngine

## Introduction:
This is a 2D Game Engine/Library, which I created for game development and learning.
Maybe today, there is not any code line, but this Engine was created
from a [Breakout Tutorial](https://learnopengl.com/In-Practice/2D-Game/Breakout).  
The code owner is Joey de Vries, who has the
[learnopengl.com](https://learnopengl.com/) webpage too.
Its [License](https://learnopengl.com/About).

The concept was, that the Engine will be generic for making any 2D Games.
That is why, I try to implement every feature, that this will be/stay true.
At the moment, it support only Windows 64bit and it use only OpenGL as Graphic API.
But who knows, maybe they will change later (or maybe in the next version).  
The project has 2 folders:  
- Learning2DEngine: it contains the Engine source code.
- Plugins: this contains the glad.c.
  (This folder is not the same as in the main folder)

## License:
Learning2DEngine © 2024 by Zsolt Sziráczki is licensed under [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/?ref=chooser-v1)

## Properties:
- Configuration Type:  
  - Static library (.lib)
- VC++ Directories => Library Directories:  
  - $(SolutionDir)Plugins\Engine\lib\GLFW\lib-vc2022
  - $(SolutionDir)Plugins\Engine\lib\freetype\vs2015-2022\win64
- C/C++ => General => Additional Include Directories:  
  - $(SolutionDir)Plugins\Engine\include
- Linker => Input => Additional Dependencies:
  - opengl32.lib
  - glfw3.lib
  - freetype.lib

## Source code:
The most constructors and destructors won't really be described,
because the most of time, they just initialize some variables.  
Only those functions, variables and macros will be explained,
which can be useful to use that class.  
I think the names and/or their comments can explain them simple,
what they are. (But there can be some exception.)

### Namespaces:
- Learning2DEngine
  - DebugTool
  - EventSystem
  - ParticleSimulator
  - Physics
  - Render
  - System
  - UI