#include "canvas.h"
#include <chrono>
#include <ctime>
#include <sstream>
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
//          std::cout << "bar " << i << std::endl; 
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
//          std::cout << "object " << i << std::endl; 
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
        if (event.mouseButton.button == sf::Mouse::Left)
            {
            m_tMouseEventLeft = 
                SMouse{true, 
                       m_oCtx.mapPixelToCoords(
                            sf::Vector2i{event.mouseButton.x,
                                         event.mouseButton.y})};
            }
        if (event.mouseButton.button == sf::Mouse::Right)
            {
            m_tMouseEventRight = 
                SMouse{true, 
                       m_oCtx.mapPixelToCoords(
                            sf::Vector2i{event.mouseButton.x,
                                         event.mouseButton.y})};
            }
        }

    if (event.type == sf::Event::MouseWheelScrolled)
        {
//        auto v (m_oCtx.getView());
//        v.zoom( (event.mouseWheelScroll.delta > 0.0f) ? .9f : 1.1f );
//        m_oCtx.setView(v);
        }

    if (event.type == sf::Event::MouseButtonReleased)
        {
        m_tMousePos = event.mouseButton;
        if (event.mouseButton.button == sf::Mouse::Left)
            {
            m_tMouseEventLeft.d = false;
            if ( m_tCollision.eWhat  == SCollision::EWhat::Button )
                {
                DoButtonAction( m_tCollision.nIndex );
                }
            }
        if (event.mouseButton.button == sf::Mouse::Right)
            {
            m_tMouseEventRight.d = false;
            m_vDrawing.emplace_back( m_tMouseEventRight.p - m_tMousePos, m_tMousePos );
            }
        }
    }


void CCanvas::DoButtonAction(int const & n)
    {
    switch (n)
        {
        case 0: std::cout << (char)(n+'a') << '\n'; break;
        case 1: std::cout << (char)(n+'a') << '\n'; break;
        case 2: std::cout << (char)(n+'a') << '\n'; break;
        case 3: std::cout << (char)(n+'a') << '\n'; break;
        case 4: std::cout << (char)(n+'a') << '\n'; break;
        case 5: std::cout << (char)(n+'a') << '\n'; break;
        case 6: std::cout << (char)(n+'a') << '\n'; break;
        case 7: std::cout << (char)(n+'a') << '\n'; break;
        }
    }

bool CCanvas::m_bHasFocus{true}; 

