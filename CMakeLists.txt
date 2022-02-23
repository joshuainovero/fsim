cmake_minimum_required(VERSION 3.16.0)

project(fsim)

find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

set(APP_FOLDER_TITLE MU_FES)

link_directories(
	${PROJECT_SOURCE_DIR}/vendor/SFML/lib
	${PROJECT_SOURCE_DIR}/vendor/mailio/lib
)

include_directories(
	${PROJECT_SOURCE_DIR}/vendor/SFML/include
	${PROJECT_SOURCE_DIR}/vendor/imgui
	${PROJECT_SOURCE_DIR}/vendor/mailio/include
	${Boost_INCLUDE_DIR}
	${OPENSSL_INCLUDE_DIR}
)

add_executable(fsim ${PROJECT_SOURCE_DIR}/src/EntryPoint.cpp)

set(project_sources
	${PROJECT_SOURCE_DIR}/src/Algorithms.cpp
	${PROJECT_SOURCE_DIR}/src/Controller.cpp
	${PROJECT_SOURCE_DIR}/src/FireGraphics.cpp
	${PROJECT_SOURCE_DIR}/src/Floormap.cpp
	${PROJECT_SOURCE_DIR}/src/Node.cpp
	${PROJECT_SOURCE_DIR}/src//StartingPoints.cpp
	${PROJECT_SOURCE_DIR}/src/Units.cpp
	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui.cpp
	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp
	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui-SFML.cpp
)

set(project_headers
	${PROJECT_SOURCE_DIR}/src/Algorithms.hpp
	${PROJECT_SOURCE_DIR}/src/Constants.hpp
	${PROJECT_SOURCE_DIR}/src/Controller.hpp
	${PROJECT_SOURCE_DIR}/src/FireGraphics.hpp
	${PROJECT_SOURCE_DIR}/src/Floormap.hpp
	${PROJECT_SOURCE_DIR}/src/Node.hpp
	${PROJECT_SOURCE_DIR}/src//Results.hpp
	${PROJECT_SOURCE_DIR}/src/StartingPoints.hpp
	${PROJECT_SOURCE_DIR}/src/Units.hpp
)

add_library(fes_lib ${project_headers} ${project_sources})

target_link_libraries(
	${PROJECT_NAME}
	fes_lib
	mailio.dll
	sfml-window
	sfml-graphics
	sfml-system
	wsock32 gdi32
	opengl32
	${OPENSSL_LIBRARIES}
)

install(TARGETS fsim DESTINATION ${APP_FOLDER_TITLE})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/resource/ DESTINATION ${APP_FOLDER_TITLE}/resource)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/floordata/ DESTINATION  ${APP_FOLDER_TITLE}/floordata)

install(FILES ${PROJECT_SOURCE_DIR}/libmailio.dll DESTINATION ${APP_FOLDER_TITLE})
install(FILES ${PROJECT_SOURCE_DIR}/sfml-audio-2.dll DESTINATION ${APP_FOLDER_TITLE})
install(FILES ${PROJECT_SOURCE_DIR}/sfml-graphics-2.dll DESTINATION ${APP_FOLDER_TITLE})
install(FILES ${PROJECT_SOURCE_DIR}/sfml-window-2.dll DESTINATION ${APP_FOLDER_TITLE})
install(FILES ${PROJECT_SOURCE_DIR}/sfml-system-2.dll DESTINATION ${APP_FOLDER_TITLE})
install(FILES ${PROJECT_SOURCE_DIR}/sfml-network-2.dll DESTINATION ${APP_FOLDER_TITLE})