#pragma once
#include <SFML/Graphics.hpp>
#include "Node.hpp"
#include "Constants.hpp"

// enum FloorLabel { GROUND = 0, SECOND = 1, THIRD = 2, FOURTH = 3 };

namespace fsim
{
    class FireGraphics
    {
    public:
        FireGraphics(Node* fireSourceNode, const FloorLabel& floor, sf::Texture* iconTexture, const float& heatFluxValue_);
        ~FireGraphics();

        // Draws the sprite and the danger area
        void draw(sf::RenderWindow* window);

    public:
        Node* node = nullptr; // Fire point node
        float heatFluxValue;

    private:
        sf::Sprite      sprite; // Fire icon
        sf::CircleShape area;   // Graphical representation of danger area
    };
}