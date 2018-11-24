#include "canvas.h"
#include <math.h>


template<typename P, typename T>
    float Distance( P const & a, T const & b )
        {
        return sqrt( pow((a.x-b.x),2) + pow((a.y-b.y),2) );
        }

template<typename P, typename R>
    bool Inside( P const & a, R const & b )
        {
        return b.Inside(a);
        }

bool CCanvas::Collision(SPoint const & tPoint)
    {
    m_tCollision.eWhat  = SCollision::EWhat::none;

    int i{0};
    for ( auto & a:m_vButtons )
        {
        if ( Inside(tPoint, a) )
            {
            std::cout << "bar " << i << std::endl; 
            m_tCollision.tWhere  = tPoint;
            m_tCollision.tOffset = tPoint - a.a;
            m_tCollision.eWhat   = SCollision::EWhat::Button;
            m_tCollision.nIndex  = i;
            return std::move(true);
            }
        ++i;
        }

    i = 0;
    for ( auto & a:m_vDrawing )
        {
        if ( Inside(tPoint, a) )
            {
            std::cout << "object " << i << std::endl; 
            m_tCollision.tWhere  = tPoint;
            m_tCollision.tOffset = tPoint - a.a;
            m_tCollision.eWhat   = SCollision::EWhat::Drawing;
            m_tCollision.nIndex  = i;
            return std::move(true);
            }
        ++i;
        }
    return false;
    }


void CCanvas::Event(sf::Event const & event)
    {
    if (event.type == sf::Event::LostFocus)
        { m_bHasFocus = false; }

    if (event.type == sf::Event::GainedFocus)
        { m_bHasFocus = true; }

    if (event.type == sf::Event::KeyPressed)
        {
        std::cout << "a key was pressed" << std::endl;
        std::cout << "    code:" << event.key.code << ' ' 
                  << (char)('a'+event.key.code) << std::endl;
        std::cout << "    control:" << event.key.control << std::endl;
        std::cout << "    alt:" << event.key.alt << std::endl;
        std::cout << "    shift:" << event.key.shift << std::endl;
        std::cout << "    system:" << event.key.system << std::endl;
        if (event.key.code == sf::Keyboard::Escape)
            {
            }
        }

    if (event.type == sf::Event::MouseButtonPressed)
        {
        if (event.mouseButton.button == sf::Mouse::Right)
            {
            m_tMouseEventRight = 
                SMouse{true, 
                       m_oCtx.mapPixelToCoords(
                            sf::Vector2i{event.mouseButton.x,
                                         event.mouseButton.y})};
            }
        }

    if (event.type == sf::Event::MouseButtonReleased)
        {
        m_tMousePos = event.mouseButton;
        if (event.mouseButton.button == sf::Mouse::Right)
            {
            m_tMouseEventRight.d = false;
            m_vDrawing.emplace_back( m_tMouseEventRight.p - m_tMousePos, m_tMousePos );
            }
        }
    }

void CCanvas::OnDraw()
    {
    auto s = m_oCtx.getSize();
    // clear the m_oCtx
    m_oCtx.clear(sf::Color::White);

    // show mouse position
    static auto const r{7.f};
    sf::CircleShape shape(r);
    shape.move(m_tMousePos.x-r,m_tMousePos.y-r);
    shape.setFillColor(sf::Color(100, 250, 50));
    m_oCtx.draw(shape);

    bool b = Collision(m_tMousePos);

    // initiate buttons
    if ( 0 == m_vButtons.size() )
        {
        float w{50}, h{50}, b{5};
        for ( int i{0}; i < 8; ++i )
            {
            m_vButtons.emplace_back( SRect{w,h, i*(w+b)+b, b} );
            }
        }

    // show mouse position
    if (m_tMouseEventRight.d)
        {
        sf::RectangleShape m(sf::Vector2f(m_tMouseEventRight.p.x-m_tMousePos.x,
                                          m_tMouseEventRight.p.y-m_tMousePos.y));
        m.move(m_tMousePos.x,m_tMousePos.y);
        if (m_bHasFocus)
            m.setFillColor(sf::Color(50, 50, 250));
        else
            m.setFillColor(sf::Color(250, 50, 50));
        m_oCtx.draw(m);
        }

    // draw everything ...
    for (auto const & a:m_vDrawing)
        {
        sf::RectangleShape rect(sf::Vector2f(a.a.x, a.a.y));
        rect.move(a.b.x,a.b.y);
        rect.setFillColor(sf::Color(50, 250, 50));
        m_oCtx.draw(rect);
        }
    if ( m_tCollision.eWhat  == SCollision::EWhat::Drawing )
        {
        auto a = m_vDrawing[m_tCollision.nIndex];
        sf::RectangleShape rect(sf::Vector2f(a.a.x, a.a.y));
        rect.move(a.b.x,a.b.y);
        rect.setFillColor(sf::Color(250, 50, 50));
        m_oCtx.draw(rect);
        }

    // draw buttons ...
    int i{0};
    for (auto const & a:m_vButtons)
        {
        sf::RectangleShape rect(sf::Vector2f(a.a.x, a.a.y));
        rect.move(a.b.x,a.b.y);
        if ( m_tCollision.eWhat  == SCollision::EWhat::Button &&
             m_tCollision.nIndex == i )
            rect.setFillColor(sf::Color(150, 250, 150));
        else
            rect.setFillColor(sf::Color(150, 150, 150));
        m_oCtx.draw(rect);
        ++i;
        }

    // draw animation ...
    sf::RectangleShape line(sf::Vector2f(s.x/4, 25.f));
    line.move(s.x/2,s.y/2);
    line.rotate(360.f * m_dAnimate);
    if (m_bHasFocus)
        line.setFillColor(sf::Color(50, 50, 250));
    else
        line.setFillColor(sf::Color(150, 150, 150));
    m_oCtx.draw(line);

    // end the current frame
    m_oCtx.display();
    }

