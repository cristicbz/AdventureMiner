#pragma once

#include "Vectors.hpp"
#include "Util/Debug.hpp"

namespace cge {
	class AlignedBox {
	public:
		AlignedBox(): min_(0.0), max_(0.0) { cgeAssert2( min_ <= max_, min_, max_ ); }
		AlignedBox( const vec3& min, const vec3& max ): min_(min), max_(max) { cgeAssert2( min_<=max_, min_, max_ ); }

		bool contains( const vec3& point ) {
			return point >= min_ && point <= max_;
		}

		void translate( const vec3& with ) {
			min_ += with;
			max_ += with;
		}

		void translateX( Scalar with ) {
			min_.x += with;
			max_.x += with;
		}

		void translateY( Scalar with ) {
			min_.y += with;
			max_.y += with;
		}
		
		void translateZ( Scalar with ) {
			min_.z += with;
			max_.z += with;
		}

		void merge( const vec3& point ) {
			if( point.x > max_.x )
				max_.x = point.x;
			else if( point.x < min_.x )
				min_.x = point.x;

			if( point.y > max_.y )
				max_.y = point.y;
			else if( point.y < min_.y )
				min_.y = point.y;

			if( point.z > max_.z )
				max_.z = point.z;
			else if( point.z < min_.z )
				min_.z = point.z;
		}

		void merge( const AlignedBox &box ) {
			merge(box.min_);
			merge(box.max_);
		}
		
		void setMinAndMax( const vec3& min, const vec3& max ) {
			min_ = min;
			max_ = max;
			cgeAssert2( min_<=max_, min_, max_ );
		}

		void setMinAndSize( const vec3& min, const vec3& size ) {
			min_ = min;
			max_ = min + size;
			cgeAssert2( min_<=max_, min_, max_ );
		}

		void setTwoCorners( const vec3& c1, const vec3& c2 ) {
			if( c1.x < c2.x ) { min_.x = c1.x; max_.x = c2.x; }
			else              { min_.x = c2.x; max_.x = c1.x; }
			if( c1.y < c2.y ) { min_.y = c1.y; max_.y = c2.y; }
			else              { min_.y = c2.y; max_.y = c1.y; }
			if( c1.z < c2.z ) { min_.z = c1.z; max_.z = c2.z; }
			else              { min_.z = c2.z; max_.z = c1.z; }
		}

		const vec3& getMinimum() const { return min_; }
		const vec3& getMaximum() const { return max_; }
		vec3&       getMinimum()       { return min_; }
		vec3&       getMaximum()       { return max_; }

		vec3 getSize()           const { return max_ - min_; }
		vec3 getCenter()         const { return (min_ + max_) / 2; }

	private:
		vec3 min_;
		vec3 max_;
	};

}