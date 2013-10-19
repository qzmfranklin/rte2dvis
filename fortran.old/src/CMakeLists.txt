add_library(rte2dvis SHARED zdotx2.f90 inputs.f90 cmd.f90 global.f90)
add_executable(main main.f90)
target_link_libraries(main rte2dvis)
