#include "canvas.h"
#include "elements.h"

#include <chrono>
#include <ctime>
#include <sstream>
#include <fstream>
#include <math.h>
#include <random>
#include <GL/glu.h>
#include <unistd.h> // usleep
#include <random>
#include <utility>


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
    return false;
    }


void CCanvas::Event(sf::Event const & event)
    {
    if (event.type == sf::Event::LostFocus)
        { m_bHasFocus = false; }

    if (event.type == sf::Event::GainedFocus)
        { m_bHasFocus = true; }

    if (event.type == sf::Event::KeyReleased)
        {
        m_cKeyDown = 0; // m_bPAniAutoStop = true;
        }

    if (event.type == sf::Event::KeyPressed)
        {
        m_cKeyDown = 'a'+event.key.code; // m_bPAniAutoStop = false;

        if (event.key.shift)
            {
	    switch (event.key.code)
		{
		case 73: break; // up
		case 74: break; // down
		case 71: break; // left
		case 72: break; // right
		case 60: m_iActivWhite = (m_iActivWhite == 0) ? m_vPawnsWhite.size()-1 : m_iActivWhite-1; break; // tab
		default:
		    std::cout << event.key.code << ", " << m_cKeyDown << '\n';
		}

            switch (m_cKeyDown)
                {
                case 'a': m_iActivBlack = (m_iActivBlack == 0) ? m_vPawnsBlack.size()-1 : m_iActivBlack-1; break;
                case 's': m_nKiSleep += m_nKiSleepStep; break;
                case 'd': break;
                case 'e': DumpBrainWhite(); break;
                case 'f': m_bKiTest=true; break;
                case 'g':
                case 'h': m_bByHand = !m_bByHand; break;
                case 'u': 
                case 'i': if ( m_cKeyDown == 'i' ) m_cKeyDown='I';
                case 'j':
                case 'o': 
		case 'k': 
                case 'l': m_tPengi.cCurrentMove=m_cKeyDown; break;
                case 'n': StartLevel(1); break;
                case 'm': break;
                case 'w': Win(); break;
                case 'x': ++m_fRotateX; break;
                case 'y': ++m_fRotateY; break;
                case 'z': ++m_fRotateZ; break;
                }
            }
        else
            {
	    switch (event.key.code)
		{
		case 27: StartLevel(1); break;
		case 28: StartLevel(2); break;
		case 29: StartLevel(3); break;
		case 30: StartLevel(4); break;
		case 31: StartLevel(5); break;
		case 32: StartLevel(6); break;
		case 33: StartLevel(7); break;
		case 34: StartLevel(8); break;
		case 35: StartLevel(9); break;

		case 73: break; // up
		case 74: break; // down
		case 71: break; // left
		case 72: break; // right
		case 60: ++m_iActivWhite; if (m_iActivWhite >= (int)m_vPawnsWhite.size()) m_iActivWhite = 0; break; // tab
		default:
		    std::cout << event.key.code << ", " << m_cKeyDown << '\n';
		}

	    switch (m_cKeyDown)
                {
                case 'a': ++m_iActivBlack; if (m_iActivBlack >= (int)m_vPawnsBlack.size()) m_iActivBlack = 0; break;
                case 's': if (m_nKiSleep > m_nKiSleepStep) m_nKiSleep -= m_nKiSleepStep; break;
                case 'd': DumpGame(); break;
                case 'e': DumpBrainBlack(); break;
                case 'f': m_bKiTest=false; break;
                case 'g':
                case 'h': m_bByHand = !m_bByHand; break;
                case 'u': 
                case 'i': 
                case 'j': 
                case 'o': 
		case 'k': 
                case 'l': m_tPengi.cCurrentMove=m_cKeyDown; break;
                case 'n': StartLevel(0); break;
                case 'm': break;
                case 'x': --m_fRotateX; break;
                case 'y': --m_fRotateY; break;
                case 'z': --m_fRotateZ; break;
                case 'r': m_fRotateX = 0.f; m_fRotateY = 0.f; m_fRotateZ = 0.f; break;
                }
            }
        }
    }


