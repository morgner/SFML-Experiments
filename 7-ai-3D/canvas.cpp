#include "canvas.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <fstream>
#include <math.h>
#include <random>
#include <GL/glu.h>


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
            switch (m_cKeyDown)
                {
                case 'a': break;
                case 's': m_bShowField = !m_bShowField; break;
                case 'd': break;
                case 'f': break;

                case 'i': 
                case 'j': 
                case 'k': 
                case 'l': Startsquence(m_cKeyDown); break;
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
            switch (m_cKeyDown)
                {
                case 'a': break;
                case 's': m_bShowField = !m_bShowField; break;
                case 'd': break;
                case 'f': break;
                case 'i': 
                case 'j': 
                case 'k': 
                case 'l': Startsquence(m_cKeyDown); break;
                case 'n': StartLevel(0); break;
                case 'm': break;
                case 'x': --m_fRotateX; break;
                case 'y': --m_fRotateY; break;
                case 'z': --m_fRotateZ; break;
                case 'r': m_fRotateX = 0.f; m_fRotateY = 0.f; m_fRotateZ = 0.f; break;
                }
            }
        }
    if (event.type == sf::Event::MouseButtonPressed)
        {
/*
        if (event.mouseButton.button == sf::Mouse::Left)
            {
            m_tMouseEventLeft = 
                SMouse{true, 
                       m_oCtx.mapPixelToCoords(
                            sf::Vector2i{event.mouseButton.x,
                                         event.mouseButton.y})};
            }
*/
/*
        if (event.mouseButton.button == sf::Mouse::Right)
            {
            m_tMouseEventRight = 
                SMouse{true, 
                       m_oCtx.mapPixelToCoords(
                            sf::Vector2i{event.mouseButton.x,
                                         event.mouseButton.y})};
            }
*/
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
    StartLevel(n);
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


void CCanvas::StartLevel(int i)
    {
    m_tOffset = { 25, 50 };
    switch (i)
        {
        case 2:m_tBoard  = {8,8};
               m_tOffset = { 0, 25 };
               maze =
               "-X-X-X-X"
               "X-X-P-X-"
               "-X-X-X-X"
               "X-X-X-X-"
               "-X-X-X-X"
               "X-X-X-X-"
               "-X-XAA-X"
               "AAX-X-X-"
               ;
               mzbg =
               "-X-X-X-X"
               "X-X-X-X-"
               "-X-X-X-X"
               "X-X-X-X-"
               "-X-X-X-X"
               "X-X-X-X-"
               "-X-X-X-X"
               "X-X-X-X-"
               ;
               break;

        case 1:m_tBoard  = {4,4};
               m_tOffset = { 50, 50 };
               maze =
               "PPPP"
               "-X-X"
               "X-X-"
               "AAAA"
               ;
               mzbg =
               "-X-X"
               "X-X-"
               "-X-X"
               "X-X-"
               ;
               break;

        default:m_tBoard = {6,6};
                maze =
                "-X-X-X"
                "X-X-P-"
                "-X-X-X"
                "X-A-X-"
                "-X-X-X"
                "X-X-X-"
                ;
                mzbg =
                "-X-X-X"
                "X-X-X-"
                "-X-X-X"
                "X-X-X-"
                "-X-X-X"
                "X-X-X-"
                ;
        }
    }

void CCanvas::Win()
    {
    int ic{0};
    for ( auto const & a:maze )
        {
        maze[ic] = (/* mzbg[ic] */ a == 'X') ? 'A' : mzbg[ic];
        ++ic;
        }
    }


int CCanvas::Move(std::string const & sWorld,
         SPoint const      & tBoard,
         int const         & nWorldIndex,
         std::string const & allowedFields, /*"-XA"*/
         std::string const & hasToMove, /*"A"*/
         CCanvas::EDirection const & e,
         bool bRecursion)
    {
    int i{nWorldIndex};

    switch ( e )
        {
        case CCanvas::EDirection::up    : i = nWorldIndex +tBoard.x; break;
        case CCanvas::EDirection::left  : i = nWorldIndex        -1; break;
        case CCanvas::EDirection::down  : i = nWorldIndex -tBoard.x; break;
        case CCanvas::EDirection::right : i = nWorldIndex        +1; break;
        }

    if ( (i < (int)sWorld.length()) && (i >= 0) && (allowedFields.find(sWorld[i]) != std::string::npos) )
        {
        if ( bRecursion && sWorld[i] == 'A' ) 
            if ( Move(sWorld, tBoard, i, "-X", "", e, false) == i ) return nWorldIndex;
        }
    else
        return nWorldIndex;

    return i;
    }


struct SColor
    {
    float r{0}, g{0}, b{0};

    bool operator()() const
        {
        glColor3f(r,g,b);
        return true;
        }

    bool operator()(float const & r, float const & g, float const & b ) const
        {
        glColor3f(r,g,b);
        return true;
        }

    };


struct SMaterial
    {
    float m[4];
    bool operator()() const
        {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m );
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m );
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m );
        return true;
        }
    bool operator()(float const * rgba) const
        {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  rgba );
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  rgba );
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, rgba );
        return true;
        }
    };

