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
        
        // Updates neighbors of each node in the vector
        void updateNeighbors(std::vector<Node*>* nodes, uint32_t minCols, uint32_t maxCols);

        // Changes the color of a node
        void switchColor(sf::Color color_);

        // Declare node as a starting point - FOR DEBUGGING
        void setStart();

        // Declare node as a target point - FOR DEBUGGING
        void setTarget();

        // Sets a node into a path that was calculated by the pathfinding algorithm
        void setPath();

        // Sets a node into a default path of a floor map - FOR DEBUGGING
        void setDefaultPath();

        // Sets a node into a default exit of a floor map - FOR DEBUGGING
        void setDefaultExit();

        // Sets node into an obstruction
        void setObstruction();

        // Completely resets the settings of a node
        void reset();

        //getters
        sf::Vector2i getPosition() const;
        sf::Vector2f getWorldPos() const;
        float getTileSize() const;

        

    public:
        std::vector<Node*> neighbors; // Neighbor nodes of this instance
        sf::VertexArray    quad;      // 4 Vertex positions and colors - QUAD

        NODETYPE type; // Node type
        bool     exit; // Determines if the node is an exit
        float x, y;     // Position of the node with respect to the camera view


    private:
        uint32_t row; // Horizontal
        uint32_t col; // Vertical

        float tileSize; // Size of a single node with respect to the monitor

        uint32_t totalRows; // Total rows of the graph
        uint32_t totalCols; // Total columns of the graph

    };
}