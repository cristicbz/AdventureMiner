#pragma once

#include <iostream>
#include <cmath>

#include "Constants.hpp"

namespace cge {
	template< class T >
	class base_vec2 {
	public:
		T						x;
		T						y;

								base_vec2(): x(0), y(0) {}
								base_vec2( T initialX, T initialY ): x(initialX), y(initialY) {}
								base_vec2( const base_vec2& r ): x(r.x), y(r.y) {}
		template< class U >		base_vec2( const base_vec2<U>& r ): x( static_cast<T>(r.x) ), y( static_cast<T>(r.y) ) {}
		explicit				base_vec2( T s ): x(s), y(s) {}
		

		T&						operator[]( int i ) { return *(static_cast<T*>(*this) + i); }
		const T					operator[]( int i ) const { return *(static_cast<const T*>(*this) + i); }

		void					set( T newX, T newY ) { x=newX; y=newY; }
		void					copyTo( T* arr ) const { arr[0]=x; arr[1]=y; }

		base_vec2&				operator= ( const base_vec2& r ) { set(r.x,r.y); return *this; }
		
		base_vec2&				operator+=( const base_vec2& r ) { x+=r.x; y+=r.y; return *this; }
		base_vec2&				operator-=( const base_vec2& r ) { x-=r.x; y-=r.y; return *this; }
		base_vec2&				operator*=( T s ) { x*=s; y*=s; return *this; }
		base_vec2&				operator/=( T s ) { x/=s; y/=s; return *this; }
		
		const base_vec2			operator+( const base_vec2& r ) const { return base_vec2(x+r.x,y+r.y); }
		const base_vec2			operator-( const base_vec2& r ) const { return base_vec2(x-r.x,y-r.y); }
		const base_vec2			operator*( T s ) const { return base_vec2(x*s,y*s); }
		const base_vec2			operator/( T s ) const { return base_vec2(x/s,y/s); }
		const base_vec2			operator-()      const { return base_vec2(-x,-y); }
		
		friend const base_vec2	operator*(T s, const base_vec2& v) { return v*s; }
		
		const base_vec2			unit()                         const { base_vec2 v(*this); v.normalize(); return v; }
		const base_vec2			modulate( const base_vec2 &v ) const { return base_vec2(x*v.x,y*v.y); }
		
		T						dot( const base_vec2& r ) const { return x*r.x + y*r.y; }
		T						norm()                    const { return sqrt(dot(*this)); }
		T						normSquared()             const { return dot(*this); }
		

		void					normalize() { T n = norm(); if(n!=0.0) (*this)/=n; }

								operator T*() { return reinterpret_cast<T*>(this); }
								operator const T*()   const { return reinterpret_cast<const T*>(this); }

		friend std::ostream&	operator<<( std::ostream &os, const base_vec2 &v ) { return os << '(' <<  v.x << ", " << v.y << ')'; }
	};

	template< class T >
	class base_vec3 {
	public:
		T						x;
		T						y;
		T						z;

								base_vec3(): x(0), y(0), z(0) {}
								base_vec3( T *arr ): x(arr[0]), y(arr[1]), z(arr[2]) {}
								base_vec3( T initialX, T initialY, T initialZ ): x(initialX), y(initialY), z(initialZ) {}
								base_vec3( const base_vec3& r ): x(r.x), y(r.y), z(r.z) {}
		template< class U >		base_vec3( const base_vec3<U>& r ): x( static_cast<T>(r.x) ), y( static_cast<T>(r.y) ), z( static_cast<T>(r.z) ) {}
		explicit				base_vec3( T s ): x(s), y(s), z(s) {}

		T&						operator[]( int i ) { return *(static_cast<T*>(*this) + i); }
		const T					operator[]( int i ) const { return *(static_cast<const T*>(*this) + i); }

		void					set( T *arr ) { x=arr[0]; y=arr[1]; z=arr[2]; }
		void					set( T newX, T newY, T newZ ) { x=newX; y=newY; z=newZ; }

