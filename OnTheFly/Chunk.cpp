#include "Chunk.hpp"
#include "World.hpp"
#include <cstring>
#include <algorithm>
#include <numeric>


using namespace cge;
using namespace cge::game;

Chunk::Chunk(): world_( 0 ) {
	std::fill_n(
		blocks_,
		ctBlocksPerChunk,
		BlockInfo::air()
	);
}

void Block::checkTr_( bool oldTr ) {
	bool newTr = isTransparent();
	if( newTr != oldTr ) {
		World& w = ch_.getWorld();
		for(int i = 0; i < 6; ++i ) {
			w.getNeighbourOf( *this, static_cast< Direction >( i ) )
			.adjTrChanged_( oppositeDirection( static_cast< Direction >( i ) ), newTr );
		}
	}
}

void Block::checkVis_( bool oldVis ) {
	bool newVis = isVisible();
	if( newVis != oldVis ) {
		if( newVis ) {
			auto r = ch_.visible_.insert( id_ );
			//ch_.visible_.push_back( id_ );
			//cgeAssert1( r.second, getXyz() );
		} else {
			 auto r = ch_.visible_.erase( id_ );
			// ch_.visible_.erase( std::remove( ch_.visible_.begin(), ch_.visible_.end(), id_ ) );
			//cgeAssert1( r, getXyz() );
		}
	}
}

void Block::adjTrChanged_( Direction d, bool newTr ) {
	bool oldVis = isVisible();
	
	if( newTr ) {
		info_->showFace( d );
		checkVis_( oldVis );
	} else {
		info_->hideFace( d );
		checkVis_( oldVis );
	}

	float l = (info_->countFaces()/6.0f) * 5.0f + pow(std::min((float)getY(),100.0f)/100.0f,3.0f)*7.0f + 3.0f;
	info_->setLightLevel( uint8( l ) );
}

void Block::setDef( BlockDefId newId ) {
	BlockDefId oldId = info_->getDef();

	if( oldId == newId )
		return;

	bool oldTr	= isTransparent();
	bool oldVis	= isVisible();

	World& w = ch_.getWorld();
	info_->setDef( newId );

	checkTr_( oldTr );
	checkVis_( oldVis );
}

unsigned Chunk::computeVisibleFaces() {
	return std::accumulate( visible_.begin(), visible_.end(), 0,
		[this]( int sum, BlockId id ) { return sum + blocks_[id].countFaces(); }
	);
}
