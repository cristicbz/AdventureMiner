#include "InputSource.hpp"

namespace cge {
	InputSource::InputSource( Engine &e )
		: e_(e) {}
	
	InputSource::~InputSource() {
	
	}

	void InputSource::addInputListener ( InputListener& inputListener ) {
		addListener(inputListener);
	}
	
	void InputSource::removeInputListener( InputListener& inputListener ) {
		removeListener(inputListener);
	}
	

	Engine& InputSource::getEngine() const {
		return e_;	
	}
	
}