void CCanvas::StartLevel(int const & i)
    {
//    std::cout << "--------------------\n";
    m_tGameWhite.clear();
    m_tGameBlack.clear();
    if ( m_nChosenGame != i )
	{
	m_nWinsWhite=0;
	m_nWinsBlack=0;
	m_mBrainWhite.clear();
	m_mBrainBlack.clear();
	}
    m_nChosenGame = i;
    m_bPhase = true;

    m_tOffset = {25,50};
    switch (i)
        {
	case 7: m_tBoard = {{7,7},
		{"PPPPPPPPPPPPPP.....................AAAAAAAAAAAAAA"},
		{"-X-X-X-X-X-X-X-X-X-X-X-X-X-X-X-X-X-X-X-X-X-X-X-X-"}}; break;

	case 6: m_tBoard = {{6,6},
		{"PPPPPP........................AAAAAA"},
		{"-X-X-XX-X-X--X-X-XX-X-X--X-X-XX-X-X-"}}; break;

	case 5: m_tOffset = {37.5,62.5};
		m_tBoard = {{5,5},
		{"PPPPP...............AAAAA"},
		{"-X-X-X-X-X-X-X-X-X-X-X-X-"}}; break;

        case 4:	m_tOffset = {50,50};
		m_tBoard = {{4,4},
		{"PPPP........AAAA"},
		{"-X-XX-X--X-XX-X-"}}; break;

        case 3:	m_tOffset = {50,50};
		m_tBoard = {{3,3},
		{"PPP...AAA"},
		{"-X-X-X-X-"}}; break;

        case 9: m_tOffset = { 0, 25 };
		m_tBoard = {{8,8},
		{"-.-.P.-..-.-.-.--.-.-.-..-.-.-.--.-.-.-..-.-.-.--.-AAA-.AA.-.-.-"},
		{"-X-X-X-XX-X-X-X--X-X-X-XX-X-X-X--X-X-X-XX-X-X-X--X-X-X-XX-X-X-X-"}}; break;

        default:m_tBoard = {{6,6},
        	{"-X-XPXX-X-X--X-X-XX-XAX--X-X-XX-X-X-"},
		{"-X-X-XX-X-X--X-X-XX-X-X--X-X-XX-X-X-"}}; break;
        }
    }

int CCanvas::MoveWhiteIA(SPawn const & p, CCanvas::EDirection const & e) const
    {
    int i{0}, i0{(int)p.p.x + (int)m_tBoard.d.x * (int)p.p.y};
    switch ( e )
        {
	case CCanvas::EDirection::none   :                               break;
        case CCanvas::EDirection::upleft : i = i0   -1 +  m_tBoard.d.x;  break;
        case CCanvas::EDirection::up     : i = i0      +  m_tBoard.d.x;  break;
        case CCanvas::EDirection::upup   : i = i0      +2*m_tBoard.d.x;  break;
        case CCanvas::EDirection::upright: i = i0   +1 +  m_tBoard.d.x;  break;
        case CCanvas::EDirection::left   : i = i0   -1                ;  break;
        case CCanvas::EDirection::down   : i = i0      -  m_tBoard.d.x;  break;
        case CCanvas::EDirection::right  : i = i0   +1                ;  break;
        }

    VSMoves pm = PossibleMovesWhite(p);

    for ( auto const & a:pm )
        {
        if ( a.p.x + m_tBoard.d.x * a.p.y == i )
            {
            return i;
            }
        }
    return i0;
    }

void CCanvas::Win()
    {
    int ic{0};
    for ( auto const & a:m_tBoard.b )
        {
	m_tBoard.g[ic] = (/* m_tBoard.b[ic] */ a == 'X') ? 'A' : m_tBoard.b[ic];
        ++ic;
        }
    }


std::random_device rd;
std::mt19937 mt(rd());
int CCanvas::MoveBlack(SPawn const & p)
    {
    int i{0},i0{(int)p.p.x + (int)m_tBoard.d.x * (int)p.p.y};

    VSMoves pm = PossibleMovesBlack(p);

    for (auto const & a:pm)
	std::cout << " + " << a.p.x << ", " << a.p.y;
    std::cout << '\n';

    if ( ! pm.size() ) return i0;

    std::uniform_int_distribution<int> distribution(0,pm.size());
    do
	{
	i = (int)distribution(mt);
	} while ( (i < 0) || (i >= (int)pm.size()) );


    return pm[i].p.x + pm[i].p.y * m_tBoard.d.x;
    }

