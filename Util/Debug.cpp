#include "Util/Debug.hpp"
#include "OpenGL.hpp"
#include "Math/AlignedBox.hpp"

namespace cge {
	void debugCube( Scalar w, bool wire ) {
		Scalar s = w/2.0f;
		if(wire)
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glBegin(GL_QUADS);
		
		// top
		glNormal3fv( ctVec::direction6[ DR_UP ] );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-s, s, s);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( s, s, s);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( s, s,-s);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, s,-s);

		// bottom
		glNormal3fv( ctVec::direction6[ DR_DOWN ] );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-s,-s,-s);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( s,-s,-s);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( s,-s, s);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-s,-s, s);

		// left
		glNormal3fv( ctVec::direction6[ DR_LEFT ] );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-s,-s, s);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-s, s, s);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, s,-s);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-s,-s,-s);

		// right
		glNormal3fv( ctVec::direction6[ DR_RIGHT ] );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( s,-s,-s);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( s, s,-s);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( s, s, s);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( s,-s, s);
		
		// front
		glNormal3fv( ctVec::direction6[ DR_FRONT ] );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-s, s,-s);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( s, s,-s);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( s,-s,-s);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-s,-s,-s);

		// back
		glNormal3fv( ctVec::direction6[ DR_BACK ] );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-s,-s,s);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( s,-s,s);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( s, s,s);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, s,s);
		
		glEnd();

		if(wire)
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	}

	void debugBox( const AlignedBox &b, bool wire ) {
		vec3 move,scale;
		move = b.getCenter();
		scale = b.getSize();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(move.x,move.y,move.z);
		glScalef(scale.x,scale.y,scale.z);
		debugCube(1.0,wire);
		glPopMatrix();
	}

	void debugBillboard( Scalar w ) {
		Scalar s = w/2.0f;
		glColor4d(0.01,0.006,0.0,0.5);
		
		glPointSize(20.0);
		glBegin(GL_POINTS);
		// front
		glVertex3d( 0,0,0);
		glEnd();
	}
}