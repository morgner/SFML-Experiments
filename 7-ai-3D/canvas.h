#ifndef CANVAS_H
#define CANVAS_H

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>    // std::swap
#include <map>    // std::swap
#include <cstdint>      // uint_32
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

struct SPoint
    {
    float x{0}, y{0};

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
    int x{0},y{0};

    bool operator == (SPosition const & lhs) const { return  ((lhs.x == x) && (lhs.y == y)); }
    bool operator != (SPosition const & lhs) const { return !(lhs == *this); }
    bool operator <  (SPosition const & lhs) const
	{
	if (lhs.x == x) return (lhs.y < y);
	return lhs.x < x;
	}
    };

struct SSize
    {
    int x{0},y{0};
    };

struct SBoard
    {
    SSize	d{5,5};				// dimension
    std::string g{"PPPPP.-.-.-.-.-.-.-.AAAAA"};	// game
    std::string b{"-X-X-X-X-X-X-X-X-X-X-X-X-"}; // board
    };

struct SStep
    {
    std::string g;
    SPosition  	f;
    SPosition  	t;
    };

using VSSteps=std::vector<SStep>;

using MPos2Pos = std::map<SPosition,SPosition>;

struct SBrainCell
    {
    SPosition  	f;
    SPosition  	t;
    long      v{0}; // value
    long      u{0}; // usage counter

    SBrainCell() = default;
    SBrainCell(SPosition const & pf, SPosition const & pt, long const & weight)
    : f(pf), t(pt), v(weight) {}

    bool operator == (SBrainCell const & lhs) const { return lhs.f == f && lhs.t == t; }
    bool operator <  (SBrainCell const & lhs) const
	{
	if ( lhs.u != 0 && u != 0 )
		return (double)lhs.v / lhs.u < (double)v / u;
	return (double)lhs.v / (lhs.u == 0)?1:lhs.u < (double)v / (u == 0)?1:u;
	}
    };

using VSBrainCells = std::vector<SBrainCell>;
using MBrain       = std::map<std::string, VSBrainCells>;

struct SMove // remove
    {
    SPosition p;
    double    v{.0}; // value
    int32_t   remove{-1};
    SMove(SPosition const & p, double const & v, int32_t const & remove):p(p),v(v),remove(remove){};
    SMove operator = (SBrainCell const & m) { p = m.t; return *this; }
    bool operator == (SBrainCell const & bc) { return p == bc.t; }
    };
inline bool operator == (SBrainCell const & bc, SMove const & m) { return m.p == bc.t; }

using VSMoves=std::vector<SMove>;

struct SPawn
    {
    SPosition p{0,0};

    template<typename T>
	SPawn(T const & x, T const & y) : p{(int)x,(int)y} {}
    };

using VSPawns=std::vector<SPawn>;

using VDrags = std::vector<std::pair<SPawn,SMove>>;


