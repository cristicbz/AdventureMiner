#include "SceneNode.hpp"
#include "Util/Debug.hpp"
#include "Util/OpenGL.hpp"
#include "SceneManager.hpp"

#include <algorithm>
#include <functional>


#define log std::cout << "SceneNode: "

namespace cge {
	// ctors & dtors
	SceneNode::SceneNode( SceneManager &m )
		: mngr_           (m),
	      parent_         (0),
		  entity_         (0),
		  needUpdate_     (true),
		  dirtyTransform_ (true),
		  visible_        (false),
		  renderable_     (false) {
	
	}

	SceneNode::SceneNode( SceneNode &prnt )
		: mngr_           (prnt.mngr_),
	      parent_         (0),
		  entity_         (0),
		  needUpdate_     (true),
		  dirtyTransform_ (true),
		  visible_        (false),
		  renderable_     (false) {
		
		setParent(prnt);
	}

	SceneNode::~SceneNode() {
		if(parent_)
			parent_->removeChild_(this);

		std::for_each( begin(), end(),
			[]( SceneNode *n ) {
				n->parent_ = 0;
				delete n;
			}
		);

		if(entity_)
			entity_->onDestroyed();
	}

	
	// tree methods
	void SceneNode::setParent( SceneNode& newParentRef ) {
		SceneNode* newParent = &newParentRef;
		
		cgeAssert( newParent->mngr_ == mngr_ );

		if( parent_ == newParent )
			return;

		if(parent_)
			parent_->removeChild_(this);

		if(newParent)
			newParent->addChild_(this);

		parent_ = newParent;
		requireUpdate_();
	}

	void SceneNode::explode() {
		cgeAssert( parent_ );
		std::for_each( begin(), end(),
			[this]( SceneNode *n ) {
				n->parent_ = 0;
				n->setParent(*parent_);
				n->parentRotate(localRot_);
				n->parentTranslate(localPos_);
			}
		);
		children_.clear();
	}

	// transformation methods
	void SceneNode::setPosition( const vec3& newPosition ) {
		localPos_ = newPosition;
		requireUpdate_();
	}
	
	void SceneNode::setRotation( const Quaternion& newRotation ) {
		localRot_ = newRotation;
		requireUpdate_();
	}

	void SceneNode::localTranslate ( const vec3& with ) {
		localPos_ += localRot_.unrotateVector(with);
		requireUpdate_();
	}
	
	void SceneNode::worldTranslate ( const vec3& with ) {
		localPos_ += getWorldRotation().unrotateVector(with);
		requireUpdate_();
	}
	
	void SceneNode::parentTranslate( const vec3& with ) {
		localPos_ += with;
		requireUpdate_();
	}

	void SceneNode::localRotate( const Quaternion& by ) {
		localRot_ *= by;
		requireUpdate_();
	}
	
	void SceneNode::worldRotate( const Quaternion& by ) {
		const Quaternion& r = getWorldRotation();
		localRot_ = localRot_ * r.inverse()*by*r;
		requireUpdate_();
	}
	
	void SceneNode::parentRotate( const Quaternion& by ) {
		localRot_.premultiply(by);
		requireUpdate_();
	}

	// entity methods
	void SceneNode::attachEntity( Entity& e ) {
		if( entity_ )
			entity_->onDetached();
		entity_ = &e;
		entity_->node_ = this;
		entity_->onAttached();
	}

	void SceneNode::detachEntity() {
		if( entity_ )
			entity_->onDetached();
		entity_ = 0;
	}


	// rendering methods
	void SceneNode::render() {
		update();
		if( isRenderable() ) {
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			applyTransform_();
			// since we check isRenderable(), entity_!=0
			entity_->onRendered();
			glPopMatrix();
		}
	}

	void SceneNode::renderAll() {
		render();
		for(auto i=children_.begin(); i!=children_.end(); ++i)
			(*i)->renderAll();
	}

	void SceneNode::setVisibility( bool newVisibility, bool propagate ) {
		visible_ = newVisibility;
		if( propagate ) {
			std::for_each( begin(), end(),
				[newVisibility]( SceneNode *n ) {
					n->setVisibility( newVisibility, true );
				}
			);
		}
	}

	void SceneNode::setRenderable ( bool newRenderable ) {
		if( renderable_ == newRenderable )
			return;
		
		renderable_ = newRenderable;
		if( renderable_ )
			mngr_.addRenderable( *this );
		else
			mngr_.removeRenderable( *this );
	}


	// private methods
	void SceneNode::requireUpdate_() {
		needUpdate_ = true;
		std::for_each( begin(), end(),
			std::mem_fn( &SceneNode::requireUpdate_ )
		);
	}
	
	void SceneNode::addChild_( SceneNode *child ) {
		children_.push_back(child);
	}
	
	void SceneNode::removeChild_( SceneNode *child ) {
		children_.remove(child);
	}

	void SceneNode::updateTransform_() {
		dirtyTransform_ = true;
		if(parent_) {
			localRot_.renormalize();
			worldPos_ = parent_->getWorldRotation().rotateVector(localPos_) + parent_->getWorldPosition();
			worldRot_ = parent_->getWorldRotation()*localRot_;
		} else {
			localRot_.renormalize();
			worldPos_ = localPos_;
			worldRot_ = localRot_;
		}
	}

	void SceneNode::applyTransform_() {
		if(dirtyTransform_) {
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
				glLoadIdentity();
				getWorldRotation().toMatrix(transform_);
				glTranslatef(getWorldPosition().x,getWorldPosition().y,getWorldPosition().z);
				glMultMatrixf(transform_);
				
				glGetFloatv(GL_MODELVIEW_MATRIX,transform_);
			glPopMatrix();
			dirtyTransform_ = false;
		}
		glMultMatrixf(transform_);
	}
}