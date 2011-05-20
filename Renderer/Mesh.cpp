#include "Mesh.hpp"
#include "Util/OpenGL.hpp"

#include <algorithm>

using namespace cge;

Mesh::Mesh( const Creator &creator ) {
	creator.create( d_ );
}

void Mesh::onAttached() {
	getNode().setVisibility(true,false);
	getNode().setRenderable(true);
}

void Mesh::onRendered() {
	glBegin(GL_TRIANGLES);
	std::for_each(d_.polys.begin(), d_.polys.end(),
		[this]( Poly& poly ) {
			glNormal3fv( d_.norms[ poly.normal[0] ] );
			glVertex3fv( d_.verts[ poly.vertex[0] ] );
		
			glNormal3fv( d_.norms[ poly.normal[1] ] );
			glVertex3fv( d_.verts[ poly.vertex[1] ] );
		
			glNormal3fv( d_.norms[ poly.normal[2] ] );
			glVertex3fv( d_.verts[ poly.vertex[2] ] );
		}
	);
	glEnd();
}
