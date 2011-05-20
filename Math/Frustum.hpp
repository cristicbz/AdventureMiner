#pragma once

#include "Constants.hpp"
#include "Plane.hpp"
#include "AlignedBox.hpp"
#include <array>

namespace cge {
	class Frustum {
	public:
		template< class T >
		void			setFromTransform( T transform[ 16 ] );

		const Plane&	getPlane( Direction direction ) const;
		Plane&			getPlane( Direction direction );

		bool			contains( const vec3 &point ) const ;
		bool			intersects(  const AlignedBox &alignedBox ) const;

		void			normalize();

	private:
		vec3 pvertex_( const vec3& normal, const AlignedBox &box ) const;
		vec3 nvertex_( const vec3& normal, const AlignedBox &box ) const;

		std::array< Plane, 6 > p_;
	};

	template< class T >
	inline void Frustum::setFromTransform( T mvp[ 16 ] ) {
		p_[ DR_RIGHT ].set( mvp[3]-mvp[0], mvp[7]-mvp[4], mvp[11]-mvp[8],  mvp[15]-mvp[12] );
		p_[ DR_LEFT  ].set( mvp[3]+mvp[0], mvp[7]+mvp[4], mvp[11]+mvp[8],  mvp[15]+mvp[12] );
		p_[ DR_DOWN  ].set( mvp[3]+mvp[1], mvp[7]+mvp[5], mvp[11]+mvp[9],  mvp[15]+mvp[13] );
		p_[ DR_UP    ].set( mvp[3]-mvp[1], mvp[7]-mvp[5], mvp[11]-mvp[9],  mvp[15]-mvp[13] );
		p_[ DR_FRONT ].set( mvp[3]-mvp[2], mvp[7]-mvp[6], mvp[11]-mvp[10], mvp[15]-mvp[14] );
		p_[ DR_BACK  ].set( mvp[3]+mvp[2], mvp[7]+mvp[6], mvp[11]+mvp[10], mvp[15]+mvp[14] );
		normalize();
	}

	inline void Frustum::normalize() {
		for( int i = 0; i < 6; ++i )
			p_[ i ].normalize();
	}

	inline Plane& Frustum::getPlane( Direction direction ) {
		return p_[ direction ];
	}
	
	inline const Plane& Frustum::getPlane( Direction direction ) const {
		return p_[ direction ];
	}

	inline bool Frustum::contains( const vec3 &point ) const {
		for( int i = 0; i < 6; ++i )
			if( !p_[ i ].isFacing( point ) ) {
				std::cout << "Offending plane: " << i << '\n';
				return false;
			}
		return true;
	}

	inline vec3 Frustum::pvertex_( const vec3& normal, const AlignedBox &box ) const {
		const vec3& boxMax = box.getMaximum();
		const vec3& boxMin = box.getMinimum();
		
		vec3 r( boxMin );

		if( normal.x >= 0.0f ) r.x = boxMax.x;
		if( normal.y >= 0.0f ) r.y = boxMax.y;
		if( normal.z >= 0.0f ) r.z = boxMax.z;

		return r;
	}

	inline bool Frustum::intersects( const AlignedBox &alignedBox ) const {
		for( int i = 0; i < 6; ++i ) {
			const Plane& p = p_[ i ];
			if( !p.isFacing( pvertex_( p.getNormal(), alignedBox ) ) )
				return false;
		}
		return true;
	}
}