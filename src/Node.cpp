#include "Node.hpp"

namespace fsim
{
    Node::Node(uint32_t row_, uint32_t col_, float tileSize_, uint32_t totalRows_, uint32_t totalCols_)
        : quad(sf::Quads, 4), row(row_), col(col_), tileSize(tileSize_), totalRows(totalRows_), totalCols(totalCols_)
    {
        x = (float)col * (float)tileSize;
        y = (float)row * (float)tileSize;

        color = sf::Color(0.0f, 0.0f, 0.0f, 0.0f);

        quad[0].position = sf::Vector2f(x, y);
        quad[1].position = sf::Vector2f(x + tileSize, y);
        quad[2].position = sf::Vector2f(x + tileSize, y + tileSize);
        quad[3].position = sf::Vector2f(x, y + tileSize);

        switchColor();
    }

    Node::~Node() {}

    sf::Vector2u Node::getPosition() const { return sf::Vector2u(row, col); }

    bool Node::isObstruction() const { return color == sf::Color::Blue; }

    void Node::setStart() 
    {
        color = sf::Color::Green;
        switchColor();
    }

    void Node::setTarget()
    {
        color = sf::Color::Magenta;
        switchColor();
    }

    void Node::setObstruction()
    {
        color = sf::Color::Blue;
        switchColor();
    }

    void Node::reset()
    {
        color = sf::Color(0.0f, 0.0f, 0.0f, 0.0f);
        switchColor();
    }

    void Node::updateNeighbors(std::vector<Node*>* nodes)
    {
        neighbors.clear();

        if (row < totalRows - 1 && !(nodes[row + 1][col]->isObstruction()))
            neighbors.push_back(nodes[row + 1][col]);
        
        if (row > 0 && !(nodes[row - 1][col]->isObstruction()))
            neighbors.push_back(nodes[row - 1][col]);

        if (col < totalCols - 1 && !(nodes[row][col + 1]->isObstruction()))
            neighbors.push_back(nodes[row][col + 1]);

        if (col > 0 && !(nodes[row][col - 1]->isObstruction()))
            neighbors.push_back(nodes[row][col - 1]);
    }

    void Node::switchColor()
    {
        quad[0].color = color;
        quad[1].color = color;
        quad[2].color = color;
        quad[3].color = color;
    }

}
