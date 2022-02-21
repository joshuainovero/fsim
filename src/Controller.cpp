#include "Controller.hpp"
#include <iostream>
namespace fsim
{
    namespace Controller
    {
        const std::vector<float> zoomValues { 1.0f, 0.95f, 0.90f, 0.85f, 0.80f, 0.75, 0.70, 0.65f, 0.60f, 0.55f, 0.50f, 0.45f, 0.40f, 0.35f, 0.30f};

        // uint32_t mouseValue = 4;

        bool mouseDown = false;


        const sf::Vector2i windowMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
        
        sf::Vector2f tempViewCenterPos;

        sf::Vector2f tempMousePosition;

        bool mouseDragStatus;
        

        void enableMouseDrag() { mouseDragStatus = true; }

        void autoAdjustView(sf::View& mapView, sf::RenderWindow* window)
        {
            if (window->mapPixelToCoords(windowMode).x - mapView.getSize().x < 0)
                mapView.move(std::abs(window->mapPixelToCoords(windowMode).x - mapView.getSize().x), 0);
            
            if (window->mapPixelToCoords(windowMode).x > 1366)
                mapView.move(-(window->mapPixelToCoords(windowMode).x - 1366), 0);

            if (window->mapPixelToCoords(windowMode).y - mapView.getSize().y < 0)
                mapView.move(0, std::abs(window->mapPixelToCoords(windowMode).y - mapView.getSize().y));

            if (window->mapPixelToCoords(windowMode).y > 768)
                mapView.move(0, -(window->mapPixelToCoords(windowMode).y - 768));
        }

        void zoomEvent(const int& mouseWheelDelta, sf::View& mapView, sf::RenderWindow* window, uint32_t& mouseValue)
        {
            if (mouseWheelDelta > 0 && mouseValue < zoomValues.size() - 1)
                mouseValue += 1;
            else if (mouseWheelDelta < 0 && mouseValue > 0)
                mouseValue -= 1;

            sf::Vector2f mapSize(1366.0f * zoomValues[mouseValue], 768.0f * zoomValues[mouseValue]);

            mapView.setSize(sf::Vector2f(mapSize.x, mapSize.y));
            window->setView(mapView);
            autoAdjustView(mapView, window);

        }

        void keyboardEvent(sf::View& view, sf::RenderWindow* window)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                if (window->mapPixelToCoords(windowMode).x - view.getSize().x > 0)
                    view.move(sf::Vector2f(-0.5f, 0.0f));
            }


            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                if (window->mapPixelToCoords(windowMode).x < 1366)
                    view.move(sf::Vector2f(0.5f, 0.0f));
            }


            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                if (window->mapPixelToCoords(windowMode).y - view.getSize().y > 0)
                    view.move(sf::Vector2f(0.0f, -0.5f));
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                if (window->mapPixelToCoords(windowMode).y < 768)
                    view.move(sf::Vector2f(0.0f, 0.5f));
            }
        }

        void dragEvent(sf::View& mapView, sf::RenderWindow* window)
        {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (!mouseDown)
            {
                tempMousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
                tempViewCenterPos = mapView.getCenter();
                mouseDown = true;
            }
            mapView.setCenter(tempViewCenterPos.x, tempViewCenterPos.y);
            window->setView(mapView);
            float differenceX = window->mapPixelToCoords(sf::Mouse::getPosition(*window)).x - tempMousePosition.x;
            float differenceY = window->mapPixelToCoords(sf::Mouse::getPosition(*window)).y - tempMousePosition.y;
            mapView.setCenter(tempViewCenterPos.x - differenceX, tempViewCenterPos.y - differenceY);
            window->setView(mapView);
            autoAdjustView(mapView, window);
            window->setView(mapView);

        }
        else
            mouseDown = false;
        }
    }
}