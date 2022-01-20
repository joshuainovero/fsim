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
        void setObstruction();
        void reset();

        void updateNeighbors(std::vector<Node*>* nodes);

        //getters
        sf::Vector2i getPosition() const;

        void switchColor(sf::Color color_);

    public:
        sf::VertexArray quad;

        std::vector<Node*> neighbors;

        NODETYPE type;

    private:
        uint32_t row, col;
        uint32_t totalRows;
        uint32_t totalCols;
        float    x, y;
        float    tileSize;
    };
}