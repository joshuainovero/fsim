#include "Algorithms.hpp"
#include <utility>
#include <set>
#include <math.h>
#include <iostream>

namespace fsim
{
    namespace Algorithms
    {

        double calc_heuristic(sf::Vector2i p1, sf::Vector2i p2)
        {
            return (std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y));
        }

        uint32_t reconstruct_path(Node* current, Node* start_node, std::unordered_map<Node*, Node*> previous_node, const bool& disp)
        {
            uint32_t nodeCount = 1;
            while (current != start_node)
            {
                if (disp)
                    current->setPath();

                current = previous_node[current];
                nodeCount++;
            }
            return nodeCount;
        }
        uint32_t astar(Node* start, Node* end, std::vector<Node*>* tiles, const uint32_t& totalRows, const uint32_t& totalCols, const bool& disp)
        {

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
                    return reconstruct_path(current, start_node, previous_node, disp);
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
            return 0;
        }

        std::unordered_map<Node*, Node*> dijkstra(Node* start, Node* end, std::vector<Node*>* tiles, const uint32_t& totalRows, const uint32_t& totalCols, const bool& disp)
        {
            std::set<std::pair<uint32_t, Node*>> priority_queue;
            std::unordered_map<Node*, uint32_t> g_score;
            std::unordered_map<Node*, Node*> previous_node;

            for (size_t i = 0; i < totalRows; ++i){
                for (size_t k = 0; k < totalCols; ++k){
                    g_score[tiles[i][k]] = INT_MAX;
                }
            }

            g_score[start] = 0;
            priority_queue.insert(std::make_pair(g_score[start], start));

            while(!priority_queue.empty()){
                Node* current_node = (priority_queue.begin())->second;

                uint32_t current_dist = (priority_queue.begin())->first;
                priority_queue.erase(priority_queue.begin());

                for (auto neighbor : current_node->neighbors){
                    if (current_dist + 1 < g_score[neighbor]){
                        auto find_node = priority_queue.find(std::make_pair(g_score[neighbor], neighbor));
                        if (find_node != priority_queue.end())
                            priority_queue.erase(find_node);

                        g_score[neighbor] = current_dist + 1;
                        priority_queue.insert(std::make_pair(g_score[neighbor], neighbor));
                        previous_node[neighbor] = current_node;
                    }
                }
            }

            return previous_node;
        }

        Node* bfsGetNearestStart(Node* selectedNode, std::vector<Node*>* tiles, const uint32_t& totalRows, const uint32_t& totalCols)
        {
            std::unordered_map<Node*, bool> visited;
            std::queue<Node*> pQueue;
            pQueue.push(selectedNode);
            visited[selectedNode] = true;

            while (!pQueue.empty())
            {
                Node* currentNode = pQueue.front();
                pQueue.pop();

                if (currentNode->type == NODETYPE::DefaultPath)
                    return currentNode;

                sf::Vector2i nodePosition = currentNode->getPosition();
                int row = nodePosition.x;
                int col = nodePosition.y;

                if (row  < (int)totalRows - 1)
                {
                    if (!visited[tiles[row + 1][col]])
                    {
                        pQueue.push(tiles[row + 1][col]);
                        visited[currentNode] = true;
                    }
                }
                
                if (row  > 0)
                {
                    if (!visited[tiles[row - 1][col]])
                    {
                        pQueue.push(tiles[row - 1][col]);
                        visited[currentNode] = true;
                    }
                }
                if (col  < (int)totalCols - 1)
                {
                    if (!visited[tiles[row][col + 1]])
                    {
                        pQueue.push(tiles[row][col + 1]);
                        visited[currentNode] = true;
                    }
                }

                if (col > 0)
                {
                    if (!visited[tiles[row][col - 1]])
                    {
                        pQueue.push(tiles[row][col - 1]);
                        visited[currentNode] = true;
                    }
                }
                
            }

            return nullptr;
        }
    }
}