		void					copyTo( T* arr ) const { arr[0]=x; arr[1]=y; arr[2]=z; }

		base_vec3&				operator= ( const base_vec3& r ) { set(r.x,r.y,r.z); return *this; }
		
		base_vec3&				operator+=( const base_vec3& r ) { x+=r.x; y+=r.y; z+=r.z; return *this; }
		base_vec3&				operator-=( const base_vec3& r ) { x-=r.x; y-=r.y; z-=r.z; return *this; }
		base_vec3&				operator*=( T s ) { x*=s; y*=s; z*=s; return *this; }
		base_vec3&				operator/=( T s ) { x/=s; y/=s; z/=s; return *this; }
		
		const base_vec3			operator+( const base_vec3& r ) const { return base_vec3(x+r.x,y+r.y,z+r.z); }
		const base_vec3			operator-( const base_vec3& r ) const { return base_vec3(x-r.x,y-r.y,z-r.z); }
		const base_vec3			operator*( T s ) const { return base_vec3(x*s,y*s,z*s); }
		const base_vec3			operator/( T s ) const { return base_vec3(x/s,y/s,z/s); }
		const base_vec3			operator-()      const { return base_vec3(-x,-y,-z); }
		

		bool					operator>=( const base_vec3& r ) const { return x>=r.x && y>=r.y && z>=r.z; }
		bool					operator<=( const base_vec3& r ) const { return x<=r.x && y<=r.y && z<=r.z; }
		
		friend base_vec3		operator*(T s, const base_vec3& v) { return v*s; }
		
		const base_vec3			unit()                      const { base_vec3 v(*this); v.normalize(); return v; }
		const base_vec3			cross( const base_vec3& r ) const { return base_vec3(y*r.z-z*r.y,z*r.x-x*r.z,x*r.y-y*r.x); }
		template< class Fun >
		const base_vec3			map( const Fun &f )			const { return base_vec3(f(x),f(y),f(z)); }
		T						dot( const base_vec3& r )   const { return x*r.x + y*r.y + z*r.z; }
		T						norm()                      const { return sqrt(dot(*this)); }
		T						normSquared()               const { return dot(*this); }
		const base_vec3			modulate( const base_vec3 &v ) const { return base_vec3(x*v.x,y*v.y,z*v.z); }
		
		void					normalize() { T n = norm(); if(n) (*this)/=n; }
		const base_vec3<int>	intFloor() { return base_vec3<int>( intFloor(x), intFloor(y), intFloor(z) ); }
		
								operator T*()               { return reinterpret_cast<T*>(this); }
								operator const T*()   const { return reinterpret_cast<const T*>(this); }

		friend std::ostream&	operator<<( std::ostream &os, const base_vec3 &v ) { return os << '(' <<  v.x << ", " << v.y << ", " << v.z << ')'; }

		const base_vec3			minArg()					const {
			base_vec3 v(*this);
			if( v.x < v.y ) {
				v.y = 0;
				if( v.x < v.z )
					v.z = 0;
				else
					v.x = 0;
			} else {
				v.x = 0;
				if( v.y < v.z )
					v.z = 0;
				else
					v.y = 0;
			}
			
			return v;
		}
	};

	template< class T >
	class base_vec4 {
	public:
		T						w;
		T						x;
		T						y;
		T						z;
		
								base_vec4(): w(0), x(0), y(0), z(0) {}
								base_vec4( T initialW, T initialX, T initialY, T initialZ): w(initialW), x(initialX), y(initialY), z(initialZ) {}
								base_vec4( const base_vec4& r ): w(r.w), x(r.x), y(r.y), z(r.z) {}
		template< class U >		base_vec4( const base_vec4<U>& r ): w( static_cast<T>(r.w) ), x( static_cast<T>(r.x) ), y( static_cast<T>(r.y) ), z( static_cast<T>(r.z) ) {}
		explicit				base_vec4( T s ): w(s), x(s), y(s), z(s) {}