int CCanvas::MoveWhite(SPawn const & p) const
    {
    int i{0},i0{(int)p.p.x + (int)m_tBoard.d.x * (int)p.p.y};

    VSMoves pm = PossibleMovesWhite(p);

    for (auto const & a:pm)
	std::cout << " + " << a.p.x << ", " << a.p.y;
    std::cout << '\n';

    if ( ! pm.size() ) return i0;

    std::uniform_int_distribution<int> distribution(0,pm.size());
    do
	{
	i = (int)distribution(mt);
	} while ( (i < 0) || (i >= (int)pm.size()) );


    return pm[i].p.x + pm[i].p.y * m_tBoard.d.x;
    }

VSMoves CCanvas::PossibleMovesWhite(SPawn const & p) const
    {
    VSMoves tMoves{};
    int  y = m_tBoard.d.x;	// width of the board
    int  l = m_tBoard.g.length();	// stellung
    int  i = p.p.x + p.p.y * y;		// input position

    int  id = i + y;
    if ( ( id < l ) && ( m_tBoard.g[id] != 'A' ) && ( m_tBoard.g[id] != 'P' ) )
        {
        tMoves.emplace_back( SPosition{id%y, id/y}, 0.0, -1 );

        id = i + 2*y;
        if ( ( id < l ) && ( i < y ) && ( m_tBoard.g[id] != 'A' ) && ( m_tBoard.g[id] != 'P' ) )
            { tMoves.emplace_back( SPosition{id%y, id/y}, 0.0, -1 ); }
        }

    id = i + y -1;
    if ( ( id < l ) && ( i%y != 0 ) && ( m_tBoard.g[id] == 'A' ) )
        { tMoves.emplace_back( SPosition{id%y, id/y}, 0.0, -1 ); }

    id = i + y +1;
    if ( ( id < l ) && ( (i+1)%y != 0 ) && ( m_tBoard.g[id] == 'A' ) )
        { tMoves.emplace_back( SPosition{id%y, id/y}, 0.0, -1 ); }

    return std::move(tMoves);
    }

VSMoves CCanvas::PossibleMovesBlack(SPawn const & p) const
    {
    VSMoves tMoves{};
    int  y = m_tBoard.d.x;	// width of the board
    int  l = m_tBoard.g.length();	// stellung
    int  i = p.p.x + p.p.y * y;		// input position


    int  id = i - y;
    if ( ( id >= 0 ) && ( m_tBoard.g[id] != 'A' ) && ( m_tBoard.g[id] != 'P' ) )
        {
        tMoves.emplace_back( SPosition{id%y, id/y}, 0.0, -1 );

        id = i - 2*y;
        if ( ( id >= 0 ) && ( i >= l - y ) && ( m_tBoard.g[id] != 'A' )  && ( m_tBoard.g[id] != 'P' ) )
            { tMoves.emplace_back( SPosition{id%y, id/y}, 0.0, -1 ); }
        }

    id = i - y -1;
    if ( ( id >= 0 ) && ( i%y != 0 ) && ( m_tBoard.g[id] == 'P' ) )
        { tMoves.emplace_back( SPosition{id%y, id/y}, 0.0, -1 ); }

    id = i - y +1;
    if ( ( id >= 0 ) && ( (i+1)%y != 0 ) && ( m_tBoard.g[id] == 'P' ) )
        { tMoves.emplace_back( SPosition{id%y, id/y}, 0.0, -1 ); }

    return std::move(tMoves);
    }

using VDrags = std::vector<std::pair<SPawn,SMove>>;

VDrags CCanvas::DragBlack( VSPawns const & pawns )
    {
    VDrags zugs; 	// ALL possible black moves
    for ( auto const & a:m_vPawnsBlack )	// for all pawns
	{
	auto pm = PossibleMovesBlack( a );
	for ( auto const & b:pm )		// all moves
	    {
	    zugs.emplace_back( std::pair{a,b} ); // pawn,newPos
	    }
	}
    return std::move(zugs);
    }

VDrags CCanvas::DragWhite( VSPawns const & pawns )
    {
    VDrags zugs; 	// ALL possible White moves
    for ( auto const & a:m_vPawnsWhite )	// for all pawns
	{
	auto pm = PossibleMovesWhite( a );
	for ( auto const & b:pm )		// all moves
	    {
	    zugs.emplace_back( std::pair{a,b} ); // pawn,newPos
	    }
	}
    return std::move(zugs);
    }

