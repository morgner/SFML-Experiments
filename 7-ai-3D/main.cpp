#include "canvas.h"


int main()
{
    // create the window
    sf::Window window(sf::VideoMode(1600, 1200), "SFML Demo: AI 3D", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(60);

    window.setActive(true);

    CCanvas oCanvas{window};

    bool running = true;
    while (running)
        {
        sf::Event event;
        while (window.pollEvent(event))
            {
            if (event.type == sf::Event::Closed)
                {
                running = false;
                }
            else if (event.type == sf::Event::Resized)
                {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
                }
            oCanvas.Event(event);
            }
        oCanvas.Mouse (sf::Mouse::getPosition(window));
        oCanvas.OnDraw();
        }
    return 0;
    }