VSMoves CCanvas::PossibleMoves(SBoard const & b, SPawn const & p) const
    {
    VSMoves tMoves{};
    auto y = b.tTotalDimension.x;
    auto l = b.sSituation.length();
    auto i = p.p.x + p.p.y * y;

    auto id = i + 1*y;
    if ( ( id < l ) && ( b.sSituation[id] != 'A' ) )
        {
        tMoves.emplace_back( SPosition{id%y, id/y}, 0.0, -1 );

        id = i + 2*y;
        if ( ( id < l ) && ( i < y ) && ( b.sSituation[id] != 'A' ) )
            { tMoves.emplace_back( SPosition{id%y, id/y}, 0.0, -1 ); }
        }

    id = i + 1*y -1;
    if ( ( id < l ) && ( i%y != 0 ) && ( b.sSituation[id] == 'A' ) )
        { tMoves.emplace_back( SPosition{id%y, id/y}, 0.0, -1 ); }

    id = i + 1*y +1;
    if ( ( id < l ) && ( (i+1)%y != 0 ) && ( b.sSituation[id] == 'A' ) )
        { tMoves.emplace_back( SPosition{id%y, id/y}, 0.0, -1 ); }

    return std::move(tMoves);
    }

bool CCanvas::Drag(SBoard const & crtBoard, SPawn const & crtPawn)
    {
    return true;
    }

void CCanvas::DrawBoard(SBoard const & crtBoard)
    {
    }

constexpr SColor c155{1.f,.5f,.5f};
constexpr SColor c515{.5f,1.f,.5f}; 
constexpr SColor c551{.5f,.5f,1.f}; 
constexpr SColor c151{1.f,.5f,1.f}; 
constexpr SColor c511{.5f,1.f,1.f}; 
constexpr SColor c115{1.f,1.f,.5f}; 

constexpr SColor c055{0.f,.5f,.5f}; 
constexpr SColor c505{.5f,0.f,.5f}; 
constexpr SColor c550{.5f,.5f,0.f}; 
constexpr SColor c050{0.f,.5f,0.f}; 
constexpr SColor c500{.5f,0.f,0.f}; 
constexpr SColor c005{0.f,0.f,.5f}; 

constexpr SColor c111{1.f,1.f,1.f}; 
constexpr SColor c333{.3f,.3f,.3f}; 

constexpr SMaterial a500{ .5, 0.0, 0.0, 1.0};
constexpr SMaterial a050{0.0,  .5, 0.0, 1.0};
constexpr SMaterial a005{0.0, 0.0,  .5, 1.0};
constexpr SMaterial a550{ .5,  .5, 0.0, 1.0};
constexpr SMaterial a055{0.0,  .5,  .5, 1.0};
constexpr SMaterial a505{ .5, 0.0,  .5, 1.0};

constexpr SMaterial a100{1.0, 0.0, 0.0, 1.0};
constexpr SMaterial a010{0.0, 1.0, 0.0, 1.0};
constexpr SMaterial a001{0.0, 0.0, 1.0, 1.0};
constexpr SMaterial a110{1.0, 1.0, 0.0, 1.0};
constexpr SMaterial a011{0.0, 1.0, 1.0, 1.0};
constexpr SMaterial a101{1.0, 0.0, 1.0, 1.0};

constexpr SMaterial a155{1.0, 0.5, 0.5, 1.0};
constexpr SMaterial a515{0.5, 1.0, 0.5, 1.0};
constexpr SMaterial a551{0.5, 0.5, 1.0, 1.0};
constexpr SMaterial a115{1.0, 1.0, 0.5, 1.0};
constexpr SMaterial a511{0.5, 1.0, 1.0, 1.0};
constexpr SMaterial a151{1.0, 0.5, 1.0, 1.0};

constexpr SMaterial a111{1.0, 1.0, 1.0, 1.0};
constexpr SMaterial ahhh{0.9, 0.9, 0.9, 1.0};
constexpr SMaterial a777{0.7, 0.7, 0.7, 1.0};
constexpr SMaterial a555{0.5, 0.5, 0.5, 1.0};
constexpr SMaterial a333{0.3, 0.3, 0.3, 1.0};
constexpr SMaterial addd{0.3, 0.3, 0.3, 1.0};

