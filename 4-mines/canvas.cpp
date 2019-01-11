#include "canvas.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <fstream>
#include <math.h>
#include <random>


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
    int i{0};
    for ( auto const & a:m_vButtons )
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

    m_tCollision.eWhat  = SCollision::EWhat::none;

    i = 0;
    SPoint p = tPoint - SPoint{m_tOffset.x, m_tOffset.y};
    for ( auto & a:m_vTiles )
        {
        if ( Inside(p, a.loc) )
            {
//          std::cout << "Tile " << i << std::endl; 
            m_tCollision.tWhere  = tPoint;
            m_tCollision.tOffset = tPoint - a.loc.a;
            m_tCollision.eWhat   = SCollision::EWhat::Tile;
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
        if (event.key.shift)
            {
            switch ('a'+event.key.code)
                {
                case 'a': break;
                case 's': m_bShowField = !m_bShowField; break;
                case 'd': break;
                case 'f': break;
                case 'h': break;
                case 'j': break;
                case 'k': break;
                case 'l': break;
                case 'n': m_vTiles.clear(); m_bGameOver = false; break;
                case 'm': m_vTiles.clear(); ++m_nMines; if ( m_nMines > m_tBoard.x * m_tBoard.y -2 ) --m_nMines; break;
                case 'w': m_bGameWon = true; for ( auto & a:m_vTiles ) if (a.mine) a.mark=true; else a.on=false; break;
                }
            }
        else
            {
            switch ('a'+event.key.code)
                {
                case 'a': break;
                case 's': m_bShowField = !m_bShowField; break;
                case 'd': break;
                case 'f': break;
                case 'h': break;
                case 'j': break;
                case 'k': break;
                case 'l': break;
                case 'n': m_vTiles.clear(); m_bGameOver = false; break;
                case 'm': m_vTiles.clear(); --m_nMines; if ( m_nMines <= 1 ) m_nMines = 2; break;
                }
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
            if ( m_tCollision.eWhat == SCollision::EWhat::Tile )
                {       
                if ( !m_vTiles[m_tCollision.nIndex].mark && !m_bGameOver && !m_bGameWon)
                    {
                    m_vTiles[m_tCollision.nIndex].on = false;
                    if ( m_vTiles[m_tCollision.nIndex].mine ) m_bGameOver = true;
                    }
                }
            }
        if (event.mouseButton.button == sf::Mouse::Right)
            {
            m_tMouseEventRight.d = false;
            m_vDrawing.emplace_back( m_tMouseEventRight.p - m_tMousePos, m_tMousePos );

            if ( m_tCollision.eWhat == SCollision::EWhat::Tile )
                {       
                m_vTiles[m_tCollision.nIndex].mark = !m_vTiles[m_tCollision.nIndex].mark;
                }
            }
        }
    }


void CCanvas::DoButtonAction(int const & n)
    {
    switch (n)
        {
        case 0: break;
        case 1: break;
        case 2: break;
        case 3: break;
        case 4: break;
        case 5: break;
        case 6: break;
        case 7: break;
        }
    }

bool CCanvas::m_bHasFocus{true}; 

