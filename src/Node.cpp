#include "Node.hpp"

namespace fsim
{
    Node::Node(uint32_t row_, uint32_t col_, float tileSize_, uint32_t totalRows_, uint32_t totalCols_)
        : quad(sf::Quads, 4), row(row_), col(col_), tileSize(tileSize_), totalRows(totalRows_), totalCols(totalCols_)
    {
        x = (float)col * (float)tileSize;
        y = (float)row * (float)tileSize;

        quad[0].position = sf::Vector2f(x, y);
        quad[1].position = sf::Vector2f(x + tileSize, y);
        quad[2].position = sf::Vector2f(x + tileSize, y + tileSize);
        quad[3].position = sf::Vector2f(x, y + tileSize);

        switchColor(sf::Color(0.0f, 0.0f, 0.0f, 0.0f));
        type = NODETYPE::None;
        exit = false;
    }

    Node::~Node() {}

    void Node::updateNeighbors(std::vector<Node*>* nodes, uint32_t minCols, uint32_t maxCols)
    {
        neighbors.clear();

        if (row < totalRows - 1 && (nodes[row + 1][col]->type == NODETYPE::DefaultPath))
            neighbors.push_back(nodes[row + 1][col]);
        
        if (row > 0 && (nodes[row - 1][col]->type == NODETYPE::DefaultPath))
            neighbors.push_back(nodes[row - 1][col]);

        if (col < maxCols - 1 && (nodes[row][col + 1]->type == NODETYPE::DefaultPath))
            neighbors.push_back(nodes[row][col + 1]);

        if (col > minCols && (nodes[row][col - 1]->type == NODETYPE::DefaultPath))
            neighbors.push_back(nodes[row][col - 1]);
    }

    void Node::switchColor(sf::Color color_)
    {
        quad[0].color = color_;
        quad[1].color = color_;
        quad[2].color = color_;
        quad[3].color = color_;
    }



    void Node::setStart() 
    {
        switchColor(sf::Color::Green);
    }

    void Node::setTarget()
    {
        switchColor(sf::Color::Magenta);
    }

    void Node::setPath()
    {
        switchColor(sf::Color(255.0f, 254.0f, 106.0f, 255.0f));
    }

    void Node::setDefaultPath()
    {
        switchColor(sf::Color(0.0f, 0.0f, 0.0f, 0.0f));
        type = NODETYPE::DefaultPath;
    }

    void Node::setDefaultExit()
    {
        switchColor(sf::Color(0.0f, 0.0f, 0.0f, 0.0f));
        type = NODETYPE::DefaultPath;
    }

    void Node::setObstruction()
    {
        switchColor(sf::Color::Red);
        type = NODETYPE::Obstruction;
    }

    void Node::reset()
    {
        switchColor(sf::Color(0.0f, 0.0f, 0.0f, 0.0f));
        type = NODETYPE::None;
        exit = false;
    }

    sf::Vector2i Node::getPosition() const { return sf::Vector2i(row, col); }

    sf::Vector2f Node::getWorldPos() const { return sf::Vector2f(x, y); }

    float Node::getTileSize() const { return tileSize; }

}
