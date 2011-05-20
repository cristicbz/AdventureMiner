#pragma once

#include "System/Task.hpp"

#include <boost/noncopyable.hpp>
#include <memory>

namespace cge {
	class Engine;
	class SceneNode;
	class Camera;

	class SceneManager: public Task, boost::noncopyable {
	public:
		// engine access
		Engine&				getEngine() { return eng_; }

		// tree access
		virtual	SceneNode&	getRoot() = 0;

		// rendering
		virtual void		setCamera( Camera& camera ) = 0;
		virtual void		addRenderable( SceneNode& node ) = 0;
		virtual void		removeRenderable( SceneNode& node ) = 0;

		virtual Camera&		getCamera() = 0;

		// since this object is noncopyable, comparison is address-based
		bool				operator==( const SceneManager &rhs ) { return this == &rhs; }

		virtual				~SceneManager() {}

	protected:
		SceneManager( Engine &e ): eng_(e) {}

		Engine &eng_;	
	};
}