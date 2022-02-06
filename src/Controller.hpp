#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace fsim
{
    namespace Controller
    {
        extern const std::vector<float> zoomValues; // Constant mouse zoom multipliers
        // extern uint32_t                 mouseValue; // Current mouse value

        extern bool mouseDown; // Determines if mouse is on click
        
        extern const sf::Vector2i windowMode; // Constant window mode of the app

        extern sf::Vector2f tempViewCenterPos; // Stores the previous center view of the map
        extern sf::Vector2f tempMousePosition; // Stores the previous mouse position
        extern bool mouseDragStatus;           // Determines if the mouse is on drag

        // Enables mouse drag navigation
        void enableMouseDrag();

        // Auto adjust the view when zooming out of scope
        void autoAdjustView(sf::View& mapView, sf::RenderWindow* window);

        // Event that handles scroll zooms
        void zoomEvent(const int& mouseWheelDelta, sf::View& mapView, sf::RenderWindow* window, uint32_t& mouseValue);

        // Event that handles keyboard navigation
        void keyboardEvent(sf::View& view, sf::RenderWindow* window);

        // Event that handles drag navigation
        void dragEvent(sf::View& mapView, sf::RenderWindow* window);

    }
}