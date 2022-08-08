
cmake_minimum_required(VERSION 3.12)


project(assignment_1)


find_package(OpenGL REQUIRED)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)



add_subdirectory(external/glfw-3.3.2)


# (https://github.com/Perlmint/glew-cmake)
add_subdirectory(external/glew-cmake-2.1.0)


add_subdirectory(external/glm-0.9.9.8)



# Adds the levels folder to the bin directory, making the content available for our program to read
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/levels DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Adds the shader folder to the bin directory, making its content available for out program to read.
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Adds the resource folder to the bin directory, making the content available for our program to read
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)



add_executable(assignment_1
	main.cpp
	src/ScenarioLoader.h
	src/ScenarioLoader.cpp   
	src/Shader.h
	src/Shader.cpp
	src/VertexBuffer.h
	src/VertexBuffer.cpp
	src/IndexBuffer.h
	src/IndexBuffer.cpp 
	src/VertexArray.h 
	src/VertexArray.cpp
	src/VertexBufferLayout.h 
	src/Renderer.h
	src/Renderer.cpp 
	src/Maze.h
	src/Maze.cpp 
	src/MovableObject.h
	src/MovableObject.cpp
	src/Pacman.cpp
	src/Pacman.h
	src/Ghost.h
	src/Ghost.cpp
	src/Pellets.h
	src/Pellets.cpp 
	src/Texture.h
	src/Texture.cpp "src/Animator.h" "src/Animator.cpp")


target_compile_definitions(assignment_1 PRIVATE GLEW_STATIC)

target_link_libraries(assignment_1
  PRIVATE
  libglew_static
  glfw
  glm
  OpenGL::GL)