void CCanvas::OnDraw()
    {
    m_tText.setFont(m_fFont);
    m_tText.setCharacterSize(24); // in pixels, not points!
    m_tText.setFillColor(FillColor{0,0,0});

    auto s = m_oCtx.getSize();
    // clear the m_oCtx
    m_oCtx.clear({255,255,212});

/*
    // show mouse position
    static auto const r{7.f};
    sf::CircleShape shape(r);
    shape.move(m_tMousePos.x-r,m_tMousePos.y-r);
    shape.setFillColor(FillColor(100, 250, 50));
    m_oCtx.draw(shape);
*/
    bool b = Collision(m_tMousePos);

/*
    // initiate buttons
    if ( 0 == m_vButtons.size() )
        {
        size_t max = m_sButCmds.length();
        float w{50}, h{50}, b{8};
        for ( int i{0}; i < max; ++i )
            {
            m_vButtons.emplace_back( SRect{w,h, i*(w+b)+b, b} );
            }
        }
*/

    // initiate tiles
    static sf::Texture texture;
    if ( (0 == m_vTiles.size()) && texture.loadFromFile("texture.jpeg") )
        {
        sf::Sprite sprite;
        float w{m_tField.x}, h{m_tField.y}, b{2};
        m_tBoard = {14,10};
        m_tOffset = {30,40};
        for ( int y{0}; y < m_tBoard.y; ++y )
            {
            for ( int x{0}; x < m_tBoard.x; ++x )
                {
                sprite.setTexture(texture);
                sprite.setTextureRect(sf::IntRect(m_tField.x*x, m_tField.y*y, m_tField.x, m_tField.y));
                sprite.setPosition(sf::Vector2f( m_tOffset.x+(m_tField.x+2)*x, m_tOffset.y+(m_tField.y+2)*y));
                m_vTiles.emplace_back(
                    STile{
                        SRect{w,h, (float)x*(w+b)+b, (float)y*(w+b)+b},
                        sprite,
                        false,
                        false,
                        true,
                        0
                        }
                    );
                }
            }
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> distribution(0,m_vTiles.size());
        int mines{0};
        while ( mines < m_nMines )
            {
            int number = (int)distribution(mt);
            if ( (number > 0) && (number < m_vTiles.size()) && !m_vTiles[number].mine )
                {
                ++mines;
//                  std::cout << "mine " << number << std::endl; 
                m_vTiles[number].mine = true;
                }
            }
        int i{0}, n{0};
        int m{(int)m_vTiles.size()};
        for ( auto & a:m_vTiles )
            {
            a.count = 0;
            n = i-m_tBoard.x-1; a.count += ( i%(int)m_tBoard.x == 0     ) ? 0 : ( n < 0 ) ? 0 : m_vTiles[ n ].mine ? 1 : 0 ; // lo
            n = i-m_tBoard.x  ; a.count += ( false                      ) ? 0 : ( n < 0 ) ? 0 : m_vTiles[ n ].mine ? 1 : 0 ; //  o
            n = i-m_tBoard.x+1; a.count += ( (i+1)%(int)m_tBoard.x == 0 ) ? 0 : ( n < 0 ) ? 0 : m_vTiles[ n ].mine ? 1 : 0 ; // ro
            n = i           -1; a.count += ( i%(int)m_tBoard.x == 0     ) ? 0 : ( n < 0 ) ? 0 : m_vTiles[ n ].mine ? 1 : 0 ; // l
            n = i           +1; a.count += ( (i+1)%(int)m_tBoard.x == 0 ) ? 0 : ( n > m ) ? 0 : m_vTiles[ n ].mine ? 1 : 0 ; // r
            n = i+m_tBoard.x-1; a.count += ( i%(int)m_tBoard.x == 0     ) ? 0 : ( n > m ) ? 0 : m_vTiles[ n ].mine ? 1 : 0 ; // lu
            n = i+m_tBoard.x  ; a.count += ( false                      ) ? 0 : ( n > m ) ? 0 : m_vTiles[ n ].mine ? 1 : 0 ; //  u
            n = i+m_tBoard.x+1; a.count += ( (i+1)%(int)m_tBoard.x == 0 ) ? 0 : ( n > m ) ? 0 : m_vTiles[ n ].mine ? 1 : 0 ; // ru
            ++i;
            }
        }

    // show board
    m_bGameWon = true;
    for ( auto a:m_vTiles )
        {
        if ( a.on && !a.mine ) m_bGameWon = false;
        if ( a.on && !m_bShowField )
            {
            if ( a.mark )
                a.sprite.setColor(sf::Color(112, 112, 255, 200));
            else
                a.sprite.setColor(sf::Color(255, 255, 255, 255));
            m_oCtx.draw(a.sprite); // 827,656 else
            }
        else if ( a.mine )
            {
            static auto const r{12.f};
            sf::CircleShape shape(r);
            shape.move(m_tOffset.x+m_tField.x/2-r+a.loc.b.x,
                       m_tOffset.y+m_tField.y/2-r+a.loc.b.y);
            shape.setFillColor(FillColor(0,0,0));
            m_oCtx.draw(shape);
            }
        else
            {
            m_tText.setCharacterSize(36); // in pixels, not points!
            m_tText.setFillColor(FillColor{0,0,0});
            m_tText.setString(std::to_string(a.count));
            m_tText.setOrigin(-(m_tOffset.x+m_tField.x/2+a.loc.b.x-m_tText.getLocalBounds().width/2),
                              -(m_tOffset.y+m_tField.y/2+a.loc.b.y-m_tText.getLocalBounds().height/2-8));
            m_oCtx.draw(m_tText);
            }
        m_tText.setString("Mines: " + std::to_string(m_nMines) + " - (s,W,m,M,n)");
        m_tText.setCharacterSize(24); // in pixels, not points!
        m_tText.setFillColor(FillColor{0,0,0});
        m_tText.setOrigin(-8, -8);
        m_oCtx.draw(m_tText);
        }
    if ( m_bGameWon )
        {
        m_tText.setString("gewonnen");
        }
    if ( m_bGameOver )
        {
        m_tText.setString("verloren");
        for ( auto a:m_vTiles )
            {
            if ( a.on && a.mine )
                {
                static auto const r{12.f};
                sf::CircleShape shape(r);
                shape.move(m_tOffset.x+m_tField.x/2-r+a.loc.b.x,
                           m_tOffset.y+m_tField.y/2-r+a.loc.b.y);
                shape.setFillColor(FillColor(96,0,0));
                m_oCtx.draw(shape);
                }
            }
        }
    if ( m_bGameWon || m_bGameOver )
        {
        m_tText.setCharacterSize(112); // in pixels, not points!
        m_tText.setFillColor(FillColor{0,0,0});
        m_tText.setStyle(sf::Text::Bold);

        sf::RectangleShape rect(sf::Vector2f(m_tText.getLocalBounds().width+16,
                                             m_tText.getLocalBounds().height+16));
        rect.move((s.x-m_tText.getLocalBounds().width)/2,
                  (s.y-m_tText.getLocalBounds().height)/2);
        rect.setFillColor(FillColor(250,250,250));
        rect.setOutlineColor(FillColor(0, 0, 0));
        rect.setOutlineThickness(2.f);
        m_oCtx.draw(rect);

        m_tText.setOrigin(-(s.x-m_tText.getLocalBounds().width)/2,
                          -(s.y-m_tText.getLocalBounds().height)/2+16);
        m_oCtx.draw(m_tText);

        if ( m_bGameOver )
            m_tText.setFillColor(FillColor{96,0 ,0});
        else
            m_tText.setFillColor(FillColor{0,96,0});
        m_tText.setStyle(sf::Text::Regular);
        m_oCtx.draw(m_tText);
        }
    else
        {
        // draw animation ...
        sf::RectangleShape line(sf::Vector2f(s.x, 1.f));
        line.move(0, s.y*m_dAnimate);
        line.setFillColor(FillColor(50, 50, 250));
        m_oCtx.draw(line);
        }


    // draw buttons ...
    if ( m_vButtons.size() )
        {
        m_tText.setCharacterSize(36); // in pixels, not points!
        m_tText.setFillColor(FillColor{0,0,0});

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

            m_tText.setString((char)(m_sButCmds[i]));
            m_tText.setOrigin(i*-58 -25-m_tText.getLocalBounds().width/2 , -8);
            m_oCtx.draw(m_tText);

            rect.move(a.a.x+8,0);
            ++i;
            }
        }


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

