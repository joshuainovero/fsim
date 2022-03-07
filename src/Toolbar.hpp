#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <vector>
#include "RangeCoordinates.hpp"

namespace fsim
{
    class Toolbar
    {
    public:
        Toolbar(const std::string& position, const sf::Color& color);
        ~Toolbar();

        void AddTool(const std::string& imagePath);
        void draw(sf::RenderWindow* window);
        void triggerEvents(const sf::Vector2f& mapPixelCoords);

        std::pair<sf::Texture, sf::Sprite> toolTextures[10];
        RangeCoordinates iconRangeCoords[10];


    private:
        sf::RectangleShape bar;
        sf::RectangleShape toolSelectedBar;
        inline static const sf::Vector2f s_pos = sf::Vector2f(6.0f, 18.0f);
        inline static const float toolGap = 10.0f;
        float toolStartPos;
        size_t counter = 0;
        float spriteHeight = 0.0f;
    };
}