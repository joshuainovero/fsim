#include "Toolbar.hpp"
#include <iostream>

namespace fsim
{
    Toolbar::Toolbar(const std::string& position, const sf::Color& color)
    {

        if (position == "left")
        {
            bar.setSize(sf::Vector2f(42.0f, 756.0f));
            bar.setPosition(sf::Vector2f(0.0f, 18.0f));
            bar.setFillColor(color);
        }
        toolStartPos = s_pos.y + toolGap;

    }

    void Toolbar::draw(sf::RenderWindow* window)
    {
        window->draw(bar);
        window->draw(toolSelectedBar);
        for (const auto& toolTex : toolTextures)
            window->draw(toolTex.second);
    }

    void Toolbar::triggerEvents(const sf::Vector2f& mapPixelCoords)
    {

        for (int i = 0; i < 10; ++i)
        {
            if (mapPixelCoords.x >= iconRangeCoords[i].x1 && mapPixelCoords.x <= iconRangeCoords[i].x2 &&
                mapPixelCoords.y >= iconRangeCoords[i].y1 && mapPixelCoords.y <= iconRangeCoords[i].y2
            )
            {
                std::cout << "Shit" << std::endl;
                toolSelectedBar.setPosition(sf::Vector2f(0.0f, (s_pos.y + (toolGap/2.0f)) + (toolSelectedBar.getSize().y * (float)i)        ));   
                break;  
            }
        }
    }

    Toolbar::~Toolbar(){}

    void Toolbar::AddTool(const std::string& imagePath)
    {
        toolTextures[counter].first.loadFromFile(imagePath);
        toolTextures[counter].first.setSmooth(true);
        toolTextures[counter].second.setTexture(toolTextures[counter].first);
        toolTextures[counter].second.scale(0.23f, 0.23f);

        const sf::Vector2f spriteSize(
        toolTextures[counter].second.getTexture()->getSize().x * toolTextures[counter].second.getScale().x,
        toolTextures[counter].second.getTexture()->getSize().y * toolTextures[counter].second.getScale().y);

        if (spriteHeight == 0.0f)
        {
            spriteHeight = spriteSize.y;

            toolSelectedBar.setSize(sf::Vector2f(42.0f, (toolGap) + spriteHeight));
            toolSelectedBar.setFillColor(sf::Color(145.0f, 145.0f, 145.0f, 255.0f));
            toolSelectedBar.setPosition(sf::Vector2f(0.0f, (s_pos.y + (toolGap/2.0f)) + (toolSelectedBar.getSize().y * counter)        ));
        }

        iconRangeCoords[counter].x1 = 0.0f;
        iconRangeCoords[counter].x2 = 42.0f;
        iconRangeCoords[counter].y1 = (s_pos.y + (toolGap/2.0f)) + (toolSelectedBar.getSize().y * counter);
        iconRangeCoords[counter].y2 = iconRangeCoords[counter].y1 + toolSelectedBar.getSize().y;

        if (counter == 0)
            toolTextures[counter].second.setPosition(sf::Vector2f(s_pos.x + 3.0f, toolStartPos));
        else
            toolTextures[counter].second.setPosition(sf::Vector2f(s_pos.x + 3.0f, toolStartPos + ((spriteSize.y + toolGap) * (counter)) ));

        counter++;
    }

}