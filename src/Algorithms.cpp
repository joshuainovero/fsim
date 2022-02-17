#include "Algorithms.hpp"
#include "Units.hpp"
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

        Results reconstruct_path(Node* current, Node* start_node, std::map<Node*, Node*> previous_node, const bool& disp)
        {
            uint32_t nodeCount = 1;
            uint32_t obstructionCount = 0;
            float danger_indicator_ave = 0.0f;
            Node* previous = nullptr;
            while (current != start_node)
            {
                // if (!previous_node.count(current))
                // {
                //     nodeCount = FSIM_NOPATHDIST;
                //     break;
                // }

                if (disp)
                {
                    // std::cout << current->riskValue << " ";
                    // if (current->riskValue >= 0.6096f)
                    // {
                    //     current->switchColor(sf::Color(current->riskValue * 255.0f, 255.0f, 0.0f, 255.0f));
                    // }
                    // else
                        // current->setPath();
                    if (current->obstruction == true)
                    {
                        danger_indicator_ave += current->danger_indicator;
                        obstructionCount++;
                    }

                    if (previous == nullptr)
                    {
                        current->switchColor(sf::Color(current->r, current->g, 0.0f, 255.0f));
                    }
                    else
                    {
                        if (current->col > previous->col)
                        {
                            current->quad[0].color = previous->quad[1].color;
                            current->quad[1].color = sf::Color(current->r, current->g, 0.0f, 255.0f);
                            current->quad[2].color = sf::Color(current->r, current->g, 0.0f, 255.0f);
                            current->quad[3].color = previous->quad[2].color;
                        }
                        else if (current->col < previous->col)
                        {
                            current->quad[0].color = sf::Color(current->r, current->g, 0.0f, 255.0f);
                            current->quad[1].color = previous->quad[0].color;
                            current->quad[2].color = previous->quad[3].color;  
                            current->quad[3].color = sf::Color(current->r, current->g, 0.0f, 255.0f);                      
                        }
                        else if (current->row < previous->row)
                        {
                            current->quad[0].color = sf::Color(current->r, current->g, 0.0f, 255.0f);
                            current->quad[1].color = sf::Color(current->r, current->g, 0.0f, 255.0f);
                            current->quad[2].color = previous->quad[1].color;
                            current->quad[3].color = previous->quad[0].color;
                        }
                        else if (current->row > previous->row)
                        {
                            current->quad[0].color = previous->quad[3].color;
                            current->quad[1].color = previous->quad[2].color;
                            current->quad[2].color = sf::Color(current->r, current->g, 0.0f, 255.0f);
                            current->quad[3].color = sf::Color(current->r, current->g, 0.0f, 255.0f);        
                        }

                    }
                    // current->switchColor(sf::Color(current->r, current->g, 0.0f, 255.0f));
                }
                previous = current;
                current = previous_node[current];
                nodeCount++;
            }
            // std::cout << std::endl;

            Results result;
            result.node_count = nodeCount - 1;
            result.obstructions_count = obstructionCount;
            result.distance_traveled = ((float)nodeCount - 1.0f) * FSIM_UNIT_DISTANCE_GROUND;
            result.danger_indicator_average = (obstructionCount == 0) ? 0.0f : (danger_indicator_ave / (float)obstructionCount);
            result.safe_path_proportion = 1.0f - ((float)obstructionCount / ((float)nodeCount - 1.0f));
            result.risky_path_proportion = (float)1.0f - result.safe_path_proportion;
            
            return result;
        }
        uint32_t astar(Node* start, Node* end, std::vector<Node*>* tiles, const uint32_t& totalRows, const uint32_t& totalCols, const bool& disp)
        {

            std::set<std::pair<std::pair<double, uint32_t>, Node*>> priority_queue;
            std::unordered_map<Node*, uint32_t> g_score;
            std::unordered_map<Node*, double> f_score;
            std::map<Node*, Node*> previous_node;

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
                
                // if (current == end_node){
                //     return reconstruct_path(current, start_node, previous_node, disp);
                // }
                
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

        std::map<Node*, Node*> dijkstra(Node* start, Node* end, std::vector<Node*>* tiles, const uint32_t& totalRows, const std::pair<uint32_t, uint32_t>& cols, const bool& disp, const bool& safePath)
        {
            std::set<std::pair<uint32_t, Node*>> priority_queue;
            std::unordered_map<Node*, uint32_t> g_score;
            std::map<Node*, Node*> previous_node;

            for (size_t i = 0; i < totalRows; ++i){
                for (size_t k = cols.first; k < cols.second; ++k){
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
                    if (neighbor->obstruction == true && safePath == true)
                        continue;
                        
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
                    if (!visited[tiles[row + 1][col]] && tiles[row + 1][col] != nullptr)
                    {
                        pQueue.push(tiles[row + 1][col]);
                        visited[currentNode] = true;
                    }
                }
                
                if (row  > 0)
                {
                    if (!visited[tiles[row - 1][col]] && tiles[row - 1][col] != nullptr)
                    {
                        pQueue.push(tiles[row - 1][col]);
                        visited[currentNode] = true;
                    }
                }
                if (col  < (int)totalCols - 1)
                {
                    if (!visited[tiles[row][col + 1]] && tiles[row][col + 1] != nullptr)
                    {
                        pQueue.push(tiles[row][col + 1]);
                        visited[currentNode] = true;
                    }
                }

                if (col > 0)
                {
                    if (!visited[tiles[row][col - 1]] && tiles[row][col - 1] != nullptr)
                    {
                        pQueue.push(tiles[row][col - 1]);
                        visited[currentNode] = true;
                    }
                }
                
            }

            return nullptr;
        }
        void calculateRisk(std::vector<Node*>* nodes, std::vector<FireGraphics>& fireGraphicsList, const uint32_t& totalRows, const std::pair<uint32_t, uint32_t>& cols)
        {

            for (size_t i = 0; i < totalRows; ++i)
            {
                for (size_t k = cols.first; k < cols.second; ++k)
                {
                    if (!fireGraphicsList.empty())
                    {
                        std::vector<float> distanceValues;
                        for (const auto& fireP : fireGraphicsList)
                        {
                            distanceValues.push_back(
                            std::sqrt(std::pow((float)fireP.node->col - (float)nodes[i][k]->col, (float)2.0f) + std::pow((float)fireP.node->row - (float)nodes[i][k]->row, (float)2.0f))
                            );
                        }
                        float minValue = (*std::min_element(distanceValues.begin(), distanceValues.end())) * fsim::units::UNIT_DISTANCE;
                        float indicator = 1 - (minValue  / fsim::units::STANDARD_HEAT_FLUX_RADIUS);
                        nodes[i][k]->danger_indicator = indicator > 0 ? indicator : 0.0f;
                        if (minValue > 15.0f)
                        {
                            nodes[i][k]->r = 0.0f;
                            nodes[i][k]->g = 255.0f;
                        }
                        else if (minValue <= 15.0f && minValue >= fsim::units::STANDARD_HEAT_FLUX_RADIUS)
                        {
                            nodes[i][k]->r = ((float)fsim::units::STANDARD_HEAT_FLUX_RADIUS/minValue) * (float)255.0f;
                            nodes[i][k]->g = 255.0f;
                        }
                        else
                        {
                            nodes[i][k]->r = 255.0f;
                            nodes[i][k]->g = (float)255.0f - ((1 - ((float)minValue/(float)fsim::units::STANDARD_HEAT_FLUX_RADIUS)) * (float)255.0);
                        }
                    }
                    else
                    {
                        nodes[i][k]->r = 0.0f;
                        nodes[i][k]->g = 255.0f;
                    }
                    
                    // float riskValue_ = (float)9.144f/minValue;
                    // nodes[i][k]->riskValue = riskValue_;
                }
            }  
        }
    }
}