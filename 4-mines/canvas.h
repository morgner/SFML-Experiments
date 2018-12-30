#ifndef CANVAS_H
#define CANVAS_H

#include <iostream>
#include <vector>
#include <algorithm>    // std::swap
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>


struct SPoint
    {
    float x{.0}, y{0};

    SPoint() = default;

    template<typename T>
    SPoint(T const & p)
        : x((float)(p.x)), y((float)(p.y)) {}

    template<typename T>
    SPoint(T const & x, T const & y)
        : x(x), y(y) {}

    operator sf::Vector2f()
        {
        return { (float)x, (float)y };
        }

    template<typename T>
    SPoint operator = (T const & s)
        {
        x = (float)(s.x); y = (float)(s.y);
        return *this;
        }

    SPoint operator + (SPoint const & s) const
        {
        return { x + s.x, y + s.y };
        }

    SPoint operator - (SPoint const & s) const
        {
        return { x - s.x, y - s.y };
        }
    };

struct SRect
    {
    SPoint a{.0,.0}, b{.0,.0};

    template<typename T, typename U>
    SRect(T const & s, U const & p)
        : a{(float)(s.x),(float)(s.y)}, b{(float)(p.x),(float)(p.y)}
        {
        if ( a.x < 0 ) { b.x += a.x; a.x = -a.x; }
        if ( a.y < 0 ) { b.y += a.y; a.y = -a.y; }
        }

    template<typename T, typename U>
    SRect(T const & w, T const & h, U const & x, U const & y)
        : a{(float)w,(float)h}, b{(float)x,(float)y}
        {
        if ( a.x < 0 ) { b.x += a.x; a.x = -a.x; }
        if ( a.y < 0 ) { b.y += a.y; a.y = -a.y; }
        }

    bool Inside(SPoint const & p) const
        {
        return ( ((a.x >= 0) && (p.x >= b.x && p.x <= (b.x + a.x )))
                &&
                 ((a.y >= 0) && (p.y >= b.y && p.y <= (b.y + a.y ))));
        }
    };

using VSRect=std::vector<SRect>;

struct STile
    {
    SRect loc;
    sf::Sprite sprite;
    bool mine{false};
    bool mark{false};
    bool on{true};
    int  count{0};
    };

using VSTile=std::vector<STile>;

struct SMouse
    {
    bool   d{false}; // is down
    SPoint p{.0,.0}; // = event.mouseButton

    SMouse(bool const & b, SPoint const & m)
        : d(b), p(m) {}

    template<typename T>
    SPoint operator = (T const & s)
        {
        p = s;
        return *this;
        }
    };


class CCanvas
    {
    public:

        CCanvas() = delete;

        CCanvas(sf::RenderWindow & ctx)
            : m_fTicker(&CCanvas::fTicker, this),
              m_oCtx(ctx)
            {
            m_fTicker.launch();
            m_bHasFont = m_fFont.loadFromFile("/usr/share/texlive/texmf-dist/fonts/opentype/public/fira/FiraSans-Regular.otf");
            m_tText.setFont(m_fFont);
            }

        virtual ~CCanvas()
            {
            m_bAnimating = false;
            m_fTicker.wait();
            }

        void OnDraw();

        sf::Vector2f Mouse(sf::Vector2i const & p)
            {
            // convert it to world coordinates
            return m_tMousePos = m_oCtx.mapPixelToCoords(p);
            }

        struct FillColor
            {
            sf::Uint8 r{255},g{255},b{255};

            FillColor(sf::Uint8 const & r,sf::Uint8 const & g,sf::Uint8 const & b)
                : r(r),g(g),b(b) {}

            operator sf::Color()
                {
                if ( m_bHasFocus )
                    {
                    return {r,g,b};
                    }
                else
                    {
                    sf::Uint8 c = ((float)r+g+b)/3;
                    return {c,c,c};
                    }
                } 
            };

        bool Collision(SPoint const & tPoint);
        void Event(sf::Event const & event);

        void DoButtonAction(int const & n);

    public:
        bool   m_bShowField{false};
        bool   m_bGameOver{false};
        bool   m_bGameWon{false};
        int    m_nMines{20};
        SPoint m_tOffset{0,60};
        SPoint m_tField{50,50};
        SPoint m_tBoard{14,12};

        static bool m_bHasFocus;
        bool m_bHasFont{false};

        sf::Text m_tText;
        sf::Font m_fFont;

        VSRect m_vDrawing{};
        VSTile m_vTiles{};
        VSRect m_vButtons{};
        std::string m_sButCmds{"asdfASDF"};

        sf::RenderWindow & m_oCtx;
        SPoint m_tMousePos;

        SMouse m_tMouseEventLeft  {false, {.0,.0}};
        SMouse m_tMouseEventMiddle{false, {.0,.0}};
        SMouse m_tMouseEventRight {false, {.0,.0}};

        struct SCollision
            {
            SPoint tWhere  { .0,.0 };
            SPoint tOffset { .0,.0 };
            enum class EWhat
                {
                none,       // there was no collision
                Drawing,    // move a Drawing primitive
                Line,       // move a Line
                Button,     // hit a button
                Tile,       // hit a tile
                }eWhat {EWhat::none};
            int nIndex {0};     // O: L1, L2, L3
            int nSubIx {0};     // L: P1, PM, P2
            } m_tCollision;

        bool  m_bAnimating{true};
        float m_dAnimate{.0};
        float m_dAnimateStep{.001};

        void fTicker()
            {
            while (m_bAnimating)
                {
                static auto const m_dAnimateSleep = sf::milliseconds(10);
                m_dAnimate = (m_dAnimate > 1.-m_dAnimateStep) ? 0 : m_dAnimate + m_dAnimateStep;
                sf::sleep(m_dAnimateSleep);
                }
            }
        sf::Thread m_fTicker;
    };

// CANVAS_H
#endif