void CCanvas::RecognizeBoard()
    {
    m_vPawnsWhite.clear();
    m_vPawnsBlack.clear();

    for ( int i{0},y{0}; y < m_tBoard.d.y; ++y )
        {
        for ( int x{0}; x < m_tBoard.d.x; ++x )
            {
            glPushMatrix();
            glTranslatef( m_tOffset.x+(m_tField.x+5)*x, m_tOffset.y+(m_tField.y+5)*y, -00.f);
            if ( m_tBoard.b[i] == 'X' ) glCallList(m_nFieldWhite); else glCallList(m_nFieldBlack);
            if ( m_tBoard.g[i] == 'P' )
        	{
        	if ( (int)m_vPawnsWhite.size() == m_iActivWhite ) glCallList(m_nFigurGreen); else glCallList(m_nFigurWhite);
        	m_vPawnsWhite.emplace_back(x,y);
        	}
            if ( m_tBoard.g[i] == 'A' )
        	{
        	if ( (int)m_vPawnsBlack.size() == m_iActivBlack ) glCallList(m_nFigurRed); else glCallList(m_nFigurBlack);
        	m_vPawnsBlack.emplace_back(x,y);
        	}
            glPopMatrix();
            ++i;
            }
        }
    }

bool CCanvas::PlayWhite(bool const & bPhase)
    {
    RecognizeBoard();
    if ( m_iActivWhite < 0 ) m_iActivWhite=0;

    VDrags zugs{ DragWhite(m_vPawnsWhite) };
    if ( !zugs.size() )
	{
	RecognizeBoard();
	VDrags zb{ DragBlack(m_vPawnsBlack) };
	VDrags zw{ DragWhite(m_vPawnsWhite) };
	if ( m_vPawnsWhite.size() > m_vPawnsBlack.size() )
	    {
	    m_nWinsWhite++;
	    std::cout << m_bKiTest << " " << m_nWinsWhite << ":" << m_nWinsBlack << "  +++ White wins\n";
	    KiAddExpirienceBlack(m_tGameBlack, -1);
	    KiAddExpirienceWhite(m_tGameWhite,  1);
	    }
	if ( m_vPawnsWhite.size() == m_vPawnsBlack.size() )
	    {
	    std::cout << m_bKiTest << " " << m_nWinsWhite << ":" << m_nWinsBlack << "  ::: no win\n";
	    KiAddExpirienceBlack(m_tGameBlack, 0);
	    KiAddExpirienceWhite(m_tGameWhite, 0);
	    }
	if ( m_vPawnsWhite.size() < m_vPawnsBlack.size() )
	    {
	    m_nWinsBlack++;
	    std::cout << m_bKiTest << " " << m_nWinsWhite << ":" << m_nWinsBlack << "  --- Black wins\n";
	    KiAddExpirienceBlack(m_tGameBlack,  1);
	    KiAddExpirienceWhite(m_tGameWhite, -1);
	    }
	StartLevel(m_nChosenGame);
	RecognizeBoard();
	m_iActivWhite=0;
	}


    if ( (m_vPawnsWhite.size() <= (size_t)m_iActivWhite) ) m_iActivWhite = m_vPawnsWhite.size()-1;
    auto & tPawn = m_vPawnsWhite[m_iActivWhite];

    EDirection eDir;
    switch ( m_tPengi.cCurrentMove )
	{
	case 'u': eDir = CCanvas::EDirection::upleft;   break;
	case 'i': eDir = CCanvas::EDirection::up;       break;
	case 'I': eDir = CCanvas::EDirection::upup;     break;
	case 'o': eDir = CCanvas::EDirection::upright;  break;
	case 'j': eDir = CCanvas::EDirection::left;     break;
	case 'k': eDir = CCanvas::EDirection::down;     break;
	case 'l': eDir = CCanvas::EDirection::right;    break;
	}
    m_tPengi.cCurrentMove=0;
    auto i = (int)tPawn.p.x + (int)m_tBoard.d.x * (int)tPawn.p.y;
    int nMove{MoveWhiteIA(tPawn, eDir)};
    if ( nMove != i )
	{
	SPosition const fp{i%(m_tBoard.d.x), i/(m_tBoard.d.x)}; // from pos
	tPawn.p = {nMove%(m_tBoard.d.x), nMove/(m_tBoard.d.x)}; // to   pos
	m_tGameWhite.emplace_back(SStep{m_tBoard.g, fp, tPawn.p});

	m_tBoard.g[nMove] = 'P';
	m_tBoard.g[i] = ':';

	std::cout << "W: (" << i     << ") " << fp.x << ", " << fp.y << " => ";
	std::cout << "W: (" << nMove << ") " << tPawn.p.x << ", " << tPawn.p.y << '\n';
	return !bPhase; // moved, next cycle is black's
	}
    return bPhase; // not yet moved, next cycle is ours
    } //


