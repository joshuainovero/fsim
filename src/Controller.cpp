#include "Controller.hpp"
#include <iostream>
namespace fsim
{
    namespace Controller
    {
        const std::vector<float> zoomValues { 1.0f, 0.95f, 0.90f, 0.85f, 0.80f, 0.75, 0.70, 0.65f, 0.60f, 0.55f, 0.50f, 0.45f, 0.40f, 0.35f, 0.30f};

        // uint32_t mouseValue = 4;

        bool mouseDown = false;


        // const sf::Vector2i windowMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
        
        sf::Vector2f tempViewCenterPos;

        sf::Vector2f tempMousePosition;

        bool mouseDragStatus;
        

        void enableMouseDrag() { mouseDragStatus = true; }

        void autoAdjustView(sf::View& mapView, sf::RenderWindow* window)
        {
            if (window->mapPixelToCoords((sf::Vector2i)window->getSize()).x - mapView.getSize().x < 0)
                mapView.move(std::abs(window->mapPixelToCoords((sf::Vector2i)window->getSize()).x - mapView.getSize().x), 0);
            
            if (window->mapPixelToCoords((sf::Vector2i)window->getSize()).x > screenRef.x)
                mapView.move(-(window->mapPixelToCoords((sf::Vector2i)window->getSize()).x - screenRef.x), 0);

            if (window->mapPixelToCoords((sf::Vector2i)window->getSize()).y - mapView.getSize().y < 0)
                mapView.move(0, std::abs(window->mapPixelToCoords((sf::Vector2i)window->getSize()).y - mapView.getSize().y));

            if (window->mapPixelToCoords((sf::Vector2i)window->getSize()).y > screenRef.y)
                mapView.move(0, -(window->mapPixelToCoords((sf::Vector2i)window->getSize()).y - screenRef.y));
        }

        void zoomEvent(const int& mouseWheelDelta, sf::View& mapView, sf::RenderWindow* window, uint32_t& mouseValue)
        {
            if (mouseWheelDelta > 0 && mouseValue < zoomValues.size() - 1)
                mouseValue += 1;
            else if (mouseWheelDelta < 0 && mouseValue > 0)
                mouseValue -= 1;

            sf::Vector2f mapSize(screenRef.x * zoomValues[mouseValue], screenRef.y * zoomValues[mouseValue]);

            mapView.setSize(sf::Vector2f(mapSize.x, mapSize.y));
            window->setView(mapView);
            autoAdjustView(mapView, window);

        }

        void keyboardEvent(sf::View& view, sf::RenderWindow* window)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                if (window->mapPixelToCoords((sf::Vector2i)window->getSize()).x - view.getSize().x > 0)
                    view.move(sf::Vector2f(-0.5f, 0.0f));
            }


            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                if (window->mapPixelToCoords((sf::Vector2i)window->getSize()).x < 1366)
                    view.move(sf::Vector2f(0.5f, 0.0f));
            }


            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                if (window->mapPixelToCoords((sf::Vector2i)window->getSize()).y - view.getSize().y > 0)
                    view.move(sf::Vector2f(0.0f, -0.5f));
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                if (window->mapPixelToCoords((sf::Vector2i)window->getSize()).y < 768)
                    view.move(sf::Vector2f(0.0f, 0.5f));
            }
        }

        void dragEvent(sf::View& mapView, sf::RenderWindow* window, const sf::Vector2f& mapPixelCoords)
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (!mouseDown)
                {
                    tempMousePosition = mapPixelCoords;
                    tempViewCenterPos = mapView.getCenter();
                    mouseDown = true;
                }
                mapView.setCenter(tempViewCenterPos.x, tempViewCenterPos.y);
                window->setView(mapView);
                float delta_x = window->mapPixelToCoords(sf::Mouse::getPosition(*window)).x - tempMousePosition.x;
                float delta_y = window->mapPixelToCoords(sf::Mouse::getPosition(*window)).y - tempMousePosition.y;
                mapView.setCenter(tempViewCenterPos.x - delta_x, tempViewCenterPos.y - delta_y);
                window->setView(mapView);
                autoAdjustView(mapView, window);
                window->setView(mapView);

            }
            else
                mouseDown = false;
        }
    }
}