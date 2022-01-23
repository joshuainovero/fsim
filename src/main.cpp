#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "Controller.hpp"
#include "Algorithms.hpp"
#include "StartingPoints.hpp"
#include <iostream>
#include <algorithm>
#include <string>

enum modifyPointState { CREATE, MODIFY, NONE};


static bool checkBoxDefault = false;

static int radioButtonDefault = 1;

static modifyPointState modalModify = modifyPointState::NONE;

static bool mouseOnImGui = false;

static bool mouseDown = false;


// States
static bool enableWASD = false;

static bool startPointMoving = false;

static int screenClickHandle = 0;

static std::vector<fsim::StartingPoints*> startingPoints;

fsim::StartingPoints* startingPointTemp = nullptr;

typedef void (*ImGuiDemoMarkerCallback)(const char* file, int line, const char* section, void* user_data);
extern ImGuiDemoMarkerCallback  GImGuiDemoMarkerCallback;
extern void*                    GImGuiDemoMarkerCallbackUserData;
ImGuiDemoMarkerCallback         GImGuiDemoMarkerCallback = NULL;
void*                           GImGuiDemoMarkerCallbackUserData = NULL;
#define IMGUI_DEMO_MARKER(section)  do { if (GImGuiDemoMarkerCallback != NULL) GImGuiDemoMarkerCallback(__FILE__, __LINE__, section, GImGuiDemoMarkerCallbackUserData); } while (0)


static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

static bool alpha_preview = true;
static bool alpha_half_preview = false;
static bool drag_and_drop = true;
static bool options_menu = true;
static bool hdr = false;

ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

static std::vector<std::pair<fsim::Node*, uint32_t>> exitsStored;

