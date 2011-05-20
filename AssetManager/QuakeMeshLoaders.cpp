//#include "QuakeMeshLoaders.hpp"
//
//#include <SDL/SDL_types.h>
//#include <vector>
//#include <algorithm>
//#include <iterator>
//
//#include "Util/Debug.hpp"
//
//using namespace cge;
//
//namespace cge {
//	namespace bsp {
//		struct Point3f { Sint16 x,y,z; };
//
//		struct Edge    { Uint16 a,b; };
//
//		struct Lump    {
//			Uint32 offset;
//			Uint32 length;
//		};
//
//		struct Header  {
//			char magic[4];
//			Uint32 version;
//			Lump lump[19];
//		};
//
//		struct Face {
//			Uint16 plane;
//			Uint16 plane_side;
//
//			Uint32 first_edge;
//			Uint16 num_edges;
//
//			Uint16 texture_info;
//
//			Uint8  lightmap_styles[4];
//			Uint32 lightmap_offset;
//		};
//
//		// TODO: Proper error handling.
//		template< class T >
//		void readVector( std::istream& is, std::vector< T >& v, const Lump& l, std::size_t elemSize = sizeof( T ) ) {
//			v.resize( l.length / elemSize );
//			is.seekg( l.offset );
//			is.read(
//				reinterpret_cast<char*>(&v[0]),
//				l.length
//			);
//
//			cgeEnforce( !is.bad()  );
//		}
//	}
//}
//
//
//BspMeshLoader::BspMeshLoader( std::istream& is ): is_(is) {}
//
//
//void BspMeshLoader::create( MeshData &data ) const {
//	// TODO: Proper error handling.
//	// TODO: Break up into multiple functions
//	using namespace bsp;
//	Header hdr;
//	is_.seekg(0);
//	is_.read((char*)&hdr, sizeof(hdr));
//	cgeEnforce( hdr.magic[0] == 'I' && 
//		    hdr.magic[1] == 'B' &&
//			hdr.magic[2] == 'S' &&
//			hdr.magic[3] == 'P' );
//
//	cgeEnforce( hdr.version == 38 );
//
//	// read verts
//	readVector( is_, data.verts, hdr.lump[2] );
//	
//	// transform verts
//	// TODO: Proper vertex transform.
//	static const float scaling = 1.0f/40.0f;
//	vec3f center;
//	std::for_each( data.verts.begin(), data.verts.end(),
//		[&center]( vec3f& v ) { v *= scaling; center += v; }
//	);
//
//	center /= static_cast<float>(data.verts.size());
//
//	// not using std::transform for inplace sub - performance
//	std::for_each( data.verts.begin(), data.verts.end(),
//		[&center]( vec3f& v ) { v -= center; }
//	);
//
//	// read edges & face edges
//	std::vector<Edge> edges;
//	std::vector<Sint32> faceEdges;
//	
//	readVector( is_, edges, hdr.lump[11] );
//	readVector( is_, faceEdges, hdr.lump[12] );
//
//	// read faces
//	std::vector<Face> faces;
//	readVector( is_, faces, hdr.lump[6] );
//
//	unsigned nFaces = faces.size();
//
//	std::vector<unsigned>     fvs;
//	std::vector<Mesh::Poly>&  polys   = data.polys;
//	std::vector<vec3f>&		  normals = data.norms;
//
//	polys.reserve   (nFaces*3);
//	normals.reserve (nFaces*3);
//	std::for_each( faces.begin(), faces.end(),
//		[&fvs,&polys,&normals,&faceEdges,&edges,&data]( const Face& face ) {
//			auto& ledges = edges; // lower edges to this scope
//			
//			int first_edge = face.first_edge;
//			int end_edge   = face.first_edge + face.num_edges;
//
//			fvs.clear();
//			fvs.reserve( end_edge - first_edge + 1 );
//		
//			if( faceEdges[first_edge] < 0 )
//				fvs.push_back( edges[ -faceEdges[ first_edge ] ].b );
//			else
//				fvs.push_back( edges[  faceEdges[ first_edge ] ].a );
//		
//			std::transform(
//				faceEdges.begin() + first_edge,
//				faceEdges.begin() + end_edge,
//				std::back_inserter( fvs ),
//				[&ledges]( Sint32 fe ) {
//					return fe < 0 ?
//							ledges[ -fe ].a
//						:
//							ledges[  fe ].b;
//				}
//			);
//
//			vec3f      n;
//			Mesh::Poly poly;
//			for(unsigned vi=1;vi<fvs.size()-2;++vi) {
//				vec3f &v1 = data.verts[fvs[0]];
//				vec3f &v2 = data.verts[fvs[vi]];
//				vec3f &v3 = data.verts[fvs[vi+1]];
//			
//				n  = -(v1-v2).cross(v2-v3);
//				n.normalize();
//
//				poly.vertex[0] = fvs[0];
//				poly.vertex[1] = fvs[vi];
//				poly.vertex[2] = fvs[vi+1];
//				poly.normal[0] = poly.normal[1] = poly.normal[2] = normals.size();
//				polys.push_back( poly );
//				normals.push_back(n);
//			}
//		}
//	);
//}

