#include "Id4MapLexer.hpp"
#include <sstream>
#include <iostream>

using namespace cge;

Id4MapLexer::Id4MapLexer() {
	Input ws      = createClass( getWhiteSpaceEof() );
	Input alpha   = createClass( getAlphanumerics() +'/' );
	Input letters = createClass( getLetters() );
	Input digits  = createClass( getDigits() );
	Input syms    = createClass( "(){}," );

	//// ignore whitespace ////
	addRule( S_START, ws,           Action(A_IGNORE) );


	//// parse symbol      ////
	addRule( S_START, syms,         Action(A_ADD,S_START,T_SYMBOL,A_RETURN) );


	//// parse words       ////
	addRule( S_START, letters,      Action(A_ADD, S_WORD) );
	addRule( S_WORD, I_DEFAULT,     Action(A_PUTBACK,S_START,T_WORD,A_RETURN) );
	addRule( S_WORD, alpha,         Action(A_ADD) );


	//// parse numbers    ////
	addRule( S_START, '-',          Action(A_ADD, S_MINUS) );
	addRule( S_START, digits,       Action(A_ADD, S_INT) );

	addRule( S_MINUS, digits,       Action(A_ADD, S_INT) );

	addRule( S_INT,      I_DEFAULT, Action(A_PUTBACK,S_START,T_INT,    A_RETURN) );
	addRule( S_DECIMAL,  I_DEFAULT, Action(A_PUTBACK,S_START,T_DOUBLE, A_RETURN) );
	addRule( S_EXPONENT, I_DEFAULT, Action(A_PUTBACK,S_START,T_DOUBLE, A_RETURN) );
	addRule( S_INT,      '.',       Action(A_ADD, S_DOT) );
	
	addRule( S_INT,      'e',       Action(A_ADD, S_E) );
	addRule( S_DECIMAL,  'e',       Action(A_ADD, S_E) );
	
	addRule( S_INT,      digits,    Action(A_ADD) );
	addRule( S_DECIMAL,  digits,    Action(A_ADD) );
	addRule( S_EXPONENT, digits,    Action(A_ADD) );

	addRule( S_DOT,      digits,    Action(A_ADD,S_DECIMAL) );
	addRule( S_E,        '-',       Action(A_ADD,S_E_MINUS) );
	addRule( S_E,        digits,    Action(A_ADD,S_EXPONENT) );	
	

	//// parse string   ////
	addRule( S_START,  '\"',      Action(A_IGNORE, S_STRING) );
	addRule( S_STRING, I_DEFAULT, Action(A_ADD) );
	addRule( S_STRING, '\\',      Action(A_IGNORE, S_ESCAPE) );
	addRule( S_STRING, '\"',      Action(A_IGNORE, S_START, T_STRING, A_RETURN) );
	
	// FIXME: Escaping t and n etc. Requires Action model modifications.
	addRule( S_ESCAPE, I_DEFAULT, Action(A_ADD, S_STRING) );

	compile();
}
