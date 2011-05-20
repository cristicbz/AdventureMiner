#pragma once

#include <vector>

#include "BlockInfo.hpp"
#include "BlockDef.hpp"
#include "Util/Noncopyable.hpp"

#include "Util/Debug.hpp"
#include <iostream>
#include <set>

namespace cge { namespace game {
	class World;

	class Chunk : NonCopyable {
		friend class Block;
		
		struct CompareY {
			bool operator()( BlockId a, BlockId b );
		};

	public:
		// constants & types
		static const unsigned int		ctChunkSizeY		= 128;
		static const unsigned int		ctChunkSizeXZ		= 16;
		static const unsigned int		ctBlocksPerChunk	= ctChunkSizeXZ*ctChunkSizeXZ*ctChunkSizeY;

		typedef std::vector<BlockData>		DataVector;
		typedef std::set<BlockId>		BlockSet;
		typedef BlockSet::iterator			BlockIter;

								Chunk();

		void					setWorld( World &w ) { world_ = &w; }
		World&					getWorld() const { return *world_; }

		// public methods
		Block					getBlock( int x, int y, int z );
		Block					getBlock( BlockId id );
		unsigned				getUsedMemory();
		unsigned				computeVisibleFaces();
		unsigned				getVisibleBlockCount();

		BlockIter				visBegin();
		BlockIter				visEnd();

		// id -> coords
		static unsigned char	idToX( BlockId id );
		static unsigned char	idToY( BlockId id );
		static unsigned char	idToZ( BlockId id );
		static BlockId			coordToId( int x, int y, int z );


	private:
		// private members
		World*		world_;
		BlockInfo	blocks_[ ctBlocksPerChunk ];
		BlockSet	visible_;
	};

	class Block {
		friend class Chunk;

	public:
						Block( BlockId id, Chunk& chunk );
						Block( const Block& other );

		Block&			operator=( const Block& rhs );
		bool			operator==( const Block& rhs ) const;
		bool			operator!=( const Block& rhs ) const;

		uint16			isFaceExposed( Direction face ) const;
		uint16			isExposed() const;

		void			setDef( BlockDefId newDefId );

		unsigned char	getX() const;
		unsigned char	getY() const;
		unsigned char	getZ() const;
		vec3i			getXyz() const;

		Chunk&			getChunk() const;
		BlockDef&		getBlockDef() const;
		BlockData&		getBlockData() const;
		uint8			getLightLevel() const;
		
		bool			isVisible() const		{ return isRenderable() && (isTransparent() || isExposed()); }
		bool			isRenderable() const	{ return info_->getDef() != 0x0; }
		bool			isTransparent() const	{ return info_->getDef() == 0x0; }
		bool			isObstacle() const		{ return info_->getDef() != 0x0; }
		BlockDefId		getBlockDefId() const	{ return info_->getDef(); }

	private:
		void checkVis_( bool newVis );
		void adjTrChanged_( Direction d, bool newVis );
		void checkTr_( bool newTr );

		BlockInfo*	info_;
		BlockId		id_;
		Chunk&		ch_;
	};

	// inline implementations 
	inline bool Chunk::CompareY::operator()( BlockId a, BlockId b ) {
		uint8 ay = idToY( a ), by = idToY( b );
		if( ay != by )
			return ay < by;
		else
			return a < b;
	}

	inline unsigned Chunk::getVisibleBlockCount() {
		return visible_.size(); 
	}

	inline Block Chunk::getBlock( int x, int y, int z )	{ return getBlock( coordToId(x,y,z) ); }
	inline Block Chunk::getBlock( BlockId id )			{ return Block( id, *this ); }

	inline unsigned Chunk::getUsedMemory()		{ return sizeof( blocks_ ) + sizeof( visible_ ) + visible_.size() * ( sizeof( BlockId ) + 8) + 4; }
	inline Chunk::BlockIter Chunk::visBegin()	{ return visible_.begin(); }
	inline Chunk::BlockIter Chunk::visEnd()		{ return visible_.end(); }

	// id -> coords
	inline unsigned char Chunk::idToX( BlockId id )			{ return (id >>  7) & 0x0f; }
	inline unsigned char Chunk::idToZ( BlockId id )			{ return (id >> 11) & 0x0f; }
	inline unsigned char Chunk::idToY( BlockId id )			{ return (id      ) & 0x7f; }
	inline BlockId Chunk::coordToId( int x, int y, int z )	{ return (z << 11) + (x << 7) + y; }

	inline Block::Block( BlockId id, Chunk& chunk )				: info_( chunk.blocks_ + id ), id_(id),  ch_(chunk) {}
	inline uint16 Block::isFaceExposed( Direction face ) const	{ return info_->isFaceExposed( face ); }
	inline uint16 Block::isExposed() const						{ return info_->isExposed();  }
	inline Chunk& Block::getChunk() const						{ return ch_; }
	inline unsigned char Block::getX() const					{ return Chunk::idToX( id_ ); }
	inline unsigned char Block::getY() const					{ return Chunk::idToY( id_ ); }
	inline unsigned char Block::getZ() const					{ return Chunk::idToZ( id_ ); }
	inline vec3i Block::getXyz() const							{ return vec3i( getX(), getY(), getZ() ); }
	inline uint8 Block::getLightLevel() const					{ return info_->getLightLevel()*16 + 15; }

	inline Block::Block( const Block& other ): ch_( other.ch_ ), id_( other.id_ ), info_( other.info_ ) {}
	inline Block& Block::operator=( const Block& rhs ) { id_ = rhs.id_; info_ = rhs.info_; return *this; }
	inline bool Block::operator==( const Block& rhs ) const { return id_ == rhs.id_; }
	inline bool Block::operator!=( const Block& rhs ) const { return id_ != rhs.id_; }

} } // namespace cge::game