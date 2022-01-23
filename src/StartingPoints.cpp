#include "StartingPoints.hpp"

namespace fsim
{
    StartingPoints::StartingPoints()
        : point_rgba(255.0f, 0.0f, 0.0f, 255.0f)
    {
        point.setRadius(4.0f);
        point.setOrigin(sf::Vector2f(point.getRadius(), point.getRadius()));
    }

    StartingPoints::~StartingPoints() {}


}