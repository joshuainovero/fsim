#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace fsim
{
    class Node
    {
    public:
        Node(uint32_t row_, uint32_t col_, float tileSize_, uint32_t totalRows_, uint32_t totalCols_);
        ~Node();

        bool isObstruction() const; 

        void setStart();
        void setTarget();
        void setObstruction();
        void reset();

        void updateNeighbors(std::vector<Node*>* nodes);

        //getters
        sf::Vector2u getPosition() const;

    private:
        void switchColor();

    public:
        sf::VertexArray quad;

    private:
        uint32_t row, col;
        uint32_t totalRows;
        uint32_t totalCols;
        float    x, y;
        float    tileSize;

        sf::Color color;


        std::vector<Node*> neighbors;

    };
}