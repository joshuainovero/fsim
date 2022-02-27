cmake_minimum_required(VERSION 3.16.0)

project(fsim VERSION 0.1.0)

find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

set(APP_FOLDER_TITLE MU_FES)

set(APP_TITLE FireSimulator)
set(APP_LIB_TITLE mufes)

if(WIN32)
	link_directories(${PROJECT_SOURCE_DIR}/vendor/SFML/lib)
endif()

link_directories(${PROJECT_SOURCE_DIR}/vendor/mailio/lib)

include_directories(
	${PROJECT_SOURCE_DIR}/vendor/SFML/include
	${PROJECT_SOURCE_DIR}/vendor/imgui
	${PROJECT_SOURCE_DIR}/vendor/mailio/include
)

add_executable(${APP_TITLE} ${PROJECT_SOURCE_DIR}/src/EntryPoint.cpp)

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

add_library(${APP_LIB_TITLE} SHARED ${project_headers} ${project_sources})

target_link_libraries(${APP_LIB_TITLE} sfml-window sfml-graphics sfml-system)

if(WIN32)
	target_link_libraries(${APP_LIB_TITLE} mailio.dll ws2_32 opengl32)
endif()

if(Boost_FOUND)
	target_include_directories(${APP_LIB_TITLE} PUBLIC ${Boost_INCLUDE_DIR})
endif()

if(OPENSSL_FOUND)
	target_include_directories(${APP_LIB_TITLE} PUBLIC ${OPENSSL_INCLUDE_DIR})
	target_link_libraries(${APP_LIB_TITLE} ${OPENSSL_LIBRARIES})
endif()

target_link_libraries(${APP_TITLE} ${APP_LIB_TITLE})

install(TARGETS ${APP_TITLE} DESTINATION ${APP_FOLDER_TITLE})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/resource/ DESTINATION ${APP_FOLDER_TITLE}/resource)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/floordata/ DESTINATION  ${APP_FOLDER_TITLE}/floordata)

if(WIN32)
	install(FILES ${PROJECT_SOURCE_DIR}/sfml-audio-2.dll DESTINATION ${APP_FOLDER_TITLE})
	install(FILES ${PROJECT_SOURCE_DIR}/sfml-graphics-2.dll DESTINATION ${APP_FOLDER_TITLE})
	install(FILES ${PROJECT_SOURCE_DIR}/sfml-window-2.dll DESTINATION ${APP_FOLDER_TITLE})
	install(FILES ${PROJECT_SOURCE_DIR}/sfml-system-2.dll DESTINATION ${APP_FOLDER_TITLE})
	install(FILES ${PROJECT_SOURCE_DIR}/sfml-network-2.dll DESTINATION ${APP_FOLDER_TITLE})	
endif()

get_target_property(APP_LIB_TYPE ${APP_LIB_TITLE} TYPE)
if(${APP_LIB_TYPE} STREQUAL SHARED_LIBRARY)
	if (WIN32)
		install(FILES ${PROJECT_SOURCE_DIR}/libmailio.dll DESTINATION ${APP_FOLDER_TITLE})
		install(TARGETS ${APP_LIB_TITLE} DESTINATION ${APP_FOLDER_TITLE})
	else()
		install(FILES ${PROJECT_SOURCE_DIR}/libmailio.so DESTINATION ${APP_FOLDER_TITLE})
		install(TARGETS ${APP_LIB_TITLE} DESTINATION ${APP_FOLDER_TITLE})
	endif()
endif()