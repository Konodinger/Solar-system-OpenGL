# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src

# Include any dependencies generated for this target.
include dep/glfw/tests/CMakeFiles/opacity.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include dep/glfw/tests/CMakeFiles/opacity.dir/compiler_depend.make

# Include the progress variables for this target.
include dep/glfw/tests/CMakeFiles/opacity.dir/progress.make

# Include the compile flags for this target's objects.
include dep/glfw/tests/CMakeFiles/opacity.dir/flags.make

dep/glfw/tests/CMakeFiles/opacity.dir/opacity.c.obj: dep/glfw/tests/CMakeFiles/opacity.dir/flags.make
dep/glfw/tests/CMakeFiles/opacity.dir/opacity.c.obj: dep/glfw/tests/CMakeFiles/opacity.dir/includes_C.rsp
dep/glfw/tests/CMakeFiles/opacity.dir/opacity.c.obj: dep/glfw/tests/opacity.c
dep/glfw/tests/CMakeFiles/opacity.dir/opacity.c.obj: dep/glfw/tests/CMakeFiles/opacity.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object dep/glfw/tests/CMakeFiles/opacity.dir/opacity.c.obj"
	cd /d C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\tests && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT dep/glfw/tests/CMakeFiles/opacity.dir/opacity.c.obj -MF CMakeFiles\opacity.dir\opacity.c.obj.d -o CMakeFiles\opacity.dir\opacity.c.obj -c C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\tests\opacity.c

dep/glfw/tests/CMakeFiles/opacity.dir/opacity.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/opacity.dir/opacity.c.i"
	cd /d C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\tests && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\tests\opacity.c > CMakeFiles\opacity.dir\opacity.c.i

dep/glfw/tests/CMakeFiles/opacity.dir/opacity.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/opacity.dir/opacity.c.s"
	cd /d C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\tests && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\tests\opacity.c -o CMakeFiles\opacity.dir\opacity.c.s

dep/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad_gl.c.obj: dep/glfw/tests/CMakeFiles/opacity.dir/flags.make
dep/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad_gl.c.obj: dep/glfw/tests/CMakeFiles/opacity.dir/includes_C.rsp
dep/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad_gl.c.obj: dep/glfw/deps/glad_gl.c
dep/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad_gl.c.obj: dep/glfw/tests/CMakeFiles/opacity.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object dep/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad_gl.c.obj"
	cd /d C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\tests && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT dep/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad_gl.c.obj -MF CMakeFiles\opacity.dir\__\deps\glad_gl.c.obj.d -o CMakeFiles\opacity.dir\__\deps\glad_gl.c.obj -c C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\deps\glad_gl.c

dep/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/opacity.dir/__/deps/glad_gl.c.i"
	cd /d C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\tests && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\deps\glad_gl.c > CMakeFiles\opacity.dir\__\deps\glad_gl.c.i

dep/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/opacity.dir/__/deps/glad_gl.c.s"
	cd /d C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\tests && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\deps\glad_gl.c -o CMakeFiles\opacity.dir\__\deps\glad_gl.c.s

# Object files for target opacity
opacity_OBJECTS = \
"CMakeFiles/opacity.dir/opacity.c.obj" \
"CMakeFiles/opacity.dir/__/deps/glad_gl.c.obj"

# External object files for target opacity
opacity_EXTERNAL_OBJECTS =

dep/glfw/tests/opacity.exe: dep/glfw/tests/CMakeFiles/opacity.dir/opacity.c.obj
dep/glfw/tests/opacity.exe: dep/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad_gl.c.obj
dep/glfw/tests/opacity.exe: dep/glfw/tests/CMakeFiles/opacity.dir/build.make
dep/glfw/tests/opacity.exe: dep/glfw/src/libglfw3.a
dep/glfw/tests/opacity.exe: dep/glfw/tests/CMakeFiles/opacity.dir/linklibs.rsp
dep/glfw/tests/opacity.exe: dep/glfw/tests/CMakeFiles/opacity.dir/objects1.rsp
dep/glfw/tests/opacity.exe: dep/glfw/tests/CMakeFiles/opacity.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable opacity.exe"
	cd /d C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\opacity.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
dep/glfw/tests/CMakeFiles/opacity.dir/build: dep/glfw/tests/opacity.exe
.PHONY : dep/glfw/tests/CMakeFiles/opacity.dir/build

dep/glfw/tests/CMakeFiles/opacity.dir/clean:
	cd /d C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\tests && $(CMAKE_COMMAND) -P CMakeFiles\opacity.dir\cmake_clean.cmake
.PHONY : dep/glfw/tests/CMakeFiles/opacity.dir/clean

dep/glfw/tests/CMakeFiles/opacity.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\tests C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\tests C:\Users\berge\Documents\Cours\igr\TP00-OpenGL\src\dep\glfw\tests\CMakeFiles\opacity.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : dep/glfw/tests/CMakeFiles/opacity.dir/depend

