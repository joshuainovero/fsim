#pragma once
#include "Node.hpp"
#include <queue>

namespace fsim
{

    namespace Algorithms
    {

        double calc_heuristic(sf::Vector2i p1, sf::Vector2i p2);

        uint32_t reconstruct_path(Node* current, Node* start_node, std::unordered_map<Node*, Node*> previous_node, const bool& disp);
        
        uint32_t astar(Node* start, Node* end, std::vector<Node*>* tiles, const uint32_t& totalRows, const uint32_t& totalCols, const bool& disp);

        std::unordered_map<Node*, Node*> dijkstra(Node* start, Node* end, std::vector<Node*>* tiles, const uint32_t& totalRows, const std::pair<uint32_t, uint32_t>& cols, const bool& disp);

        Node* bfsGetNearestStart(Node* selectedNode, std::vector<Node*>* tiles, const uint32_t& totalRows, const uint32_t& totalCols);
    }
}