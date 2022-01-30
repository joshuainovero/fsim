#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui-SFML.h"
#include "Node.hpp"

namespace fsim
{
    struct StartingPoints
    {
        StartingPoints();
        ~StartingPoints();

        sf::CircleShape point;      // Point circle UI
        std::string     label;      // Label of a point
        ImVec4          point_rgba; // RGBA values of a point

        Node* node = nullptr; // Node pointer

        char buffer[7] = ""; // Char buffer of label

    };  
}