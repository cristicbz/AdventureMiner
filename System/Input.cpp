#include "Input.hpp"

#include "Engine.hpp"
#include "Log.hpp"
#include "Event.hpp"
#include <SDL/SDL.h>

#define LOG lg_ << "Input: "

namespace cge {

	Input::Input( Engine &engine )
		: InputSource(engine), lg_(engine.getLog()) {
	}
	
	Input::~Input() {

	}

	void Input::setCursorVisible( bool b ) {
		SDL_ShowCursor(b?1:0);
	}

	void Input::moveMouse(unsigned x, unsigned y) {
		SDL_WarpMouse((Uint16)x,(Uint16)y);
	}

	void Input::updateMods (EventImpl x) {
		SDL_Event *ev = reinterpret_cast<SDL_Event*>(x);
		bool state = ev->key.state == SDL_PRESSED;
		switch(ev->key.keysym.sym) {
		case SDLK_LCTRL:
		case SDLK_RCTRL:
			ms_.ctrl = state;
			break;
		case SDLK_LSHIFT:
		case SDLK_RSHIFT:
			ms_.shift = state;
			break;
		case SDLK_LALT:
		case SDLK_RALT:
			ms_.alt = state;
			break;
		}
	}

	KeyEvent Input::makeKey (EventImpl x) {
		SDL_Event *ev = reinterpret_cast<SDL_Event*>(x);
		return KeyEvent(ms_,
			ev->key.keysym.sym,
			ev->key.state == SDL_PRESSED );
	}

	MouseButtonEvent Input::makeBut (EventImpl x) {
		SDL_Event *ev = reinterpret_cast<SDL_Event*>(x);
		return MouseButtonEvent(ms_,
			ev->button.button,
			ev->button.state == SDL_PRESSED,
			ev->button.x,
			ev->button.y );
	}

	MouseMovedEvent Input::makeMov (EventImpl x) {
		SDL_Event *ev = reinterpret_cast<SDL_Event*>(x);
		return MouseMovedEvent(ms_,
			ev->motion.x,
			ev->motion.y );
	}

	void Input::runTask() {
		SDL_Event ev;
		while( SDL_PollEvent( &ev ) ) {
			SDL_Delay(0);
			switch(ev.type) {
			case SDL_KEYDOWN:
				updateMods(&ev);
				inject( &InputListener::onKeyPressed, makeKey(&ev) );
				break;
			case SDL_KEYUP:
				updateMods(&ev);
				inject( &InputListener::onKeyReleased, makeKey(&ev) );
				break;
			case SDL_MOUSEBUTTONDOWN:
				inject( &InputListener::onMouseButtonPressed, makeBut(&ev) );
				break;
			case SDL_MOUSEBUTTONUP:
				inject( &InputListener::onMouseButtonReleased, makeBut(&ev) );
				break;
			case SDL_MOUSEMOTION:
				inject( &InputListener::onMouseMoved, makeMov(&ev) );
				break;
			case SDL_VIDEORESIZE:
				inject( &InputListener::onVideoResize );
				break;
			case SDL_QUIT:
				inject( &InputListener::onClose );
				break;
			case SDL_ACTIVEEVENT:
				if(ev.active.state & SDL_APPACTIVE) {
					if(  ev.active.gain == 0 )
						inject( &InputListener::onLostFocus );
					else
						inject( &InputListener::onRegainedFocus );
				}
				break;
			default:
				break;
			}
		}
	}
}