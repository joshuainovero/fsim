#pragma once
#include "Node.hpp"
#include <queue>

namespace fsim
{

    namespace Algorithms
    {
        // Calculates the heuristic value between two points
        double calc_heuristic(sf::Vector2i p1, sf::Vector2i p2);

        // Reconstructs the path after calling a pathfinding function
        uint32_t reconstruct_path(Node* current, Node* start_node, std::unordered_map<Node*, Node*> previous_node, const bool& disp);
        
        // A* pathfinding algorithm
        uint32_t astar(Node* start, Node* end, std::vector<Node*>* tiles, const uint32_t& totalRows, const uint32_t& totalCols, const bool& disp);

        // Dijkstra's Algorithm
        std::unordered_map<Node*, Node*> dijkstra(Node* start, Node* end, std::vector<Node*>* tiles, const uint32_t& totalRows, const std::pair<uint32_t, uint32_t>& cols, const bool& disp);

        // Gets the nearest node when mouse position is not at the valid node
        Node* bfsGetNearestStart(Node* selectedNode, std::vector<Node*>* tiles, const uint32_t& totalRows, const uint32_t& totalCols);
    }
}