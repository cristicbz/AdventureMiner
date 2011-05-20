#pragma once

#include "Math/Vectors.hpp"
#include "System/InputListener.hpp"
#include "System/Task.hpp"

namespace cge {
	// ================================== FORWARD DECLARATIONS =================================== //
	class Camera;
	class Input;
	class Engine;
	class Quaternion;

	// ==================================== CLASS DECLARATION ==================================== //
	class FpsCameraControl: public InputListener, public Task {
	public:
		// ========================= C-Tor =========================== //
						FpsCameraControl( Input& input, Camera& camera );

		Camera&			getCamera();
		bool			isMoving();


		// ==================== Listener Control ===================== //
		void			connect();
		void			disconnect();


		// ========================== Task =========================== //
		virtual void	runTask();
	
	protected:
		// ===================== InputListener ======================= //
		void			onMouseMoved( MouseMovedEvent &ev );
		void			onMouseButtonPressed( MouseButtonEvent &ev );
		void			onMouseButtonReleased( MouseButtonEvent &ev );
		void			onKeyPressed( KeyEvent &ev );
		void			onKeyReleased( KeyEvent &ev );


		virtual void	checkTranslation( vec3& translateBy )		{}
		virtual void	checkRotation( Quaternion& newRotation )	{}
		

	private:
		// ===================== Private Members ===================== //
		KeyCode		kw_, ks_, ka_, kd_, kspace_, kc_;
		bool		dragging_;
		Scalar		rotSpeed_, movSpeed_;
		vec2		angles_, mc_;
		vec2		newAngles_;
		vec3		move_;
		Camera&		cam_;
		Input&		inp_;
		Engine&		eng_;
	};


	// =================== Inline Implementations =================== //
	inline Camera& FpsCameraControl::getCamera()	{ return cam_;}
	inline bool FpsCameraControl::isMoving()		{ return move_ != ctVec::zero; }
}