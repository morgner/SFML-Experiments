#include "canvas.h"

int main()
    {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    window.setFramerateLimit(60); // call it once, after creating the window

    CCanvas oCanvas{window};

    while (window.isOpen())
        {
        sf::Event event;
        while (window.pollEvent(event))
            {
            if (event.type == sf::Event::Closed)
                {
                window.close();
                }
            if (event.type == sf::Event::Resized)
                {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                }
            oCanvas.Event(event);
            }
        oCanvas.Mouse (sf::Mouse::getPosition(window));
        oCanvas.OnDraw();
        }
    }
