#pragma once
#include "Node.hpp"

namespace fsim
{

    namespace Algorithms
    {
        namespace
        {
            double calc_heuristic(sf::Vector2i p1, sf::Vector2i p2);

            void reconstruct_path(Node* current, Node* start_node, std::unordered_map<Node*, Node*> previous_node);
        }
        void astar(Node* start, Node* end, std::vector<Node*>* tiles, const uint32_t& totalRows, const uint32_t& totalCols);
    }
}