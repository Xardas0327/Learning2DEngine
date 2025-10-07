# Learning2DEngine

## Introduction
This is a 2D Game Engine, which I created for game development and learning.
Maybe today, there is not any code line, but this Engine was created
from a [Breakout Tutorial](https://learnopengl.com/In-Practice/2D-Game/Breakout).  
The code owner is Joey de Vries, who has the
[learnopengl.com](https://learnopengl.com/) webpage too.
Its [License](https://learnopengl.com/code_viewer_gh.php?code=src/7.in_practice/3.2d_game/0.full_source/progress/2.program.cpp).

## Concept
The concept was, that the Engine will be generic for making any 2D games.
That is why, I try to implement every features, that this will be/stay true.
At the moment, it support only Windows 64bit and it use only OpenGL as Graphic API.
But who knows, maybe they will change later.  
The project has 2 folders:  
- Learning2DEngine: it contains the Engine source code.
- Plugins: this contains the glad.c.
  (This folder is not the same as in the main folder)  

A game has to be inhereted from the class `Game`. This class has Init and Terminate funcitions,
but maybe the most important function the Run, which run the game.
If you want more info please check the Order of Execution.  
The game has `GameObject`(s), which has a `Transform` and the components. The transform
contains the position, scale and rotation. The components contains `Component` items,
which can have data and functions. There are some special components, which can run automatically.  
If you want to read more, please check the Order of Execution
or check the classes documentations under Namespaces.

## Order of Execution
1. Calculate delta time: It is simple, just the engine asks the time by `glfwGetTime()` and
the engine subtracts the previous time from the current one. Finally the result is multiplied 
with time scale.  
Note: In debug mode, The delta time has a max value (before time scale), which is in
`L2DE_DEBUG_MAX_BASE_DELTATIME`. It is useful, because when the game stop by a breakpoint the
delta time won't be huge.
1. Refresh Events (Keyboard, Mouse, Window etc): The engine call the `glfwPollEvents`. But before
this, the engine "fixes" the values, if keyboard or cursor buttons are `KEY_DOWN`, they become
`KEY_HOLD`.
1. Update: Those components will run here automatically, which are inherited
from `UpdaterComponent` and they and their gameobjects are active.
Every UpdaterComponents should run independy from each other.  
Furthermore, it supports the multithreading.
1. Check Collisions: Those components will be checked here automatically, which are inherited
from `BoxColliderComponent` or `CircleColliderComponent` and they and their gameobjects are active.
2 colliders can collide if their mask layer has a intersection (maskLayer1 & maskLayer2 > 0) and
one from colliders is not kinematic. But if there is a collision, both object's OnCollision function
will be called (so, the kinematic OnCollision will also be called).  
Furthermore, it supports the multithreading.
1. LateUpdate: It is almost same like the Update, just the components has to be inherited from
`LateUpdaterComponent` and they run after the collision checking and not before.  
Furthermore, it also supports the multithreading.
1. Clear Window to default color: the screen buffer will be cleared.
1. Render: Those components will be rendered, which are inherited from `RendererComponent`
and their mode is RenderMode::RENDER. They are ordered by layer and those object, which use
the same renderer and same layer, they can be rendered at the same time.
If an object has layer with lower number, it will be rendered earlier
and in the image, they will be further back.  
Furthermore, the engine can do the Render with MSAA and/or post process effects.
1. LateRender: It is almost same as the Render, just it does not have MSAA and post process effects
and their mode is RenderMode::LATERENDER. It is recommand for UI.  
Note: The LateRender run after Render. So if a Render component has a higher layer number,
it will be still rendered before the LateRender component.
1. Update Window: The `glfwSwapBuffers` will be called on the window.
1. Destroy Marked GameObjects: When the `GameObject::Destroy` is called, the gameobject will not be
destroyed immediately. So, every reference pointers will be fine until end of the frame.

## License
Learning2DEngine © 2025 by Zsolt Sziráczki is licensed under [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/).  

## Properties
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

## Source code
A couple functions/variables will not really be described,
because I think their names and/or their comments can explain simple, what they are.  
The unit tests of a class will not really be described here, but their files will be added to the class's
documentation.  
Any helper for the testing will be in the Test namespace.

### Namespaces
- Learning2DEngine
  - [Animator](Learning2DEngine/Animator.md)
  - [DebugTool](Learning2DEngine/DebugTool.md)
  - [Editor](Learning2DEngine/Editor.md)
  - [EventSystem](Learning2DEngine/EventSystem.md)
  - [Object](Learning2DEngine/Object.md)
  - [ParticleSimulator](Learning2DEngine/ParticleSimulator.md)
  - [Physics](Learning2DEngine/Physics.md)
  - [Render](Learning2DEngine/Render.md)
  - [System](Learning2DEngine/System.md)
  - [UI](Learning2DEngine/UI.md)
  ###
  - [Test](Learning2DEngine/Test.md)