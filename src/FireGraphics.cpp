#include "FireGraphics.hpp"
#include "Units.hpp"

namespace fsim
{
    FireGraphics::FireGraphics(Node* fireSourceNode, const FloorLabel& floor, sf::Texture* iconTexture,  const float& heatFluxValue_)
        : node(fireSourceNode), heatFluxValue(heatFluxValue_)
    {

        // if (floor == FloorLabel::GROUND)
        // {
            sprite.setTexture(*iconTexture);
            sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x/2.0f, sprite.getTexture()->getSize().y/2.0f));
            sprite.setScale(0.1f, 0.1f);
            area.setRadius(fsim::units::STANDARD_HEAT_FLUX_RADIUS_PIXELS);
            area.setFillColor(sf::Color(255,115, 0, 90.0f));
            area.setOutlineThickness(0.7f);
            area.setOutlineColor(sf::Color::Black);
            area.setOrigin(sf::Vector2f(area.getRadius(), area.getRadius()));
            const float& unit_s = fsim::units::UNIT_SIZE_IN_PIXELS;
            const float t_half_size = unit_s/2.0f;
            area.setPosition(sf::Vector2f(fireSourceNode->col * unit_s + t_half_size, fireSourceNode->row * unit_s + t_half_size));   
            sprite.setPosition(sf::Vector2f(fireSourceNode->col * unit_s + t_half_size, fireSourceNode->row * unit_s + t_half_size));   
        // }
    }

    FireGraphics::~FireGraphics() {}

    void FireGraphics::draw(sf::RenderWindow* window)
    {
        window->draw(area);
        window->draw(sprite);
    }
}