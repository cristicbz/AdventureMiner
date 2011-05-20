#include "System/Input.hpp"
#include "System/Engine.hpp"
#include "System/Event.hpp"
#include "Util/OpenGL.hpp"

#include "FpsCameraControl.hpp"
#include "Camera.hpp"

using namespace cge;

FpsCameraControl::FpsCameraControl( Input& input, Camera& camera)
: cam_(camera), inp_(input), eng_(input.getEngine()) {
	kw_ = SDLK_w; ks_ = SDLK_s;
	ka_ = SDLK_a; kd_ = SDLK_d;
	kc_ = SDLK_c;
	kspace_ = SDLK_SPACE;
	dragging_ = false;
	rotSpeed_ = 12.0;
	movSpeed_ = 8.0;
}

void FpsCameraControl::connect() {
	inp_.addInputListener(*this);
	eng_.addTask(*this);
}


void FpsCameraControl::disconnect() {
	eng_.removeTask(*this);
	inp_.removeInputListener(*this);
}

void FpsCameraControl::onMouseMoved( MouseMovedEvent &ev ) {
	if( ev.isConsumed() )
		return;

	if( dragging_ ) {
		vec2 rel = vec2(static_cast<Scalar>(ev.getX()),static_cast<Scalar>(ev.getY())) - mc_;
		newAngles_ += rel/1024.0f;
		// TODO: Proper clamping.
		if(newAngles_.y < -ctPiDiv2+0.01f ) newAngles_.y = -ctPiDiv2+0.01f;
		else if(newAngles_.y > ctPiDiv2-0.01f ) newAngles_.y = ctPiDiv2-0.01f;
		inp_.moveMouse(static_cast<unsigned>(mc_.x), static_cast<unsigned>(mc_.y));
		ev.consume();
	}
}

void FpsCameraControl::runTask() {
	// interpolate rotations
	Scalar distance = (angles_-newAngles_).normSquared();
	if( distance>=1e-16 ) {
		distance = sqrt(distance);
		Scalar delta = eng_.getFrameDelta()*rotSpeed_/distance;
		if(delta >= 1.0f)
			delta = 1.0f;
		angles_ = linearInterpolate(angles_,newAngles_,delta);
		
		auto newRot	= Quaternion(angles_.y,angles_.x,0.0f);
		checkRotation( newRot );
		cam_.getNode().setRotation( newRot );
	}

	if( move_.x!=0.0 || move_.y!=0.0 || move_.z!=0.0 ) {
		auto transBy = cam_.getNode().getWorldRotation().unrotateVector(vec3(move_.x,0.0f,move_.z));
		transBy.y = move_.y;
		transBy.normalize();
		transBy *= movSpeed_ * eng_.getFrameDelta();
		checkTranslation( transBy );
		
		cam_.getNode().parentTranslate( transBy );
		//cam_.getNode().worldTranslate( vec3(0.0f,transBy.y,0.0f) );
	}
}

void FpsCameraControl::onMouseButtonPressed( MouseButtonEvent &ev ) {
	if( ev.isConsumed() )
		return;
	if( ev.getButton() == SDL_BUTTON_RIGHT ) {
		dragging_ = true;
		inp_.setCursorVisible(false);
		mc_.set(static_cast<float>(ev.getX()),static_cast<float>(ev.getY()));
		ev.consume();
	}
}

void FpsCameraControl::onMouseButtonReleased( MouseButtonEvent &ev ) {
	if( ev.getButton() == SDL_BUTTON_RIGHT ) {
		dragging_ = false;
		inp_.setCursorVisible(true);
	}
}

void FpsCameraControl::onKeyPressed( KeyEvent &ev ) {
	if( ev.isConsumed() )
		return;

	bool consume = true;

	if( ev.getCode() == kw_ )
		move_.z = -1.0f;
	else if( ev.getCode() == ks_ )
		move_.z = 1.0f;
	else if( ev.getCode() == ka_ )
		move_.x = -1.0f;
	else if( ev.getCode() == kd_ )
		move_.x = 1.0f;
	else if( ev.getCode() == kspace_ )
		move_.y = 1.0f;
	else if( ev.getCode() == kc_ )
		move_.y = -1.0f;
	else
		consume = false;

	if( consume )
		ev.consume();
}

void FpsCameraControl::onKeyReleased( KeyEvent &ev ) {
	bool consume = true;

	if( ev.getCode() == kw_  || ev.getCode() == ks_ )
		move_.z = 0;
	else if( ev.getCode() == ka_ || ev.getCode() == kd_ )
		move_.x = 0;
	else if( ev.getCode() == kspace_ || ev.getCode() == kc_ )
		move_.y = 0;
	else
		consume = false;

	if( consume )
		ev.consume();
}