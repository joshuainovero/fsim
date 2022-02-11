#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui-SFML.h"
#include "Floormap.hpp"
#include "Controller.hpp"
#include "Algorithms.hpp"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <utility>
#include <math.h>
// 400 DPI
// Then PhotoShop 3840 x 2160 300 DPI
// Black and white -50% all

// enum FloorLabel { GROUND = 0, SECOND = 1, THIRD = 2, FOURTH = 3 };
FloorLabel currentEnumFloor = FloorLabel::GROUND;

const std::vector<std::string> mapTexturePaths =
    { "resource/Ground-2160.png", "resource/2nd-2160.png", "resource/3rd-2160.png", "resource/4th-2160.png" };

const std::vector<std::string> mapDataPaths = 
    { "floordata/ground-level.map", "floordata/2nd-level.map", "floordata/3rd-level.map", "floordata/4th-level.map" };

const std::vector<std::pair<uint32_t, std::string>> FloorLevelStrings =
    { std::make_pair(1, "(Ground)"), std::make_pair(2, "(2nd)"), std::make_pair(3, "(3rd)"), std::make_pair(4, "(4th)") };


std::vector<sf::RectangleShape> pixels;

sf::Texture* currentMapTexture = nullptr;

static bool mouseOnImGui = false;

static bool mouseDown = false;

static bool mouseDownDebug = false;

static bool showDefaultPaths = false;
static bool showDefaultPathTriggered = false;

static std::vector<std::pair<fsim::Node*, uint32_t>> exitsStored;

static void loadMapTexture(fsim::Floormap& map, const FloorLabel& floor)
{
    if (currentMapTexture != nullptr)
        delete currentMapTexture;
    
    currentMapTexture = new sf::Texture();
    currentMapTexture->loadFromFile(mapTexturePaths[floor]);
    currentMapTexture->setSmooth(true);
    map.setMapTexture(currentMapTexture);
}