class CCanvas
    {
    public:

        CCanvas() = delete;

        CCanvas(sf::Window & ctx)
            : m_oCtx(ctx),
	      m_fTicker(&CCanvas::fTicker, this),
              m_fFigur(&CCanvas::fFigur, this)
            {
            StartLevel(5);
            m_fTicker.launch();
            m_fFigur.launch();
            m_bHasFont = m_fFont.loadFromFile("FiraSans-Regular.otf");
            m_tText.setFont(m_fFont);
            }

        virtual ~CCanvas()
            {
            m_bAnimating = false;
            m_bPAnimating = false;
            m_fTicker.wait();
            m_fFigur.wait();
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

            operator sf::Color() const
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

    public:
        bool   m_bGameOver{false};
        bool   m_bGameWon{false};
        int    m_nMines{20};
        SPoint m_tOffset{0,0};
        SPoint m_tField{23,23};

        // display lists
        GLuint const m_nFieldBlack { glGenLists(1) };
        GLuint const m_nFigurWhite { glGenLists(2) };
        GLuint const m_nFieldWhite { glGenLists(3) };
        GLuint const m_nFigurBlack { glGenLists(4) };
        GLuint const m_nFrameRed   { glGenLists(5) };
        GLuint const m_nFrameGreen { glGenLists(6) };
        GLuint const m_nFigurRed   { glGenLists(7) };
        GLuint const m_nFigurGreen { glGenLists(8) };


        int const m_nKiSleepStep{10000};
        int	  m_nKiSleep{00000};
        int       m_iActivWhite{-1};
        int       m_iActivBlack{-1};
        VSPawns   m_vPawnsWhite{};
        VSPawns   m_vPawnsBlack{};

        enum class EDirection {none,upleft,up,upup,upright,left,down,right,};
        bool	  m_bByHand{false};
        bool 	  PlayWhite(bool const & bPhase);

        void      KiLearnWhite();
        void      KiLearnBlack();

        float     m_fRotateX{0};
        float     m_fRotateY{0};
        float     m_fRotateZ{0};

        char      m_cKeyDown{0};


        MBrain m_mBrainWhite;
        MBrain m_mBrainBlack;

        void DumpBrainWhite() const { std::cout << "White Brain\n"; DumpBrain(m_mBrainWhite); }
        void DumpBrainBlack() const { std::cout << "Black Brain\n"; DumpBrain(m_mBrainBlack); }

        void DumpBrain( MBrain const & brain ) const
            {
            for ( auto const & a:brain )
                for ( auto const & b:a.second )
		    {
		    std::cout << "{" << a.first << "}"
			      << ": x " << b.f.x << ", y " << b.f.y
			      << ", X " << b.t.x << ", Y " << b.t.y
			      << ", V " << b.v   << ", u " << b.u << '\n';
		    }
            } // void DumpBrain( MBrain const & brain ) const


        bool m_bKiTest{false};
        bool m_bPhase{true};

	size_t m_nWinsWhite{0};
	size_t m_nWinsBlack{0};

        VDrags KiAddTrainingWhite(VDrags const & pm) { return KiAddTraining(pm, m_mBrainWhite); }
	VDrags KiAddTrainingBlack(VDrags const & pm) { return KiAddTraining(pm, m_mBrainBlack); }

        VDrags KiAddTraining(VDrags const & pm, MBrain const & brain)
            {
            auto const pbf =  brain.find(m_tBoard.g);
            if ( pbf == brain.end() )
        	{
                return pm;
        	}
            auto & vbc = pbf->second;
            SBrainCell bc{vbc[0].f, vbc[0].t, 0};
            SPawn      pw{bc.f.x, bc.f.y};

            std::multiset<SBrainCell> setbc{{bc}};
            for (auto const & a:pm)
        	{
        	auto const pbc = find(vbc.begin(), vbc.end(), a.second);
        	if (pbc == vbc.end())
        	    {
		    setbc.emplace(SBrainCell{a.first.p,a.second.p,0});
        	    }
        	else
		    {
		    for (auto const & b:vbc)
			{
			bc = b;
			setbc.emplace(bc);
			}
		    }
        	}
            VDrags vd;
            for ( auto const & a:setbc )
        	{
        	vd.emplace_back(std::pair{SPawn{a.f.x, a.f.y}, SMove{a.t, 0, 0}});
        	if (vd.size() > 4) break;
        	}
            return vd;
            }


        bool KiAddExpirienceWhite(VSSteps const & game, double const & weight) { return KiAddExpirience(game, weight, m_mBrainWhite); }
        bool KiAddExpirienceBlack(VSSteps const & game, double const & weight) { return KiAddExpirience(game, weight, m_mBrainBlack); }

        bool KiAddExpirience(VSSteps const & game, double const & weight, MBrain & brain)
            {
           for (auto const & a:game)
        	{
        	if ( brain.find(a.g) == brain.end() ) brain[a.g].emplace_back(SBrainCell{a.f, a.t, 0});
        	auto & vbc = brain.find(a.g)->second;
        	auto pbc = find(vbc.begin(), vbc.end(), SBrainCell{a.f, a.t, 0});
        	if (pbc == vbc.end())
        	    {
        	    vbc.emplace_back(SBrainCell{a.f, a.t, 0});
        	    pbc = find(vbc.begin(), vbc.end(), SBrainCell{a.f, a.t, 0});
        	    }
        	pbc->v += weight;
        	pbc->u++;
        	}
            return true;
            }

        void DumpBoard(size_t const & width, std::string const & sBoard) const
            {
            std::cout << '\n';
            for ( size_t is{sBoard.size()-(size_t)width}; is>0 ; is-=width )
        	{
        	std::cout << "   " << sBoard.substr(is, width) << '\n';
        	}
            }

        VSSteps m_tGameWhite;
        VSSteps m_tGameBlack;

        void DumpGame()
            {
            std::cout << "white" << '\n';
            for ( auto const & a:m_tGameWhite )
        	{
        	std::cout << "{ G: {" << a.g << "}, X: " << a.f.x << ", Y: " << a.f.y
        		                     <<  ", X: " << a.t.x << ", Y: " << a.t.y << "}" << '\n';
        	}
            std::cout << "black" << '\n';
            for ( auto const & a:m_tGameBlack )
        	{
        	std::cout << "{ G: {" << a.g << "}, X: " << a.f.x << ", Y: " << a.f.y
        		                     <<  ", X: " << a.t.x << ", Y: " << a.t.y << "}" << '\n';
        	}
            }

        SBoard m_tBoard;
        int m_nChosenGame{5};

        int MoveWhiteIA(SPawn const & p, EDirection const & e) const;

        int MoveBlack(SPawn const & p);
        int MoveWhite(SPawn const & p) const;
        VSMoves PossibleMovesWhite(SPawn const & crtPawn) const;
        VSMoves PossibleMovesBlack(SPawn const & crtPawn) const;
        VDrags DragBlack( VSPawns const & pawns );
        VDrags DragWhite( VSPawns const & pawns );

        void RecognizeBoard();



        struct SPengi
            {
            bool        bActive;
            std::string sPossibleMoves{"ijkl"};
            char        cCurrentMove{0};
            } m_tPengi;

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

        void StartLevel(int const & i);

        void Win();

        void Startsquence(char c)
            {
            if ( c == 0 ) return;
            if ( m_bPAnimating ) return;  
            if ( m_bPAniJustStopped ) return;  
            if ( std::string("ijkl").find(c) == std::string::npos ) return;  
            m_tPengi.cCurrentMove = c;
            m_bPAnimating = true;
            m_bPAniJustStopped = false;
            m_fFigur.launch();
            }

        bool  m_bPAnimating{true};
        bool  m_bPAniAutoStop{true};
        bool  m_bPAniJustStopped{false};
        float m_dPAnimate{.0};
        float m_dPAnimateStep{.025};

        void fFigur()
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
        sf::Thread m_fFigur;
    };

// CANVAS_H
#endif
