# Chess Game

## Overview
This project is a cross-platform chess game developed using C++ and Qt. The game supports Windows, macOS, and Linux. It provides a simple interface for playing chess.

## Features
- Classic chess gameplay against bot or another player
- The bot has three difficulty levels (easy, medium, hard)
- User-friendly graphical interface
- Cross-platform support (Windows, macOS, Linux)
- Bundled application for easy installation

## Installation

### Windows
1. Download `Chess-Windows.zip` from the latest release.
2. Extract the zip file.
3. Run `Chess.exe` to start the game.

### macOS
1. Download `Chess-macos.dmg` from the latest release.
2. Open the `.dmg` file and drag the application to the Applications folder.
3. Launch the game from the Applications folder.

### Linux
1. Download `Chess-Linux.tar.gz` from the latest release.
2. Extract the archive using:
   ```sh
   tar -xvzf Chess-Linux.tar.gz
   ```
3. Run the executable:
   ```sh
   ./Chess
   ```

## Development Setup
To build the project from source, ensure you have the following dependencies installed:
- C++ Compiler (MSVC, Clang, or GCC)
- CMake
- Qt 6.7.2

### Build Instructions
1. Clone the repository:
   ```sh
   git clone <repository_url>
   cd chess-game
   ```
2. Create a build directory and configure the project:
   ```sh
   mkdir build
   cd build
   cmake .. -DCMAKE_PREFIX_PATH=<path_to_qt>
   ```
3. Build the project:
   ```sh
   cmake --build . --config Release
   ```

## Contribution
Contributions are welcome! Feel free to submit pull requests or report issues.

## License
This project is licensed under the MIT License.

## Acknowledgments
- Developed with Qt 6.
- Inspired by classic chess games.

## Image sources
https://opengameart.org/content/chess-pieces-and-board-squares
