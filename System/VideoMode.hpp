#pragma once

#include <vector>
#include <iosfwd>

namespace cge {
	class VideoMode {
	public:
		VideoMode( unsigned width=0, unsigned height=0, unsigned bpp=0 );
		VideoMode( const VideoMode &rhs );

		unsigned getWidth() const;
		unsigned getHeight() const;
		unsigned getBpp() const;
		bool     isFullscreen() const;
		bool     isDecorated() const;
		bool     hasVSync() const;

		bool     isValid() const;

		void     setSize( unsigned width, unsigned height );
		void     setBpp( unsigned bpp );
		void     setFullscreen( bool fullscreen );
		void     setDecorated( bool decorated );
		void     setVSync( bool vsync );

		friend std::ostream& operator<<( std::ostream &ostream,
		                                 const VideoMode &videoMode );
		
		bool operator!=( const VideoMode &rhs ) const;
		bool operator==( const VideoMode &rhs ) const;
		bool operator<( const VideoMode &rhs ) const;

		static bool                     isAlphaSupported();
		static std::vector<VideoMode>   getAvailable();

	private:
		unsigned w_,h_,bpp_;
		bool     fs_,dec_,vsy_;
	};
}