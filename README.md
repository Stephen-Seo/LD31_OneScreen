

# Compiling

## Building with CMake

CMake gui recommended. (point CMake gui to the src directory as the source directory.)
Otherwise:

To build project, make directory "build" and cd to it. Then use the commands:
cmake ../src
make

To build in Release instead of Debug, use option `-DCMAKE_BUILD_TYPE=Release`

To setup Makefile with SFML-2 in a non-standard directory:
cmake -D SFML\_ROOT=${Directory} ../src

To set install directory, use -D CMAKE\_INSTALL\_PREFIX=${Directory}