void CCanvas::KiLearnWhite()
    {
    RecognizeBoard();
    m_iActivWhite = -1;

    VDrags zugs{ DragWhite(m_vPawnsWhite) };

    if ( m_bKiTest ) zugs = KiAddTrainingWhite(zugs);

    // only if we have some ...
    if ( zugs.size() && m_vPawnsWhite.size() )
	{
	int z{0};
	if ( zugs.size() > 1 )
	    {
	    std::uniform_int_distribution<int> distribution(0,zugs.size()-1);
	    do
		{
		z = (int)distribution(mt);
		} while ( (z < 0) || (z >= (int)zugs.size()) );
	    }
	SPawn tPawn = zugs[z].first;
	auto i = (int)tPawn.p.x + (int)m_tBoard.d.x * (int)tPawn.p.y;

	auto tMove{zugs[z].second};
	auto nMove = (int)tMove.p.x + (int)m_tBoard.d.x * (int)tMove.p.y;

	SPosition const fp{i%(m_tBoard.d.x), i/(m_tBoard.d.x)}; // from pos
	tPawn.p = {nMove%(m_tBoard.d.x), nMove/(m_tBoard.d.x)}; // to   pos
	m_tGameWhite.emplace_back(SStep{m_tBoard.g, fp, tPawn.p});

	m_tBoard.g[nMove] = 'P';
	m_tBoard.g[i] = ':';

	if (m_nKiSleep > 100000-1)
	    {
	    std::cout << "W: (" << i     << ") " << fp.x << ", " << fp.y << " => ";
	    std::cout << "W: (" << nMove << ") " << tPawn.p.x << ", " << tPawn.p.y << '\n';
	    }
	}
    else
	{
	RecognizeBoard();
	VDrags zb{ DragBlack(m_vPawnsBlack) };
	VDrags zw{ DragWhite(m_vPawnsWhite) };
	if ( m_vPawnsWhite.size() > m_vPawnsBlack.size() )
	    {
	    m_nWinsWhite++;
	    std::cout << m_bKiTest << " " << m_nWinsWhite << ":" << m_nWinsBlack << "  +++ White wins\n";
	    KiAddExpirienceBlack(m_tGameBlack, -1);
	    KiAddExpirienceWhite(m_tGameWhite,  1);
	    }
	if ( m_vPawnsWhite.size() == m_vPawnsBlack.size() )
	    {
	    std::cout << m_bKiTest << " " << m_nWinsWhite << ":" << m_nWinsBlack << "  ::: no win\n";
	    KiAddExpirienceBlack(m_tGameBlack, 0);
	    KiAddExpirienceWhite(m_tGameWhite, 0);
	    }
	if ( m_vPawnsWhite.size() < m_vPawnsBlack.size() )
	    {
	    m_nWinsBlack++;
	    std::cout << m_bKiTest << " " << m_nWinsWhite << ":" << m_nWinsBlack << "  --- Black wins\n";
	    KiAddExpirienceBlack(m_tGameBlack,  1);
	    KiAddExpirienceWhite(m_tGameWhite, -1);
	    }
	StartLevel(m_nChosenGame);
	}
    } //

