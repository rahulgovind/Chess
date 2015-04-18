#Chess
CS 101 Project

- System requirements:

	Main requirements:
	Dependencies: OpenGL API Version 3.2 or higher
	Hard Disk: 20 MB of free hard disk space

	Other requirements for Developer:
	MinGW Developer Environment ( 32 bit ) with g++ version 4.7.1 or higher
	CMake Version 2.8 or higher

- To compile directly
	- Run install.bat
	
	- In case, MinGW folder is not present in the PATH, the batch file will prompt you to enter the MinGW folder
	For e.g. "C:\Program files (x86)\MinGW\bin" ( Quotation marks required )
	
	- In case, CMake folder is not present in the PATH, the batch file will prompt you to enter the CMake folder
	For e.g. "C:\Program files (x86)\CMake\bin" ( Quotation marks required )

- To make MAKEFILE and compile separately
	
	- For making MAKEFILE
		- Make a new folder
		- Run cmake -G "MinGW Makefiles" from this folder
		( Make sure that MinGW and CMake directories are present in the PATH)

	- For compiling
		- Run mingw32-make from the new folder

- Function of each code file 
	The functions performed by the code in each of the code files is given below.
	
	- main.cpp - Arguments passed to program checked here and the Chess window is loaded.
	
	- Chess2D.h - Change window settings and load it, load graphics, and process keyboard and mouse input.
	
	- board.h and Board.cpp - Basic validation of moves and save positions of all chess pieces
	
	- engine.h and Engine.cpp - Provide interface between the front end, the board and the AI
	
	- engine_ai.h and Engine_AI.cpp - Predict the best possible move for the computer i.e. implementation of Minimax and the necessary evaluation functions
	
	- image_loader.h and image_loader.cpp - Loads BMP image into memory
	
	- gl.h and gl.cpp - Custom GL Library to load shaders, text and menus.
	
	- shader_source.h - GLSL code required for OpenGL Shader
	
	- CMakeLists.txt - Instructions for making MAKEFILE
	
	- compile.bat - Automate the process to build the final executable
	
- Other folders
	- images - Contains all the necessary images for the graphics
	
	- dlls - Contains the necessary dlls to run the executable
	
	- include - Contains the header files of the dependencies
	
	- lib - Contains the precompiled binaries required for linking 
	
	
	
	
