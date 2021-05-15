# Top-Level CMakeList.txt

cmake_minimum_required (VERSION 2.6)
project (BasicRaytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find_package(SDL2 REQUIRED)
# find_package(OpenGL REQUIRED)
# find_package(GLEW REQUIRED) # sudo apt-get install libglew-dev

set(MY_SOURCE_FILES
	src/main.cpp
    src/Material.h
    src/Material.cpp
    src/Air.h
    src/Flat.cpp
    src/Flat.h
    src/Glass.h
    src/Glass.cpp
    src/Ray.h
    src/Ray.cpp
    src/Camera.cpp
    src/Camera.h
    src/Light.h
    src/PointLight.h
    src/PointLight.cpp
    src/Scene.cpp
    src/Scene.h
    src/Renderable.h
    src/Sphere.h
    src/Sphere.cpp
    src/Intersection.h
    src/Intersection.cpp
    src/Raytracer.h
    src/Raytracer.cpp
    src/Shiny.h
    src/Shiny.cpp
    src/Triangle.h
    src/Triangle.cpp
    src/Mirror.h
    src/Mirror.cpp
    # src/Texture.cpp
    # src/Texture.h
    src/TriangleMesh.h
    src/Cube.h
    src/Cube.cpp
    src/CornellBoxScene.h
    src/CornellBoxScene.cpp
    src/GlassTestScene.h
    src/RectangleLight.h
    src/RectangleLight.cpp
    # src/Ray.cpp
    # src/Sphere.cpp
    # src/Scene.cpp
    # src/Renderer.cpp
    # src/Triangle.cpp
)



include_directories(
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/libs
)
add_executable(BasicRaytracer
	${MY_SOURCE_FILES}
)
#link_libraries(${SDL_LIBRARY})
# target_link_libraries(BasicRaytracer ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${GLEW_LIBRARIES})