void CCanvas::KiLearnBlack()
    {
    RecognizeBoard();
    m_iActivBlack = -1;

    VDrags zugs{ DragBlack(m_vPawnsBlack) };

    if ( m_bKiTest )
	{
	zugs = KiAddTrainingBlack(zugs);
	}

    // only if we have some ...
    if ( zugs.size() && m_vPawnsBlack.size() )
	{
	int z{0};
	if ( zugs.size() > 1 )
	    {
	    std::uniform_int_distribution<int> distribution(0,zugs.size() -1 );
	    do
		{
		z = (int)distribution(mt);
		} while ( (z < 0) || (z >= (int)zugs.size()) );
	    }
	SPawn tPawn = zugs[z].first;
	auto i = (int)tPawn.p.x + (int)m_tBoard.d.x * (int)tPawn.p.y;

	auto tMove{zugs[z].second};
	auto nMove = (int)tMove.p.x + (int)m_tBoard.d.x * (int)tMove.p.y;

	SPosition const fp{i%(m_tBoard.d.x), i/(m_tBoard.d.x)}; // from pos
	tPawn.p = {nMove%(m_tBoard.d.x), nMove/(m_tBoard.d.x)}; // to   pos
	m_tGameBlack.emplace_back(SStep{m_tBoard.g, fp, tPawn.p});

	m_tBoard.g[nMove] = 'A';
	m_tBoard.g[i] = ':';

	if (m_nKiSleep > 100000-1)
	    {
	    std::cout << "B: (" << i     << ") " << fp.x << ", " << fp.y << " => ";
	    std::cout << "B: (" << nMove << ") " << tPawn.p.x << ", " << tPawn.p.y << '\n';
	    }
	}
    else
	{
	RecognizeBoard();
	VDrags zb{ DragBlack(m_vPawnsBlack) };
	VDrags zw{ DragWhite(m_vPawnsWhite) };
	if ( m_vPawnsWhite.size() > m_vPawnsBlack.size() )
	    {
	    m_nWinsWhite++;
	    std::cout << m_bKiTest << " " << m_nWinsWhite << ":" << m_nWinsBlack << "  +++ White wins\n";
	    KiAddExpirienceBlack(m_tGameBlack, -1);
	    KiAddExpirienceWhite(m_tGameWhite,  1);
	    }
	if ( m_vPawnsWhite.size() == m_vPawnsBlack.size() )
	    {
	    std::cout << m_bKiTest << " " << m_nWinsWhite << ":" << m_nWinsBlack << "  ::: no win\n";
	    KiAddExpirienceBlack(m_tGameBlack, 0);
	    KiAddExpirienceWhite(m_tGameWhite, 0);
	    }
	if ( m_vPawnsWhite.size() < m_vPawnsBlack.size() )
	    {
	    m_nWinsBlack++;
	    std::cout << m_bKiTest << " " << m_nWinsWhite << ":" << m_nWinsBlack << "  --- Black wins\n";
	    KiAddExpirienceBlack(m_tGameBlack,  1);
	    KiAddExpirienceWhite(m_tGameWhite, -1);
	    }
	StartLevel(m_nChosenGame);
	}
    } //

bool CCanvas::m_bHasFocus{true}; 

