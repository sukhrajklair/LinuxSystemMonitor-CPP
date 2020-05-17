# Cpp-System-Monitor
This project implements a Linux System Monitor similar to htop. It reads and parses various system files to extract the useful information and displays the information in the console using ncurses library.
![System Monitor](images/monitor.png)
## Starter Code
Starter code came from this repo: https://github.com/udacity/CppND-System-Monitor-Project-Updated

# Dependencies
## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

Within the Udacity Workspace, `.student_bashrc` automatically installs ncurses every time you launch the Workspace.

If you are not using the Workspace, install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

# Build and Run Instructions

1. Clone the project repository: https://github.com/sukhrajklair/LinuxSystemMonitor-CPP.git

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`
![Starting System Monitor](images/starting_monitor.png)
