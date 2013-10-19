cmake_minimum_required(VERSION 2.8)
#set(CMAKE_VERBOSE_MAKEFILE On)
project(RTE2DVolumeIntegralSolver)
enable_language(Fortran)
add_subdirectory("src" "bin")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/../install)
install(PROGRAMS "build/bin/main" DESTINATION "bin")
install(PROGRAMS "build/bin/librte2dvis.dylib" DESTINATION "lib")
install(DIRECTORY "doc/" DESTINATION "doc")

#install(TARGETS "bin/main" "bin/librte2dvis.dylib"
	#RUNTIME DESTINATION bin
	#LIBRARY DESTINATION lib
	#ARCHIVE DESTINATION lib
	#)

message("This project is built in\n\t${PROJECT_BINARY_DIR}")
