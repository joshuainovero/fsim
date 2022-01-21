#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui-SFML.h"
#include "Map.hpp"
#include "Controller.hpp"
#include "Algorithms.hpp"
#include <iostream>

static bool mouseOnImGui = false;

static bool showDefaultPaths = true;

int main()
{
    auto videoMode = sf::VideoMode::getDesktopMode();
    videoMode.height += 1;
    sf::RenderWindow window(videoMode, "Window", sf::Style::None);
    ImGui::SFML::Init(window);
    sf::Vector2f ImGuiWindowSize((float)((float)300.0f/768.0f) * (videoMode.height - 1), videoMode.height - 1);

    fsim::Map map(400, "resource/floor12160.png", "MapData/floor1", &window);
    sf::Clock deltaClock;
    static int e = 1;
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
                fsim::Controller::zoomEvent(event.mouseWheel.delta, map.mapView, &window);
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());



        fsim::Controller::keyboardEvent(map.mapView, &window);

        window.clear(sf::Color::White);

        window.draw(map.mapSprite);



        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            sf::Vector2u position = map.clickPosition(worldPos);
            map.nodes[position.x][position.y]->reset();
            map.initVertexArray();
        }

        window.draw(*map.nodePositions);

        ImGui::SetNextWindowSize(ImVec2(ImGuiWindowSize.x, ImGuiWindowSize.y), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::StyleColorsDark();

        // ImGui::ShowDemoWindow();
        bool* p_open = nullptr;
        ImGui::Begin("Debugger - Thesis", p_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::Text("Actions");
        ImGui::RadioButton("Place Path", &e, 0);  ImGui::SameLine(); 
        ImGui::RadioButton("Navigate", &e, 1); ImGui::SameLine();
        ImGui::RadioButton("Set Start", &e, 2);
        ImGui::RadioButton("Set Target", &e, 3);
        if (ImGui::Button("Visualize"))
        {
            fsim::Algorithms::astar(map.getStart(), map.getTarget(), map.nodes, map.getTotalRows(), map.getTotalCols());
            map.initVertexArray();
        }
        ImGui::End();  


        ImGui::SFML::Render(window);

        window.display();

        if (mouseOnImGui && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            mouseOnImGui = false;
            
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        {
            for (size_t i = 0; i < map.getTotalRows(); ++i)
            {
                for (auto node : map.nodes[i])
                {
                    if (node->type == fsim::NODETYPE::DefaultPath && (map.getStart() != node && map.getTarget() != node))
                        node->switchColor(sf::Color(0.0f, 0.0f, 0.0f, 0.0f));
                }
            }
            map.initVertexArray();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
        {
            for (size_t i = 0; i < map.getTotalRows(); ++i)
            {
                for (auto node : map.nodes[i])
                {
                    if (node->type == fsim::NODETYPE::DefaultPath && (map.getTarget() != node && map.getStart() != node))
                        node->switchColor(sf::Color::Blue);
                }
            }
            map.initVertexArray();
        }
        if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
        {
            if (!ImGui::IsAnyItemHovered() && !mouseOnImGui){
                if (e == 0){
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        sf::Vector2u position = map.clickPosition(worldPos);
                        map.nodes[position.x][position.y]->setDefaultPath();
                        map.initVertexArray();
                    }

                }

                else if (e == 1){
                    fsim::Controller::dragEvent(map.mapView, &window);
                }
                else if (e == 2)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        sf::Vector2u position = map.clickPosition(worldPos);
                        if (map.nodes[position.x][position.y]->type == fsim::NODETYPE::DefaultPath){
                            map.nodes[position.x][position.y]->setStart();
                            map.setStart(map.nodes[position.x][position.y]);
                            map.initVertexArray();
                        }
                    }
                }

                else if (e ==3)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        sf::Vector2u position = map.clickPosition(worldPos);
                        if (map.nodes[position.x][position.y]->type == fsim::NODETYPE::DefaultPath){
                            map.nodes[position.x][position.y]->setTarget();
                            map.setTarget(map.nodes[position.x][position.y]);
                            map.initVertexArray();
                        }
                    }
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

    }

    ImGui::SFML::Shutdown();
    return 0;
}