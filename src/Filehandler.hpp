#pragma once
#include <json/json.h>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <cstring>
#include <memory>
#include "Floormap.hpp"

extern bool startingPointsChanged;
extern sf::Texture targetIconTexture;

namespace fsim
{
    namespace file
    {
        static float heatFluxValue = 200;
        template<typename T, typename... _T_Pack>
        inline std::vector<T> recurseArgs(T n, _T_Pack... args){
            std::vector<T> vecOne {n};
            if constexpr (sizeof...(_T_Pack) > 0){
                auto vecFrom =  recurseArgs(args...);
                for (const auto& elem : vecFrom)
                    vecOne.push_back(elem);
                return vecOne;
            }
            else
                return vecOne;
        }

        template<typename T, typename... _T_Pack>
        inline Json::Value createArray(T n, _T_Pack... args)
        {
            Json::Value arrayObj;
            auto vectorValues = recurseArgs(n, args...);
            for (const auto& elem : vectorValues)
            {
                arrayObj.append(elem);
            }
            return arrayObj;
        }

        inline void ImGuiOpenFileDialog(bool& fileDialogOpen, std::string& filePathName, bool& manipulateFile)
        { 

        // display
            if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) 
            {
                // action if OK
                if (ImGuiFileDialog::Instance()->IsOk())
                {
                    filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                    std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                    std::replace(filePathName.begin(), filePathName.end(), '\\', '/');
                    //std::cout << filePathName << std::endl;
                    fileDialogOpen = false;
                    manipulateFile = true;
                // action
                }
                
                // close
                ImGuiFileDialog::Instance()->Close();
            }
        }

        static void modifyNodes(fsim::Floormap& map, std::function<void(fsim::Node*)> operations)
        {
            for (size_t i = 0; i < map.getTotalRows(); ++i)
            {
                for (size_t k = map.minCols; k < map.maxCols; ++k)
                {
                    operations(map.nodes[i][k]);
                }
            }
        }

        inline void saveFile(const std::vector<std::shared_ptr<fsim::Floormap>>& FloorMapObjects, const std::string& path)
        {
            
            Json::Value data;

            unsigned int floor = 1;
            for (const auto& map : FloorMapObjects)
            {
                Json::Value floorDictionary;
                floorDictionary["floor-level"] = floor;

                // Get starting points data
                for (const auto& s_point : map->startingPoints)
                {
                    Json::Value singleStartValue;
                    singleStartValue["point-name"] = std::string(s_point->buffer);
                    if (s_point->node != nullptr)
                        singleStartValue["grid-pos"] = createArray<uint32_t>(s_point->node->row, s_point->node->col);
                    else
                        singleStartValue["grid-pos"] = "null";
                    singleStartValue["color"] = createArray<float>(
                        s_point->point_rgba.x, s_point->point_rgba.y, s_point->point_rgba.z, s_point->point_rgba.w);
                    floorDictionary["starting-points"].append(singleStartValue);
                }

                // Get fire points area
                for (const auto& f_point : map->fireGraphicsList)
                {
                    Json::Value singleFirePointValue;
                    if (f_point.node != nullptr)
                    {
                        singleFirePointValue["grid-pos"] = createArray<uint32_t>(f_point.node->row, f_point.node->col);
                    }
                    else
                        singleFirePointValue["grid-pos"] = "null";
                    floorDictionary["fire-points"].append(singleFirePointValue);
                }
                data.append(floorDictionary);
                floor++;
            }
            std::ofstream fileStreamJSON(path);
                Json::StyledWriter styledwriter;
                fileStreamJSON << styledwriter.write(data);
            fileStreamJSON.close();
        }

