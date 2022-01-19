#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace fsim
{
    namespace Controller
    {
        extern bool mouseDown;

        namespace
        {
            extern const sf::Vector2i windowMode;

            extern const std::vector<float> zoomValues;

            extern sf::Vector2f tempViewCenterPos;
            
            extern sf::Vector2f tempMousePosition;
            
            extern bool mouseDragStatus;

            extern uint32_t mouseValue;

        }

        void enableMouseDrag();

        void autoAdjustView(sf::View& mapView, sf::RenderWindow* window);

        void zoomEvent(const int& mouseWheelDelta, sf::View& mapView, sf::RenderWindow* window);

        void keyboardEvent(sf::View& view, sf::RenderWindow* window);

        void dragEvent(sf::View& mapView, sf::RenderWindow* window);

    }
}