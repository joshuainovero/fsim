#include "Floormap.hpp"
#include <math.h>
#include <fstream>
#include <iostream>

namespace fsim
{
    Floormap::Floormap(uint32_t columns, const std::string& mapDataPath_, sf::RenderWindow* window, std::vector<Node*>* nodes_)
        : totalNodesGenerated(0), mouseValue(4), totalCols(columns), mapDataPath(mapDataPath_), start(nullptr), target(nullptr)
    {
        const float boardWidth = 1366.0f;

        tileSize = boardWidth / (float)totalCols;
        totalRows = std::ceil((float)768.0f / tileSize);

        if (nodes_ == nullptr)
        {
            std::cout << "New" << std::endl;
            nodes = new std::vector<Node*>[totalRows];

            std::ifstream dataStream(mapDataPath);
                for (size_t row_i = 0; row_i < totalRows; ++row_i)
                {
                    for (size_t col_i = 0; col_i < totalCols; ++col_i)
                    {
                        std::string state;
                        dataStream >> state;
                        if (col_i >= minCols && col_i < maxCols)
                        {
                            nodes[row_i].push_back(new Node(row_i, col_i, tileSize, totalRows, totalCols));
                            if (state == "1")
                            {
                                nodes[row_i][col_i]->setDefaultPath();
                                totalNodesGenerated++;
                            }
                            else if (state == "2")
                            {
                                nodes[row_i][col_i]->setDefaultExit();
                                nodes[row_i][col_i]->exit = true;
                                exitNodes.push_back(nodes[row_i][col_i]);
                                totalNodesGenerated++;
                            }
                        }
                        else
                            nodes[row_i].push_back(nullptr);
                    }
                }
            dataStream.close();
        }
        else
        {
            nodes = nodes_;
            std::ifstream dataStream(mapDataPath);
                for (size_t row_i = 0; row_i < totalRows; ++row_i)
                {
                    for (size_t col_i = 0; col_i < totalCols; ++col_i)
                    {
                        char state;
                        dataStream >> state;
                        if (col_i >= minCols && col_i < maxCols)
                        {
                            nodeDatas.push_back(state);
                        }

                    }
                }
            dataStream.close();
            std::cout << "Copied" << std::endl;
        }

        nodePositions = std::make_unique<sf::VertexArray>(sf::Quads, (totalRows * (maxCols - minCols)) * 4);
        initVertexArray();

        mapView.setSize(sf::Vector2f(1366.0f, 768.0f));
        mapView.setCenter(sf::Vector2f(1366.0f / 2.0f, 768.0f / 2.0f));

        sf::Vector2f mapSize(1366.0f * fsim::Controller::zoomValues[mouseValue], 
            768.0f * fsim::Controller::zoomValues[mouseValue]);

        mapView.setSize(sf::Vector2f(mapSize.x, mapSize.y));
        window->setView(mapView);

    }

    Floormap::~Floormap() {
        std::cout << "destructor calling" << std::endl;
        for (size_t i = 0; i < totalRows; ++i)
        {
            for (size_t k = minCols; k < maxCols; ++k)
            {
                if (nodes[i][k] != nullptr)
                {
                    delete nodes[i][k];
                    nodes[i][k] = nullptr;
                }

            }
        }
        
    }

    void Floormap::initVertexArray()
    {
        size_t node_count = 0;

        for (size_t i = 0; i < totalRows; ++i)
        {
            for (size_t k = minCols; k < maxCols; ++k)
            {
                if (nodes[i][k] != nullptr)
                {
                    (*nodePositions)[node_count].position = nodes[i][k]->quad[0].position;
                    (*nodePositions)[node_count + 1].position = nodes[i][k]->quad[1].position;
                    (*nodePositions)[node_count + 2].position = nodes[i][k]->quad[2].position;
                    (*nodePositions)[node_count + 3].position = nodes[i][k]->quad[3].position;
                    
                    (*nodePositions)[node_count].color = nodes[i][k]->quad[0].color;
                    (*nodePositions)[node_count + 1].color = nodes[i][k]->quad[1].color;
                    (*nodePositions)[node_count + 2].color = nodes[i][k]->quad[2].color;
                    (*nodePositions)[node_count + 3].color = nodes[i][k]->quad[3].color;
                    node_count += 4;

                }
            }
        }
    }

