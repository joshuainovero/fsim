#include "StartingPoints.hpp"

namespace fsim
{
    StartingPoints::StartingPoints(sf::Texture* targetTexture)
        : point_rgba(255.0f, 0.0f, 0.0f, 255.0f)
    {
        point.setRadius(4.0f);
        point.setOrigin(sf::Vector2f(point.getRadius(), point.getRadius()));
        point.setOutlineThickness(0.7f);
        point.setOutlineColor(sf::Color::Black);
        targetSprite.setTexture(*targetTexture);
        targetSprite.setOrigin(sf::Vector2f(targetSprite.getTexture()->getSize().x/2.0f, targetSprite.getTexture()->getSize().y/2.0f));
        targetSprite.setScale(sf::Vector2f(0.1f, 0.1f));
    }

    StartingPoints::~StartingPoints() {}


}