void CCanvas::OnDraw()
    {
    m_tText.setFont(m_fFont);
    m_tText.setCharacterSize(24); // in pixels, not points!
    m_tText.setFillColor(FillColor{0,0,0});
//    m_tText.setStyle(sf::Text::Bold | sf::Text::Underlined);

    auto s = m_oCtx.getSize();
    // clear the m_oCtx
    m_oCtx.clear(sf::Color::White);

    // show mouse position
    static auto const r{7.f};
    sf::CircleShape shape(r);
    shape.move(m_tMousePos.x-r,m_tMousePos.y-r);
    shape.setFillColor(FillColor(100, 250, 50));
    m_oCtx.draw(shape);

    bool b = Collision(m_tMousePos);

    // initiate buttons
    if ( 0 == m_vButtons.size() )
        {
        float w{50}, h{50}, b{8};
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
            m.setFillColor(FillColor(50, 50, 250));
        else
            m.setFillColor(FillColor(250, 50, 50));
        m_oCtx.draw(m);
        }
    if (m_tMouseEventLeft.d)
        {
//        auto v (m_oCtx.getView());
//        v.move({ m_tMouseEventLeft.p.x-m_tMousePos.x,
//                 m_tMouseEventLeft.p.y-m_tMousePos.y });
//        m_oCtx.setView(v);
        }

    // draw everything ...
    for (auto const & a:m_vDrawing)
        {
        sf::RectangleShape rect(sf::Vector2f(a.a.x, a.a.y));
        rect.move(a.b.x,a.b.y);
        rect.setFillColor(FillColor(150, 250, 150));
        m_oCtx.draw(rect);
        }
    if ( m_tCollision.eWhat  == SCollision::EWhat::Drawing )
        {
        auto a = m_vDrawing[m_tCollision.nIndex];
        sf::RectangleShape rect(sf::Vector2f(a.a.x, a.a.y));
        rect.move(a.b.x,a.b.y);
        rect.setFillColor(FillColor(250,250,250));
        rect.setOutlineColor(FillColor(0, 0, 0));
        rect.setOutlineThickness(2.f);
        m_oCtx.draw(rect);

        rect.setSize({a.a.x, 64});
        rect.setFillColor(FillColor(225,225,225));
        rect.setOutlineThickness(0.f);
        m_oCtx.draw(rect);

        rect.setSize({a.a.x, 32});
        rect.setFillColor(FillColor(200,200,200));
        rect.setOutlineColor(FillColor(0, 0, 0));
        rect.setOutlineThickness(2.f);
        m_oCtx.draw(rect);

        rect.setFillColor(FillColor(250, 250, 50));
        rect.setOutlineColor(FillColor(0, 0, 0));
        rect.setOutlineThickness(2.f);
        rect.setSize({10,10});
        rect.move( a.a.x/2-5, a.a.y/2-5 );

        // middle handles
        rect.move( a.a.x/2,       0 ); m_oCtx.draw(rect);
        rect.move( - a.a.x,       0 ); m_oCtx.draw(rect);
        rect.move( a.a.x/2, a.a.y/2 ); m_oCtx.draw(rect);
        rect.move(       0, - a.a.y ); m_oCtx.draw(rect);
/*
        // corner handles
        rect.move( a.a.x/2, a.a.y/2 ); m_oCtx.draw(rect);
        rect.move( - a.a.x,       0 ); m_oCtx.draw(rect);
        rect.move(       0, - a.a.y ); m_oCtx.draw(rect);
        rect.move( + a.a.x,       0 ); m_oCtx.draw(rect);
*/
        m_tText.setString("File  Edit  View  Search  Help");
        m_tText.setCharacterSize(20); // in pixels, not points!
        m_tText.setFillColor(FillColor{0,0,0});
        m_tText.setOrigin(-a.b.x-2,-a.b.y-34);
        m_oCtx.draw(m_tText);
        }

    // draw buttons ...
    if ( m_vButtons.size() )
        {
        m_tText.setCharacterSize(36); // in pixels, not points!
        m_tText.setFillColor(FillColor{0,0,0});
//        m_tText.setStyle(sf::Text::Bold);

        sf::RectangleShape rect(sf::Vector2f(m_vButtons[0].a.x, m_vButtons[0].a.y));
        rect.move(8,8);
        int i{0};
        for (auto const & a:m_vButtons)
            {
            if ( m_tCollision.eWhat  == SCollision::EWhat::Button &&
                 m_tCollision.nIndex == i )
                if ( m_tMouseEventLeft.d )
                    rect.setFillColor(FillColor( 50, 200, 200));
                else
                    rect.setFillColor(FillColor(150, 250, 150));

            else
                rect.setFillColor(FillColor(150, 150, 150));
            m_oCtx.draw(rect);

            m_tText.setString((char)(i+'a'));
            m_tText.setOrigin(i*-58 -25-m_tText.getLocalBounds().width/2 , -8);
            m_oCtx.draw(m_tText);

            rect.move(a.a.x+8,0);
            ++i;
            }
        }

    // draw animation ...
    sf::RectangleShape line(sf::Vector2f(s.x/4, 25.f));
    line.move(s.x/2,s.y/2);
    line.rotate(360.f * m_dAnimate);
    line.setFillColor(FillColor(50, 50, 250));
    m_oCtx.draw(line);

    auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); 
    std::stringstream ss;
    ss << ctime(&timenow);

    m_tText.setString(ss.str());
    m_tText.setCharacterSize(24); // in pixels, not points!
    m_tText.setFillColor(FillColor{0,0,250});
//    m_tText.setStyle(sf::Text::Bold | sf::Text::Underlined);
//    m_tText.setOrigin(s.x-m_tText.getLocalBounds().width, 30);
    m_tText.setOrigin(m_tText.getLocalBounds().width-s.x+8, -8);
    m_oCtx.draw(m_tText);

    // end the current frame
    m_oCtx.display();

    }

