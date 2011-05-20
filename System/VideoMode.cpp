#pragma once

#include "VideoMode.hpp"
#include <SDL/SDL.h>

#include <iostream>

namespace cge {
	VideoMode::VideoMode( unsigned width, unsigned height, unsigned bpp ) {
		w_   = width;
		h_   = height;
		bpp_ = bpp;
		fs_  = false;
		dec_ = true;
		vsy_ = false;
	}

	VideoMode::VideoMode( const VideoMode &rhs ) {
		w_   = rhs.w_;
		h_   = rhs.h_;
		bpp_ = rhs.bpp_;
		fs_  = rhs.fs_;
		dec_ = rhs.dec_;
		vsy_ = rhs.vsy_;
	}

	unsigned VideoMode::getWidth() const {
		return w_;
	}

	unsigned VideoMode::getHeight() const {
		return h_;
	}

	unsigned VideoMode::getBpp() const {
		return bpp_;
	}

	bool     VideoMode::isFullscreen() const {
		return fs_;
	}

	bool     VideoMode::isDecorated() const {
		return dec_;
	}

	bool     VideoMode::hasVSync() const {
		return vsy_;
	}


	bool     VideoMode::isValid() const {
		int flags = SDL_OPENGL;
		if( fs_ )
			flags |= SDL_FULLSCREEN;

		if( !dec_ )
			flags |= SDL_NOFRAME;
		return SDL_VideoModeOK(w_,h_,bpp_,flags) == bpp_;
	}

	void     VideoMode::setSize( unsigned width, unsigned height ) {
		w_ = width;
		h_ = height;
	}

	void     VideoMode::setBpp( unsigned bpp ) {
		bpp_ = bpp;
	}

	void     VideoMode::setFullscreen( bool fullscreen ) {
		fs_ = fullscreen;
		if( fs_ ) dec_ = false;
		else vsy_ = false;
	}

	void     VideoMode::setDecorated( bool decorated ) {
		if( fs_ ) return;
		dec_ = decorated;
	}

	void     VideoMode::setVSync( bool vsync ) {
		if( !fs_ ) return;
		vsy_ = vsync;
	}

	std::ostream& operator<<( std::ostream &os,
			                  const VideoMode &m ) {
		
		os << '(' << m.w_ << ", " << m.h_ << ", " << m.bpp_;
		
		if( m.fs_ )
			os << ", fullscreen";
		if( !m.dec_ )
			os << ", noframe";
		if( m.vsy_ )
			os << ", vsync";

		os << ')';
		return os;
	}

	bool VideoMode::operator==( const VideoMode &rhs ) const {
		return w_   == rhs.w_   &&
		       h_   == rhs.h_   &&
		       bpp_ == rhs.bpp_ &&
			   fs_  == rhs.fs_  &&
			   dec_ == rhs.dec_ &&
			   vsy_ == rhs.vsy_;
	}

	bool VideoMode::operator!=( const VideoMode &rhs ) const {
		return !(*this == rhs);
	}

	bool VideoMode::operator<( const VideoMode &rhs ) const {
		if( bpp_ < rhs.bpp_ ) return true;
		if( w_ < rhs.w_ )     return true;
		if( h_ < rhs.h_ )     return true;
		if( fs_ < rhs.fs_ )   return true;
		if( dec_ < rhs.dec_ ) return true;
		if( vsy_ < rhs.vsy_ ) return true;
		return false;
	}

	bool VideoMode::isAlphaSupported() {
		return SDL_VideoModeOK(32,32,32,SDL_OPENGL) == 32;
	}

	std::vector<VideoMode> VideoMode::getAvailable() {
		std::vector<VideoMode> ms;
		ms.reserve(16);

		VideoMode tpl;
		tpl.setFullscreen(true);
		tpl.setDecorated(false);
		tpl.setVSync(false);

		SDL_Rect** mode;
		mode = SDL_ListModes(0,SDL_FULLSCREEN|SDL_OPENGL);
		do  {
			ms.push_back(tpl);
			ms.back().setSize( (*mode)->w, (*mode)->h );
		} while( *(++mode) );

		return ms;
	}

}