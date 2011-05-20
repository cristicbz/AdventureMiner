#include "Camera.hpp"
#include "Util/Debug.hpp"
#include "Util/OpenGL.hpp"

#include <iostream>


namespace cge {
	Camera::Camera( double aspect )
		: fov_(45.0), aspect_( aspect ), znear_(0.1), zfar_(64.0),
		  needProjUpdate_(true), needModelUpdate_(true) {}


	Camera::~Camera() {

	}

	void Camera::setProjection( double fov, double aspectRatio, double zNear, double zFar ) {
		fov_    = fov;
		aspect_ = aspectRatio;
		znear_  = zNear;
		zfar_   = zFar;
		requireProjectionUpdate();
	}

	void Camera::applyView() {
		cgeAssert( isAttached() );
		getNode().update();

		bool needFrustumUpdate = needProjUpdate_ || needModelUpdate_;

		glMatrixMode(GL_MODELVIEW);
		if( needModelUpdate_ ) {
			needModelUpdate_ = false;
			glLoadIdentity();
			updateModelview();
			glGetFloatv(GL_MODELVIEW_MATRIX,model_);
		} else {
			glLoadMatrixf(model_);
		}

		glMatrixMode(GL_PROJECTION);
		if( needProjUpdate_ ) {
			needProjUpdate_ = false;
			glLoadIdentity();
			updateProjection();
			glGetDoublev(GL_PROJECTION_MATRIX,proj_);
		} else {
			glLoadMatrixd(proj_);
		}

		

		if( needFrustumUpdate ) {
			glPushMatrix();
			glMultMatrixf( model_ );
			
			float transform[ 16 ];
			glGetFloatv( GL_PROJECTION_MATRIX, transform );
			glPopMatrix();

			frustum_.setFromTransform( transform );
		}
	}

	void Camera::setFov( double fov ) {
		fov_ = fov;
		requireProjectionUpdate();
	}

	void Camera::setAspectRatio( double aspectRatio ) {
		aspect_ = aspectRatio;
		requireProjectionUpdate();
	}

	void Camera::setZLimits( double zNear, double zFar ) {
		znear_ = zNear;
		zfar_  = zFar;
		requireProjectionUpdate();
	}

	void Camera::requireProjectionUpdate() {
		needProjUpdate_ = true;
	}

	void Camera::requireModelviewUpdate() {
		needModelUpdate_ = true;
	}

	void Camera::onTransformed() {
		requireModelviewUpdate();
	}

	void Camera::onAttached() {
		requireModelviewUpdate();
	}

	void Camera::updateProjection() {
		gluPerspective(fov_,aspect_,znear_,zfar_);
	}

	void Camera::updateModelview() {
		cgeAssert( isAttached() );
		const vec3&       pos = getNode().getWorldPosition();
		const Quaternion& rot = getNode().getWorldRotation();
		rot.toMatrix(model_);
		glLoadMatrixf(model_);
		glTranslatef(-pos.x,-pos.y,-pos.z);
	}
}