void CCanvas::OnDraw()
    {
    //------------------------------------------------------ light
    // Set lighting intensity and color
    GLfloat aAmbientLight[]  = {0.3, 0.3, 0.3, 1.0};
    GLfloat aDiffuseLight[]  = {0.7, 0.7, 0.7, 1.0};
    GLfloat aSpecularLight[] = {1.0, 1.0, 1.0, 1.0};

    // Light source position
    GLfloat aLightPosition [] = {0, 0, 1, 1}; // Positional Light

    // Set lighting intensity and color
    glLightfv(GL_LIGHT0, GL_AMBIENT,  aAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  aDiffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, aLightPosition);
    glLightfv(GL_LIGHT0, GL_SPECULAR, aSpecularLight);
    //------------------------------------------------------ light

    static bool init{false};
    if ( !init )
        {
        init = true;
        //------------------------------------------------------ light
        // Enable light
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        //------------------------------------------------------ objects
        CGlQuader g;
        CGlPawn   p;

        glNewList(m_nFieldBlack, GL_COMPILE); g(13.0f,13.0f, 2.9f, addd, addd, addd, addd, addd, addd ); glEndList(); // FieldBlack
        glNewList(m_nFigurWhite, GL_COMPILE); p( 6.0f, 6.0f, 9.5f, a777 ); glEndList(); // FigurWhite
        glNewList(m_nFieldWhite, GL_COMPILE); g(13.0f,13.0f, 2.9f, ahhh, ahhh, ahhh, ahhh, ahhh, ahhh ); glEndList(); // FieldWhite
        glNewList(m_nFigurBlack, GL_COMPILE); p( 6.5f, 6.5f, 9.5f, a333 ); glEndList(); // FigurBlack
        glNewList(m_nFrameRed,   GL_COMPILE); g(11.0f,11.0f, 3.0f, a155, a155, a155, a155, a155, a155 ); glEndList(); // FrameRed,
        glNewList(m_nFrameGreen, GL_COMPILE); g( 8.5f, 8.5f, 3.1f, a010, a010, a010, a010, a010, a010 ); glEndList(); // FrameGreen
        glNewList(m_nFigurRed,   GL_COMPILE); p( 6.0f, 6.0f, 9.5f, a155 ); glEndList(); // FigurRed
        glNewList(m_nFigurGreen, GL_COMPILE); p( 6.0f, 6.0f, 9.5f, a010 ); glEndList(); // FigurGreen
        }

    auto const s = m_oCtx.getSize();

    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(.1f, .1f, .3f, 1.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.f, s.x/s.y, 1.f, 500.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(  0,  0, 100, 0, 0, 0, 0, 1, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // ???    
    glTranslatef(-100.f, -100.f, -200.f);

    glRotatef(  0, 1.f, 0.f, 0.f);
    glRotatef(m_fRotateY, 0.f, 1.f, 0.f);
    glRotatef(m_fRotateZ, 0.f, 0.f, 1.f);
    glRotatef(m_fRotateX, 1.f, 0.f, 0.f);

	for ( auto const & tPawn:m_vPawnsWhite )
	    {
	    VSMoves pm{PossibleMovesWhite(tPawn)};
	    for ( auto const & a:pm )
		{
		glPushMatrix();
		glRotatef( m_dPAnimate, 0, 0, 0 );
		glTranslatef( m_tOffset.x+(m_tField.x+5)*a.p.x, m_tOffset.y+(m_tField.y+5)*a.p.y, -00.f);
		glCallList(m_nFrameGreen);
		glPopMatrix();
		}
	    }

	for ( auto const & tPawn:m_vPawnsBlack )
	    {
	    VSMoves pm{PossibleMovesBlack(tPawn)};
	    for ( auto const & a:pm )
		{
		glPushMatrix();
		glRotatef( m_dPAnimate, 0, 0, 0 );
		glTranslatef( m_tOffset.x+(m_tField.x+5)*a.p.x, m_tOffset.y+(m_tField.y+5)*a.p.y, -00.f);
		glCallList(m_nFrameRed);
		glPopMatrix();
		}
	    }


if (m_bPhase)
    {
    if ( m_bByHand )
	m_bPhase = PlayWhite(m_bPhase);
    else
	{
	KiLearnWhite();
	m_bPhase = !m_bPhase;
	}
    usleep(m_nKiSleep);
    }
else
    {
    KiLearnBlack();
    usleep(m_nKiSleep);
    m_bPhase = !m_bPhase;
    }



    if ( m_iActivWhite >= 0 )
	{
	for ( auto const & tPawn:m_vPawnsWhite )
	    {
	    VSMoves pm{PossibleMovesWhite(tPawn)};
	    for ( auto const & a:pm )
		{
		glPushMatrix();
		glTranslatef( m_tOffset.x+(m_tField.x+5)*a.p.x, m_tOffset.y+(m_tField.y+5)*a.p.y, -00.f);
		glCallList(m_nFrameGreen);
		glPopMatrix();
		}
	    }
	}

    if ( m_iActivBlack >= 0 )
	{
	auto const tPawn = m_vPawnsBlack[m_iActivBlack];
	    {
	    VSMoves pm{PossibleMovesBlack(tPawn)};
	    for ( auto const & a:pm )
		{
		glPushMatrix();
		glTranslatef( m_tOffset.x+(m_tField.x+5)*a.p.x, m_tOffset.y+(m_tField.y+5)*a.p.y, -00.f);
		glCallList(m_nFrameRed);
		glPopMatrix();
		}
	    }    // end the current frame
	}
    m_oCtx.display();
    usleep(m_nKiSleep);
    }
