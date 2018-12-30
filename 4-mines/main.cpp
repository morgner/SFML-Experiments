#include "canvas.h"

int main()
    {
    sf::ContextSettings settings;
                        settings.antialiasingLevel = 14;

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(800,600), "SFML window");
//                   window.create(sf::VideoMode::getDesktopMode(), "SFML window", sf::Style::Fullscreen);
                     window.setFramerateLimit(60);

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
//                oCanvas.m_bPovRec = true;
                oCanvas.OnDraw();
                }
            oCanvas.Event(event);
            }
        oCanvas.Mouse (sf::Mouse::getPosition(window));
        oCanvas.OnDraw();
        }
    }
