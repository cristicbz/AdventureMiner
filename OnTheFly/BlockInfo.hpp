#pragma once

#include "Util/Debug.hpp"
#include "Util/IntTypes.hpp"
#include "Math/Constants.hpp"
#include <iostream>
namespace cge { namespace game {
	typedef uint16	BlockId;
	typedef uint16	BlockDefId;
	typedef uint8	LightLevel;
	typedef uint8	BlockData;

	class BlockDefManager;
	class BlockDef;
	class Block;
	class World;

	struct BlockInfo {
		typedef uint32 IntType;

		BlockDefId			getDef() const;
		BlockData			getData() const;
		uint16				isFaceExposed( Direction face ) const;
		uint16				isExposed() const;
		LightLevel			getLightLevel() const;

		void				setDef( BlockDefId newDef );
		void				setData( BlockData newDat );
		void				setLightLevel( LightLevel newLevel );
		void				showFace( Direction face );
		void				hideFace( Direction face );
		unsigned			countFaces();
		void				hideAllFaces();
		void				showAllFaces();

		static BlockInfo	fromInteger( IntType i );
		static BlockInfo	air();

	private:
		static const uint16	lightShift_	= 0;
		static const uint16	dataShift_	= 4;
		static const uint16	facesShift_	= 10;
		static const uint16	lightMask_	= 0xf;
		static const uint16	dataMask_	= 0x3f;
		static const uint16	facesMask_	= 0x3f;
		static const uint16 rightMask_	= 1 << facesShift_;

		BlockDefId	def_;
		uint16		dat_;

		// ffffffxxxxxxllll
		// f - faces
		// x - extra
		// l - light-level
		
	};

	inline BlockDefId BlockInfo::getDef() const						{ return def_; }
	inline BlockData BlockInfo::getData() const						{ return (dat_ >> dataShift_) & dataMask_; }
	inline uint16 BlockInfo::isFaceExposed( Direction face ) const	{ return (dat_ & (rightMask_ << face)) != 0; }
	inline uint16 BlockInfo::isExposed() const						{ return dat_ & (facesMask_ << facesShift_); }
	inline LightLevel BlockInfo::getLightLevel() const				{ return (dat_ << lightShift_) & lightMask_; }
	inline void BlockInfo::setDef( BlockDefId newDef )				{ def_ = newDef; }
	inline void BlockInfo::setData( BlockData newDat )				{ cgeAssert( newDat <= dataMask_ ); dat_ = dat_ & ~(dataMask_ << dataShift_) | (newDat << dataShift_); } 
	inline void BlockInfo::setLightLevel( LightLevel newLevel )		{ cgeAssert( newLevel <= lightMask_ ); dat_ = dat_ & ~lightMask_ | newLevel; }
	inline void BlockInfo::showFace( Direction face )				{ dat_ |= rightMask_ << face; }
	inline void BlockInfo::hideFace( Direction face )				{ dat_ &= ~(rightMask_ << face); }
	inline void	BlockInfo::hideAllFaces()							{ dat_ &= ~(facesMask_ << facesShift_); }
	inline void	BlockInfo::showAllFaces()							{ dat_ |= facesMask_ << facesShift_; }
	inline BlockInfo BlockInfo::fromInteger( IntType i )			{ return *( reinterpret_cast< BlockInfo* > ( &i ) ); }
	inline BlockInfo BlockInfo::air()								{ return fromInteger( 0xfc0f0000 ); }
	inline unsigned	BlockInfo::countFaces()							{
		unsigned n = 0;
		for(int i=0; i<6; ++i )
			if( isFaceExposed( Direction(i) ) )
				++n;
		return n;
	}
} }