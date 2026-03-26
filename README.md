# My First Big C++ Project

This repository tracks my progress with Low Level Game Dev's course on creating
a large C++ game project.

## The Build System

The common wisdom is to avoid using `GLOB_RECURSE` to collect your source
files, but there is an aspect of this discussion that I was missing until now.
The `file` command accepts a flag called `CONFIGURE_DEPENDS` as of CMake
version 3.12 which will regenerate the build system if the source files change.
This is huge, since it basically gets rid of the main issue of doing a
recursive glob. I will probably be using this more in the future. The usage
looks like this:

```cmake
# This places the list of source files in the SOURCES variable.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
```

One should take note of the warning from the CMake documentation.

> We do not recommend using GLOB to collect a list of source files from your
> source tree. If no CMakeLists.txt file changes when a source is added or
> removed then the generated build system cannot know when to ask CMake to
> regenerate. The `CONFIGURE_DEPENDS` flag may not work reliably on all
> generators, or if a new generator is added in the future that cannot support
> it, projects using it will be stuck. Even if `CONFIGURE_DEPENDS` works
> reliably, there is still a cost to perform the check on every rebuild.

Here is my answer to this. If it works reliably on the Ninja generator, then I
do not care about anything else. This is a toy project, and I do not plan on
supporting Windows builds. Also, who cares if it adds a little bit to the build
time? It probably will not be significant enough for me to care.

Another cool thing is the `RESOURCES_PATH` macro. One big headache of game
development is wiring together all of the asset paths correctly. If every path
is accompanied by the aformentioned macro, you can manage all of the paths at
once.