int main()
{
    auto videoMode = sf::VideoMode::getDesktopMode();
    videoMode.height += 1;
    // sf::ContextSettings settings;
    // settings.antialiasingLevel = 1;
    sf::RenderWindow window(videoMode, "Window", sf::Style::None);
    window.setFramerateLimit(120);
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(200.0f, 200.0f));
    shape.setFillColor(sf::Color::Green);

    bool imGuiInit = ImGui::SFML::Init(window);
    if (imGuiInit)
        std::cout << "ImGui successs" << std::endl;
    sf::Vector2f ImGuiWindowSize(330.0f, videoMode.height);

    fsim::Floormap map(400, mapDataPaths[currentEnumFloor], &window, currentEnumFloor);
    loadMapTexture(map, currentEnumFloor);
    const char* LabelLevelCStr = FloorLevelStrings[currentEnumFloor].second.c_str();
    char ImGuiTitle[] = "Debugger - Floor: ";
    strcat(ImGuiTitle, LabelLevelCStr);

    sf::CircleShape point;
    point.setRadius(4.0f);
    point.setFillColor(sf::Color(0.0f, 200.0f, 0.0f, 255.0f));
    point.setOrigin(point.getRadius(), point.getRadius());

    sf::Clock deltaClock;
    static int e = 1;

    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;

    float deltaTime = 0.0f;
    float fps;

    while (window.isOpen())
    {

        window.setView(map.mapView);

        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseWheelMoved)
            {
                fsim::Controller::zoomEvent(event.mouseWheel.delta, map.mapView, &window, map.mouseValue);
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        fsim::Controller::keyboardEvent(map.mapView, &window);

        start = std::chrono::high_resolution_clock::now();

        window.clear(sf::Color::White);

        map.drawMap(&window);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            sf::Vector2u position = map.clickPosition(worldPos);
            if (map.nodes[position.x][position.y]->type != fsim::NODETYPE::None)
            {
                map.nodes[position.x][position.y]->reset();
                auto it = std::find(map.exitNodes.begin(), map.exitNodes.end(), map.nodes[position.x][position.y]);
                if (it != map.exitNodes.end())
                    map.exitNodes.erase(it);
                map.initVertexArray();
                map.totalNodesGenerated -= 1;
            }

        }

        window.draw(*map.nodePositions);
        for (const auto& pixel: pixels)
        {
            window.draw(pixel);
        }
        // window.draw(shape);

        ImGui::SetNextWindowSize(ImVec2(ImGuiWindowSize.x, ImGuiWindowSize.y), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::StyleColorsDark();

        // ImGui::ShowDemoWindow();
        bool* p_open = nullptr;
        ImGui::Begin(ImGuiTitle, p_open,  ImGuiWindowFlags_NoMove);
        ImGui::Text("Camera View");
        ImGui::RadioButton("Navigate", &e, 1);
        ImGui::Text("FPS: %.2f", fps);
        ImGui::Text("XYZ: %.3f / %.3f / 0.000", map.mapView.getCenter().x, map.mapView.getCenter().y);
        ImGui::Text("Zoom: %d%%", (int)((((float)map.mouseValue) / (fsim::Controller::zoomValues.size() - 1.0f)) * 100.0f)); 
        ImGui::Separator();
        ImGui::Text("Map Editor");
        ImGui::RadioButton("Place Path", &e, 0);  
        ImGui::RadioButton("Place Target/Exit", &e, 4);
        ImGui::Checkbox("Show default paths", &showDefaultPaths);
        ImGui::Text("Total nodes: %u", map.totalNodesGenerated);
        ImGui::Text("Exit nodes: %llu", map.exitNodes.size());
        if (ImGui::Button("Save Changes"))
        {
            map.saveChanges();
        }
        ImGui::Separator();
        ImGui::Text("Tests");
        ImGui::RadioButton("Set Start", &e, 2); 
        ImGui::RadioButton("Place fire", &e, 500);
        ImGui::RadioButton("Place 1 pixel", &e, 100);
        // ImGui::RadioButton("Set Target", &e, 3); 


        if (ImGui::Button("Visualize"))
        {
            exitsStored.clear();
            if (map.exitNodes.size() != 0)
            {
                for (size_t i = 0; i < map.getTotalRows(); ++i)
                {
                    for (auto node : map.nodes[i])
                    {
                        if (node != nullptr)
                            node->updateNeighbors(map.nodes, map.minCols, map.maxCols);

                    }
                }

                auto previous_nodes =  fsim::Algorithms::dijkstra(map.getStart(), nullptr, map.nodes, map.getTotalRows(), std::make_pair(map.minCols, map.maxCols), false);

                for (auto exitNode : map.exitNodes)
                {
                    if (previous_nodes.find(exitNode) == previous_nodes.end())
                        continue;
                        
                    uint32_t nodeCount = fsim::Algorithms::reconstruct_path(exitNode, map.getStart(), previous_nodes, false);
                    exitsStored.push_back(std::make_pair(exitNode, nodeCount));

                }
                auto minExitNode = *std::min_element(exitsStored.begin(), exitsStored.end(), [](auto &left, auto &right) {
                                    return left.second < right.second;});
                uint32_t finalCount = fsim::Algorithms::reconstruct_path(minExitNode.first, map.getStart(), previous_nodes, true);

                map.initVertexArray();
            }
        }
        ImGui::End();  


        ImGui::SFML::Render(window);

        if (map.getStart() != nullptr)
            window.draw(point);

        window.display();

        if (mouseOnImGui && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            mouseOnImGui = false;
            

        if (showDefaultPaths)
        {
            if (!showDefaultPathTriggered)
            {
                for (size_t i = 0; i < map.getTotalRows(); ++i)
                {
                    for (auto node : map.nodes[i])
                    {
                        if (node != nullptr)
                        {
                            if (node->type == fsim::NODETYPE::DefaultPath && !node->exit && (map.getTarget() != node && map.getStart() != node))
                                node->switchColor(sf::Color::Blue);

                            if (node->type == fsim::NODETYPE::DefaultPath && node->exit && (map.getTarget() != node && map.getStart() != node))
                                node->switchColor(sf::Color::Red);
                        }

                    }
                }

                map.initVertexArray();
                showDefaultPathTriggered = true;
            }
        }
        else
        {
            if (showDefaultPathTriggered)
            {
                for (size_t i = 0; i < map.getTotalRows(); ++i)
                {
                    for (auto node : map.nodes[i])
                    {
                        if (node != nullptr)
                        {
                            if (node->type == fsim::NODETYPE::DefaultPath && (map.getStart() != node && map.getTarget() != node))
                                node->switchColor(sf::Color(0.0f, 0.0f, 0.0f, 0.0f));
                        }
                    }
                }

                map.initVertexArray();
                showDefaultPathTriggered = false;
            }
        }


        if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
        {
            if (!ImGui::IsAnyItemHovered() && !mouseOnImGui){
                if (e == 0){
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        sf::Vector2u position = map.clickPosition(worldPos);
                        if (map.nodes[position.x][position.y]->type == fsim::NODETYPE::None)
                        {
                            map.nodes[position.x][position.y]->setDefaultPath();
                            if(showDefaultPaths)
                                map.nodes[position.x][position.y]->switchColor(sf::Color::Blue);
                            map.initVertexArray();
                            map.totalNodesGenerated += 1;
                        }

                    }

                }

                else if (e == 1){
                    fsim::Controller::dragEvent(map.mapView, &window);
                }
                else if (e == 2)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if (!mouseDown){
                            for (size_t i = 0; i < map.getTotalRows(); ++i)
                            {
                                for (auto node : map.nodes[i])
                                {
                                    if (node != nullptr)
                                        node->switchColor(sf::Color(0.0f, 0.0f, 0.0f, 0.0f));
                                }
                            }
                            sf::Vector2u position = map.clickPosition(worldPos);
                            fsim::Node* selectedNode = map.nodes[position.x][position.y];
                            fsim::Node* calculatedSelectedNode = fsim::Algorithms::bfsGetNearestStart(selectedNode, map.nodes, map.getTotalRows(), map.getTotalCols());
                            calculatedSelectedNode->setStart();
                            map.setStart(calculatedSelectedNode);
                            map.initVertexArray();
                            point.setPosition(sf::Vector2f(map.getStart()->getWorldPos().x + (map.getStart()->getTileSize()/2.0f), 
                            map.getStart()->getWorldPos().y + (map.getStart()->getTileSize()/2.0f)));
                            showDefaultPaths = false;
                            mouseDown = true;
                        } 
                    }
                    else mouseDown = false;

                }

                else if (e ==3)
                {
                    // if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    // {
                    //     sf::Vector2u position = map.clickPosition(worldPos);
                    //     if (map.nodes[position.x][position.y]->type == fsim::NODETYPE::DefaultPath){
                    //         map.nodes[position.x][position.y]->setTarget();
                    //         map.setTarget(map.nodes[position.x][position.y]);
                    //         map.initVertexArray();
                    //     }
                    // }
                }
                else if (e == 4)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        sf::Vector2u position = map.clickPosition(worldPos);
                        if (map.nodes[position.x][position.y]->type != fsim::NODETYPE::None && !map.nodes[position.x][position.y]->exit){
                            map.nodes[position.x][position.y]->setDefaultExit();
                            map.nodes[position.x][position.y]->exit = true;
                            map.exitNodes.push_back(map.nodes[position.x][position.y]);

                            if(showDefaultPaths)
                                map.nodes[position.x][position.y]->switchColor(sf::Color::Red);

                            map.initVertexArray();
                        }  
                    }
                }
                else if (e == 100)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if (!mouseDown)
                        {
                            pixels.push_back(sf::RectangleShape());
                            pixels[pixels.size() - 1].setSize(sf::Vector2f(1.0f, 1.0f));
                            pixels[pixels.size() - 1].setFillColor(sf::Color::Green);
                            pixels[pixels.size() - 1].setPosition(sf::Vector2f(worldPos.x, worldPos.y));
                            mouseDown = true;
                        }
                    }
                    else
                        mouseDown = false;
                }
                else if (e == 500)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if (!mouseDown)
                        {
                            sf::Vector2u position = map.clickPosition(worldPos);
                            fsim::Node* selectedNode = map.nodes[position.x][position.y];
                            selectedNode->switchColor(sf::Color(255, 70, 0, 255.0f));
                            for (size_t i = 0; i < map.getTotalRows(); ++i)
                            {
                                for (size_t k = map.minCols; k < map.maxCols; ++k)
                                {
                                    if (map.nodes[i][k] != selectedNode)
                                    {
                                        float distance = std::sqrt(std::pow((float)selectedNode->col - (float)map.nodes[i][k]->col, (float)2.0f) + std::pow((float)selectedNode->row - (float)map.nodes[i][k]->row, (float)2.0f));
                                        if (distance * 0.5544342178f <= 9.144f)
                                        {
                                            map.nodes[i][k]->switchColor(sf::Color(255,115, 0, 90.0f));
                                            map.nodes[i][k]->obstruction = true;
                                            
                                        }
                                    }

                                }
                            }
                            map.initVertexArray();
                            mouseDown = true;
                        }
                    } else mouseDown = false;
                }
            }
            else
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    mouseOnImGui = true;
                }
            }
        }


        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (!mouseDownDebug)
            {   
                sf::Vector2u position = map.clickPosition(worldPos);
                if (map.nodes[position.x][position.y] != nullptr)
                {
                    std::cout << position.x << " " << position.y << std::endl;
                    // map.nodes[position.x][position.y]->switchColor(sf::Color::Magenta);
                    map.initVertexArray();
                }
                mouseDownDebug = true;  
            }
        }
        else
            mouseDownDebug = false;


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        end = std::chrono::high_resolution_clock::now();
        float timeElapsed = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();

        if (deltaTime >= 0.5f)
        {
            fps = (float)1e9/timeElapsed;
            deltaTime = 0.0f;
        }
        deltaTime += deltaClock.getElapsedTime().asSeconds();
    }

    ImGui::SFML::Shutdown();
    return 0;
}