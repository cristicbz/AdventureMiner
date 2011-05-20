#pragma once

namespace cge {
	typedef unsigned KeyCode;
	typedef unsigned MouseButton;

	struct ModState {
		bool ctrl;
		bool shift;
		bool alt;
		bool super;
	};

	class Event {
	public:
		Event( const Event &rhs );

		void consume();
		bool isConsumed() const;

		bool isCtrlDown() const;
		bool isShiftDown() const;
		bool isAltDown() const;
		bool isSuperDown() const;

		const ModState& getModState() const;

	protected:
		Event( const ModState& modState );

	private:
		ModState ms_;
		bool     c_;
	};

	class KeyEvent: public Event {
	public:
		KeyEvent( const ModState &modState, KeyCode code, bool pressed );
		KeyEvent( const KeyEvent &rhs );

		KeyCode getCode() const;
		bool    isPressed() const;

	private:
		KeyCode kc_;
		bool    p_;
	};

	class MouseButtonEvent: public Event {
	public:
		MouseButtonEvent( const ModState &modState, MouseButton button, bool pressed, unsigned x, unsigned y );
		MouseButtonEvent( const MouseButtonEvent &rhs );

		MouseButton getButton() const;
		bool        isPressed() const;
		unsigned    getX() const;
		unsigned    getY() const;
	
	private:
		MouseButton b_;
		bool        p_;
		unsigned    x_, y_;
	};

	class MouseMovedEvent: public Event {
	public:
		MouseMovedEvent( const ModState &modState, unsigned x, unsigned y );
		MouseMovedEvent( const MouseMovedEvent &rhs );

		unsigned getX() const;
		unsigned getY() const;
	
	private:
		unsigned x_, y_;
	};
}