static void displayModifyModal(const modifyPointState& state)
{
    if (state == modifyPointState::CREATE)
        ImGui::OpenPopup("Add Starting Point");
    if (ImGui::BeginPopupModal("Add Starting Point", NULL, ImGuiWindowFlags_Modal))
    {
        if (ImGui::Button("Close"))
        {
            if (modalModify == modifyPointState::CREATE)
            {
                delete startingPointTemp;
                startingPointTemp = nullptr;
                std::cout << "Cancelled" << std::endl;
            }
            modalModify = modifyPointState::NONE;
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return;
        }
        if (startingPointTemp == nullptr)
        {
            startingPointTemp = new fsim::StartingPoints();
            std::cout << "Created" << std::endl;
        }
        ImGui::InputText("Input Label", startingPointTemp->buffer, 7); ImGui::SameLine();

        static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

        IMGUI_DEMO_MARKER("Widgets/Color/ColorPicker");
        ImGui::Text("Color picker:");
        static bool alpha = true;
        static bool alpha_bar = true;
        static bool side_preview = true;
        static bool ref_color = false;
        static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
        static int display_mode = 0;
        static int picker_mode = 0;
        ImGui::Checkbox("With Alpha", &alpha);
        ImGui::Checkbox("With Alpha Bar", &alpha_bar);
        ImGui::Checkbox("With Side Preview", &side_preview);
        if (side_preview)
        {
            ImGui::SameLine();
            ImGui::Checkbox("With Ref Color", &ref_color);
            if (ref_color)
            {
                ImGui::SameLine();
                ImGui::ColorEdit4("##RefColor", &ref_color_v.x, ImGuiColorEditFlags_NoInputs | misc_flags);
            }
        }
        ImGui::Combo("Display Mode", &display_mode, "Auto/Current\0None\0RGB Only\0HSV Only\0Hex Only\0");
        ImGui::SameLine(); HelpMarker(
            "ColorEdit defaults to displaying RGB inputs if you don't specify a display mode, "
            "but the user can change it with a right-click.\n\nColorPicker defaults to displaying RGB+HSV+Hex "
            "if you don't specify a display mode.\n\nYou can change the defaults using SetColorEditOptions().");
        ImGui::Combo("Picker Mode", &picker_mode, "Auto/Current\0Hue bar + SV rect\0Hue wheel + SV triangle\0");
        ImGui::SameLine(); HelpMarker("User can right-click the picker to change mode.");
        ImGuiColorEditFlags flags = misc_flags;
        if (!alpha)            flags |= ImGuiColorEditFlags_NoAlpha;        // This is by default if you call ColorPicker3() instead of ColorPicker4()
        if (alpha_bar)         flags |= ImGuiColorEditFlags_AlphaBar;
        if (!side_preview)     flags |= ImGuiColorEditFlags_NoSidePreview;
        if (picker_mode == 1)  flags |= ImGuiColorEditFlags_PickerHueBar;
        if (picker_mode == 2)  flags |= ImGuiColorEditFlags_PickerHueWheel;
        if (display_mode == 1) flags |= ImGuiColorEditFlags_NoInputs;       // Disable all RGB/HSV/Hex displays
        if (display_mode == 2) flags |= ImGuiColorEditFlags_DisplayRGB;     // Override display mode
        if (display_mode == 3) flags |= ImGuiColorEditFlags_DisplayHSV;
        if (display_mode == 4) flags |= ImGuiColorEditFlags_DisplayHex;

        ImGui::ColorPicker4("MyFUCKINGCOLOR##4", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL);
        
        if (ImGui::Button("Save"))
        {
            modalModify = modifyPointState::NONE;
            std::cout << "Saved" << std::endl;
            startingPointTemp->point_rgba = color;
            startingPoints.push_back(startingPointTemp);
            startingPointTemp = nullptr;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

int main()
{
    auto videoMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(videoMode, "Window", sf::Style::Fullscreen);
    bool imGuiInit = ImGui::SFML::Init(window);
    if (imGuiInit)
        std::cout << "ImGui Success!" << std::endl;
        ImGui::GetStyle().Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

    sf::Vector2f ImGuiWindowSize((float)((float)300.0f/768.0f) * (videoMode.height), videoMode.height);

    fsim::Map map(400, "resource/floor12160.png", "MapData/floor1", &window);
    sf::Clock deltaClock;
    while (window.isOpen())
    {
        window.setView(map.mapView);
        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Event event;
        while(window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseWheelMoved)
            {
                fsim::Controller::zoomEvent(event.mouseWheel.delta, map.mapView, &window);
            }
        }
            ImGui::SFML::Update(window, deltaClock.restart());

            window.clear(sf::Color::White);

            window.draw(map.mapSprite);

            window.draw(*map.nodePositions);

            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            ImGui::Begin("MU Fire Escape Simulator");
            if (ImGui::CollapsingHeader("What is this?"))
            {

            }

            if (ImGui::CollapsingHeader("Help"))
            {
            }

            if (ImGui::CollapsingHeader("Simulator"))
            {
                if (ImGui::CollapsingHeader("View Controls"))
                {
                    if (ImGui::BeginTable("split", 2))
                    {
                        // std::string text = "Navigates";
                        ImGui::TableNextColumn(); ImGui::Checkbox("WASD Movement", &enableWASD); 
                        ImGui::TableNextColumn(); 
                        // auto posX = (ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(text.c_str()).x 
                        //     - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
                        // if(posX > ImGui::GetCursorPosX())
                        //     ImGui::SetCursorPosX(posX);
                        ImGui::RadioButton("Navigate", &screenClickHandle, 0);
 
                        ImGui::EndTable();
                    }
                    ImGui::Separator();
                    ImGui::Text("Zoom: 0"); ImGui::SameLine();
                    ImGui::Button("+"); ImGui::SameLine();
                    ImGui::Button("-");
                }
                if (ImGui::CollapsingHeader("Starting Points"))
                {
                    if (ImGui::BeginTable("table1", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
                    {
                        // Display headers so we can inspect their interaction with borders.
                        // (Headers are not the main purpose of this section of the demo, so we are not elaborating on them too much. See other sections for details)

                            ImGui::TableSetupColumn("Starting Points"); ImGui::SameLine();
                            ImGui::TableHeadersRow();

                        for (int row = 0; row < startingPoints.size(); row++)
                        {
                            ImGui::TableNextRow();
                            for (int column = 0; column < 1; column++)
                            {
                                ImGui::TableSetColumnIndex(column);
                                ImGui::Text(startingPoints[row]->buffer); ImGui::SameLine();
                                if(ImGui::Button(("Modify##" + std::to_string(row)).c_str()))
                                {
                                    modalModify = modifyPointState::MODIFY;
                                } ImGui::SameLine();
                                ImGui::Button("Locate"); ImGui::SameLine();
                                if (ImGui::Button(("Move##" + std::to_string(row)).c_str()))
                                {
                                    sf::Color color(sf::Color(
                                        startingPoints[row]->point_rgba.x * 255.0f,
                                        startingPoints[row]->point_rgba.y * 255.0f,
                                        startingPoints[row]->point_rgba.z * 255.0f,
                                        startingPoints[row]->point_rgba.w * 255.0f)
                                    );
                                    startingPoints[row]->point.setFillColor(color);

                                    startPointMoving = true;
                                    startingPointTemp = startingPoints[row];
                                    enableWASD = true;
                                }
                                ImGui::SameLine();
                                ImGui::Button("Delete"); ImGui::SameLine();
                                ImGui::ColorEdit4("", (float*)&(startingPoints[row]->point_rgba), ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoInputs);
                                // ImGui::ColorPicker4("My Color", (float*)&(startingPoints[row]->point_rgba), misc_flags, NULL);
                                // else if (contents_type)
                                //     ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f));
                            }
                        }

                        ImGui::EndTable();
                    }
                    if (ImGui::Button("Add"))
                    {
                        modalModify = modifyPointState::CREATE;
                    }
                }
                if (ImGui::Button("Visualize"))
                {

                    for (size_t i = 0; i < map.getTotalRows(); ++i)
                    {
                        for (auto node : map.nodes[i])
                            node->updateNeighbors(map.nodes);
                    }


                    for (auto startNode : startingPoints)
                    {

                        if (startNode->node != nullptr)
                        {
                            exitsStored.clear();
                            auto previous_nodes =  fsim::Algorithms::dijkstra(startNode->node, nullptr, map.nodes, map.getTotalRows(), map.getTotalCols(), false);

                            for (auto exitNode : map.exitNodes)
                            {
                                uint32_t nodeCount = fsim::Algorithms::reconstruct_path(exitNode, startNode->node, previous_nodes, false);
                                exitsStored.push_back(std::make_pair(exitNode, nodeCount));
                            }
                            auto minExitNode = *std::min_element(exitsStored.begin(), exitsStored.end(), [](auto &left, auto &right) {
                                                return left.second < right.second;});
                            uint32_t finalCount = fsim::Algorithms::reconstruct_path(minExitNode.first, startNode->node, previous_nodes, true);
                        }

                    }


                    map.initVertexArray();
                }
            }

            if (ImGui::CollapsingHeader("Results"))
            {
            }
            
            if (modalModify == modifyPointState::CREATE)
                displayModifyModal(modifyPointState::CREATE);

            ImGui::End();

            ImGui::SFML::Render(window);

            for (auto startNode : startingPoints)
            {
                if (startNode->node != nullptr)
                    window.draw(startNode->point);
            }

            if (startPointMoving)
            {
                startingPointTemp->point.setPosition(sf::Vector2f(worldPos.x, worldPos.y));
                window.draw(startingPointTemp->point);

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    if (!mouseDown){
                        for (size_t i = 0; i < map.getTotalRows(); ++i)
                        {
                            for (auto node : map.nodes[i])
                            {
                                node->switchColor(sf::Color(0.0f, 0.0f, 0.0f, 0.0f));
                            }
                        }
                        sf::Vector2u position = map.clickPosition(worldPos);
                        fsim::Node* selectedNode = map.nodes[position.x][position.y];
                        fsim::Node* calculatedSelectedNode = fsim::Algorithms::bfsGetNearestStart(selectedNode, map.nodes, map.getTotalRows(), map.getTotalCols());
                        startingPointTemp->node = calculatedSelectedNode;

                        startingPointTemp->point.setPosition(sf::Vector2f(startingPointTemp->node->getWorldPos().x + (startingPointTemp->node->getTileSize()/2.0f), 
                        startingPointTemp->node->getWorldPos().y + (startingPointTemp->node->getTileSize()/2.0f)));

                        startPointMoving = false;
                        startingPointTemp = nullptr;
                        mouseDown = true;
                    } 
                }
                else mouseDown = false;
            }
            window.display();


            // * Backends *//
            if (enableWASD)
            {
                fsim::Controller::keyboardEvent(map.mapView, &window);
            }


            if (screenClickHandle == 0)
                fsim::Controller::dragEvent(map.mapView, &window);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        
    }
    ImGui::SFML::Shutdown();
    return 0;
}