#include "QuakeMeshLoaders.hpp"

#include <SDL/SDL_types.h>
#include <vector>
#include <list>


using namespace cge;

namespace cge {
	namespace bsp {
		struct Point3s { Sint16 x,y,z; };

		struct Edge    { Uint16 a,b; };

		struct Lump    {
			Uint32 offset;
			Uint32 length;
		};

		struct Header  {
			char magic[4];
			Uint32 version;
			Lump lump[19];
		};

		struct Face {
			Uint16 plane;
			Uint16 plane_side;

			Uint32 first_edge;
			Uint16 num_edges;

			Uint16 texture_info;

			Uint8  lightmap_styles[4];
			Uint32 lightmap_offset;
		};
	}
}


BspMeshLoader::BspMeshLoader( std::istream& is ): is_(is) {}


void BspMeshLoader::create( MeshData &data ) const {
	using namespace bsp;
	Header hdr;
	is_.seekg(0);
	is_.read((char*)&hdr, sizeof(hdr));
	cgeAssert( hdr.magic[0] == 'I' && 
		    hdr.magic[1] == 'B' &&
			hdr.magic[2] == 'S' &&
			hdr.magic[3] == 'P' );

	cgeAssert( hdr.version == 38 );

	// read verts
	float scaling = 1.0f/40.0f;
	data.verts.resize( hdr.lump[2].length / 12 );
	is_.seekg( hdr.lump[2].offset );
	is_.read( (char*)&data.verts[0], hdr.lump[2].length );
	
	vec3f center;
	int nVerts = data.verts.size();
	for(int i=0; i<nVerts; ++i) {
		data.verts[i] *= scaling;
		center += data.verts[i] / (float)nVerts;
	}

	for( int i=0; i<nVerts; ++i )
		data.verts[i] -= center;

	// read edges
	std::vector<Edge> edges(hdr.lump[11].length / 4);
	is_.seekg(hdr.lump[11].offset);
	is_.read( (char*)&edges[0], hdr.lump[11].length );

	// read faceEdges
	std::vector<Sint32> faceEdges(hdr.lump[12].length / 4);
	is_.seekg(hdr.lump[12].offset);
	is_.read( (char*)&faceEdges[0], hdr.lump[12].length );

	// read faces
	std::vector<Face> faces( hdr.lump[6].length / sizeof(Face) );
	int nFaces = faces.size();
	
	is_.seekg(hdr.lump[6].offset);
	is_.read( (char*)&faces[0], hdr.lump[6].length );

	std::vector<unsigned>     fvs;
	std::vector<Mesh::Poly>   polys;
	std::vector<vec3f> normals;
	polys.reserve(nFaces*3);
	normals.reserve(nFaces*3);
	fvs.reserve(16);
	for(int i=0; i<nFaces; ++i) {
		fvs.clear();
		int first_edge = faces[i].first_edge;
		int end_edge   = faces[i].first_edge + faces[i].num_edges;
		
		if(faceEdges[first_edge]<0)
			fvs.push_back( edges[-faceEdges[first_edge]].b );
		else
			fvs.push_back( edges[faceEdges[first_edge]].a );
		
		for(int fei=first_edge; fei < end_edge; ++fei ) {
			unsigned v;
			if(faceEdges[fei] < 0)
				v = edges[-faceEdges[fei]].a;
			else
				v = edges[faceEdges[fei]].b;
				
			fvs.push_back( v );
		}

		vec3f      n;
		Mesh::Poly poly;
		for(unsigned vi=1;vi<fvs.size()-2;++vi) {
			vec3f &v1 = data.verts[fvs[0]];
			vec3f &v2 = data.verts[fvs[vi]];
			vec3f &v3 = data.verts[fvs[vi+1]];
			
			n  = -(v1-v2).cross(v2-v3);
			n.normalize();

			poly.vertex[0] = fvs[0];
			poly.vertex[1] = fvs[vi];
			poly.vertex[2] = fvs[vi+1];
			poly.normal[0] = poly.normal[1] = poly.normal[2] = normals.size();
			polys.push_back( poly );
			normals.push_back(n);
		}
	}
	std::cout << polys.size() << '\n';
	data.polys = polys;
	data.norms = normals;
}