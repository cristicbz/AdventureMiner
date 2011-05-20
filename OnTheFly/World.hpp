#pragma once

#include "Renderer/Entity.hpp"
#include "Math/Vectors.hpp"
#include "Chunk.hpp"

#include <array>

namespace cge {
	class Camera;
}

namespace cge { namespace game {

	static const Scalar ctBlockRadius	= 0.5f;
	static const Scalar ctBlockSize		= 2.0f * ctBlockRadius;

	class World : public Entity {
	public:
		explicit		World( Camera &cam );
		virtual			~World();

		virtual void	onRendered();
		virtual void	onAttached();
		virtual void	onDestroyed();
		virtual void	onDetached();
		
		BlockDef&		getBlockDef( BlockDefId id );
		Block			getBlockAt( const vec3& pos );
		Block			getNeighbourOf( const Block& block, Direction d );
		vec3			getWorldPosOf( const Block& block );
		void			setCamera( Camera &cam );


	private:
		// types
		#pragma pack(push,1)
		struct BufferedVertex {
			float	s, t;
			short	x, y, z;
			uint8	r, g, b;
		};
		#pragma pack(pop)

		// constants
		static const uint32	ctBufferVertexCount		= 32768 / sizeof( BufferedVertex );

		// methods
		// static inline void	addFace_( BufferedVertex * buf, int dir, BufferedVertex base );
		static inline void	addRightFace_( BufferedVertex * buf, BufferedVertex base );
		static inline void	addLeftFace_( BufferedVertex * buf, BufferedVertex base );
		static inline void	addUpFace_( BufferedVertex * buf, BufferedVertex base );
		static inline void	addDownFace_( BufferedVertex * buf, BufferedVertex base );
		static inline void	addFrontFace_( BufferedVertex * buf, BufferedVertex base );
		static inline void	addBackFace_( BufferedVertex * buf, BufferedVertex base );
		void				renderChunk_( const vec3s& pos, Chunk &ch );
		inline void			flushRenderBuffer_( );

		BufferedVertex*		renderBuffer_;
		BufferedVertex*		pbuf_;
		BufferedVertex*		bufferEnd_;


		// members
		std::array< Chunk, 1 >		chunks_;
		unsigned					vboId_;
		Camera*						cam_;
	};
	
	inline Block World::getNeighbourOf( const Block& block, Direction d ) {
		return getBlockAt(
			static_cast< vec3 >( block.getXyz() ) + ctVec::direction6[ d ]
		);
	}

	inline void World::setCamera( Camera &cam ) { cam_ = &cam; }
} }