		T&						operator[]( int i ) { return *(static_cast<T*>(*this) + i); }
		const T					operator[]( int i ) const { return *(static_cast<const T*>(*this) + i); }

		void					set( T newW, T newX, T newY, T newZ ) { w=newW; x=newX; y=newY; z=newZ; }

		base_vec4&				operator= ( const base_vec4& r ) { set(r.w,r.x,r.y,r.z); return *this; }
		
		base_vec4&				operator+=( const base_vec4& r ) { x+=r.x; y+=r.y; z+=r.z; w+=r.w; return *this; }
		base_vec4&				operator-=( const base_vec4& r ) { x-=r.x; y-=r.y; z-=r.z; w-=r.w; return *this; }
		base_vec4&				operator*=( T s ) { x*=s; y*=s; z*=s; w*=s; return *this; }
		base_vec4&				operator/=( T s ) { x/=s; y/=s; z/=s; w/=s; return *this; }
		
		const base_vec4			operator+( const base_vec4& r ) const { return base_vec4(w+r.w,x+r.x,y+r.y,z+r.z); }
		const base_vec4			operator-( const base_vec4& r ) const { return base_vec4(w-r.w,x-r.x,y-r.y,z-r.z); }
		const base_vec4			operator*( T s ) const { return base_vec4(w*s,x*s,y*s,z*s); }
		const base_vec4			operator/( T s ) const { return base_vec4(w/s,x/s,y/s,z/s); }
		const base_vec4			operator-()      const { return base_vec4(-w,-x,-y,-z); }
		
		friend base_vec4		operator*(T s, const base_vec4& v) { return v*s; }
		
		const base_vec4			modulate( const base_vec4& v) const { return base_vec4(w*r.w,x*r.x,y*r.y,z*r.z); }
		const base_vec4			unit()                        const { base_vec4 v(*this); v.normalize(); return v; }
		T						dot( const base_vec4& r )     const { return x*r.x + y*r.y + z*r.z + w*r.w; }
		T						norm()                        const { return sqrt(dot(*this)); }
		T						normSquared()                 const { return dot(*this); }
		
		
		void					normalize() { T n = norm(); if(n) (*this)/=n; }

								operator T*() { return reinterpret_cast<T*>(this); }
								operator const T*()   const { return reinterpret_cast<const T*>(this); }
		
		friend std::ostream&	operator<<( std::ostream &os, const base_vec4 &v ) { return os << '(' << v.w << ", " << v.x << ", " << v.y << ", " << v.z << ')'; }
	};

	typedef base_vec2<int>    vec2i;
	typedef base_vec3<int>    vec3i;
	typedef base_vec4<int>    vec4i;
	typedef base_vec2<short>  vec2s;
	typedef base_vec3<short>  vec3s;
	typedef base_vec4<short>  vec4s;
	typedef base_vec2<double> vec2d;
	typedef base_vec3<double> vec3d;
	typedef base_vec4<double> vec4d;
	typedef base_vec2<float>  vec2f;
	typedef base_vec3<float>  vec3f;
	typedef base_vec4<float>  vec4f;
	typedef base_vec2<char>  vec2b;
	typedef base_vec3<char>  vec3b;
	typedef base_vec4<char>  vec4b;

	typedef base_vec2<Scalar> vec2;
	typedef base_vec3<Scalar> vec3;
	typedef base_vec4<Scalar> vec4;

	struct ctVec {
		static const vec3 x;
		static const vec3 y;
		static const vec3 z;
		static const vec3 zero;

		static const vec3		direction6[ 6 ];
		//static const unsigned	normal6[ 6 ];
	};


	template< class T >
	T linearInterpolate( const T& a, const T& b, Scalar delta ) {
		return a*(1.0f-delta) + b*delta;
	}
}