class CGlQuader
    {
    public:

    bool operator()(float const & x, float const & y, float const & z, 
                    SMaterial const & c1 , SMaterial const & c2 , SMaterial const & c3 ,  
                    SMaterial const & c4 , SMaterial const & c5 , SMaterial const & c6   )
        {
        glBegin(GL_QUADS);

	    glPushMatrix();
		  c1();
		    glVertex3f(-x, -y, -0);
		    glVertex3f(-x,  y, -0);
		    glVertex3f( x,  y, -0);
		    glVertex3f( x, -y, -0);
	    glPopMatrix();

	    glPushMatrix();
		  c2();
		    glVertex3f(-x, -y,  z*2);
		    glVertex3f(-x,  y,  z*2);
		    glVertex3f( x,  y,  z*2);
		    glVertex3f( x, -y,  z*2);
	    glPopMatrix();

	    glPushMatrix();
		  c3();
		    glVertex3f(-x, -y, -0);
		    glVertex3f(-x,  y, -0);
		    glVertex3f(-x,  y,  z*2);
		    glVertex3f(-x, -y,  z*2);
	    glPopMatrix();

	    glPushMatrix();
		  c4();
		    glVertex3f( x, -y, -0);
		    glVertex3f( x,  y, -0);
		    glVertex3f( x,  y,  z*2);
		    glVertex3f( x, -y,  z*2);
	    glPopMatrix();

	    glPushMatrix();
		  c5();
		    glVertex3f(-x, -y,  z*2);
		    glVertex3f(-x, -y, -0);
		    glVertex3f( x, -y, -0);
		    glVertex3f( x, -y,  z*2);
	    glPopMatrix();

	    glPushMatrix();
		  c6();
		    glVertex3f(-x,  y,  z*2);
		    glVertex3f(-x,  y, -0);
		    glVertex3f( x,  y, -0);
		    glVertex3f( x,  y,  z*2);
	    glPopMatrix();

        glEnd();

        return true;
        }
    };


bool CCanvas::m_bHasFocus{true}; 

