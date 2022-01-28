#include "Map.hpp"
#include <math.h>
#include <fstream>
#include <iostream>

namespace fsim
{
    Map::Map(uint32_t columns, const std::string& mapDataPath_, sf::RenderWindow* window)
        : totalCols(columns), mapDataPath(mapDataPath_), start(nullptr), target(nullptr)
    {
        const float boardWidth = 1366.0f;

        tileSize = boardWidth / (float)totalCols;
        totalRows = std::ceil((float)768.0f / tileSize);

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
                            nodes[row_i][col_i]->setDefaultPath();
                        else if (state == "2")
                        {
                            nodes[row_i][col_i]->setDefaultExit();
                            nodes[row_i][col_i]->exit = true;
                            exitNodes.push_back(nodes[row_i][col_i]);
                        }
                    }
                    else
                        nodes[row_i].push_back(nullptr);
                }
            }
        dataStream.close();

        nodePositions = std::make_unique<sf::VertexArray>(sf::Quads, (totalRows * (maxCols - minCols)) * 4);
        initVertexArray();

        // mapTexture.loadFromFile(mapTexturePath);
        // mapTexture.setSmooth(true);
        // mapSprite.setTexture(mapTexture);
        // const sf::Vector2f targetSize(1366.0f, 768.0f);
        // mapSprite.setScale(
        //     targetSize.x / mapSprite.getLocalBounds().width, 
        //     targetSize.y / mapSprite.getLocalBounds().height
        // );

        mapView.setSize(sf::Vector2f(1366.0f, 768.0f));
        mapView.setCenter(sf::Vector2f(1366.0f / 2.0f, 768.0f / 2.0f));

        sf::Vector2f mapSize(1366.0f * fsim::Controller::zoomValues[fsim::Controller::mouseValue], 
            768.0f * fsim::Controller::zoomValues[fsim::Controller::mouseValue]);

        mapView.setSize(sf::Vector2f(mapSize.x, mapSize.y));
        window->setView(mapView);

        point.setRadius(4.0f);
        point.setFillColor(sf::Color::Blue);
        point.setOrigin(point.getRadius(), point.getRadius());

    }

    Map::~Map() {
        std::cout << "destructor calling" << std::endl;
        for (size_t i = 0; i < totalRows; ++i)
        {
            for (size_t k = minCols; k < maxCols; ++k)
            {
                delete nodes[i][k];
            }
        }
        
    }

    void Map::initVertexArray()
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

    void Map::saveChanges()
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

    void Map::setStart(Node* node) { start = node; }
    void Map::setTarget(Node* node) { target = node; }

    void Map::setMapTexture(sf::Texture* texture)
    {
        mapSprite.setTexture(*texture);
        const sf::Vector2f targetSize(1366.0f, 768.0f);
        mapSprite.setScale(
            targetSize.x / mapSprite.getLocalBounds().width, 
            targetSize.y / mapSprite.getLocalBounds().height
        );
    }

    void Map::drawMap(sf::RenderWindow* window) { window->draw(mapSprite); }

    sf::Vector2u Map::clickPosition(sf::Vector2f worldPos) const
    {
        uint32_t x = worldPos.x;
        uint32_t y = worldPos.y;
        uint32_t row = y / tileSize;
        uint32_t col = x / tileSize;
        return sf::Vector2u(row,col);
    }
    
    uint32_t Map::getTotalRows() const { return totalRows; }
    uint32_t Map::getTotalCols() const { return totalCols; }
    Node* Map::getStart() const { return start; }
    Node* Map::getTarget() const { return target; }
    float Map::getTileSize() const { return tileSize; }
}