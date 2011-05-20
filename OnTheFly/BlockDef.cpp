#include "BlockDef.hpp"

namespace {
	const cge::vec3 ruf(  1.0f,  1.0f,  1.0f );
	const cge::vec3 luf( -1.0f,  1.0f,  1.0f );
	const cge::vec3 lub( -1.0f,  1.0f, -1.0f );
	const cge::vec3 rub(  1.0f,  1.0f, -1.0f );
	const cge::vec3 rdf(  1.0f, -1.0f,  1.0f );
	const cge::vec3 ldf( -1.0f, -1.0f,  1.0f );
	const cge::vec3 ldb( -1.0f, -1.0f, -1.0f );
	const cge::vec3 rdb(  1.0f, -1.0f, -1.0f );
}

const cge::vec3 cge::game::BlockDef::fv_[ 6 ][ 4 ] = { 
	{ rdb, rdf, ruf, rub },
	{ lub, rub, ruf, luf },
	{ rdf, ldf, luf, ruf },
	{ ldf, ldb, lub, luf },
	{ ldf, rdf, rdb, ldb },
	{ ldb, rdb, rub, lub }
};