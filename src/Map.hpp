#pragma once
#include <memory>
#include "Node.hpp"
#include "Controller.hpp"

namespace fsim
{
    class Map
    {
    public:
        Map(uint32_t columns, const std::string& mapDataPath_, sf::RenderWindow* window);
        ~Map();

        void initVertexArray();
        void saveChanges();

        void setStart(Node* node);
        void setTarget(Node* node);
        void setMapTexture(sf::Texture* texture);
        void drawMap(sf::RenderWindow* window);
                
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

        sf::View        mapView;
        sf::CircleShape point;

        const size_t minCols = 55;
        const size_t maxCols = 343;
        

    private:
        uint32_t totalRows, totalCols;
        float    tileSize;

        sf::Sprite  mapSprite;
        std::string mapDataPath;

        Node* start;
        Node* target;
    };
}