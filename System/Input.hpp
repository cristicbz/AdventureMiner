#pragma once

#include <boost/shared_ptr.hpp>
#include "Task.hpp"
#include "InputSource.hpp"
#include "InputListener.hpp"

namespace cge {
	class Log;

	class Input: public Task, public InputSource {
	public:
		Input( Engine &engine );
		~Input();

		void setCursorVisible( bool b );
		void moveMouse(unsigned x, unsigned y);

		virtual void runTask();

	private:
		typedef void* EventImpl;

		void             updateMods (EventImpl ev);
		KeyEvent         makeKey    (EventImpl ev);
		MouseButtonEvent makeBut    (EventImpl ev);
		MouseMovedEvent  makeMov    (EventImpl ev);

		Log &lg_;
		ModState ms_;
	};
}