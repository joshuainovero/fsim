#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui-SFML.h"
#include "Node.hpp"

namespace fsim
{
    class StartingPoints
    {
    public:
        StartingPoints();

        ~StartingPoints();

        ImVec4 point_rgba;

        std::string label;

        Node* node = nullptr;

        sf::CircleShape point;

        char buffer[7] = "";


    };  
}