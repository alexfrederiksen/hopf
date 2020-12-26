find_package(glfw3 3.3 REQUIRED)

add_executable(manifold main.cpp App.cpp Mesh.cpp Hopf.cpp)

target_link_libraries(manifold glfw epoxy)
# include_directories(${CMAKE_SOURCE_DIR}/include)
