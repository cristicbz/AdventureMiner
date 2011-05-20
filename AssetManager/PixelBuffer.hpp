#pragma once

#include "Math/Vectors.hpp"

#include <functional>
#include <vector>

namespace cge {
	class BinaryBuffer;
	class PixelBuffer {
	public:
		typedef std::function<vec4 (int x, int y, const vec4&)> ApplyFuncInt;
		typedef std::function<vec4 (const vec2&, const vec4&)>  ApplyFuncVec2;
		typedef std::function<vec4 (const vec4&)>               ApplyFuncSimple;
		
		enum BoundsPolicy {
			B_WRAP,
			B_CLAMP,
			B_EXCEPTION
		};

		enum Filtering {
			F_NEAREST,
			F_LINEAR
		};

		PixelBuffer();
		PixelBuffer( const std::vector<char>& buffer );
		PixelBuffer( int width, int height );
		~PixelBuffer();

		void load( const std::vector<char>& buffer );

		int  getWidth() const;
		int  getHeight() const;

		void setBoundsPolicy( BoundsPolicy boundsPolicy );
		void setFiltering( Filtering filtering );

		void clear( const vec4& clearColor = vec4(0.0,0.0,0.0,1.0) );
		void fill ( const vec2& pos, const vec2& size, const vec4& color );
		void fill ( int x, int y, int w, int h, const vec4& color );
		void blit ( PixelBuffer& source, int dx, int dy, int sx=0, int sy=0, int sw=-1, int sh=-1);
		void blit ( PixelBuffer& source, const vec2& to, const vec2& from, const vec2& size );

		vec4 getPixel( int x, int y );
		vec4 getPixel( const vec2& position );

		void setPixel( int x, int y, const vec4& color );
		void setPixel( const vec2& position, const vec4& color );

		// TODO: make templates, use more efficient impl
		void apply( const ApplyFuncInt&    function );
		void apply( const ApplyFuncVec2&   function );
		void apply( const ApplyFuncSimple& function );

		void* getRawPixels();

		vec2 map(int x, int y);
		
		template< class Int >
		void unmap(const vec2& pos, Int &x, Int &y) {
			x = static_cast<Int>(pos.x*getWidth());
			y = static_cast<Int>(pos.y*getHeight());
		}
	
	private:
		void          applyBounds_( int& x, int& y );
		vec4          int2col_( unsigned int col );
		unsigned int  col2int_( const vec4 &col );
		unsigned int& pixAddr_( int x, int y );

		BoundsPolicy bp_;
		Filtering    fl_;
		void*        s_;
	};
}