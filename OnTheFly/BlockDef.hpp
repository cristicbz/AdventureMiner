#pragma once

#include "Math/Vectors.hpp"

namespace cge { namespace game {
	enum BlockVertex {
		BV_RUF, BV_RUB, BV_LUB, BV_LUF,
		BV_RDF, BV_RDB, BV_LDB, BV_LDF,
	};

	enum FaceVertex { FV_BL, FV_BR, FV_UR, FV_UL };
	enum Transparency { TR_OPAQUE, TR_TRANSPARENT, TR_INVISIBLE };

	class BlockDef {
	public:
		bool				isTransparent() const;
		bool				isObstacle() const;
		bool				hasGravity() const;
		bool				getDataRequired() const;
		
		const vec2&			getVertexSt( Direction face, FaceVertex vertex ) const;
		static const vec3&	getVertexXyz( Direction face, FaceVertex vertex );

		void				setTransparent( bool transparent );
		void				setObstacle( bool obstacle );
		void				setGravity( bool gravity );
		void				setDataRequired( bool requireData );
		void				setFaceSt( Direction face, Scalar tileSize, const vec2& tileXy );
		void				setAllSt( Scalar tileSize, const vec2& tileXy );

	private:
		static const vec3	fv_[ 6 ][ 4 ];
		vec2				st_[ 6 ][ 4 ];
		bool				tr_, ob_, gr_, dr_;
	};


	// one-line inline implementations
	inline bool				BlockDef::isTransparent() const								{ return tr_; }
	inline bool				BlockDef::isObstacle() const								{ return ob_; }
	inline bool				BlockDef::hasGravity() const								{ return gr_; }
	inline bool				BlockDef::getDataRequired() const							{ return dr_; }
	inline const vec2&		BlockDef::getVertexSt( Direction f, FaceVertex v ) const	{ return st_[ f ][ v ]; }
	inline const vec3&		BlockDef::getVertexXyz( Direction f, FaceVertex v )			{ return fv_[ f ][ v ]; }
	inline void				BlockDef::setTransparent( bool tr )							{ tr_ = tr; }
	inline void				BlockDef::setObstacle( bool ob )							{ ob_ = ob; }
	inline void				BlockDef::setGravity( bool gr )								{ gr_ = gr; }
	inline void				BlockDef::setDataRequired( bool dr ) 						{ dr_ = dr; }
	
	// longer inline implementations
	inline void	BlockDef::setFaceSt( Direction face, Scalar sz, const vec2& xy ) {
		vec2* f = st_[ face ];
		f[0].set( xy.x*sz, (xy.y + 1.0f)*sz );
		f[1].set( f[0].x + sz, f[0].y );
		f[2].set( f[1].x, f[1].y - sz );
		f[3].set( f[2].x - sz, f[2].y );
	}

	inline void BlockDef::setAllSt( Scalar tileSize, const vec2& tileXy ) {
		for( int i = 0; i < 6; ++i )
			setFaceSt( static_cast<Direction>(i), tileSize, tileXy );
	}
} }