#pragma once

#include <boost/noncopyable.hpp>

#include "SceneNode.hpp"
#include "Math/Frustum.hpp"

namespace cge {
	// ==================================== CLASS DECLARATION ==================================== //
	class Camera: public Entity {
	public:
		// ====================== SceneManager Access ========================= //
		void			applyView();

		// ======================= Projection Params ========================== //
		void			setFov( double fov );
		void			setAspectRatio( double aspectRatio );
		void			setZLimits( double zNear, double zFar );
		void			setProjection( double fov, double aspectRatio, double zNear, double zFar );
		const Frustum&	getFrustum() const;

		// ======================== C-tors & D-tors =========================== //
		explicit		Camera( double aspect = 4.0/3.0 );
		virtual			~Camera();
	
	protected:
		// ========================== Camera Detail =========================== //
		void			requireProjectionUpdate();
		void			requireModelviewUpdate();

		virtual void	updateProjection();
		virtual void	updateModelview();

	
	private:
		// ========================= Private Methods ========================= //
		// override SceneNodeListener
		virtual void	onTransformed();
		virtual void	onAttached();

		// ========================= Private Members ========================= //
		double			fov_;
		double			aspect_;
		double			znear_;
		double			zfar_;
		double			proj_[16];
		Scalar			model_[16];
		bool			needProjUpdate_;
		bool			needModelUpdate_;
		Frustum			frustum_;
	};

	inline const Frustum& Camera::getFrustum() const { return frustum_; }
}