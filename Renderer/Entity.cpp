#include "Entity.hpp"
#include "SceneNode.hpp"

using namespace cge;

Entity::~Entity() { 
	if( isAttached() )
		getNode().detachEntity();
}