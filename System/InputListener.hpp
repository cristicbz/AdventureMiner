#pragma once

#include "Event.hpp"
#include "Util/BaseListener.hpp"
#include <list>

namespace cge {
	class KeyEvent;
	class MouseButtonEvent;
	class MouseEvent;
	class MouseMovedEvent;
	class InputSource;

	class InputListener: public BaseListener<InputListener> {
	public:
		virtual ~InputListener();

		virtual void onKeyPressed( KeyEvent &ev ) {}
		virtual void onKeyReleased( KeyEvent &ev ) {}
		virtual void onMouseButtonPressed( MouseButtonEvent &ev ) {}
		virtual void onMouseButtonReleased( MouseButtonEvent &ev ) {}
		virtual void onMouseMoved( MouseMovedEvent &ev ) {}
		virtual void onClose() {}
		virtual void onLostFocus() {}
		virtual void onRegainedFocus() {}
		virtual void onVideoResize() {}

		friend class InputSource;
	protected:
		InputListener();

	private:
		InputListener( const InputListener& );
	};
}