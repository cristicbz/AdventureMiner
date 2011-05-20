#pragma once

#include "Util/BaseSource.hpp"
#include "InputListener.hpp"

namespace cge {
	class Engine;

	class InputSource: public BaseSource<InputListener> {
	public:
		InputSource( Engine &e );
		virtual ~InputSource();

		void    addInputListener   ( InputListener& inputListener );
		void    removeInputListener( InputListener& inputListener );

		Engine& getEngine() const;

	private:
		Engine& e_;
	};

	class InputGroup: public InputListener, public InputSource {
	public:
		InputGroup( Engine &e ): InputSource(e) {}

		void onKeyPressed( KeyEvent &ev )                  { inject( &InputListener::onKeyPressed, ev ); }
		void onKeyReleased( KeyEvent &ev )                 { inject( &InputListener::onKeyReleased, ev ); }
		void onMouseButtonPressed( MouseButtonEvent &ev )  { inject( &InputListener::onMouseButtonPressed, ev ); }
		void onMouseButtonReleased( MouseButtonEvent &ev ) { inject( &InputListener::onMouseButtonReleased, ev ); }
		void onMouseMoved( MouseMovedEvent &ev )           { inject( &InputListener::onMouseMoved, ev ); }
		void onClose()                                     { inject( &InputListener::onClose ); }
		void onLostFocus()                                 { inject( &InputListener::onLostFocus ); }
		void onRegainedFocus()                             { inject( &InputListener::onRegainedFocus ); }
		void onVideoResize()                               { inject( &InputListener::onVideoResize ); }
	};
}