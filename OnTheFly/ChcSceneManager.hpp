#pragma once

#include "Renderer/SceneManager.hpp"
#include <memory>

using namespace cge;

class ChcSceneManager: public SceneManager {
public:
	ChcSceneManager( Engine &e );
	~ChcSceneManager();

	virtual SceneNode& getRoot();

	// rendering
	virtual void setCamera( Camera& camera );
	virtual void addRenderable( SceneNode& node );
	virtual void removeRenderable( SceneNode& node );
	virtual void runTask();

	virtual Camera&	getCamera() { return *cam_; }

private:
	struct Node;
	Camera* cam_;
	std::auto_ptr<Node>   root_;
};
