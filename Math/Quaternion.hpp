#pragma once

#include "Vectors.hpp"

namespace cge {
	class Quaternion {
	public:
		Quaternion();
		Quaternion( const vec4& v );
		Quaternion( const Quaternion& other );
		Quaternion( Scalar angle, const vec3& axis );
		Quaternion( Scalar pitch, Scalar yaw, Scalar roll );

		void toMatrix( Scalar matrix[16] ) const;
		void toEuler( Scalar& pitch, Scalar& yaw, Scalar& roll ) const;

		void setDirection( const vec3& direction );
		void setAngleAxis( Scalar angle, const vec3& axis );
		void setEulerAngles( Scalar pitch, Scalar yaw, Scalar roll );
		void setYaw( Scalar yaw );
		void setPitch( Scalar pitch );
		void setRoll( Scalar roll );

		void setNlerp( const Quaternion& start, const Quaternion& end, Scalar delta );
		void setSlerp( const Quaternion& start, const Quaternion& end, Scalar delta );

		void premultiply( const Quaternion &q );
		void rotateAngleAxis( Scalar angle, const vec3 &axis );

		vec3 rotateVector( const vec3& point ) const;
		vec3 unrotateVector( const vec3& point ) const;

		void       invert();
		Quaternion inverse() const;

		Quaternion& operator=( const Quaternion &q );

		Quaternion& operator*=( const Quaternion &q );
		Quaternion& operator*=( const vec3& point );

		Quaternion  operator*( const Quaternion &q ) const;
		Quaternion  operator*( const vec3& point ) const;

		friend std::ostream& operator<<( std::ostream &os, const Quaternion &q ) { return os << q.v_; }
		void renormalize() {
			Scalar n2 = v_.normSquared();
			if( abs(n2-1.0) >= 1e-8) {
				v_/=sqrt(n2);
			}
		}

		Scalar norm() { return v_.norm(); }

	private:
		Quaternion( const vec3& p );
		Quaternion( Scalar w, Scalar x, Scalar y, Scalar z ): v_(w,x,y,z) {}
		vec4 v_;
	};
}