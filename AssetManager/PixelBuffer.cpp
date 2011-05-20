#include "PixelBuffer.hpp"
#include "Util/Debug.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <cassert>

#define S ((SDL_Surface*)s_)

namespace cge {
	inline Uint8 dbl2byte( Scalar dbl ) {
		if(dbl<0.0f) return 0;
		else if(dbl>1.0f) return 255;
		return static_cast<Uint8>(dbl*255.0f);
	}

	vec4  PixelBuffer::int2col_( unsigned int col ) {
		Uint8 r,g,b,a;
		SDL_GetRGBA(col,S->format,&r,&g,&b,&a);
		return vec4(r/255.0f,g/255.0f,b/255.0f,a/255.0f);
	}

	unsigned int PixelBuffer::col2int_( const vec4 &col ) {
		return SDL_MapRGBA(S->format,
			dbl2byte(col.x),
			dbl2byte(col.y),
			dbl2byte(col.z),
			dbl2byte(col.w));
	}

	void PixelBuffer::applyBounds_( int& x, int& y ) {
		assert(S);
		if( bp_ == B_EXCEPTION )
			cgeAssert2( x<getWidth() && y<getHeight() && x>0 && y>0, x,y );
		else if( bp_ == B_CLAMP ) {
			if( x >= getWidth() ) x = getWidth()-1;
			else if( x < 0 ) x = 0;

			if( y >= getHeight() ) y = getHeight()-1;
			else if( y < 0 ) y = 0;
		} else if( bp_ == B_WRAP ) {
			int w = getWidth();
			int h = getHeight();
			if( x>=w )
				do { x-=w; } while(x>=w);
			else if( x<0 )
				do { x+=w; } while(x<0);

			if( y>=h )
				do { y-=h; } while(y>=h);
			else if( y<0 )
				do { y+=h; } while(y<0);
		} else
			cgeAssert1( false && bp_, bp_ );
	}

	unsigned int& PixelBuffer::pixAddr_( int x, int y ) {
		cgeAssertM( S, "Null surface" );
		applyBounds_(x,y);
		return *(Uint32*)((Uint8*)S->pixels + y*S->pitch + x*S->format->BytesPerPixel);
	}
	
	PixelBuffer::PixelBuffer()
		: s_(0),bp_(B_CLAMP),fl_(F_NEAREST) {}
	
	PixelBuffer::PixelBuffer( const std::vector<char>& buffer ): s_(0),bp_(B_CLAMP),fl_(F_NEAREST) {
		s_ = 0;
		load(buffer);
	}
	
	PixelBuffer::PixelBuffer( int width, int height ) {
		cgeStub(std::cerr);
	}
	
	PixelBuffer::~PixelBuffer() {
		if( S )
			SDL_FreeSurface(S);
	}

	void PixelBuffer::load( const std::vector<char>& buffer ) {
		// TODO: Proper error handling.
		if( S )
			SDL_FreeSurface(S);
		SDL_RWops *rw = SDL_RWFromConstMem(&buffer[0],buffer.size());
		cgeEnforce( rw );
		SDL_Surface *raw = IMG_Load_RW(rw,0);
		SDL_FreeRW(rw);
		cgeEnforce( raw );
		s_ = SDL_DisplayFormatAlpha(raw);
		SDL_FreeSurface(raw);
		cgeEnforce( s_ );
	}
	

	int PixelBuffer::getWidth() const {
		if(S)
			return S->w;
		else
			return 1;
	}
	
	int PixelBuffer::getHeight() const {
		if(S)
			return S->h;
		else
			return 1;
	}
	

	void PixelBuffer::setBoundsPolicy( BoundsPolicy boundsPolicy ) {
		bp_ = boundsPolicy;
	}
	
	void PixelBuffer::setFiltering( Filtering filtering ) {
		fl_ = filtering;
	}
	

	vec2 PixelBuffer::map(int x, int y) {
		return vec2((Scalar)x/getWidth(),(double)y/getHeight());
	}

	void PixelBuffer::clear( const vec4& clearColor ) {
		cgeAssert( S );
		SDL_FillRect(S,0,col2int_(clearColor));
	}
	
	void PixelBuffer::fill ( const vec2& pos, const vec2& size, const vec4& color ) {
		cgeAssert( S );
		SDL_Rect r;
		unmap(pos,r.x,r.y);
		unmap(size,r.w,r.h);
		SDL_FillRect(S,&r,col2int_(color));
	}
	
	void PixelBuffer::fill ( int x, int y, int w, int h, const vec4& color ) {
		cgeAssert( S );
		SDL_Rect r = {x,y,w,h};
		SDL_FillRect(S,&r,col2int_(color));
	}
	
	void PixelBuffer::blit ( PixelBuffer& source, int dx, int dy, int sx, int sy, int sw, int sh) {
		cgeStub(std::cerr);
	}
	
	void PixelBuffer::blit ( PixelBuffer& source, const vec2& to, const vec2& from, const vec2& size ) {
		cgeStub(std::cerr);
	}
	

	vec4 PixelBuffer::getPixel( int x, int y ) {
		return int2col_(pixAddr_(x,y));
	}
	
	vec4 PixelBuffer::getPixel( const vec2& position ) {
		vec2 diff(position.x*getWidth(),position.y*getHeight());
		int x1,y1;

		x1 = (int)diff.x;
		y1 = (int)diff.y;
		if( fl_ == F_LINEAR ) {
			diff -= vec2(x1,y1);

			vec4 c1 = getPixel(x1,y1)*(1.0-diff.x) + getPixel(x1+1,y1)*diff.x;
			vec4 c2 = getPixel(x1,y1+1)*(1.0-diff.x) + getPixel(x1+1,y1+1)*diff.x;
			return c1*(1.0-diff.y)+c2*diff.y;
		} else
			return getPixel(x1,y1);
	}
	

	void PixelBuffer::setPixel( int x, int y, const vec4& color ) {
		pixAddr_(x,y) = col2int_(color);
	}
	
	void PixelBuffer::setPixel( const vec2& position, const vec4& color ) {
		int x,y;
		unmap(position,x,y);
		pixAddr_(x,y) = col2int_(color);
	}
	

	void PixelBuffer::apply( const ApplyFuncInt& function ) {
		assert(S);
		int w = getWidth(), h = getHeight(),i,j;
		for(i=0; i<w; ++i)
			for(j=0; j<h; ++j)
				setPixel(i,j, function(i,j,getPixel(i,j)));
	}
	
	void PixelBuffer::apply( const ApplyFuncVec2& function ) {
		assert(S);
		int w = getWidth(), h = getHeight(),i,j;
		for(i=0; i<w; ++i)
			for(j=0; j<h; ++j)
				setPixel(i,j, function(map(i,j),getPixel(i,j)));
	}
	
	void PixelBuffer::apply( const ApplyFuncSimple& function ) {
		assert(S);
		int w = getWidth(), h = getHeight(),i,j;
		for(i=0; i<w; ++i)
			for(j=0; j<h; ++j)
				setPixel(i,j, function(getPixel(i,j)));
	}

	void* PixelBuffer::getRawPixels() {
		return S->pixels;
	}
	
}