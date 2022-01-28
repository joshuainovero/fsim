#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace fsim
{
    enum NODETYPE {DefaultPath, Path, Obstruction, None};
    
    class Node
    {
    public:
        Node(uint32_t row_, uint32_t col_, float tileSize_, uint32_t totalRows_, uint32_t totalCols_);
        ~Node();

        void setStart();
        void setTarget();
        void setPath();
        void setDefaultPath();
        void setDefaultExit();
        void setObstruction();
        void reset();

        void updateNeighbors(std::vector<Node*>* nodes, uint32_t minCols, uint32_t maxCols);

        //getters
        sf::Vector2i getPosition() const;
        sf::Vector2f getWorldPos() const;
        float getTileSize() const;

        void switchColor(sf::Color color_);
        

    public:
        sf::VertexArray quad;

        std::vector<Node*> neighbors;

        NODETYPE type;

        bool exit;

    private:
        uint32_t row, col;
        float    tileSize;
        uint32_t totalRows;
        uint32_t totalCols;
        float    x, y;
    };
}