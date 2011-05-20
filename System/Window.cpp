#include "Util/OpenGL.hpp"
#include "Window.hpp"
#include "Engine.hpp"
#include "Task.hpp"
#include "Log.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>

#define log lg_ << "Window: "

namespace cge {
	Window::Window( Engine &e  ) 
		: TaskGroup(e), lg_(e.getLog()) {}

	Window::~Window()  {
		log << "Destroyed.\n";
	}
		
	void Window::setVideoMode( const VideoMode& mode ) {
		log << "Setting video mode " << mode << ".\n";

		if(!mode.isValid()) {
			log << "VideoModeNotSupportedException raised.\n";
			throw VideoModeNotSupportedException();
		}

		if( mode == mode_ ) {
			log << "No changes detected. Not doing anything.\n";
			return;
		}

		bool opOk;
		int w = mode.getWidth();
		int h = mode.getHeight();
		int bpp = mode.getBpp();
		int flags = SDL_OPENGL;

		if( mode.isFullscreen() )
			flags |= SDL_FULLSCREEN;

		if( !mode.isDecorated() )
			flags |= SDL_NOFRAME;

		_putenv(_strdup("SDL_VIDEO_CENTERED=1")); 
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL,mode.hasVSync()?1:0);
		
		opOk = SDL_SetVideoMode(w,h,bpp,flags) != 0;
		if( !opOk ) {
			log << "Warning: Mode reported it was valid, but SDL_SetVideoMode() failed.\n";
			throw VideoModeNotSupportedException();
		}

		mode_ = mode;
		log << "Video mode set.\n";
	}
		
	void Window::setTitle( std::string title )  {
		log << "Title changed to '" << title << "'.\n";
		SDL_WM_SetCaption(title.c_str(),0);
	}

	void Window::setDecorated( bool decorated ) {
		log << "Decoration change to " << decorated << ".\n";
		VideoMode newMode = mode_;
		newMode.setDecorated(decorated);
		setVideoMode(newMode);
	}
		
	void Window::setFullscreen( bool fullscreen ) {
		log << "Fullscreen change to " << fullscreen << ".\n";
		VideoMode newMode = mode_;
		newMode.setFullscreen(fullscreen);
		setVideoMode(newMode);
	}
		
	void Window::setVSync( bool vsync ) {
		log << "VSync change to " << vsync << ".\n";
		VideoMode newMode = mode_;
		newMode.setVSync(vsync);
		setVideoMode(newMode);
	}

	void Window::setSize(unsigned width, unsigned height) {
		log << "Resizing to (" << width << ", " << height << ").\n";
		VideoMode newMode = mode_;
		newMode.setSize(width,height);
		setVideoMode(newMode);		
	}

	const VideoMode& Window::getVideoMode() const {
		return mode_;
	}
		
	unsigned Window::getWidth() const {
		return mode_.getWidth();
	}
		
	unsigned Window::getHeight() const {
		return mode_.getHeight();
	}

	bool     Window::isDecorated() const {
		return mode_.isDecorated();
	}
		
	bool     Window::isVSync() const {
		return mode_.hasVSync();
	}
		
	bool     Window::isFullscreen() const {
		return mode_.isFullscreen();
	}
		

	void     Window::swapBuffers() const {
		SDL_GL_SwapBuffers();
	}
	
	Window::VideoModeNotSupportedException::VideoModeNotSupportedException() 
		: runtime_error("Window: Selected video mode is unsupported.") {}
}