# Breakout:

## Introduction:
This is a Breakout game, which Joey de Vries implemented and its tutorial is [here](https://learnopengl.com/In-Practice/2D-Game/Breakout).  
Its [license](https://learnopengl.com/About).  
But I recreated it with my own Engine.  

![Breakout Game](Images/breakout.png)

## License:
Breakout © 2024 by Zsolt Sziráczki is licensed under [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/?ref=chooser-v1)

For sounds, there is the irrKlang, which I use it with
its non pro licence, which is [here](https://www.ambiera.com/irrklang/license.html).

## Control
- Start: Press Enter
- Close Window: Press Esc or use your mouse.
- Select Map: Press W and S
- Player control: Press A and D
- Shooting the ball: Press Space

## Assets:
The assets files came from learnopengl tutorial.

## Properties:
- Configuration Type:  
  - Application (.exe)
- VC++ Directories => Library Directories:  
  - $(SolutionDir)Plugins\Game\lib\irrKlang
- C/C++ => General => Additional Include Directories:  
  - ..\Learning2DEngine  
  - $(SolutionDir)Plugins\Engine\include  
  - $(SolutionDir)Plugins\Game\include
- Linker => Input => Additional Dependencies:  
  - irrKlang.lib
- Post-Build Event:  
  When you build the game, the assets folder will be copied into output directory.
  Plus the irrKlang DLL files are also copied into output directory from Plugin folder for the sounds.  
  Note: if some files exist, they will be overwritten.

### Source code: [here](../Breakout)