void CCanvas::OnDraw()
    {
    CGlQuader g;

//------------------------------------------------------ light

    // Set lighting intensity and color
    GLfloat qaAmbientLight[]  = {0.3, 0.3, 0.3, 1.0};
    GLfloat qaDiffuseLight[]  = {0.7, 0.7, 0.7, 1.0};
    GLfloat qaSpecularLight[] = {1.0, 1.0, 1.0, 1.0};

    // Light source position
    GLfloat qaLightPosition [] = {0, 0, 1, 1}; // Positional Light

    // Set lighting intensity and color
    glLightfv(GL_LIGHT0, GL_AMBIENT,  qaAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  qaDiffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);
    ////////////////////////////////////////////////
//------------------------------------------------------ light

    static bool init{false};
    if ( !init )
        {
        init = true;
        //------------------------------------------------------ objects
        //                                                                              top
        block = glGenLists(1); glNewList(block, GL_COMPILE); g(13.0f,13.0f, 2.9f, addd, addd, addd, addd, addd, addd ); glEndList(); // graufeld
        pengi = glGenLists(2); glNewList(pengi, GL_COMPILE); g( 6.0f, 6.0f, 9.5f, a777, a111, a777, a777, a777, a777 ); glEndList(); // weissfigur
        monst = glGenLists(3); glNewList(monst, GL_COMPILE); g(13.0f,13.0f, 2.9f, ahhh, ahhh, ahhh, ahhh, ahhh, ahhh ); glEndList(); // weissfeld
        chest = glGenLists(4); glNewList(chest, GL_COMPILE); g( 6.5f, 6.5f, 9.5f, a333, a777, a333, a333, a333, a333 ); glEndList(); // graufigur
        walle = glGenLists(5); glNewList(walle, GL_COMPILE); g(14.0f,14.0f, 2.8f, a155, a155, a155, a155, a155, a155 ); glEndList(); // destination
        winnr = glGenLists(6); glNewList(winnr, GL_COMPILE); g(14.0f,14.0f, 0.5f, a050, a050, a050, a050, a050, a050 ); glEndList();

        //------------------------------------------------------ light
        // Enable light
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        }

    auto s = m_oCtx.getSize();

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

    int i{0};
/*
    m_bGameWon = true;
    int ic{0};
    for ( auto const & a:maze )
        {
        if ( (maze[ic] == 'A') && (mzbg[ic] != 'X') )
            {
            m_bGameWon = false;
            break;
            }
        ++ic;
        }
*/

    SPawn tPawn{ true, {1,1} };  
    for ( int i{0},y{0}; y < m_tBoard.y; ++y )
        {
        for ( int x{0}; x < m_tBoard.x; ++x )
            {
            glPushMatrix();
            glTranslatef( m_tOffset.x+(m_tField.x+5)*x, m_tOffset.y+(m_tField.y+5)*y, -00.f);
            if ( mzbg[i++] == 'X' ) glCallList(monst); else glCallList(block);
            glPopMatrix();
            }
        }



    for ( int y{0}; y < m_tBoard.y; ++y )
        {
        for ( int x{0}; x < m_tBoard.x; ++x )
            {
            glPushMatrix();
            glTranslatef( m_tOffset.x+(m_tField.x+5)*x, m_tOffset.y+(m_tField.y+5)*y, -00.f);
            CCanvas::EDirection eDir{EDirection::up};
            bool b{false};
//            if ( mzbg[i] == 'X' ) glCallList(monst); else if ( mzbg[i] != '+' ) glCallList(block);
            switch ( maze[i] )
                {
                case ' ':   break;
                case '+':   if ( m_bGameWon ) glCallList(winnr); else glCallList(walle); break;
                case 'P':   switch ( m_tPengi.cCurrentMove )
                                {
                                case 'i': eDir = CCanvas::EDirection::up;    break;
                                case 'j': eDir = CCanvas::EDirection::left;  break;
                                case 'k': eDir = CCanvas::EDirection::down;  break;
                                case 'l': eDir = CCanvas::EDirection::right; break;
                                }
    tPawn.p = {(uint32_t)x,
               (uint32_t)y};  
                            if ( m_bPAniJustStopped )
                                {
                                m_bPAniJustStopped = false;
                                int const nMove{Move(maze, m_tBoard, i, "-XA", "A", eDir )};
                    
                                int d = nMove - i;
                                if ( d != 0 )
                                    {
                                    if ( maze[nMove] == 'A' ) maze[nMove+d] = mzbg[nMove+d];
                                    maze[nMove] = 'P'; maze[i] = mzbg[i];
                                    }
                    
                                if ( m_cKeyDown ) Startsquence(m_cKeyDown); else m_tPengi.cCurrentMove = 0; 
                                break;
                                }
                            if ( m_bPAnimating )
                                {
                                int const nMove{Move(maze, m_tBoard, i, "-XA", "A", eDir )};
                                if ( nMove != i )
                                    {
                                    switch ( eDir )
                                        {
                                        case CCanvas::EDirection::up   : glTranslatef(  0, +m_dPAnimate*(m_tField.y+5), 0.f); break;
                                        case CCanvas::EDirection::left : glTranslatef( -m_dPAnimate*(m_tField.x+5), 0 , 0.f); break;
                                        case CCanvas::EDirection::down : glTranslatef(  0, -m_dPAnimate*(m_tField.y+5), 0.f); break;
                                        case CCanvas::EDirection::right: glTranslatef( +m_dPAnimate*(m_tField.x+5), 0 , 0.f); break;
                                        }
                                    b = true;
                                    glCallList(pengi);
                                    }
                                }
                            if ( !b ) glCallList(pengi);
                            break;
                case 'X':   break;//glCallList(monst); break;
                case 'A':   glCallList(chest); break;
                default :   break;//glCallList(block); break;
                }
            glPopMatrix();
            ++i;
            }
        }

    SBoard tBoard{ {(uint32_t)m_tBoard.x,(uint32_t)m_tBoard.y}, // tTotalDimension
//                 {"+++++++-X-P++X-X-++-X-X++X-X-+++++++"},    // sSituation
//                 {"+++++++-X-X++X-X-++-X-X++X-X-+++++++"} };  // sDecoration
                   maze,    // sSituation
                   mzbg };  // sDecoration
//  SPawn   tPawn{ true, {1,1} };  
    VSMoves pm{PossibleMoves(tBoard, tPawn)};
    for ( auto const & a:pm )
        {
// std::cout << tPawn.p.x << ", " << tPawn.p.y << " : " << a.p.x << "," << a.p.y << '\n';

        glPushMatrix();
        glTranslatef( m_tOffset.x+(m_tField.x+5)*a.p.x, m_tOffset.y+(m_tField.y+5)*a.p.y, -00.f);
        glCallList(walle);
        glPopMatrix();
        }
    // end the current frame
    m_oCtx.display();

    }

