#pragma once
#include <memory>
#include "Node.hpp"
#include "Controller.hpp"

namespace fsim
{
    class Map
    {
    public:
        Map(uint32_t columns, const std::string& mapTexturePath_, const std::string& mapDataPath_, sf::RenderWindow* window);
        ~Map();

        void initVertexArray();

        void setStart(Node* node);
        void setTarget(Node* node);
                
        // getters
        sf::Vector2u clickPosition(sf::Vector2f worldPos) const;
        uint32_t     getTotalRows() const;
        uint32_t     getTotalCols() const;
        Node*        getStart() const;
        Node*        getTarget() const;
        float        getTileSize() const;

    public:
        std::unique_ptr<sf::VertexArray> nodePositions;
        std::vector<Node*>*              nodes;
        std::vector<Node*>               exitNodes;


        sf::Sprite mapSprite;
        sf::View   mapView;
        sf::CircleShape point;
        

    private:
        uint32_t totalRows, totalCols;
        float    tileSize;

        sf::Texture mapTexture;
        std::string mapTexturePath;

        std::string mapDataPath;

        Node* start;
        Node* target;
    };
}