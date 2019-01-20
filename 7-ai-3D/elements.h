#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "canvas.h"


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

#endif // ELEMENTS_H
