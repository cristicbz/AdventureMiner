#include "Event.hpp"

namespace cge {
	Event::Event( const Event &rhs )
		: ms_(rhs.ms_), c_(false) {}

	Event::Event( const ModState& modState )
		: ms_(modState), c_(false) {}

	void Event::consume() {
		c_ = true;
	}
	
	bool Event::isConsumed() const {
		return c_;
	}
	
	bool Event::isCtrlDown() const {
		return ms_.ctrl;
	}
	
	bool Event::isShiftDown() const {
		return ms_.shift;
	}
	
	bool Event::isAltDown() const {
		return ms_.alt;
	}
	
	bool Event::isSuperDown() const {
		return ms_.super;
	}

	const ModState& Event::getModState() const {
		return ms_;
	}
	
	KeyEvent::KeyEvent( const ModState &modState, KeyCode code, bool pressed )
		: Event(modState), kc_(code), p_(pressed) {}
	
	KeyEvent::KeyEvent( const KeyEvent &rhs )
		: Event(rhs), kc_(rhs.kc_), p_(rhs.p_) {}
	
	KeyCode KeyEvent::getCode() const {
		return kc_;
	}
	
	bool KeyEvent::isPressed() const {
		return p_;
	}
	

	MouseButtonEvent::MouseButtonEvent( const ModState &modState, MouseButton button, bool pressed, unsigned x, unsigned y )
		: Event(modState), b_(button), p_(pressed), x_(x), y_(y) {}

	MouseButtonEvent::MouseButtonEvent( const MouseButtonEvent &rhs )
		: Event(rhs), b_(rhs.b_), p_(rhs.p_), x_(rhs.x_), y_(rhs.y_) {}	

	MouseButton MouseButtonEvent::getButton() const {
		return b_;
	}
	
	bool MouseButtonEvent::isPressed() const {
		return p_;
	}
	
	unsigned MouseButtonEvent::getX() const {
		return x_;
	}
	
	unsigned MouseButtonEvent::getY() const {
		return y_;
	}
	

	MouseMovedEvent::MouseMovedEvent( const ModState &modState, unsigned x, unsigned y )
		: Event(modState), x_(x), y_(y) {}

	MouseMovedEvent::MouseMovedEvent( const MouseMovedEvent &rhs )
		: Event(rhs), x_(rhs.x_), y_(rhs.y_) {}

	unsigned MouseMovedEvent::getX() const {
		return x_;
	}
	
	unsigned MouseMovedEvent::getY() const {
		return y_;
	}
	
}