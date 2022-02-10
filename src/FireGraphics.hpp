#pragma once
#include <SFML/Graphics.hpp>
#include "Node.hpp"

enum FloorLabel { GROUND = 0, SECOND = 1, THIRD = 2, FOURTH = 3 };

namespace fsim
{
    class FireGraphics
    {
    public:
        FireGraphics(Node* fireSourceNode, const FloorLabel& floor, sf::Texture* iconTexture);
        ~FireGraphics();

        void draw(sf::RenderWindow* window);

    public:
        Node* node;

    private:
        sf::Sprite sprite;
        sf::CircleShape area;
    };
}