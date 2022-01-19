#include "Map.hpp"
#include <math.h>
#include <fstream>
#include <iostream>

namespace fsim
{
    Map::Map(uint32_t columns, const std::string& mapTexturePath_, const std::string& mapDataPath_, sf::RenderWindow* window)
        : totalCols(columns), mapTexturePath(mapTexturePath_), mapDataPath(mapDataPath_)
    {
        const float boardWidth = 1366.0f;

        tileSize = boardWidth / (float)totalCols;
        totalRows = std::ceil((float)768.0f / tileSize);

        nodes = new std::vector<Node*>[totalRows];

        std::ifstream dataStream(mapDataPath);
            for (int row_i = 0; row_i < totalRows; ++row_i)
            {
                for (int col_i = 0; col_i < totalCols; ++col_i)
                {
                    std::string state;
                    dataStream >> state;
                    nodes[row_i].push_back(new Node(row_i, col_i, tileSize, totalRows, totalCols));
                    if (state == "true")
                        nodes[row_i][col_i]->setObstruction();
                }
            }
        dataStream.close();

        nodePositions = std::make_unique<sf::VertexArray>(sf::Quads, (totalRows * totalCols) * 4);
        initVertexArray();

        mapTexture.loadFromFile(mapTexturePath);
        mapTexture.setSmooth(true);
        mapSprite.setTexture(mapTexture);
        sf::Vector2f targetSize = window->getView().getSize();
        mapSprite.setScale(
            targetSize.x / mapSprite.getLocalBounds().width, 
            targetSize.y / mapSprite.getLocalBounds().height
        );


        mapView.setSize(sf::Vector2f(1366.0f, 768.0f));
        mapView.setCenter(sf::Vector2f(1366.0f / 2.0f, 768.0f / 2.0f));

    }

    Map::~Map() {
        std::cout << "destructor calling" << std::endl;
        std::ofstream dataStream(mapDataPath);

        for (int row_i = 0; row_i < totalRows; ++row_i)
        {
            for (int col_i = 0; col_i < totalCols; ++col_i)
            {
                if (nodes[row_i][col_i]->isObstruction())
                    dataStream << "true" << std::endl;
                else
                    dataStream << "false" << std::endl;
                delete nodes[row_i][col_i];
            }
        }

        dataStream.close();
        delete nodes;
        
    }

    void Map::initVertexArray()
    {
        size_t node_count = 0;

        for (size_t i = 0; i < totalRows; ++i)
        {
            for (size_t k = 0; k < totalCols; ++k)
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

    float Map::getTileSize() const { return tileSize; }
}