    void Floormap::saveChanges()
    {
        std::ofstream dataStream(mapDataPath);

        for (size_t row_i = 0; row_i < totalRows; ++row_i)
        {
            for (size_t col_i = 0; col_i < totalCols; ++col_i)
            {
                if (nodes[row_i][col_i] != nullptr)
                {
                    if (nodes[row_i][col_i]->type == NODETYPE::DefaultPath && !nodes[row_i][col_i]->exit)
                        dataStream << "1" << std::endl;
                    else if (nodes[row_i][col_i]->type == NODETYPE::DefaultPath && nodes[row_i][col_i]->exit)
                        dataStream << "2" << std::endl;
                    else
                        dataStream << "0" << std::endl;
                }
                else
                    dataStream << "0" << std::endl;

            }
        }
        dataStream.close();
    }

    void Floormap::setStart(Node* node) { start = node; }
    void Floormap::setTarget(Node* node) { target = node; }

    void Floormap::setMapTexture(sf::Texture* texture)
    {
        mapSprite.setTexture(*texture);
        const sf::Vector2f targetSize(1366.0f, 768.0f);
        mapSprite.setScale(
            targetSize.x / mapSprite.getLocalBounds().width, 
            targetSize.y / mapSprite.getLocalBounds().height
        );
    }

    void Floormap::drawMap(sf::RenderWindow* window) { window->draw(mapSprite); }

    void Floormap::copy_node_data_to_node_pointers()
    {
        uint32_t counter = 0;
        for (size_t row_i = 0; row_i < totalRows; ++row_i)
        {
            for (size_t col_i = minCols; col_i < maxCols; ++col_i)
            {
                if (nodes[row_i][col_i] != nullptr)
                {
                    nodes[row_i][col_i]->reset();
                    if (nodeDatas[counter] == '1')
                        nodes[row_i][col_i]->setDefaultPath();
                    else if (nodeDatas[counter] == '2')
                    {
                        nodes[row_i][col_i]->setDefaultExit();
                        nodes[row_i][col_i]->exit = true;
                        exitNodes.push_back(nodes[row_i][col_i]);
                    }
                    counter++;
                }
            }
        }
    }

    void Floormap::copy_node_pointers_to_node_data()
    {
        nodeDatas.clear();
        for (size_t row_i = 0; row_i < totalRows; ++row_i)
        {
            for (size_t col_i = minCols; col_i < maxCols; ++col_i)
            {
                if (nodes[row_i][col_i] != nullptr)
                {
                    if (nodes[row_i][col_i]->type == NODETYPE::DefaultPath && !nodes[row_i][col_i]->exit)
                        nodeDatas.push_back('1');
                    else if (nodes[row_i][col_i]->type == NODETYPE::DefaultPath && nodes[row_i][col_i]->exit)
                        nodeDatas.push_back('2');
                    else
                        nodeDatas.push_back('0');
                }
            }
        }
    }

    sf::Vector2u Floormap::clickPosition(sf::Vector2f worldPos) const
    {
        uint32_t x = worldPos.x;
        uint32_t y = worldPos.y;
        uint32_t row = y / tileSize;
        uint32_t col = x / tileSize;
        return sf::Vector2u(row,col);
    }
    
    uint32_t Floormap::getTotalRows() const { return totalRows; }
    uint32_t Floormap::getTotalCols() const { return totalCols; }
    Node* Floormap::getStart() const { return start; }
    Node* Floormap::getTarget() const { return target; }
    float Floormap::getTileSize() const { return tileSize; }
}