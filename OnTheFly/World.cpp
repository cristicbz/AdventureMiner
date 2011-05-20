#include "World.hpp"

#include <algorithm>

#include "Util/Debug.hpp"
#include "Math/Vectors.hpp"
#include "Renderer/SceneNode.hpp"
#include "Renderer/Camera.hpp"
#include <iostream>

#include "Util/OpenGL.hpp"


#define OFFSETOF(type, field)    ((void*) &(((type *) 0)->field))

using namespace cge;
using namespace cge::game;

namespace {
	const Scalar ctTileSize = 1.0f / 16.0f;	

	const Scalar ctTexSt[ 4 ][ 2 ] = {
		{ 6.0f * ctTileSize, 2.0f * ctTileSize },
		{ 6.0f * ctTileSize, 2.0f * ctTileSize },
		{ 7.0f * ctTileSize, 2.0f * ctTileSize },
		{ 8.0f * ctTileSize, 2.0f * ctTileSize }
	};

	const int viewDistance = 11;
	const int worldWidth = viewDistance * 2 + 1;
	AlignedBox chunkBounds[ viewDistance * 2 + 1 ][ viewDistance * 2 + 1 ];
}


__forceinline void World::addRightFace_( BufferedVertex* buf, BufferedVertex cv ) {
	++cv.x; *buf = cv;
	++cv.y; cv.s += ctTileSize; *(++buf) = cv;
	++cv.z; cv.t += ctTileSize; *(++buf) = cv;
	--cv.y; cv.s -= ctTileSize; *(++buf) = cv;
}								 							 
__forceinline void World::addUpFace_( BufferedVertex* buf, BufferedVertex cv ) {
	++cv.y; *buf = cv;			 
	++cv.z; cv.s += ctTileSize; *(++buf) = cv;
	++cv.x; cv.t += ctTileSize; *(++buf) = cv;
	--cv.z; cv.s -= ctTileSize; *(++buf) = cv;
}								 
__forceinline void World::addFrontFace_( BufferedVertex* buf, BufferedVertex cv ) {
	++cv.z; *buf = cv;			 
	++cv.x; cv.s += ctTileSize; *(++buf) = cv;
	++cv.y; cv.t += ctTileSize; *(++buf) = cv;
	--cv.x; cv.s -= ctTileSize; *(++buf) = cv;
}								 
__forceinline void World::addLeftFace_( BufferedVertex* buf, BufferedVertex cv ) {
	*buf = cv;					 
	++cv.z; cv.s += ctTileSize; *(++buf) = cv;
	++cv.y; cv.t += ctTileSize; *(++buf) = cv;
	--cv.z; cv.s -= ctTileSize; *(++buf) = cv;
}								 
__forceinline void World::addDownFace_( BufferedVertex* buf, BufferedVertex cv ) {
	*buf = cv;					 
	++cv.x; cv.s += ctTileSize; *(++buf) = cv;
	++cv.z; cv.t += ctTileSize; *(++buf) = cv;
	--cv.x; cv.s -= ctTileSize; *(++buf) = cv;
}								 
__forceinline void World::addBackFace_( BufferedVertex* buf, BufferedVertex cv ) {
	*buf = cv;					 
	++cv.y; cv.s += ctTileSize; *(++buf) = cv;
	++cv.x; cv.t += ctTileSize; *(++buf) = cv;
	--cv.y; cv.s -= ctTileSize; *(++buf) = cv;
}


void World::renderChunk_( const vec3s &offset, Chunk &ch ) {
	BufferedVertex cv;
	auto i = ch.visBegin(), e=ch.visEnd();
	for(; i!=e; ++i) {
		Block bk = ch.getBlock( *i );
		cv.x = offset.x + bk.getX();
		cv.y = offset.y + bk.getY();
		cv.z = offset.z + bk.getZ();
		
		// init st
		auto defId = bk.getBlockDefId();
		cv.s = ctTexSt[ defId ][ 0 ];
		cv.t = ctTexSt[ defId ][ 1 ];

		uint8 light = bk.getLightLevel();
		cv.r = cv.g = cv.b = light;
		
		if( bk.isFaceExposed( DR_RIGHT ) ) { addRightFace_( pbuf_, cv );	pbuf_ += 4; }
		if( bk.isFaceExposed( DR_UP    ) ) { addUpFace_( pbuf_, cv );		pbuf_ += 4; }
		if( bk.isFaceExposed( DR_FRONT ) ) { addFrontFace_( pbuf_, cv );	pbuf_ += 4; }
		if( bk.isFaceExposed( DR_LEFT  ) ) { addLeftFace_( pbuf_, cv );		pbuf_ += 4; }
		if( bk.isFaceExposed( DR_DOWN  ) ) { addDownFace_( pbuf_, cv );		pbuf_ += 4; }
		if( bk.isFaceExposed( DR_BACK  ) ) { addBackFace_( pbuf_, cv );		pbuf_ += 4; }

		// render faces
		if( pbuf_ >= bufferEnd_ )
			flushRenderBuffer_();
	}
}


