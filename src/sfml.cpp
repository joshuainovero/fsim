#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(500,500), "Window", sf::Style::Close);

    sf::Color color = sf::Color(0.0f, 255.0f, 0.0f, 255.0f);
    std::cout << color.r << " " << color.g << " " << color.b << " " << color.a << std::endl;
    sf::RectangleShape ex;
    ex.setSize(sf::Vector2f(100.0f, 100.0f));
    ex.setFillColor(color);
    while (window.isOpen())
    {   
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(ex);
        window.display();
    }

    return 0;
}