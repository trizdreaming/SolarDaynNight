#include "solarSellObject.h"
#include "include.h"


solarSellObject::solarSellObject(void)
{
}


solarSellObject::~solarSellObject(void)
{
}

void solarSellObject::DrawTexturedSpheere( GLfloat red, GLfloat green, GLfloat blue )
{
	GLfloat x, y, z, delta = .1f;
	GLfloat t = 1.f;
	GLfloat s = .0f;

	glColor3f(red, green, blue);

	glBegin(GL_TRIANGLE_STRIP);
		for(GLfloat phi = 0 ; phi < 3.2; phi += 0.1){
			for(GLfloat theta =0; theta =6.3; theta +=0.1){
				x=sin(phi)*cos(theta);
				x=sin(phi)*sin(theta);
				z=cos(phi);

				glTexCoord2f(s, t);
				glNormal3f(x,y,z);
				glVertex3f(x,y,z);
				phi -= 0.1;
			}
		}
	glEnd();
}
