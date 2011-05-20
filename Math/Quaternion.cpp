#include "Quaternion.hpp"
#include "Util/Debug.hpp"

#include <iostream>

namespace cge {
	Quaternion::Quaternion()
		: v_(1.0,0.0,0.0,0.0) {}

	Quaternion::Quaternion( const vec4& v )
		: v_(v) {}
	
	Quaternion::Quaternion( const Quaternion& other )
		: v_(other.v_) {}
	
	Quaternion::Quaternion( Scalar angle, const vec3& axis ) {
		setAngleAxis(angle,axis);
	}
	
	Quaternion::Quaternion( Scalar pitch, Scalar yaw, Scalar roll ) {
		setEulerAngles(pitch,yaw,roll);
	}

	Quaternion::Quaternion( const vec3& p )
		: v_(0.0,p.x,p.y,p.z) {}
	

	void Quaternion::toMatrix( Scalar m[16] ) const {
		Scalar xx2 = v_.x*v_.x * 2, yy2 = v_.y*v_.y * 2;
		Scalar zz2 = v_.z*v_.z * 2, wx2 = v_.w*v_.x * 2;
		Scalar wy2 = v_.w*v_.y * 2, wz2 = v_.w*v_.z * 2;
		Scalar xy2 = v_.x*v_.y * 2, xz2 = v_.x*v_.z * 2;
		Scalar yz2 = v_.y*v_.z * 2;

		// column major
		m[0] = 1-yy2-zz2 ; m[4]  = xy2-wz2   ; m[8]  = xz2+wy2;
		m[1] = xy2+wz2   ; m[5]  = 1-xx2-zz2 ; m[9]  = yz2-wx2;
		m[2] = xz2-wy2   ; m[6]  = yz2+wx2   ; m[10] = 1-xx2-yy2;
		m[3] = 0         ; m[7]  = 0         ; m[11] = 0;
			
		// last column
		m[12] = m[13] = m[14] = 0;
		m[15] = 1;
	}
	
	void Quaternion::toEuler( Scalar& pitch, Scalar& yaw, Scalar& roll ) const {
		pitch	= atan2f( 2.0f*(v_[0]*v_[1]+v_[2]*v_[3]), 1.0f - 2.0f*(v_[1]*v_[1] + v_[2]*v_[2]) );
		yaw		= asinf( 2.0f*( v_[0] * v_[2] - v_[3] * v_[1] ) );
		roll	= atan2f( 2.0f*(v_[0]*v_[3]+v_[1]*v_[2]), 1.0f - 2.0f*(v_[2]*v_[2] + v_[3]*v_[3]) );
	}
	

	void Quaternion::setDirection( const vec3& direction ) {
		cgeStub(std::cerr);
	}
	
	void Quaternion::setAngleAxis( Scalar angle, const vec3& axis ) {
		Scalar ha = angle/2;
		Scalar c = cos(ha), s = sin(ha);
		v_.set(c,axis.x*s,axis.y*s,axis.z*s);
		renormalize();
		
	}

	void Quaternion::premultiply( const Quaternion &q ) {
		(*this) = q*(*this);
	}

	void Quaternion::rotateAngleAxis( Scalar angle, const vec3 &axis ) {
		premultiply(Quaternion(angle,axis));
	}
	
	void Quaternion::setEulerAngles( Scalar pitch, Scalar yaw, Scalar roll ) {
		vec3 ox(1.0,0.0,0.0);
		vec3 oy(0.0,1.0,0.0);
		vec3 oz(0.0,0.0,1.0);
		setAngleAxis( roll, oz );
		(*this) *= Quaternion(pitch, ox );
		(*this) *= Quaternion(yaw, oy );
	}
	
	void Quaternion::setYaw( Scalar yaw ) {
		cgeStub(std::cerr);
	}
	
	void Quaternion::setPitch( Scalar pitch ) {
		cgeStub(std::cerr);
	}
	
	void Quaternion::setRoll( Scalar roll ) {
		cgeStub(std::cerr);
	}
	
	void Quaternion::setNlerp( const Quaternion& start, const Quaternion& end, Scalar delta ) {
		v_ = start.v_*(1-delta)+end.v_*delta;
		v_.normalize();
	}
	
	void Quaternion::setSlerp( const Quaternion& start, const Quaternion& end, Scalar delta ) {
		cgeStub(std::cerr);
	}

	vec3 Quaternion::rotateVector( const vec3& point ) const {
		Quaternion q(*this);
		q*=Quaternion(point);
		q*=inverse();
		return vec3(q.v_.x,q.v_.y,q.v_.z);
	}
	
	vec3 Quaternion::unrotateVector( const vec3& point ) const {
		Quaternion q(*this);
		q.invert();
		q*=point;
		q*=(*this);
		return vec3(q.v_.x,q.v_.y,q.v_.z);
	}

	void Quaternion::invert() {
		v_.x = -v_.x;
		v_.y = -v_.y;
		v_.z = -v_.z;
	}
	
	Quaternion Quaternion::inverse() const {
		return Quaternion(v_.w,-v_.x,-v_.y,-v_.z);
	}
	

	Quaternion& Quaternion::operator=( const Quaternion &q ) {
		v_ = q.v_;
		return *this;
	}

	Quaternion Quaternion::operator*( const Quaternion &q ) const {
		const vec4 &rv = q.v_;
		vec4 v;
		v.set(
            v_.w * rv.w - v_.x * rv.x - v_.y * rv.y - v_.z * rv.z,
            v_.w * rv.x + v_.x * rv.w + v_.y * rv.z - v_.z * rv.y,
            v_.w * rv.y + v_.y * rv.w + v_.z * rv.x - v_.x * rv.z,
            v_.w * rv.z + v_.z * rv.w + v_.x * rv.y - v_.y * rv.x
        );
		return Quaternion(v);
	}

	Quaternion& Quaternion::operator*=( const vec3& point ) {
		return (*this) *= Quaternion(point);
	}

	Quaternion&  Quaternion::operator*=( const Quaternion &q ) {
		(*this) = (*this)*q;
		return *this;
	}

	Quaternion  Quaternion::operator*( const vec3& point ) const {
		Quaternion r(*this);
		return r*=point;
	}
}