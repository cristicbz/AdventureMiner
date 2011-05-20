#pragma once

namespace cge {
	typedef float Scalar;

	const Scalar ctPi      = 3.141592653589793238f;
	const Scalar ctPiDiv2  = 1.570796326794896619f;
	const Scalar ct2Pi     = 6.283185307179586476f;
	const Scalar ct4Pi     = 12.56637061435917295f;
	const Scalar ctZero    = 0.0f;
	const Scalar ctOne     = 1.0f;
	const Scalar ctTwo     = 2.0f;

	enum Direction		{ DR_RIGHT, DR_UP, DR_FRONT, DR_LEFT, DR_DOWN, DR_BACK };
	enum Intersect		{ IN_INSIDE, IN_OUTSIDE, IN_INTERSECTING };

	inline Direction oppositeDirection( Direction d ) { return static_cast<Direction>( ( d + 3 ) % 6 ); }

	inline int intFloor( float x ) {
		int trunc = static_cast<int>( x );
		if( x >= 0 || trunc == x )
			return trunc;
		else
			return trunc - 1;
	}
}