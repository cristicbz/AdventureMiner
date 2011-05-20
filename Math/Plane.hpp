#pragma once

#include "Vectors.hpp"
#include "Ray.hpp"
#include "Util/Debug.hpp"
#include <boost/optional.hpp>

namespace cge {
	class Plane {
	public:
		Plane(): d_(0.0f) {}
		Plane( Scalar a, Scalar b, Scalar c, Scalar d ): abc_(a,b,c), d_(d) {}
		Plane( const vec3 &normal, Scalar d ): abc_(normal), d_(d) {}

		void set( Scalar a, Scalar b, Scalar c, Scalar d );
		void set( const vec3& normal, Scalar d );

		Scalar signedDistance( const vec3 &p ) const { return abc_.dot(p) + d_; }
		Scalar distance( const vec3 &p )       const { return abs(signedDistance(p)); }

		bool   isFacing( const vec3 &p )     const { return signedDistance(p) > 0.0; }

		boost::optional<vec3> intersect( const Ray &r ) {
			boost::optional<vec3> ret;
			Scalar proj = abc_.dot(r.getDirection());
			if( proj == 0 )
				return ret;
			else
				return r.pointAt( -signedDistance(r.getPoint()) / proj );
		}

		const vec3& getNormal() const { return abc_; }
		Scalar      getA() const { return abc_.x; }
		Scalar      getB() const { return abc_.y; }
		Scalar      getC() const { return abc_.z; }
		Scalar      getD() const { return d_; }

		void normalize() {
			Scalar n = abc_.norm();
			cgeAssert( n > 0.0 );
			abc_ /= n;
			d_ /= n;
		}

	private:
		vec3   abc_;
		Scalar d_;
	};

	inline void Plane::set( Scalar a, Scalar b, Scalar c, Scalar d ) {
		abc_.set( a, b, c );
		d_ = d;
	}

	inline void Plane::set( const vec3& normal, Scalar d ) {
		abc_ = normal;
		d_ = d;
	}
}