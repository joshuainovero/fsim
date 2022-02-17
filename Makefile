# Makefile for windows

CC := g++
CFLAGS := -Wall

# Objects
DEBUG := Debugger.o
RELEASE := EntryPoint.o
SRCOBJS := Node.o Floormap.o Algorithms.o Controller.o StartingPoints.o FireGraphics.o Units.o
IMGUIOBJS := imgui.o imgui_draw.o imgui_tables.o imgui_widgets.o imgui-SFML.o
DEBUGOBJS := ${DEBUG} ${SRCOBJS} ${IMGUIOBJS}
RELEASEOBJS := ${RELEASE} ${SRCOBJS} ${IMGUIOBJS}

# Dependencies and Paths
INCLUDES := -Ivendor/SFML/include -Ivendor/imgui
LIBS := -Lvendor/SFML/lib -lsfml-window -lsfml-graphics -lsfml-system -lopengl32
SRC := src
IMGUISRC := vendor/imgui

# Output
TARGET := Simulator

debug: DebugInit clean
release: ReleaseInit clean

# Debug Target
DebugInit: ${DEBUGOBJS}
	${CC} ${CFLAGS} $^ ${LIBS}

# Release Target
ReleaseInit: ${RELEASEOBJS}
	${CC} ${CFLAGS} $^ ${LIBS}

# src
Debugger.o: ${SRC}/Debugger.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<

EntryPoint.o: ${SRC}/EntryPoint.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<

Node.o: ${SRC}/Node.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<

Floormap.o: ${SRC}/Floormap.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<

Algorithms.o: ${SRC}/Algorithms.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<

Controller.o: ${SRC}/Controller.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<

StartingPoints.o: ${SRC}/StartingPoints.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<

FireGraphics.o: ${SRC}/FireGraphics.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<

Units.o: ${SRC}/Units.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<

# ImGui
imgui.o: ${IMGUISRC}/imgui.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<

imgui_demo.o: ${IMGUISRC}/imgui_demo.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<

imgui_draw.o: ${IMGUISRC}/imgui_draw.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<

imgui_tables.o: ${IMGUISRC}/imgui_tables.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<

imgui_widgets.o: ${IMGUISRC}/imgui_widgets.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<

imgui-SFML.o: ${IMGUISRC}/imgui-SFML.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $<
	

# Clean objs
clean:
	del *.o