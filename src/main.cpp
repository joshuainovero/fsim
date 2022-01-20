#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1366, 768), "Window", sf::Style::Fullscreen);
    ImGui::SFML::Init(window);
    
    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            ImGui::SFML::Update(window, deltaClock.restart());

            ImGui::Begin("Window Title");
            ImGui::Text("Window text!");
            ImGui::End();   

            window.clear();
            ImGui::SFML::Render(window);
            window.display();
        }
    }
    ImGui::SFML::Shutdown();
    return 0;
}