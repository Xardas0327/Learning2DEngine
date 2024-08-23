# Snake

### Introduction:
It is just a simple snake game, where the edge of the screen also kill your snake.

### License:
Snake © 2024 by Zsolt Sziráczki is licensed under [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/?ref=chooser-v1)

### Source code: [here](https://github.com/Xardas0327/Learning2DEngine/tree/main/Snake)

### Assets:
- arial.ttf: It came from the Windows.  You can also find it in your Windows OS.
- Unit.png: it is a simple image, which I created with Paint.

### Properties:
- Configuration Type:  
    Application (.exe)
- C/C++ => Additional Include Directories:  
	..\Learning2DEngine  
	$(SolutionDir)Plugins\Engine\include
- Post-Build Event:  
	When you build the game, the assets folder will be copied into output directory.
	Note: if some files exist, they will be overwritten.