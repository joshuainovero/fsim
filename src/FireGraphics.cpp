#include "FireGraphics.hpp"

namespace fsim
{
    FireGraphics::FireGraphics(Node* fireSourceNode, const FloorLabel& floor, sf::Texture* iconTexture)
        : node(fireSourceNode)
    {

        if (floor == FloorLabel::GROUND)
        {
            sprite.setTexture(*iconTexture);
            sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x/2.0f, sprite.getTexture()->getSize().y/2.0f));
            sprite.setScale(0.1f, 0.1f);
            area.setRadius(56.32184847f);
            area.setFillColor(sf::Color(255,115, 0, 90.0f));
            area.setOrigin(sf::Vector2f(area.getRadius(), area.getRadius()));
            const float t_half_size = 3.415f/2.0f;
            area.setPosition(sf::Vector2f(fireSourceNode->col * 3.415f + t_half_size, fireSourceNode->row * 3.415f + t_half_size));   
            sprite.setPosition(sf::Vector2f(fireSourceNode->col * 3.415f + t_half_size, fireSourceNode->row * 3.415f + t_half_size));   
        }
    }

    FireGraphics::~FireGraphics() {}

    void FireGraphics::draw(sf::RenderWindow* window)
    {
        window->draw(area);
        window->draw(sprite);
    }
}