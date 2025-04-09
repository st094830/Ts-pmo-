# CMakeList.txt : CMake project for Pagani, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Pagani "Pagani.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Pagani PROPERTY CXX_STANDARD 20)
endif()
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
# TODO: Add tests and install targets if needed.
