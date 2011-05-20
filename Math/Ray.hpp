#pragma once

#include "Vectors.hpp"

namespace cge {
	class Ray {
	public:
		Ray( const vec3 &point, const vec3 &dir ): p_(point), d_(dir.unit()) {}

		const vec3& getDirection() const { return d_; }
		const vec3& getPoint()     const { return p_; }
	
		vec3 pointAt( Scalar t )   const { return p_ + d_*t; }

	private:
		vec3 p_, d_;
	};
}