#include "Map.hpp"
#include "Controller.hpp"
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1366.0f, 768.0f), "Window", sf::Style::Fullscreen);
    
    // 400
    sf::Vector2i windowMode(1366, 768);


    fsim::Map map(400, "../resource/mapscaled2.png", "../MapData/floor1", &window);
    while (window.isOpen())
    {

        window.setView(map.mapView);

        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseWheelMoved)
            {
                fsim::Controller::zoomEvent(event.mouseWheel.delta, map.mapView, &window);
            }
        }
        

        fsim::Controller::keyboardEvent(map.mapView, &window);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        window.clear(sf::Color::White);

        window.draw(map.mapSprite);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2u position = map.clickPosition(worldPos);
            map.nodes[position.x][position.y]->setObstruction();
            map.initVertexArray();
        }

        // fsim::Controller::dragEvent(map.mapView, &window);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            sf::Vector2u position = map.clickPosition(worldPos);
            map.nodes[position.x][position.y]->reset();
            map.initVertexArray();
        }


        window.draw(*map.nodePositions);
        window.display();

    }
    return 0;
}