# CMakeList.txt : CMake project for Prototypes, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (Dawn "src/Dawn.h" "src/dwpch.h" "src/dwpch.cpp" "src/Dawn/Application.h" "src/Dawn/Application.cpp" "src/Dawn/EntryPoint.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Dawn PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
