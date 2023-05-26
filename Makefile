# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/eniem/IMAC/S2/prog-algo2/Voronoi_Delaunay

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eniem/IMAC/S2/prog-algo2/Voronoi_Delaunay

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/eniem/IMAC/S2/prog-algo2/Voronoi_Delaunay/CMakeFiles /home/eniem/IMAC/S2/prog-algo2/Voronoi_Delaunay//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/eniem/IMAC/S2/prog-algo2/Voronoi_Delaunay/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -P /home/eniem/IMAC/S2/prog-algo2/Voronoi_Delaunay/CMakeFiles/VerifyGlobs.cmake
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named VoronoiApp

# Build rule for target.
VoronoiApp: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 VoronoiApp
.PHONY : VoronoiApp

# fast build rule for target.
VoronoiApp/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/VoronoiApp.dir/build.make CMakeFiles/VoronoiApp.dir/build
.PHONY : VoronoiApp/fast

src/SDL2_gfxPrimitives.o: src/SDL2_gfxPrimitives.c.o
.PHONY : src/SDL2_gfxPrimitives.o

# target to build an object file
src/SDL2_gfxPrimitives.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/VoronoiApp.dir/build.make CMakeFiles/VoronoiApp.dir/src/SDL2_gfxPrimitives.c.o
.PHONY : src/SDL2_gfxPrimitives.c.o

src/SDL2_gfxPrimitives.i: src/SDL2_gfxPrimitives.c.i
.PHONY : src/SDL2_gfxPrimitives.i

# target to preprocess a source file
src/SDL2_gfxPrimitives.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/VoronoiApp.dir/build.make CMakeFiles/VoronoiApp.dir/src/SDL2_gfxPrimitives.c.i
.PHONY : src/SDL2_gfxPrimitives.c.i

src/SDL2_gfxPrimitives.s: src/SDL2_gfxPrimitives.c.s
.PHONY : src/SDL2_gfxPrimitives.s

# target to generate assembly for a file
src/SDL2_gfxPrimitives.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/VoronoiApp.dir/build.make CMakeFiles/VoronoiApp.dir/src/SDL2_gfxPrimitives.c.s
.PHONY : src/SDL2_gfxPrimitives.c.s

src/application_ui.o: src/application_ui.cpp.o
.PHONY : src/application_ui.o

# target to build an object file
src/application_ui.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/VoronoiApp.dir/build.make CMakeFiles/VoronoiApp.dir/src/application_ui.cpp.o
.PHONY : src/application_ui.cpp.o

src/application_ui.i: src/application_ui.cpp.i
.PHONY : src/application_ui.i

# target to preprocess a source file
src/application_ui.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/VoronoiApp.dir/build.make CMakeFiles/VoronoiApp.dir/src/application_ui.cpp.i
.PHONY : src/application_ui.cpp.i

src/application_ui.s: src/application_ui.cpp.s
.PHONY : src/application_ui.s

# target to generate assembly for a file
src/application_ui.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/VoronoiApp.dir/build.make CMakeFiles/VoronoiApp.dir/src/application_ui.cpp.s
.PHONY : src/application_ui.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/VoronoiApp.dir/build.make CMakeFiles/VoronoiApp.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/VoronoiApp.dir/build.make CMakeFiles/VoronoiApp.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/VoronoiApp.dir/build.make CMakeFiles/VoronoiApp.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... VoronoiApp"
	@echo "... src/SDL2_gfxPrimitives.o"
	@echo "... src/SDL2_gfxPrimitives.i"
	@echo "... src/SDL2_gfxPrimitives.s"
	@echo "... src/application_ui.o"
	@echo "... src/application_ui.i"
	@echo "... src/application_ui.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -P /home/eniem/IMAC/S2/prog-algo2/Voronoi_Delaunay/CMakeFiles/VerifyGlobs.cmake
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

