#pragma once

#include "Util/Debug.hpp"

#include <boost/noncopyable.hpp>

namespace cge {
	// ================================== FORWARD DECLARATIONS =================================== //
	class SceneNode;
	class Entity;


	// ==================================== CLASS DECLARATION ==================================== //
	class Entity: boost::noncopyable {
		friend class SceneNode;

	public:
		// ========================= Entity Events =========================== //
		virtual void	onRendered()    {}
		virtual void	onTransformed() {}
		virtual void	onDestroyed()   {}
		virtual void	onAttached()    {}
		virtual void	onDetached()    {}

		// ========================== Node Access ============================= //
		SceneNode&		getNode();
		bool			isAttached() const;

		// =========================== Destructor ============================ //
		virtual			~Entity();

	protected:
		// ======================== Protected C-Tor ========================== //
						Entity();

	private:
		// ======================== Private Members ========================== //
		SceneNode *node_;
	};

	// =================================== INLINE IMPLEMENTATION =================================== //
	inline Entity::Entity() : node_(0)		{ }
	inline SceneNode& Entity::getNode()		{ cgeAssert( node_ ); return *node_; }
	inline bool Entity::isAttached() const	{ return node_ != 0; }
}