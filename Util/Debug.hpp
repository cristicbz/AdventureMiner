#pragma once

#include <stdexcept>
#include <string>
#include <boost/lexical_cast.hpp>
#include <Math/Constants.hpp>

#define cgeStringizeAux( token )			#token
#define cgeStringize( token )				cgeStringizeAux( token )
#define cgeLocationInfo()					__FUNCTION__ ": " cgeStringize(__LINE__)
#define cgeThrowIf( cond, exception )		do { if( cond ) throw exception; } while( false )
#define cgeThrowLogicIf( cond, message )	do { if( cond ) throw std::logic_error( cgeLocationInfo() ": " message ); } while( false )
#define cgeTraceString( x )					( (#x "=") + boost::lexical_cast<std::string>(x) )
#define cgeAssertString( cond )				"Assertion '" #cond "' failed"

#ifndef NDEBUG
#define CGE_ENABLE_ASSERTIONS
#endif

#ifdef CGE_ENABLE_ASSERTIONS

#define cgeAssert( cond )				cgeThrowLogicIf( !( cond ), cgeAssertString(cond) )
#define cgeAssertM( cond, message )		cgeThrowLogicIf( !( cond ), message )
#define cgeAssert1( cond, x )			cgeThrowLogicIf( !( cond ), cgeAssertString(cond) " (" + cgeTraceString(x) + ")" )
#define cgeAssert2( cond, x, y )		cgeThrowLogicIf( !( cond ), cgeAssertString(cond) " (" + cgeTraceString(x) + ", " + cgeTraceString(y) + ")" )
#define cgeAssert3( cond, x, y, z )		cgeThrowLogicIf( !( cond ), cgeAssertString(cond) " (" + cgeTraceString(x) + ", " + cgeTraceString(y) + ", " + cgeTraceString(z) + ")" )

#define cgeTrace( log, what )			do { (log) << cgeLocationInfo() ": " << cgeTraceString(what) << '\n'; } while( false )
#define cgeTraceIf( cond, log, what )	do { if( cond ) (log) << cgeLocationInfo() ": " <<  cgeTraceString(what) << '\n'; } while( false )


#else

#define cgeAssert( cond )
#define cgeAssertM( cond, message )	
#define cgeAssert1( cond, x )
#define cgeAssert2( cond, x, y )
#define cgeAssert3( cond, x, y, z )
#define cgeTrace( log, what )
#define cgeTraceIf( cond, log, what )

#endif

#define cgeStub( l )				(l) << (cgeLocationInfo() ": WARNING: Function is a stub.\n");
#define cgeEnforceArg( x, e, arg )	cge::priv::enforce<e>( x, arg )
#define cgeEnforceMsg( x, e, msg )	cge::priv::enforce<e>( x, cgeLocationInfo() msg )
#define cgeEnforceEx( x, e )		cgeEnforceMsg( x, e, ": Enforcement failed" )
#define cgeEnforce( x )				cgeEnforceEx( x, std::runtime_error )

namespace cge {
	class AlignedBox;

	void debugCube( Scalar w, bool wire );
	void debugBox( const AlignedBox &b, bool wire );
	void debugBillboard( Scalar w );
	
	//// PRIVATE ////
	namespace priv {	
		template< class E, class T, class A >
		const T& enforce( const T& test, const A& arg ) {
			if( !test )
				throw E(arg);

			return test;
		}

		template< class E, class T, class A >
		T& enforce( T& test, const A& arg ) {
			if( !test )
				throw E(arg);

			return test;
		}
	}
}