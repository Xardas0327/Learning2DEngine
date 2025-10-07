# The Collector:

## Introduction:
You can play with the collector and get the chance to collect all coins in time.  
The map of the game is created by [Tiled Map Editor v1.11.2](https://www.mapeditor.org/2025/01/28/tiled-1-11-2-released.html)  

![The Collector Game](Images/thecollector.png)

## License:
The Collector © 2025 by Zsolt Sziráczki is licensed under [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/).  

For sounds, there is the irrKlang, which I use it with
its non pro license, which is [here](https://www.ambiera.com/irrklang/license.html).

## Control
- Start: Press Enter
- Close Window: Press Esc or use your mouse.
- Player control: Press A and D and Jump with Space

## Assets:
The assets came form [here](https://brackeysgames.itch.io/brackeys-platformer-bundle) from Brackeys.

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

### Source code: [here](../TheCollector)