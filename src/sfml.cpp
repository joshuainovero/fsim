#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1366,728), "Window", sf::Style::Fullscreen);

    while (window.isOpen())
    {   
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.display();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();
    }

    return 0;
}