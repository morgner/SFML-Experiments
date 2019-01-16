#ifndef CANVAS_H
#define CANVAS_H

#include <iostream>
#include <vector>
#include <algorithm>    // std::swap
#include <cstdint>      // uint_32
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

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

    operator sf::Vector2f() const
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

struct SPosition
    {
    uint32_t x{0},y{0};
    };

struct SMove
    {
    SPosition p;
    double    value{.0};
    int32_t   remove{-1};
    SMove(SPosition const & p, double const & value, int32_t const & remove):p(p),value(value),remove(remove){};
    };

using VSMoves=std::vector<SMove>;

struct SBoard
    {
    SPosition   tTotalDimension{4,4};

    std::string sSituation {"PPPP"
                            "-X-X"
                            "X-X-"
                            "AAAA"};

    std::string sDecoration{"-X-X"
                            "X-X-"
                            "-X-X"
                            "X-X-"};
    };

struct SPawn
    {
    SPosition p;
    };

using VSPawns=std::vector<SPawn>;


class CCanvas
    {
    public:

        CCanvas() = delete;

        CCanvas(sf::Window & ctx)
            : m_oCtx(ctx),
	      m_fTicker(&CCanvas::fTicker, this),
              m_fPengi(&CCanvas::fPengi, this)
            {
            StartLevel(0);
            m_fTicker.launch();
            m_fPengi.launch();
            m_bHasFont = m_fFont.loadFromFile("FiraSans-Regular.otf");
            m_tText.setFont(m_fFont);
            }

        virtual ~CCanvas()
            {
            m_bAnimating = false;
            m_bPAnimating = false;
            m_fTicker.wait();
            m_fPengi.wait();
            }

        void OnDraw();

        sf::Vector2f Mouse(sf::Vector2i const & p)
            {
            // convert it to world coordinates
            return m_tMousePos; // = m_oCtx.mapPixelToCoords(p);
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
        SPoint m_tOffset{0,0};
        SPoint m_tField{23,23};

        // display lists
        GLuint block{0};
        GLuint pengi{0};
        GLuint monst{0};
        GLuint chest{0};
        GLuint walle{0};
        GLuint winnr{0};
        GLuint activ{0};

        uint32_t iActiv{0};
        VSPawns vPawns{};

        float  m_fRotateX{0};
        float  m_fRotateY{0};
        float  m_fRotateZ{0};

        char   m_cKeyDown{0};

        enum class EDirection
            {
            upleft,
            up,
            upup,
            upright,
            left,
            down,
            right,
            };

        int Move(SBoard const & b, SPawn const & p, CCanvas::EDirection const & e) const;

        VSMoves PossibleMoves(SBoard const & crtBoard, SPawn const & crtPawn) const;
        bool Drag(SBoard const & crtBoard, SPawn const & crtPawn);
        void DrawBoard(SBoard const & crtBoard);


        struct SPengi
            {
            std::string sPossibleMoves{"ijkl"};
            char        cCurrentMove{0};
            } m_tPengi;

        SPoint m_tBoard{0,0};
        std::string maze = "";
        std::string mzbg = "";


        static bool m_bHasFocus;
        bool m_bHasFont{false};

        sf::Text m_tText;
        sf::Font m_fFont;

        VSRect m_vDrawing{};

        sf::Window & m_oCtx;
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
                }eWhat {EWhat::none};
            int nIndex {0};     // O: L1, L2, L3
            int nSubIx {0};     // L: P1, PM, P2
            } m_tCollision;

        bool  m_bAnimating{true};
        float m_dAnimate{.0};
        float m_dAnimateStep{.015};

        void fTicker()
            {
            while (m_bAnimating)
                {
                static auto const m_dAnimateSleep = sf::milliseconds(10);
                m_dAnimate = (m_dAnimate > 1.-m_dAnimateStep) ? 0 : m_dAnimate + m_dAnimateStep;
                sf::sleep(m_dAnimateSleep);
                }
            }

        void StartLevel(int const i);

        void Win();

        int Move(std::string const & sWorld,
                 SPoint const      & tBoard,
                 int const         & nWorldIndex,
                 std::string const & allowedFields, /*"-XA"*/
                 std::string const & hasToMove,     /*"A"*/
                 CCanvas::EDirection const & e,
                 bool const bRecursion = true);

        void Startsquence(char c)
            {
            if ( c == 0 ) return;
            if ( m_bPAnimating ) return;  
            if ( m_bPAniJustStopped ) return;  
            if ( std::string("ijkl").find(c) == std::string::npos ) return;  
            m_tPengi.cCurrentMove = c;
            m_bPAnimating = true;
            m_bPAniJustStopped = false;
            m_fPengi.launch();
            }

        bool  m_bPAnimating{false};
        bool  m_bPAniAutoStop{true};
        bool  m_bPAniJustStopped{false};
        float m_dPAnimate{.0};
        float m_dPAnimateStep{.025};

        void fPengi()
            {
            while (m_bPAnimating)
                {
                static auto const m_dPAnimateSleep = sf::milliseconds(05);
                m_dPAnimate += m_dPAnimateStep;
                if ( m_bPAniAutoStop && m_dPAnimate >= 1 )
                    {
                    m_dPAnimate        = .0;
                    m_bPAnimating      = false;
                    m_bPAniJustStopped = true;
                    }
                sf::sleep(m_dPAnimateSleep);
                }
            }

        sf::Thread m_fTicker;
        sf::Thread m_fPengi;
    };

// CANVAS_H
#endif
