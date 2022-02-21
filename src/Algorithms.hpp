#pragma once
#include "Node.hpp"
#include "FireGraphics.hpp"
#include "Results.hpp"
#include <queue>

namespace fsim
{

    namespace Algorithms
    {
        // Calculates the heuristic value between two points
        double calc_heuristic(sf::Vector2i p1, sf::Vector2i p2);

        // Reconstructs the path after calling a pathfinding function
        Results reconstruct_path(Node* current, Node* start_node, std::map<Node*, Node*> previous_node, const bool& disp);
        
        // A* pathfinding algorithm
        uint32_t astar(Node* start, Node* end, std::vector<Node*>* tiles, const uint32_t& totalRows, const uint32_t& totalCols, const bool& disp);

        // Dijkstra's Algorithm
        std::map<Node*, Node*> dijkstra(Node* start, Node* end, std::vector<Node*>* tiles, const uint32_t& totalRows, const std::pair<uint32_t, uint32_t>& cols, const bool& disp, const bool& safePath);

        // Gets the nearest node when mouse position is not at the valid node
        Node* bfsGetNearestStart(Node* selectedNode, std::vector<Node*>* tiles, const uint32_t& totalRows, const uint32_t& totalCols);

        // Calculates the indicator risk of a node
        void calculateRisk(std::vector<Node*>* nodes, std::vector<FireGraphics>& fireGraphicsList, const uint32_t& totalRows, const std::pair<uint32_t, uint32_t>& cols);
    }
}