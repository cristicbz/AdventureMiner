#include "Vectors.hpp"


namespace cge {
	const vec3 ctVec::x(ctOne,  ctZero, ctZero);
	const vec3 ctVec::y(ctZero, ctOne,  ctZero);
	const vec3 ctVec::z(ctZero, ctZero, ctOne);
	const vec3 ctVec::zero(ctZero, ctZero, ctZero);
	const vec3 ctVec::direction6[ 6 ] = { x, y, z, -x, -y, -z };
}