inline void World::flushRenderBuffer_() {
	unsigned vertexCount = int(pbuf_ - renderBuffer_);
	glBufferSubData( GL_ARRAY_BUFFER, 0, vertexCount * sizeof( BufferedVertex ), renderBuffer_ );
	glDrawArrays( GL_QUADS, 0, vertexCount );
	pbuf_ = renderBuffer_;
}

void World::onRendered() {
	static const unsigned stride = sizeof( BufferedVertex );
	Chunk &ch = chunks_.front();
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glBindBuffer( GL_ARRAY_BUFFER, vboId_ );
	
	glVertexPointer		( 3, GL_SHORT,			stride, OFFSETOF( BufferedVertex, x ) );
	glColorPointer		( 3, GL_UNSIGNED_BYTE,	stride, OFFSETOF( BufferedVertex, r ) );
	glTexCoordPointer	( 2, GL_FLOAT,			stride, OFFSETOF( BufferedVertex, s ) );

	float camx = cam_->getNode().getPosition().x;
	float camz = cam_->getNode().getPosition().z;

	short dispx = intFloor( camx / 16.0f ) * 16;
	short dispz = intFloor( camz / 16.0f ) * 16;

	for( int i = 0; i<worldWidth; ++i )
		for( int j = 0; j<worldWidth; ++j ) {
			AlignedBox bounds = chunkBounds[i][j];

			bounds.translateX( dispx );
			bounds.translateZ( dispz );

			if( cam_->getFrustum().intersects( bounds ) )
				renderChunk_( bounds.getMinimum(), ch );
		}

	if( pbuf_ != renderBuffer_ )
		flushRenderBuffer_();
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

World::World( cge::Camera &cam )
: vboId_( 0 ) {
	setCamera( cam );
	renderBuffer_ = new BufferedVertex[ ctBufferVertexCount ];
	pbuf_ = renderBuffer_;
	bufferEnd_ = pbuf_ + ctBufferVertexCount - 24;
}

World::~World() {
	delete [] renderBuffer_;
}

void World::onAttached() {
	
	for(int i = - viewDistance; i <= viewDistance; ++i ) {
		for(int j = - viewDistance; j <= viewDistance; ++j ) {
			vec3i m(i, 0, j);
			m *= Chunk::ctChunkSizeXZ;
			chunkBounds[ i + viewDistance ][ j + viewDistance ].setMinAndMax(
				m,
				m + vec3i( Chunk::ctChunkSizeXZ, Chunk::ctChunkSizeY, Chunk::ctChunkSizeXZ )
			);
		}
	}
	glGenBuffers( 1, &vboId_ );
	cgeEnforce( vboId_ );
	glBindBuffer( GL_ARRAY_BUFFER, vboId_ );
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof( BufferedVertex[ ctBufferVertexCount ] ),
		0,
		GL_STREAM_DRAW
	);

	cgeEnforce( glGetError() == 0 );

	Chunk &ch = chunks_.front();
	ch.setWorld( *this );
	vec2 center(8,8);
	float maxd = center.norm();
	for(int x=0;x<16;++x)
		for(int z=0;z<16;++z) {
			float h = (vec2(static_cast<Scalar>(x),static_cast<Scalar>(z)) - center).norm() / maxd;
			int mx = int( pow( h, 0.7f ) * 16 ) -2 ;

			//mx+=rand()%4;
			for(int y=0;y<mx;++y) {
				if( (x >= 6 && x <= 8) && (y >= 7 && y<= 8) ) continue;
				ch.getBlock( x,y+85,z ).setDef( rand()%3 + 1  );
			}
			mx = min( int( pow( h, 32.0f ) * 120.0f ), 20 );
			for(int y=0;y<=mx;++y) {
				ch.getBlock( x,y+80,z ).setDef( rand()%3 + 1);
			}
		}
		
	getNode().setRenderable( true );
	getNode().setVisibility( true, true );
}

void World::onDestroyed() {

}

void World::onDetached() {
	if( vboId_ ) {
		glDeleteBuffers( 1, &vboId_ );
		vboId_ = 0;
	}
}

Block World::getBlockAt( const vec3& pos ) {
	Chunk &ch = chunks_.front();

	int cx, cz, bx, by, bz;

	cx = intFloor( pos.x / Chunk::ctChunkSizeXZ );
	cz = intFloor( pos.z / Chunk::ctChunkSizeXZ );
	by = intFloor( pos.y );

	//if( cx != 0 || cz != 0 || by < 0 || by >= Chunk::ctChunkSizeY )
		//return ch.getBlock(0,Chunk::ctChunkSizeY-1,0);

	bx = intFloor( pos.x ) - cx * Chunk::ctChunkSizeXZ;
	bz = intFloor( pos.z ) - cz * Chunk::ctChunkSizeXZ;

	cgeAssert3( bx >= 0 && bx < Chunk::ctChunkSizeXZ, bx, cx, pos.x );
	cgeAssert3( bz >= 0 && bz < Chunk::ctChunkSizeXZ, bz, cz, pos.z );
	cgeAssert2( by >= 0 && by < Chunk::ctChunkSizeY,  by, pos.y );

	Block b = ch.getBlock(bx,by,bz);
	
	return b;
}

vec3 World::getWorldPosOf( const Block& block ) {
	vec3 r( block.getX(), block.getY(), block.getZ() );
	r += vec3( 0.5 );
	return r;
}

