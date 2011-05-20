#pragma once

#include "Math/Vectors.hpp"
#include "SceneNode.hpp"

#include <vector>

namespace cge {
	// TODO: Revamp Mesh to include materials & stuff.
	class Mesh: public Entity {
	public:
		class Creator;
		struct Data;

		explicit Mesh( const Creator &creator );

		void onAttached();
		void onRendered();

		// types for derived classes and mesh-creators
		class Creator {
		public:
			virtual ~Creator() {}
			virtual void create( Data& data ) const = 0;
		};

		typedef unsigned short Index;
		typedef float          Scalar;
		
		struct Poly {
			Index vertex[3];
			Index normal[3];
		};

		typedef std::vector<vec3f> PointArray;
		typedef std::vector<Poly>  PolyArray;

		struct Data {
			PointArray verts;
			PointArray norms;
			PolyArray  polys;
		};

	protected:
		Data d_;
	};

	typedef Mesh::Creator MeshCreator;
	typedef Mesh::Data    MeshData;
}
