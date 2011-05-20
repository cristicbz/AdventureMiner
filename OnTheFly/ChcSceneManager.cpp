#include "ChcSceneManager.hpp"

#include "Renderer/SceneNode.hpp"
#include "Renderer/Camera.hpp"

#include "Util/OpenGL.hpp"

using namespace cge;

struct ChcSceneManager::Node: public SceneNode {
	Node( ChcSceneManager& m ): SceneNode(m) {}
	Node( Node& p )           : SceneNode(p) {}

	virtual void vUpdate() {

	}

	virtual Node* vCreateChild() {
		return new Node(*this);
	}
};

ChcSceneManager::ChcSceneManager( Engine &e )
	: SceneManager(e) {

	root_.reset( new Node(*this) );
}

ChcSceneManager::~ChcSceneManager() {}

SceneNode& ChcSceneManager::getRoot() { return *root_; }

// rendering
void ChcSceneManager::setCamera( Camera& camera ) {
	cam_ = &camera;
}

void ChcSceneManager::addRenderable( SceneNode& node ) {

}


void ChcSceneManager::removeRenderable( SceneNode& node ) {

}

void ChcSceneManager::runTask() {
	if( !cam_ || ! cam_->isAttached() )
		return;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam_->applyView();
	root_->renderAll();
}
