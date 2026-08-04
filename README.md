# Perplex

Perplex is a 2D game engine focused on making pixel art games with minimal friction. This project is currently still in development, and I plan on having a beta release for Windows this year.
It uses PerplexRenderer as its rendering engine, which can be found at https://github.com/gpiazza-tech/PerplexRenderer.

![Screenshot](https://raw.githubusercontent.com/gpiazza-tech/Perplex/main/docs/screenshots/1.png)

## How to build for Windows

 * Ensure git and Visual Studio 2026 are installed
 * Navigate to your desired installation directory and run `git clone --recurse-submodules https://github.com/gpiazza-tech/Perplex`

Before building the project, you need to set up your project directory. While there's no automated way to create a new project,
doing it manually is super simple!

 * Create a new directory for your project (e.g. C:\dev\Perplex Projects\Game)
 * Create an engine folder and an assets folder in the project root directory
 * Locate and copy the res folder in the pulled repo (located at PerplexCore/res) into the engine folder you just created

Finally, we're ready to build the engine!

 * Run the build script located at scripts/Win-GenProjects.bat
 * Copy your project directory when prompted (e.g. C:\dev\Perplex Projects\Game)
 * Open the generated .sln file in Visual Studio, and build the solution