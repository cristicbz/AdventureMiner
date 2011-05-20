#pragma once

#include "Renderer/Mesh.hpp"

#include <iostream>

namespace cge {
	struct CorruptMeshFile {};

	class BspMeshLoader: public MeshCreator {
	public:
		BspMeshLoader( std::istream &is );

		void create( MeshData &data ) const;

	private:
		std::istream &is_;
	};
}