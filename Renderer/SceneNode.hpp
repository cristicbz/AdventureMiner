#pragma once

#include "Math/Vectors.hpp"
#include "Math/Quaternion.hpp"
#include "Util/Debug.hpp"
#include "Entity.hpp"

#include <list>
#include <boost/noncopyable.hpp>

namespace cge {
	// ================================== FORWARD DECLARATIONS =================================== //
	class SceneManager;
	class SceneNode;
	class Renderer;

	
	// ==================================== CLASS DECLARATION ==================================== //
	class SceneNode: boost::noncopyable {
	public:
		// ======================== Type Definitions ========================= //
		typedef std::list<SceneNode*>		SceneNodes;
		typedef SceneNodes::iterator		child_iterator;
		typedef SceneNodes::const_iterator	child_const_iterator;

		// ========================== Tree Methods =========================== //
		SceneManager&						getManager() const;
		void								setParent ( SceneNode& newParent );
		void								explode();
		SceneNode&							getParent() const;
		SceneNode&							createChild();
		bool								isRoot() const;

		child_iterator						begin();
		child_iterator						end();

		child_const_iterator				begin() const;
		child_const_iterator				end() const;


		// ======================== Transfom Methods ========================= //
		void								setPosition ( const vec3& newPosition );
		void								setRotation ( const Quaternion& newRotation );

		void								localTranslate ( const vec3& with );
		void								worldTranslate ( const vec3& with );
		void								parentTranslate ( const vec3& with );

		void								localRotate ( const Quaternion& by );
		void								worldRotate ( const Quaternion& by );
		void								parentRotate ( const Quaternion& by );

		const vec3&							getPosition() const;
		const Quaternion&					getRotation() const;

		const vec3&							getWorldPosition();
		const Quaternion&					getWorldRotation();

		// ========================= Entity Methods ========================= //
		void								attachEntity ( Entity& entity );
		void								detachEntity();

		Entity&								getEntity();
		bool								hasEntity() const;
		
		// ======================== Rendering Methods ======================== //
		void								render();
		void								renderAll();

		bool								isVisible()		const;
		bool								isRenderable()	const;

		void								setVisibility ( bool newVisibility, bool propagate=true );
		void								setRenderable ( bool newRenderable );
		
		void								update();

		// =========================== Destructor ============================ //
		virtual								~SceneNode();

	protected:
		// ========================== Constructors =========================== //
											SceneNode( SceneManager& manager );
											SceneNode( SceneNode& parent );

		// ======================== Manager Specific ========================= //
		virtual void						vUpdate() = 0;
		virtual SceneNode*					vCreateChild() = 0;

	private:
		// ========================= Private Methods ========================= //
		void								requireUpdate_();
		void								addChild_( SceneNode *child );
		void								removeChild_( SceneNode *child );
		void								updateTransform_();
		void								applyTransform_();

		// ========================= Private Members ========================= //
		SceneManager&						mngr_;
		Quaternion							localRot_;
		Quaternion							worldRot_;
		vec3								localPos_;
		vec3								worldPos_;
		SceneNode*							parent_;
		Entity*								entity_;
		SceneNodes							children_;

		bool								needUpdate_;
		bool								dirtyTransform_;
		bool								visible_;
		bool								renderable_;
		Scalar								transform_[16];
	};
	

	// =================================== INLINE IMPLEMENTATION =================================== //
	inline void SceneNode::update() {
		if(needUpdate_) {
			needUpdate_ = false;
			updateTransform_();
				
			if( entity_ )
				entity_->onTransformed();

			vUpdate();
		}
	}

	inline Entity& SceneNode::getEntity() {
		cgeAssert( hasEntity() );
		return *entity_;
	}

	inline SceneNode& SceneNode::getParent() const {
		cgeAssert(!isRoot());
		return *parent_;
	}

	inline SceneNode& SceneNode::createChild()				{ return *vCreateChild(); }
	inline bool SceneNode::isVisible() const				{ return visible_ && renderable_; }
	inline bool SceneNode::isRenderable() const				{ return renderable_; }
	
	inline bool	SceneNode::hasEntity() const				{ return entity_ != 0; }
	inline const vec3& SceneNode::getPosition() const		{ return localPos_; }
	inline const Quaternion& SceneNode::getRotation() const { return localRot_; }
	inline const vec3& SceneNode::getWorldPosition()		{ update(); return worldPos_; }
	inline const Quaternion& SceneNode::getWorldRotation()	{ update(); return worldRot_; }
	inline bool SceneNode::isRoot() const					{ return parent_==0; }
	inline SceneManager& SceneNode::getManager() const		{ return mngr_; }

	inline SceneNode::child_iterator SceneNode::begin()		{ return children_.begin(); }
	inline SceneNode::child_iterator SceneNode::end()		{ return children_.end(); }

	inline SceneNode::child_const_iterator SceneNode::begin() const	{ return children_.begin(); }
	inline SceneNode::child_const_iterator SceneNode::end() const	{ return children_.end(); }

	
}