        inline void loadFile(const std::vector<std::shared_ptr<fsim::Floormap>>& FloorMapObjects, const std::string& path, sf::Texture& fireIconTexture, FloorLabel currentEnumFloor)
        {
            startingPointsChanged = true;
            // std::cout << "HI!" << std::endl;
            std::ifstream fileStreamJSON(path);
            Json::Value data;
            Json::Reader reader;
            reader.parse(fileStreamJSON, data);
            fileStreamJSON.close();
            
            size_t floor_i = 0;
            for (const auto& floorData : data)
            {
                // Clear all maps
                modifyNodes(*FloorMapObjects[floor_i], [](fsim::Node* node) { node->switchColor(sf::Color(0.0f, 0.0f, 0.0f, 0.0f)); });
                
                for (size_t i = 0; i < FloorMapObjects[floor_i]->startingPoints.size(); ++i)
                {
                    delete FloorMapObjects[floor_i]->startingPoints[i];
                }
                FloorMapObjects[floor_i]->startingPoints.clear();

                for (size_t i = 0; i < FloorMapObjects[floor_i]->fireGraphicsList.size(); ++i)
                {
                    if (FloorMapObjects[floor_i]->fireGraphicsList[i].node != nullptr)
                    {
                        fsim::Node* selectedNode = FloorMapObjects[floor_i]->fireGraphicsList[i].node;
                        // FloorMapObjects[floor_i]->fireGraphicsList.erase(map->fireGraphicsList.begin() + i);
                        // FloorMapObjects[floor_i]->nodeObstructionsList.erase(FloorMapObjects[floor_i]->nodeObstructionsList.begin() + i);
                        for (size_t i = 0; i < FloorMapObjects[floor_i]->getTotalRows(); ++i)
                        {
                            for (size_t k = FloorMapObjects[floor_i]->minCols; k < FloorMapObjects[floor_i]->maxCols; ++k)
                            {
                                if (FloorMapObjects[floor_i]->nodes[i][k] != selectedNode)
                                {
                                    float distance = std::sqrt(std::pow((float)selectedNode->col - (float)FloorMapObjects[floor_i]->nodes[i][k]->col, (float)2.0f) + std::pow((float)selectedNode->row - (float)FloorMapObjects[floor_i]->nodes[i][k]->row, (float)2.0f));
                                    if (distance * fsim::units::UNIT_DISTANCE <= fsim::units::STANDARD_HEAT_FLUX_RADIUS)
                                    {
                                        // map->nodes[i][k]->switchColor(sf::Color(255,115, 0, 90.0f));
                                        FloorMapObjects[floor_i]->nodes[i][k]->obstruction = false;
                                        
                                    }
                                }

                            }
                        }
                        // map->initVertexArray();
                    }
                }
                FloorMapObjects[floor_i]->fireGraphicsList.clear();
                FloorMapObjects[floor_i]->nodeObstructionsList.clear();
                
                for (const auto& start : floorData["starting-points"])
                {
                    fsim::StartingPoints* tempStartingPoint = new fsim::StartingPoints(&targetIconTexture);
                    std::vector<uint32_t> rowcol;
                    std::vector<float> rgbaVec;

                    strcpy(tempStartingPoint->buffer, start["point-name"].asString().c_str());
                    if (start["grid-pos"].isArray())
                    {
                         for (const auto& gridValues : start["grid-pos"])
                        {
                            rowcol.push_back(gridValues.asUInt());
                        }                   
                    }


                    for (const auto& rgbaValues : start["color"])
                    {
                        rgbaVec.push_back((float)rgbaValues.asDouble());
                    }
                    tempStartingPoint->point_rgba.x = rgbaVec[0];
                    tempStartingPoint->point_rgba.y = rgbaVec[1];
                    tempStartingPoint->point_rgba.z = rgbaVec[2];
                    tempStartingPoint->point_rgba.w = rgbaVec[3];

                    sf::Color color(sf::Color(
                         rgbaVec[0] * 255.0f,
                         rgbaVec[1] * 255.0f,
                         rgbaVec[2] * 255.0f,
                         rgbaVec[3] * 255.0f)
                    );
                
                    tempStartingPoint->point.setRadius(4.0f);
                    tempStartingPoint->point.setOrigin(sf::Vector2f(tempStartingPoint->point.getRadius(), tempStartingPoint->point.getRadius()));

                    if (start["grid-pos"].isArray())
                    {
                        tempStartingPoint->node = FloorMapObjects[floor_i]->nodes[rowcol[0]][rowcol[1]];
                        tempStartingPoint->point.setPosition(sf::Vector2f(tempStartingPoint->node->getWorldPos().x + (tempStartingPoint->node->getTileSize()/2.0f), 
                        tempStartingPoint->node->getWorldPos().y + (tempStartingPoint->node->getTileSize()/2.0f)));
                        tempStartingPoint->point.setFillColor(color);
                    }

                    
                    FloorMapObjects[floor_i]->startingPoints.push_back(tempStartingPoint);

                }

                for (const auto& fire : floorData["fire-points"])
                {
                    std::vector<uint32_t> rowcol;
                    if (fire["grid-pos"].isArray())
                    {
                         for (const auto& gridValues : fire["grid-pos"])
                        {
                            rowcol.push_back(gridValues.asUInt());
                        }                   
                    }
                    fsim::Node* selectedNode = FloorMapObjects[floor_i]->nodes[rowcol[0]][rowcol[1]];
                    // selectedNode->switchColor(sf::Color(255, 70, 0, 255.0f));
                    FloorMapObjects[floor_i]->generateFireGraphics(selectedNode, &fireIconTexture, heatFluxValue);
                    std::vector<fsim::Node*> nodeObstructions;
                    for (size_t i = 0; i < FloorMapObjects[floor_i]->getTotalRows(); ++i)
                    {
                        for (size_t k = FloorMapObjects[floor_i]->minCols; k < FloorMapObjects[floor_i]->maxCols; ++k)
                        {
                            if (FloorMapObjects[floor_i]->nodes[i][k] != selectedNode)
                            {
                                float distance = std::sqrt(std::pow((float)selectedNode->col - (float)FloorMapObjects[floor_i]->nodes[i][k]->col, (float)2.0f) + std::pow((float)selectedNode->row - (float)FloorMapObjects[floor_i]->nodes[i][k]->row, (float)2.0f));
                                if (distance * fsim::units::UNIT_DISTANCE < fsim::units::STANDARD_HEAT_FLUX_RADIUS)
                                {
                                    // map->nodes[i][k]->switchColor(sf::Color(255,115, 0, 90.0f));
                                    FloorMapObjects[floor_i]->nodes[i][k]->obstruction = true;
                                    nodeObstructions.push_back(FloorMapObjects[floor_i]->nodes[i][k]);
                                }
                            }

                        }
                    }
                    FloorMapObjects[floor_i]->nodeObstructionsList.push_back(nodeObstructions);
                    FloorMapObjects[floor_i]->initVertexArray();
                }

                floor_i++;
            }

            FloorMapObjects[currentEnumFloor]->copy_node_data_to_node_pointers();
        }
    }


}