#include "Algorithms.hpp"
#include <utility>
#include <set>
#include <math.h>

namespace fsim
{
    namespace Algorithms
    {
        namespace
        {
            double calc_heuristic(sf::Vector2i p1, sf::Vector2i p2)
            {
                return (std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y));
            }

            void reconstruct_path(Node* current, Node* start_node, std::unordered_map<Node*, Node*> previous_node)
            {
                while (current != start_node)
                {
                    current->setPath();
                    current = previous_node[current];
                }
            }
        }
        void astar(Node* start, Node* end, std::vector<Node*>* tiles, const uint32_t& totalRows, const uint32_t& totalCols)
        {
            for (int i = 0; i < totalRows; ++i)
            {
                for (auto node : tiles[i])
                    node->updateNeighbors(tiles);
            }

            std::set<std::pair<std::pair<double, uint32_t>, Node*>> priority_queue;
            std::unordered_map<Node*, uint32_t> g_score;
            std::unordered_map<Node*, double> f_score;
            std::unordered_map<Node*, Node*> previous_node;

            Node* start_node = start;
            Node* end_node = end;

            uint32_t precedence = 0;
            priority_queue.insert(std::make_pair(std::make_pair(0, precedence), start_node));
            for (size_t i = 0; i < totalRows; ++i)
            {
                for (size_t k = 0; k < totalCols; ++k)
                    g_score[tiles[i][k]] = INT_MAX;
            }
            g_score[start_node] = 0;

            for (size_t i = 0; i < totalRows; ++i)
            {
                for (size_t k = 0; k < totalCols; ++k)
                    f_score[tiles[i][k]] = INT_MAX;
            }
            f_score[start_node] = calc_heuristic(start_node->getPosition(), end_node->getPosition());

            std::vector<Node*> priority_queue_tracker = { start_node };

            while (!priority_queue.empty())
            {
                auto current_queue = *(priority_queue.begin());
                Node* current = current_queue.second;

                auto it1 = std::find(priority_queue_tracker.begin(), priority_queue_tracker.end(), current);
                priority_queue.erase(priority_queue.begin());
                if (it1 != priority_queue_tracker.end())
                    priority_queue_tracker.erase(it1);
                
                if (current == end_node){
                    reconstruct_path(current, start_node, previous_node);
                    return;
                }
                
                for (auto neighbor : current->neighbors)
                {
                    uint32_t temp_g_score = g_score[current] + 1;
                    
                    if (temp_g_score < g_score[neighbor])
                    {
                        previous_node[neighbor] = current;
                        g_score[neighbor] = temp_g_score;
                        f_score[neighbor] = temp_g_score + calc_heuristic(neighbor->getPosition(), end_node->getPosition());

                        if (std::find(priority_queue_tracker.begin(), priority_queue_tracker.end(), neighbor) == priority_queue_tracker.end()){
                            precedence += 1;
                            priority_queue.insert(std::make_pair(std::make_pair(f_score[neighbor], precedence), neighbor));
                            priority_queue_tracker.push_back(neighbor);
                        }
                    }
                }

            }
        }
    }
}